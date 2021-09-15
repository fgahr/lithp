#include <lithp/object/broken_heart.hpp>
#include <lithp/runtime/heap.hpp>
#include <lithp/runtime/runtime.hpp>

static const int align = 8;

namespace lithp::allocator {
namespace {
class Allocator {
  public:
    Allocator(size_t mem_size = 0x100000); // 1MB default
    Allocator(const Allocator &alloc) = delete;
    ~Allocator();
    char *allocate(size_t size);
    void do_gc();

  private:
    char *heaps[2];
    size_t heap_pos = 0;
    size_t heap_idx = 0;
    size_t mem_size;
    char *heap_ptr();
    void ensure_space(size_t amount);
    size_t align_up(size_t addr);
    void relocate(Object **obj, char *target, size_t *pos);
    void double_heap_size();
};

Allocator::Allocator(size_t mem_size) : mem_size{mem_size} {
    heaps[0] = (char *)std::calloc(mem_size, sizeof(char));
    heaps[1] = (char *)std::calloc(mem_size, sizeof(char));

    if (!heaps[0] || !heaps[1]) {
        throw std::runtime_error{"memory allocation failed"};
    }
}

Allocator::~Allocator() {
    if (heaps[0]) {
        free(heaps[0]);
    }
    if (heaps[1]) {
        free(heaps[1]);
    }
}

char *Allocator::allocate(size_t size) {
    ensure_space(size);
    char *allocated = heap_ptr();
    heap_pos += align_up(size);
    return allocated;
}

char *Allocator::heap_ptr() {
    return &heaps[heap_idx][heap_pos];
}

size_t Allocator::align_up(size_t addr) {
    return (addr * align + align - 1) / align;
}

void Allocator::ensure_space(size_t amount) {
    // For very large allocations (e.g. huge strings) we may need to double more
    // than once.
    while (heap_pos + amount >= mem_size) {
        do_gc();
        if (heap_pos > 0.8 * mem_size) { // heap more than 80% full
            double_heap_size();
        }
    }
}

void Allocator::do_gc() {
    RefStream refs = runtime::live_objects();
    char *new_heap = heaps[!heap_idx];
    size_t pos = 0;

    while (refs.has_more()) {
        relocate(refs.get(), new_heap, &pos);
    }

    // Call destructors for obsolete objects
    size_t size;
    while (pos < heap_pos) {
        Object *obj = (Object *)&heaps[heap_idx][pos];
        // TODO: Ensure alignment is correct after this step
        size = obj->size();
        delete obj;
        pos += align_up(size);
    }

    // Use new heap
    heap_idx = !heap_idx;
    heap_pos = pos;
}

void Allocator::relocate(Object **ref, char *target, size_t *pos) {
    if (ref == nullptr || *ref == nullptr) {
        throw std::runtime_error{"null pointer encounter during relocation"};
    }

    if (!(*ref)->heap_allocated()) {
        // Nothing to do
        return;
    }

    if (BrokenHeart::is_instance(*ref)) {
        // Already moved, adjust pointer
        *ref = BrokenHeart::cast(*ref)->redirect;
        return;
    }

    Object *moved = (*ref)->copy_to(&target[*pos]);
    void *old_loc = *ref;
    delete *ref;
    new (old_loc) BrokenHeart{moved};
    *ref = moved;
    *pos += moved->size();
}

void Allocator::double_heap_size() {
    mem_size *= 2;
    auto other_idx = !heap_idx;
    heaps[other_idx] = (char *)realloc(heaps[other_idx], mem_size);

    if (!heaps[other_idx]) {
        throw std::runtime_error{"memory allocation failed"};
    }

    // Another gc run is required to adjust all references to the new heap
    do_gc();
    // heap_idx flips during gc run
    other_idx = !heap_idx;
    heaps[other_idx] = (char *)realloc(heaps[other_idx], mem_size);

    if (!heaps[other_idx]) {
        throw std::runtime_error{"memory allocation failed"};
    }
}
} // namespace

static Allocator *alloc = nullptr;

void init() {
    if (alloc) {
        throw std::logic_error{"double initialization of allocator"};
    }
    alloc = new Allocator{};
}

void shutdown() {
    delete alloc;
    alloc = nullptr;
}

void run_gc() {
    if (alloc) {
        alloc->do_gc();
    } else {
        throw std::logic_error{"allocator not initialized"};
    }
}

char *get(size_t size) {
    if (alloc) {
        return alloc->allocate(size);
    }
    throw std::logic_error{"allocator not initialized"};
}

} // namespace lithp::allocator
