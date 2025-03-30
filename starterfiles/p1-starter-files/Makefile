CXX ?= g++
CXXFLAGS ?= -Wall -Werror -pedantic -g --std=c++17 -Wno-sign-compare -Wno-comment

# Run a regression test
test: stats_public_tests.exe stats_tests.exe two_sample.exe
	./stats_public_tests.exe
	./stats_tests.exe
	./two_sample.exe cats.csv food 1 2 weight > cats.out
	diff cats.out cats.out.correct


# Compile the stats_public_test executable
stats_public_tests.exe: stats_public_tests.cpp stats.cpp
	$(CXX) $(CXXFLAGS) stats_public_tests.cpp stats.cpp -o stats_public_tests.exe

# Compile the stats_tests executable
stats_tests.exe: stats_tests.cpp stats.cpp
	$(CXX) $(CXXFLAGS) stats_tests.cpp stats.cpp -o stats_tests.exe

# Compile the two_sample executable
two_sample.exe: two_sample.cpp stats.cpp library.cpp
	$(CXX) $(CXXFLAGS) two_sample.cpp stats.cpp library.cpp -o two_sample.exe

# This target is here for compatibility with the EECS 280 setup tutorial
# but is not otherwise used in the project.
main.exe: main.cpp
	$(CXX) $(CXXFLAGS) main.cpp -o main.exe


# Remove automatically generated files
clean :
	rm -rvf *.exe *~ *.out *.dSYM *.stackdump

# Disable built-in rules
.SUFFIXES:

# Run style check tools
CPD ?= /usr/um/pmd-6.0.1/bin/run.sh cpd
OCLINT ?= /usr/um/oclint-22.02/bin/oclint
FILES := stats.cpp stats_tests.cpp two_sample.cpp
style :
	$(OCLINT) \
    -rule=LongLine \
    -rule=HighNcssMethod \
    -rule=DeepNestedBlock \
    -rule=TooManyParameters \
    -rc=LONG_LINE=90 \
    -rc=NCSS_METHOD=40 \
    -rc=NESTED_BLOCK_DEPTH=4 \
    -rc=TOO_MANY_PARAMETERS=6 \
    -max-priority-1 0 \
    -max-priority-2 0 \
    -max-priority-3 0 \
    $(FILES) \
    -- -xc++ --std=c++17
	$(CPD) \
    --minimum-tokens 100 \
    --language cpp \
    --failOnViolation true \
    --files $(FILES)
	@echo "########################################"
	@echo "EECS 280 style checks PASS"
