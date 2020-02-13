#ifndef __LITHP_BUILTIN_H_
#define __LITHP_BUILTIN_H_

#include <object/funcall.hpp>

namespace lithp {

typedef Object *(fnative)(std::vector<Object *> args);

class Builtin : public Function {
public:
  virtual size_t size() override;
  virtual RefStream refs() override;
  virtual void repr(std::ostream &out) override;
  virtual Object *copy_to(void *mem) override;
  virtual Object *call(std::vector<Object *> args) override;
  static void create(std::string name, fnative *fnat);

private:
  Builtin(fnative fnat);
  fnative *native;
};
} // namespace lithp

#endif // __LITHP_BUILTIN_H_
