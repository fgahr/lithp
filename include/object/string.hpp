#ifndef _LITHP_OBJECT_STRING_H_
#define _LITHP_OBJECT_STRING_H_

#include <object.hpp>
#include <runtime/heap.hpp>

namespace lithp {
class String : public Object {
  LITHP_HEAP_OBJECT(String);

public:
  static String *make(std::string value);
  virtual Type type(void) override;
  virtual void repr(std::ostream &out) override;
  virtual RefStream refs(void) override;
  virtual Object *copy_to(void *mem) override;
  virtual ~String() = default;
  std::string display();
  static bool is_instance(Object *obj);
  static String *cast(Object *obj);

private:
  String(std::string value);
  std::string value;
};
} // namespace lithp

#endif // _LITHP_OBJECT_STRING_H_
