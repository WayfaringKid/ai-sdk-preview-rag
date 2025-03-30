# Makefile
# Build rules for EECS 280 project 4

# Compiler
CXX ?= g++

# Compiler flags
CXXFLAGS ?= --std=c++17 -Wall -Werror -pedantic -g -Wno-sign-compare -Wno-comment

# Run a regression test
test: classifier.exe
	# Train-only tests

	./classifier.exe train_small.csv > train_small_train_only.out.txt
	diff -q train_small_train_only.out.txt train_small_train_only.out.correct

	./classifier.exe w16_projects_exam.csv > w16_projects_exam_train_only.out.txt
	diff -q w16_projects_exam_train_only.out.txt w16_projects_exam_train_only.out.correct

	# Predictive tests

	./classifier.exe train_small.csv test_small.csv > test_small.out.txt
	diff -q test_small.out.txt test_small.out.correct

	./classifier.exe w16_projects_exam.csv sp16_projects_exam.csv > projects_exam.out.txt
	diff -q projects_exam.out.txt projects_exam.out.correct

	./classifier.exe w14-f15_instructor_student.csv w16_instructor_student.csv > instructor_student.out.txt
	diff -q instructor_student.out.txt instructor_student.out.correct

classifier.exe: classifier.cpp
	$(CXX) $(CXXFLAGS) classifier.cpp -o $@

# disable built-in rules
.SUFFIXES:

# these targets do not create any files
.PHONY: clean
clean :
	rm -vrf *.o *.exe *.gch *.dSYM *.stackdump *.out.txt

# Run style check tools
CPD ?= /usr/um/pmd-6.0.1/bin/run.sh cpd
OCLINT ?= /usr/um/oclint-22.02/bin/oclint
FILES := classifier.cpp
CPD_FILES := classifier.cpp
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
