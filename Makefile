##
# Lithp -- The Lisp with a lisp.
#
# @file
# @version 0.1

INCLUDE := ./include
SRC := src
OBJ := obj
LIB := lib
LSRC := $(SRC)/lithp
LOBJ := $(LSRC)/object
LRUN := $(LSRC)/runtime

TESTSRC := test/src
TESTBIN := test/bin

VPATH := $(SRC):$(LSRC):$(LOBJ):$(LRUN)

CXX := clang++
CXXFLAGS := -std=c++17 -Wall -Wextra -O2 -I$(INCLUDE)
TESTFLAGS := -lgtest -lgtest_main

MKOBJ := $(CXX) $(CXXFLAGS) -c
MKEXE := $(CXX) $(CXXFLAGS)
MKTST := $(MKEXE) $(TESTFLAGS)

.PHONY: test

test: symbol_test

symbol_test: $(TESTBIN)/symbol_test
	$<

$(TESTBIN)/symbol_test: $(TESTSRC)/symbol_test.cpp $(LIB)/lithp.a $(LIB)/util.a $(LIB)/runtime.a
	$(MKTST) -o $@ $^

$(OBJ)/refstream.o: $(SRC)/util/refstream.cpp $(INCLUDE)/util/refstream.hpp $(INCLUDE)/util/stream.hpp
	$(MKOBJ) -o $@ $<

$(OBJ)/object.o: $(LOBJ)/object.cpp $(LSRC)/types.cpp $(INCLUDE)/object.hpp $(INCLUDE)/util/refstream.hpp
	$(MKOBJ) -o $@ $<

$(OBJ)/%.o: %.cpp $(OBJ)/object.o $(INCLUDE)/*.hpp $(INCLUDE)/object/*.hpp $(INCLUDE)/runtime/*.hpp
	$(MKOBJ) -o $@ $<

$(LIB)/util.a: $(OBJ)/refstream.o
	$(AR) cr $@ $^

$(LIB)/runtime.a: $(OBJ)/allocator.o $(OBJ)/environment.o
	$(AR) cr $@ $^

lobjfiles := $(patsubst $(LOBJ)/%.cpp,$(OBJ)/%.o,$(wildcard $(LOBJ)/*.cpp))
lsrcfiles := $(patsubst $(LSRC)/%.cpp,$(OBJ)/%.o,$(wildcard $(LSRC)/*.cpp))
$(LIB)/lithp.a: $(lobjfiles) $(lsrcfiles)
	$(AR) cr $@ $^
# end
