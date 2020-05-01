#include <lithp/lib/stdlib.hpp>
#include <lithp/lithp.hpp>

#include "lib_data.hpp"
#include "lib_internal.hpp"
#include "lib_io.hpp"
#include "lib_logic.hpp"
#include "lib_numeric.hpp"

namespace lithp::lib {
void load_stdlib(Environment &env) {
    load_data(env);
    load_io(env);
    load_internal(env);
    load_logic(env);
    load_numeric(env);
}
} // namespace lithp::lib
