# Makefile
# Build rules for EECS 280 image processing project

# Compiler
CXX ?= g++

# Compiler flags
CXXFLAGS ?= --std=c++17 -Wall -Werror -pedantic -g -Wno-sign-compare -Wno-comment

# Set the following to true to build with JPEG support
USE_LIBJPEG ?=

# If necessary, set the following to the location of your
# libjpeg/libjpeg-turbo installation,
# e.g. LIBJPEG_PATH ?= /opt/homebrew/Cellar/jpeg-turbo/3.0.2
LIBJPEG_PATH ?=

ifeq "$(strip $(USE_LIBJPEG))" "true"
	export LIBJPEG_CXXFLAGS := -DJPEG_HPP_USE_LIBJPEG
	export LIBJPEG_LDFLAGS := -ljpeg
else
	export LIBJPEG_CXXFLAGS :=
	export LIBJPEG_LDFLAGS :=
endif

ifneq "$(LIBJPEG_PATH)" ""
	export LIBJPEG_CXXFLAGS += -L"$(LIBJPEG_PATH)/lib" -I"$(LIBJPEG_PATH)/include"
endif

# Run a regression test
test: Matrix_public_tests.exe Matrix_tests.exe Image_public_tests.exe Image_tests.exe processing_public_tests.exe resize.exe
	./Matrix_public_tests.exe
	./Image_public_tests.exe
	./processing_public_tests.exe
	./resize.exe dog.ppm dog_4x5.out.ppm 4 5
	diff dog_4x5.out.ppm dog_4x5.correct.ppm

Matrix_public_tests.exe: Matrix_public_tests.cpp Matrix.cpp Matrix_test_helpers.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

Matrix_tests.exe: Matrix_tests.cpp Matrix.cpp Matrix_test_helpers.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

Image_public_tests.exe: Image_public_tests.cpp Matrix.cpp Image.cpp \
			Matrix_test_helpers.cpp Image_test_helpers.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

Image_tests.exe: Image_tests.cpp Matrix.cpp Image.cpp Matrix_test_helpers.cpp \
			Image_test_helpers.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

processing_public_tests.exe: processing_public_tests.cpp Matrix.cpp \
				Image.cpp processing.cpp \
				Matrix_test_helpers.cpp Image_test_helpers.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

resize.exe: resize.cpp Matrix.cpp Image.cpp processing.cpp
	$(CXX) $(CXXFLAGS) $(LIBJPEG_CXXFLAGS) $^ $(LIBJPEG_LDFLAGS) -o $@

# Disable built-in Makefile rules
.SUFFIXES:

clean:
	rm -rvf *.exe *.out.txt *.out.ppm *.dSYM *.stackdump

# Run style check tools
CPD ?= /usr/um/pmd-6.0.1/bin/run.sh cpd
OCLINT ?= /usr/um/oclint-22.02/bin/oclint
FILES := \
  Image.cpp \
  Image_tests.cpp \
  Matrix.cpp \
  Matrix_tests.cpp \
  processing.cpp \
  resize.cpp
CPD_FILES := \
  Image.cpp \
  Matrix.cpp \
  processing.cpp \
  resize.cpp
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
