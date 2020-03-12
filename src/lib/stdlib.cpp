#include <lib/stdlib.hpp>
#include <lithp.hpp>

#include "lib_data.hpp"
#include "lib_logic.hpp"
#include "lib_numeric.hpp"

namespace lithp::lib {
void load_stdlib(Environment &env) {
  load_data(env);
  load_numeric(env);
  load_logic(env);
}
} // namespace lithp::lib
