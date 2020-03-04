#include <sstream>

#include <lithp.hpp>
#include <reader/reader.hpp>

namespace lithp {
Object *nil() { return nullptr; }
bool is_null(Object *obj) { return obj == nullptr; }
bool is_pair(Object *obj) { return List::is_instance(obj); }

List *cons(Object *car, Object *cdr) { return List::make(car, cdr); }
std::string to_string(Object *obj) {
  if (is_null(obj)) {
    return "nil";
  } else {
    std::ostringstream out;
    obj->repr(out);
    return out.str();
  }
}

Object *car(List *list) {
  if (is_null(list)) {
    return nil();
  }
  return list->car();
}

Object *cdr(List *list) {
  if (is_null(list)) {
    return nil();
  }
  return list->cdr();
}

Object *nth(size_t n, List *list) {
  for (size_t i = 0; i < n; list = List::cast(cdr(list))) {
    ++i;
  }
  return car(list);
}

bool eq(Object *o1, Object *o2) {
  if (o1 == o2) {
    return true;
  } else if (type_of(o1) != type_of(o2)) {
    return false;
  }

  switch (type_of(o1)) {
  case Type::Number:
    return Number::eq(Number::cast(o1), Number::cast(o2));
  default:
    return false;
  }
}

Type type_of(Object *obj) {
  if (obj) {
    return obj->type();
  }
  return Type::Nil;
}

bool is_true(Object *obj) { return !is_false(obj); }
bool is_false(Object *obj) { return is_null(obj) || obj == Boolean::False(); }

static Object *eval_special_form(SpecialForm *form, List *args,
                                 Environment &env) {
  Object **ptr = runtime::stack::ptr();
  size_t n = 0;
  for (; !is_null(args); args = List::cast(cdr(args))) {
    runtime::stack::push(car(args));
    n++;
  }
  return form->call(n, ptr, env);
}

static Object *eval_list(List *lst, Environment &env) {
  if (lst->empty()) {
    return nil();
  }

  Function *fun = nullptr;
  switch (type_of(car(lst))) {
  case Type::Symbol:
    if (SpecialForm::exists(Symbol::cast(car(lst)))) {
      return eval_special_form(SpecialForm::get(Symbol::cast(car(lst))),
                               List::cast(cdr(lst)), env);
    } else {
      fun = env.get_fun(Symbol::cast(car(lst)));
      break;
    }
  case Type::List:
    fun = Function::cast(eval_list(List::cast(car(lst)), env));
    break;
  default:
    throw std::runtime_error{"not a function: " + to_string(car(lst))};
  }

  if (is_null(fun)) {
    throw std::runtime_error{"no function to call"};
  }
  return apply(fun, List::cast(cdr(lst)), env);
}

static Object *eval_symbol(Symbol *sym, Environment &env) {
  if (sym->self_evaluating()) {
    return sym;
  }
  return env.get(sym);
}

Object *eval(Object *obj, Environment &env) {
  if (is_null(obj)) {
    return nil();
  }

  switch (obj->type()) {
  case Type::List:
    eval_list(List::cast(obj), env);
  case Type::Symbol:
    eval_symbol(Symbol::cast(obj), env);
  case Type::BrokenHeart:
    throw std::logic_error{"attempting to evaluate a broken heart"};
  default:
    return obj;
  }
}

Object *eval_sequence(List *seq, Environment &env) {
  Object *result = nil();
  for (List *rem = seq; !is_null(rem); rem = List::cast(rem->cdr())) {
    result = eval(rem->car(), env);
  }
  return result;
}

Object *apply(Function *fun, List *args, Environment &env) {
  if (is_null(fun)) {
    throw std::runtime_error{"not a function: " + to_string(fun)};
  }
  runtime::stack::new_frame(List::make(fun, args));
  for (size_t i = 0; i < fun->num_slots(); i++) {
    if (is_null(args)) {
      throw std::runtime_error{
          "not enough arguments for function call: " + std::to_string(i) +
          "; required: " + std::to_string(fun->num_slots())};
    }
    runtime::stack::push(eval(car(args), env));
    args = List::cast(cdr(args));
  }

  for (; !is_null(args); args = List::cast(cdr(args))) {
    runtime::stack::push(eval(car(args), env));
  }
  runtime::stack::call_in_current_frame(fun);
  return runtime::stack::yield_frame();
}

} // namespace lithp
