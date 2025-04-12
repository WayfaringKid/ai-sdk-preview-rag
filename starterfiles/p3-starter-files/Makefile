# Makefile
# Build rules for EECS 280 Euchre Project

# Compiler
CXX ?= g++

# Compiler flags
CXXFLAGS ?= --std=c++17 -Wall -Werror -pedantic -g -Wno-sign-compare -Wno-comment

# Run a regression test
test: Card_public_tests.exe Card_tests.exe Pack_public_tests.exe Pack_tests.exe \
		Player_public_tests.exe Player_tests.exe \
		euchre.exe
	./Card_public_tests.exe
	./Card_tests.exe

	./Pack_public_tests.exe
	./Pack_tests.exe

	./Player_public_tests.exe
	./Player_tests.exe

	./euchre.exe pack.in noshuffle 1 Adi Simple Barbara Simple Chi-Chih Simple Dabbala Simple > euchre_test00.out
	diff -qB euchre_test00.out euchre_test00.out.correct
	./euchre.exe pack.in shuffle 10 Edsger Simple Fran Simple Gabriel Simple Herb Simple > euchre_test01.out
	diff -qB euchre_test01.out euchre_test01.out.correct
	./euchre.exe pack.in noshuffle 3 Ivan Human Judea Human Kunle Human Liskov Human < euchre_test50.in > euchre_test50.out
	diff -qB euchre_test50.out euchre_test50.out.correct


Card_public_tests.exe: Card.cpp Card_public_tests.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

Card_tests.exe: Card.cpp Card_tests.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

Pack_public_tests.exe: Card.cpp Pack.cpp Pack_public_tests.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

Pack_tests.exe: Card.cpp Pack.cpp Pack_tests.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

Player_public_tests.exe: Card.cpp Player.cpp Player_public_tests.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

Player_tests.exe: Card.cpp Player.cpp Player_tests.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

euchre.exe: Card.cpp Pack.cpp Player.cpp euchre.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

.SUFFIXES:

.PHONY: clean

clean:
	rm -rvf *.out *.exe *.dSYM *.stackdump

# Style check
CPD ?= /usr/um/pmd-6.0.1/bin/run.sh cpd
OCLINT ?= /usr/um/oclint-22.02/bin/oclint
FILES := \
  Card.cpp \
  Card_tests.cpp \
  Pack.cpp \
  Pack_tests.cpp \
  Player.cpp \
  Player_tests.cpp \
  euchre.cpp
CPD_FILES := \
  Card.cpp \
  Pack.cpp \
  Player.cpp \
  euchre.cpp
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
