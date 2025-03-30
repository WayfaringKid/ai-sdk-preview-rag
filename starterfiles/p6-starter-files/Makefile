# Makefile
# Build rules for EECS 280 BST/Map Project

# Compiler
CXX ?= g++

# Compiler flags
CXXFLAGS ?= --std=c++17 -Wall -Werror -pedantic -g -Wno-sign-compare -Wno-comment

# Run a regression test
test: BinarySearchTree_compile_check.exe \
		BinarySearchTree_tests.exe \
		BinarySearchTree_public_tests.exe \
		Map_compile_check.exe \
		Map_tests.exe \
		Map_public_tests.exe

	./BinarySearchTree_tests.exe
	./BinarySearchTree_public_tests.exe

	./Map_tests.exe
	./Map_public_tests.exe

BinarySearchTree_public_tests.exe: BinarySearchTree_public_tests.cpp BinarySearchTree.hpp
	$(CXX) $(CXXFLAGS) $< -o $@

BinarySearchTree_compile_check.exe: BinarySearchTree_compile_check.cpp BinarySearchTree.hpp
	$(CXX) $(CXXFLAGS) $< -o $@

BinarySearchTree_tests.exe: BinarySearchTree_tests.cpp BinarySearchTree.hpp
	$(CXX) $(CXXFLAGS) $< -o $@

Map_public_tests.exe: Map_public_tests.cpp Map.hpp BinarySearchTree.hpp
	$(CXX) $(CXXFLAGS) $< -o $@

Map_compile_check.exe: Map_compile_check.cpp Map.hpp BinarySearchTree.hpp
	$(CXX) $(CXXFLAGS) $< -o $@

Map_tests.exe: Map_tests.cpp Map.hpp BinarySearchTree.hpp
	$(CXX) $(CXXFLAGS) $< -o $@

# disable built-in rules
.SUFFIXES:

# these targets do not create any files
.PHONY: clean
clean :
	rm -vrf *.o *.exe *.gch *.dSYM *.stackdump *.out.txt

# Run style check tools
CPD ?= /usr/um/pmd-6.0.1/bin/run.sh cpd
OCLINT ?= /usr/um/oclint-22.02/bin/oclint
FILES := BinarySearchTree.hpp BinarySearchTree_tests.cpp Map.hpp
CPD_FILES := BinarySearchTree.hpp Map.hpp
style :
	$(OCLINT) \
    -rule=LongLine \
    -rule=HighNcssMethod \
    -rule=DeepNestedBlock \
    -rule=TooManyParameters \
    -rc=LONG_LINE=90 \
    -rc=NCSS_METHOD=40 \
    -rc=NESTED_BLOCK_DEPTH=4 \
    -rc=TOO_MANY_PARAMETERS=4 \
    -max-priority-1 0 \
    -max-priority-2 0 \
    -max-priority-3 0 \
    $(FILES) \
    -- -xc++ --std=c++17
	$(CPD) \
    --minimum-tokens 100 \
    --language cpp \
    --failOnViolation true \
    --files $(CPD_FILES)
	@echo "########################################"
	@echo "EECS 280 style checks PASS"
