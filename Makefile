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
DOBJ := $(SRC)/object
UTIL := $(SRC)/util
RUNT := $(SRC)/runtime
SLIB := $(SRC)/lib
READ := $(SRC)/reader

TESTSRC := test/src
TESTBIN := test/bin

CXX := clang++
CXXFLAGS := -std=c++17 -Wall -Wextra -O2 -I$(INCLUDE)
TESTFLAGS := -lgtest -lgtest_main

MKOBJ := $(CXX) $(CXXFLAGS) -c
MKEXE := $(CXX) $(CXXFLAGS)
MKTEST := $(MKEXE) $(TESTFLAGS)

VPATH := $(SRC):$(LSRC):$(DOBJ):$(UTIL):$(RUNT):$(SLIB):$(READ)

.PHONY: test clean

all: $(BIN)/lithp

$(BIN)/lithp: $(SRC)/main.cpp $(LIB)/liblithp.a
	$(CXX) $(CXXFLAGS) -o $@ $^

test: symbol_test env_test lib_test reader_test sform_test #token_test

%_test: $(TESTBIN)/%_test
	$<

$(TESTBIN)/symbol_test: $(TESTSRC)/symbol_test.cpp $(LIB)/liblithp.a
	$(MKTEST) -o $@ $^

$(TESTBIN)/env_test: $(TESTSRC)/env_test.cpp $(LIB)/liblithp.a
	$(MKTEST) -o $@ $^

$(TESTBIN)/lib_test: $(TESTSRC)/lib_test.cpp $(LIB)/liblithp.a
	$(MKTEST) -o $@ $^

# $(TESTBIN)/token_test: $(TESTSRC)/token_test.cpp $(OBJ)/tokenizer.o
# 	$(MKTEST) -o $@ $^

$(TESTBIN)/reader_test: $(TESTSRC)/reader_test.cpp $(LIB)/liblithp.a
	$(MKTEST) -o $@ $^

$(TESTBIN)/sform_test: $(TESTSRC)/sform_test.cpp $(LIB)/liblithp.a
	$(MKTEST) -o $@ $^

$(OBJ)/%.o: %.cpp $(INCLUDE)/*.hpp $(INCLUDE)/object/*.hpp $(INCLUDE)/runtime/*.hpp
	$(MKOBJ) -o $@ $<

utlfiles := $(patsubst $(UTIL)/%.cpp,$(OBJ)/%.o,$(wildcard $(UTIL)/*.cpp))
runfiles := $(patsubst $(RUNT)/%.cpp,$(OBJ)/%.o,$(wildcard $(RUNT)/*.cpp))
objfiles := $(patsubst $(DOBJ)/%.cpp,$(OBJ)/%.o,$(wildcard $(DOBJ)/*.cpp))
rdrfiles := $(patsubst $(READ)/%.cpp,$(OBJ)/%.o,$(wildcard $(READ)/*.cpp))
libfiles := $(patsubst $(SLIB)/%.cpp,$(OBJ)/%.o,$(wildcard $(SLIB)/*.cpp))
allobjs := $(utlfiles) $(runfiles) $(objfiles) $(rdrfiles) $(libfiles)
$(LIB)/liblithp.a: $(OBJ)/lithp.o $(allobjs)
	$(AR) cr $@ $^

clean:
	rm -f $(OBJ)/*
	rm -f $(LIB)/*
	rm -f $(BIN)/*
	rm -f $(TESTBIN)/*
# end
