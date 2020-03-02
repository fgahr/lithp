#include <lithp.hpp>

namespace lithp {
String *String::make(std::string value) { return HEAP_NEW(String){value}; }
Type String::type() { return Type::String; }
void String::repr(std::ostream &out) { out << '"' << value << '"'; }
RefStream String::refs() { return RefStream::empty(); }
Object *String::copy_to(void *mem) { return new (mem) String{value}; }

String::String(std::string value) : value{std::move(value)} {}
} // namespace lithp
