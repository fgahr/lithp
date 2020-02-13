#ifndef __LITHP_BUILTIN_H_
#define __LITHP_BUILTIN_H_

#include <unordered_map>

#include <object/funcall.hpp>
#include <object/symbol.hpp>

namespace lithp {

typedef Object *(fnative)(FnArgs, RestArgs);

class Builtin : public Function {
public:
  virtual size_t size() override;
  virtual RefStream refs() override;
  virtual void repr(std::ostream &out) override;
  virtual Object *copy_to(void *mem) override;
  virtual Object *call(FnArgs args, RestArgs rest) override;
  virtual size_t num_args() override;
  virtual bool takes_rest() override;
  static void create(std::string name, size_t nargs, bool rest, fnative *fnat);

private:
  Builtin(size_t nargs, bool rest, fnative fnat);
  size_t nargs;
  bool has_rest;
  fnative *native;
  static std::unordered_map<Symbol *, Builtin *> builtins;
};
} // namespace lithp

#endif // __LITHP_BUILTIN_H_
