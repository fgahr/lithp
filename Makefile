##
# Lithp -- The Lisp with a lisp.
#
# @file
# @version 0.1

INCLUDE := ./include
BIN := bin
SRC := src
OBJ := obj
LIB := lib
LSRC := $(SRC)/lithp
LOBJ := $(LSRC)/object
LUTL := $(LSRC)/util
LRUN := $(LSRC)/runtime
LLIB := $(LSRC)/lib

TESTSRC := test/src
TESTBIN := test/bin

CXX := clang++
CXXFLAGS := -std=c++17 -Wall -Wextra -O2 -I$(INCLUDE)
TESTFLAGS := -lgtest -lgtest_main

MKOBJ := $(CXX) $(CXXFLAGS) -c
MKEXE := $(CXX) $(CXXFLAGS)
MKTST := $(MKEXE) $(TESTFLAGS)

VPATH := $(SRC):$(LSRC):$(LOBJ):$(LRUN):$(LLIB)

.PHONY: test clean

test: symbol_test lib_test

symbol_test: $(TESTBIN)/symbol_test
	$<

lib_test: $(TESTBIN)/lib_test
	$<

$(TESTBIN)/symbol_test: $(TESTSRC)/symbol_test.cpp $(LIB)/liblithp.a
	$(MKTST) -o $@ $^

$(TESTBIN)/lib_test: $(TESTSRC)/lib_test.cpp $(LIB)/liblithp.a $(LIB)/liblib.a
	$(MKTST) -o $@ $^

$(OBJ)/refstream.o: $(LUTL)/refstream.cpp $(INCLUDE)/util/refstream.hpp $(INCLUDE)/util/stream.hpp
	$(MKOBJ) -o $@ $<

$(OBJ)/%.o: %.cpp $(INCLUDE)/*.hpp $(INCLUDE)/object/*.hpp $(INCLUDE)/runtime/*.hpp
	$(MKOBJ) -o $@ $<

lutlfiles := $(patsubst $(LUTL)/%.cpp,$(OBJ)/%.o,$(wildcard $(LUTL)/*.cpp))
lrunfiles := $(patsubst $(LRUN)/%.cpp,$(OBJ)/%.o,$(wildcard $(LRUN)/*.cpp))
lobjfiles := $(patsubst $(LOBJ)/%.cpp,$(OBJ)/%.o,$(wildcard $(LOBJ)/*.cpp))
$(LIB)/liblithp.a: $(lutlfiles) $(lrunfiles) $(lobjfiles)
	$(AR) cr $@ $^

llibfiles := $(patsubst $(LLIB)/%.cpp,$(OBJ)/%.o,$(wildcard $(LLIB)/*.cpp))
$(LIB)/liblib.a: $(llibfiles) $(OBJ)/builtin.o $(OBJ)/function.o
	$(AR) cr $@ $^

clean:
	rm -f $(OBJ)/*
	rm -f $(LIB)/*
	rm -f $(BIN)/*
	rm -f $(TESTBIN)/*
# end
