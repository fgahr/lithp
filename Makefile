##
# Lithp -- The Lisp with a lisp.
#

INCLUDE := ./include
INCLITHP := $(INCLUDE)/lithp
BIN := bin
SRC := src
OBJ := obj
LIB := lib

TESTSRC := test/src
TESTBIN := test/bin

CXX := clang++
CXXFLAGS := -std=c++17 -Wall -Wextra -O2 -I$(INCLUDE)
TESTFLAGS := -lgtest -lgtest_main

MKOBJ := $(CXX) $(CXXFLAGS) -c
MKEXE := $(CXX) $(CXXFLAGS)
MKTEST := $(MKEXE) $(TESTFLAGS)

VPATH := $(SRC)

.PHONY: test clean $(OBJ) $(LIB)

# Keep test executables. Otherwise they are regarded as intermediate and deleted
.SECONDARY:
	$(TBIN)/*


all: $(BIN)/lithp

$(BIN)/lithp: $(SRC)/main.cpp $(LIB)/liblithp.a | $(BIN)
	$(CXX) $(CXXFLAGS) -o $@ $^

test: $(TESTBIN) symbol_test env_test lib_test reader_test sform_test #token_test

%_test: $(TESTBIN)/%_test | $(TESTBIN)
	$<

$(TESTBIN)/%_test: $(TESTSRC)/%_test.cpp $(LIB)/liblithp.a | $(TESTBIN)
	$(MKTEST) -o $@ $^

$(OBJ)/%.o: %.cpp $(INCLITHP)/*.hpp $(INCLITHP)/object/*.hpp $(INCLITHP)/runtime/*.hpp | $(OBJ)
	$(MKOBJ) -o $@ $<

allobjs := $(patsubst $(SRC)/%.cpp,$(OBJ)/%.o,$(wildcard $(SRC)/*.cpp))
$(LIB)/liblithp.a: $(OBJ)/lithp.o $(allobjs) | $(LIB)
	$(AR) cr $@ $^

$(OBJ) $(LIB) $(BIN) $(TESTBIN):
	mkdir -p $@

clean:
	rm -rf $(OBJ)
	rm -rf $(LIB)
	rm -rf $(BIN)
	rm -rf $(TESTBIN)

#end Lithp
