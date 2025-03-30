# Makefile
# Build rules for EECS 280 List/Editor Project

# Compiler
CXX ?= g++

# Compiler flags
CXXFLAGS ?= --std=c++17 -Wall -Werror -pedantic -g -Wno-sign-compare -Wno-comment

# Run regression tests
test: test-list test-text-buffer

test-list: List_compile_check.exe List_public_tests.exe List_tests.exe
	./List_public_tests.exe
	./List_tests.exe

test-text-buffer: TextBuffer_public_tests.exe TextBuffer_tests.exe line.exe
	./TextBuffer_public_tests.exe
	./TextBuffer_tests.exe

	./line.exe < line_test1.in > line_test1.out
	diff -qB line_test1.out line_test1.out.correct

	./line.exe < line_test2.in > line_test2.out
	diff -qB line_test2.out line_test2.out.correct

List_tests.exe: List_tests.cpp List.hpp
	$(CXX) $(CXXFLAGS) List_tests.cpp -o $@

List_compile_check.exe: List_compile_check.cpp List.hpp
	$(CXX) $(CXXFLAGS) List_compile_check.cpp -o $@

List_public_tests.exe: List_public_tests.cpp List.hpp
	$(CXX) $(CXXFLAGS) List_public_tests.cpp -o $@

TextBuffer_public_tests.exe: TextBuffer.cpp TextBuffer_public_tests.cpp TextBuffer.hpp List.hpp
	$(CXX) $(CXXFLAGS) TextBuffer.cpp TextBuffer_public_tests.cpp -o $@

TextBuffer_tests.exe: TextBuffer.cpp TextBuffer_tests.cpp TextBuffer.hpp List.hpp
	$(CXX) $(CXXFLAGS) TextBuffer.cpp TextBuffer_tests.cpp -o $@

line.exe: line.cpp TextBuffer.cpp TextBuffer.hpp List.hpp
	$(CXX) $(CXXFLAGS) line.cpp TextBuffer.cpp -o $@

e0.exe: e0.cpp TextBuffer.cpp TextBuffer.hpp List.hpp
	$(CXX) $(CXXFLAGS) e0.cpp TextBuffer.cpp -o $@ -lcurses

femto.exe: femto.cpp TextBuffer.cpp TextBuffer.hpp List.hpp
	$(CXX) $(CXXFLAGS) femto.cpp TextBuffer.cpp -o $@ -lcurses

# disable built-in rules
.SUFFIXES:

# these targets do not create any files
.PHONY: clean
clean:
	rm -vrf *.o *.exe *.gch *.dSYM *.stackdump *.out

# Run style check tools
CPD ?= /usr/um/pmd-6.0.1/bin/run.sh cpd
OCLINT ?= /usr/um/oclint-22.02/bin/oclint
FILES := List.hpp TextBuffer.cpp
CPD_FILES := List.hpp TextBuffer.cpp
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
