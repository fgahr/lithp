#ifndef _LITHP_OBJECT_LIST_H_
#define _LITHP_OBJECT_LIST_H_

#include <object.hpp>

namespace lithp {
class List : public Object {
public:
  virtual Object *head() = 0;
  virtual Object *tail() = 0;
  static bool is_instance(Object *obj);
  static List *cast(Object *obj);
  static size_t length(List *list);
  static List *of(std::vector<Object *> objects);
};
} // namespace lithp

#endif // _LITHP_OBJECT_LIST_H_
