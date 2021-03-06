#include <lithp/object/types.hpp>

#include <stdexcept>

namespace lithp {
#define TYPE_NAME_CASE(t)                                                      \
    case Type::t:                                                              \
        return #t

std::string type_name(Type t) {
    switch (t) {
        TYPE_NAME_CASE(Boolean);
        TYPE_NAME_CASE(BrokenHeart);
        TYPE_NAME_CASE(List);
        TYPE_NAME_CASE(Function);
        TYPE_NAME_CASE(Nil);
        TYPE_NAME_CASE(Number);
        TYPE_NAME_CASE(SpecialForm);
        TYPE_NAME_CASE(String);
        TYPE_NAME_CASE(Symbol);
        TYPE_NAME_CASE(Environment);
    default:
        throw std::logic_error{"unknown type " +
                               std::to_string(static_cast<int>(t))};
    }
}

#undef TYPE_NAME_CASE
} // namespace lithp
