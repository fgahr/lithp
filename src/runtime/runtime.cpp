#include <lib/stdlib.hpp>
#include <lithp.hpp>

namespace lithp::runtime {
namespace {
class Runtime {
public:
  ~Runtime();
  RefStream refs();
  Environment &base_env();
  void read(std::istream &in);
  void init() {
    lib::load_stdlib(env);
    env.def(Symbol::intern("true"), Boolean::True());
    env.def(Symbol::intern("false"), Boolean::False());
  }

private:
  Environment env;
};

Runtime::~Runtime() { allocator::shutdown(); }

RefStream Runtime::refs() {
  // TODO
  return RefStream::empty();
}

Environment &Runtime::base_env() { return env; }
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

Environment &global_env() {
  if (runtime == nullptr) {
    throw std::logic_error{
        "attempting to get base environment but runtime is not initialized"};
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
