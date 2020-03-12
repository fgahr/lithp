#include <lithp.hpp>

namespace lithp {
String *String::make(std::string value) { return HEAP_NEW(String){value}; }
Type String::type() { return Type::String; }
void String::repr(std::ostream &out) { out << '"' << value << '"'; }
RefStream String::refs() { return RefStream::empty(); }
Object *String::copy_to(void *mem) { return new (mem) String{value}; }
std::string String::display() { return value; }

bool String::is_instance(Object *obj) { LITHP_CHECK_TYPE(obj, String); }
String *String::cast(Object *obj) { LITHP_CAST_TO_TYPE(obj, String); }

String::String(std::string value) : value{std::move(value)} {}
} // namespace lithp
