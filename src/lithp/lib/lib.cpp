#include <lib/lib.hpp>
#include <object.hpp>
#include <object/builtin.hpp>
#include <object/cons_cell.hpp>
#include <object/symbol.hpp>

namespace lithp {

void load_stdlib(Environment &env) {
  // TODO
  env.set(Symbol::intern("cons"), nullptr);
}
} // namespace lithp
