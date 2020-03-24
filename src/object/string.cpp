#include <lithp.hpp>

namespace lithp {
String *String::make(std::string value) {
  size_t ssize = value.length() + 1;
  size_t alloc_size = sizeof(String) + ssize;
  char *ptr = (char *)allocator::get(alloc_size);
  String *str = new (ptr) String;
  ptr += sizeof(String);
  char *svalue = ptr;
  for (char c : value) {
    *ptr++ = c;
  }
  *ptr = '\0';
  str->ssize = ssize;
  str->svalue = svalue;
  return str;
}

bool String::heap_allocated() { return true; }
size_t String::size() {
  // FIXME: This will break alignment or cause other issues.
  return sizeof(String) + ssize;
}
Type String::type() { return Type::String; }
void String::repr(std::ostream &out) { out << '"' << svalue << '"'; }
RefStream String::refs() { return RefStream::empty(); }
Object *String::copy_to(void *mem) {
  String *other = new (mem) String;
  other->ssize = ssize;
  char *valptr = (char *)mem;
  valptr += sizeof(String);
  for (size_t i = 0; i < ssize; i++) {
    valptr[i] = svalue[i];
  }
  other->svalue = valptr;
  return other;
}
std::string String::display() { return std::string{svalue}; }

bool String::is_instance(Object *obj) { LITHP_CHECK_TYPE(obj, String); }
String *String::cast(Object *obj) { LITHP_CAST_TO_TYPE(obj, String); }

} // namespace lithp
