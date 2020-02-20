#ifndef _LITHP_LIB_UTIL_H_
#define _LITHP_LIB_UTIL_H_

#define ARG0 args->at(0)
#define ARG1 args->at(1)
#define ARG2 args->at(2)
#define ARG3 args->at(3)
#define ARG4 args->at(4)
#define ARG5 args->at(5)
#define ARG6 args->at(6)
#define ARG7 args->at(7)

#define SYM Symbol::intern
#define NUM Number::make
#define FUN Builtin::make

#define AS_NUM Number::cast

#endif // _LITHP_LIB_UTIL_H_
