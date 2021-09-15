#include <lithp/lib/stdlib.hpp>
#include <lithp/lithp.hpp>

namespace lithp::runtime {
namespace {
class Runtime {
  public:
    ~Runtime() {
        Symbol::clear_global_associations();
        allocator::shutdown();
    }
    RefStream refs() {
        return stack::live_objects();
    }
    Environment *base_env() {
        return env;
    }
    void init() {
        SpecialForm::init();
        stack::reset();
        lib::load_stdlib(env);
        env->def(Symbol::intern("true"), Boolean::True());
        env->def(Symbol::intern("false"), Boolean::False());
    }

  private:
    Environment *env = Environment::get_global();
};
} // namespace

static Runtime *runtime = nullptr;

void init() {
    if (runtime) {
        throw std::logic_error{"double initialization of runtime"};
    }
    runtime = new Runtime{};
    runtime->init();
    allocator::init();
}

RefStream live_objects() {
    if (runtime == nullptr) {
        throw std::logic_error{
            "attempting to get live objects but runtime is not set up"};
    }
    return runtime->refs();
}

Environment *global_env() {
    if (runtime == nullptr) {
        throw std::logic_error{"attempting to get base environment but runtime "
                               "is not initialized"};
    }
    return runtime->base_env();
}

void shutdown() {
    if (runtime == nullptr) {
        throw std::logic_error{
            "attempting to shut down runtime but is not initialized"};
    }
    delete runtime;
    runtime = nullptr;
    allocator::shutdown();
}

} // namespace lithp::runtime
