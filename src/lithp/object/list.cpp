#include <lithp.hpp>

namespace lithp {
bool List::is_instance(Object *obj) {
  return Nil::is_instance(obj) || ConsCell::is_instance(obj);
}

List *List::cast(Object *obj) {
  if (obj == nullptr) {
    throw std::runtime_error{"attempting to convert null pointer to type List"};
  }
  if (is_instance(obj)) {
    return static_cast<List *>(obj);
  }
  throw std::logic_error{"illegal type conversion from " +
                         type_name((obj)->type()) + " to List"};
}

size_t List::length(List *list) {
  if (Nil::is_instance(list)) {
    return 0;
  }

  size_t len = 0;
  for (Object *rest = list; !Nil::is_instance(rest); ++len) {
    if (!List::is_instance(rest)) {
      throw std::runtime_error{"not a list: " + Object::to_string(rest)};
    }
    rest = List::cast(rest)->tail();
  }
  return len;
}

List *List::of(std::vector<Object *> objects) {
  if (objects.empty()) {
    return Nil::nil();
  }
  ConsCell *start = ConsCell::make(objects.front(), Nil::nil());
  ConsCell *current = start;
  for (size_t i = 1; i < objects.size(); i++) {
    ConsCell *next = ConsCell::make(objects.at(i), Nil::nil());
    current->set_cdr(next);
    current = next;
  }

  return start;
}
} // namespace lithp
