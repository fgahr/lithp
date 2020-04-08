#include <lithp/lib/stdlib.hpp>
#include <lithp/lithp.hpp>

namespace lithp::runtime {
namespace {
class Runtime {
public:
  ~Runtime() {
    allocator::shutdown();
    for (Object *o : globals) {
      delete o;
    }
  }
  RefStream refs() { return stack::live_objects(); }
  Environment &base_env() { return env; }
  void add_global(Object *b) { globals.push_back(b); }
  void init() {
    SpecialForm::init();
    stack::reset();
    lib::load_stdlib(env);
    env.def(Symbol::intern("true"), Boolean::True());
    env.def(Symbol::intern("false"), Boolean::False());
  }

private:
  std::vector<Object *> globals;
  Environment env;
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

Environment &global_env() {
  if (runtime == nullptr) {
    throw std::logic_error{
        "attempting to get base environment but runtime is not initialized"};
  }
  return runtime->base_env();
}

void register_builtin(Builtin *b) {
  if (runtime == nullptr) {
    throw std::logic_error{
        "attempting to register builtin but runtime is not initialized"};
  }
  runtime->add_global(b);
}

void register_special_form(SpecialForm *s) {
  if (runtime == nullptr) {
    throw std::logic_error{
        "attempting to register special form but runtime is not initialized"};
  }
  runtime->add_global(s);
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
