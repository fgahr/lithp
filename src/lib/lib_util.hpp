#ifndef _LITHP_LIB_UTIL_H_
#define _LITHP_LIB_UTIL_H_

#define ARG0 slots.at(0)
#define ARG1 slots.at(1)
#define ARG2 slots.at(2)
#define ARG3 slots.at(3)
#define ARG4 slots.at(4)
#define ARG5 slots.at(5)
#define ARG6 slots.at(6)
#define ARG7 slots.at(7)

#define SYM Symbol::intern
#define NUM Number::make
#define FUN Builtin::make

#define AS_NUM Number::cast

#endif // _LITHP_LIB_UTIL_H_
