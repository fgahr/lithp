#include <lithp.hpp>

namespace lithp {

List::List(Object *__car, Object *__cdr) : _car{__car}, _cdr{__cdr} {}

void List::repr(std::ostream &out) {
  out << "(";
  _car->repr(out);
  Object *rest = _cdr;
  while (List::is_instance(rest)) {
    out << " ";
    List *as_list = List::cast(rest);
    as_list->_car->repr(out);
    rest = as_list->_cdr;
  }

  if (is_null(rest)) {
    out << ")";
  } else {
    out << " . ";
    rest->repr(out);
    out << ")";
  }
}

RefStream List::refs() {
  // NOTE: Nested references need to be adjusted first in a gc run
  return RefStream::concat(_car->refs(), _cdr->refs(),
                           RefStream::of(&_car, &_cdr));
}
Object *List::copy_to(void *mem) { return new (mem) List{_car, _cdr}; }

bool List::empty() { return is_null(_car) && is_null(_cdr); }
void List::set_car(Object *obj) { _car = obj; }
void List::set_cdr(Object *obj) { _cdr = obj; }
Object *List::car() { return _car; }
Object *List::cdr() { return _cdr; }

bool List::is_instance(Object *obj) { LITHP_CHECK_TYPE(obj, List); }
List *List::cast(Object *obj) {
  if (is_null(obj)) {
    return nullptr;
  }

  if ((obj)->type() != Type::List) {
    throw std::logic_error{"illegal type conversion from " +
                           type_name((obj)->type()) + " to " +
                           type_name(Type::List)};
  }
  return static_cast<List *>(obj);
}

List *List::make(Object *car, Object *cdr) { return HEAP_NEW(List){car, cdr}; }

List *List::of(std::vector<Object *> objects) {
  if (objects.empty()) {
    // TODO: Fix/remove function
    return nullptr;
  }
  List *start = List::make(objects.front(), nil());
  List *current = start;
  for (size_t i = 1; i < objects.size(); i++) {
    List *next = List::make(objects.at(i), nil());
    current->set_cdr(next);
    current = next;
  }

  return start;
}

std::vector<Object *> List::flatten(List *list) {
  std::vector<Object *> objs;
  while (list) {
    objs.push_back(list->car());
    list = List::cast(list->cdr());
  }
  return objs;
}

size_t List::length(List *list) {
  if (is_null(list)) {
    return 0;
  }

  size_t len = 0;
  for (Object *rest = list; !is_null(rest); ++len) {
    if (!List::is_instance(rest)) {
      throw std::runtime_error{"not a list: " + to_string(rest)};
    }
    rest = List::cast(rest)->cdr();
  }
  return len;
}
} // namespace lithp
