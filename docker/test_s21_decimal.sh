#!/bin/bash

echo -e "-------------------------------------------------------------------------------\n"
echo -e "Style test\n"
cd ../src
# We are interested only in STDERR. STDOUT is redirected to NULL.
OUTPUT=$(make lint 2>&1 > /dev/null)
printf 'Style test output:\n %s\n' "$OUTPUT"

if [ -z "$OUTPUT" ]; then
    STYLE_TEST_RESULT=0
else
    STYLE_TEST_RESULT=-1
fi;
printf 'Style test result: %s\n' "$STYLE_TEST_RESULT"
echo -e "-------------------------------------------------------------------------------\n"

echo -e "-------------------------------------------------------------------------------\n"
echo -e "Build test\n"
make fclean 2>&1 > /dev/null
# We are interested only in STDERR. STDOUT is redirected to NULL.
OUTPUT=$(make all 2>&1 > /dev/null)
printf 'Build test output:\n %s\n' "$OUTPUT"

if [ -z "$OUTPUT" ]; then
    BUILD_TEST_OUTPUT=0
else
    BUILD_TEST_OUTPUT=-1
fi;
printf 'Build test result: %s\n' "$BUILD_TEST_OUTPUT"
echo -e "-------------------------------------------------------------------------------\n"

# RUN "src/make 2>build_report_errors.txt > build_report_stdout.txt"

# RUN "src/make > build_report.txt"

# RUN "src/make 2>build_report_errors.txt > build_report_stdout.txt"

# launch test executable
# chmod +x ../src/test
# ../src/test

# if ../src/test; then
#     
#     echo command returned true
# else
#     echo command returned some error
# fi

# command
# status=$?
# ## run date command ##
# cmd="date"
# $cmd
# ## get status ##
# status=$?
# ## take some decision ## 
# [ $status -eq 0 ] && echo "$cmd command was successful" || echo "$cmd failed"

# set -e
# BUILD_SCRIPT=docker/build.sh
# OUTPUT=$(bash ${BUILD_SCRIPT} part1)
# printf 'Build output:\n %s\n' "$OUTPUT"
# BUILD_RESULT=${OUTPUT: -1}
# printf 'Build result: %s\n' "$BUILD_RESULT"
# echo -e "-------------------------------------------------------------------------------\n"
# echo -e "\n"
# FUNCTIONAL_SCRIPT=docker/functional.sh
# OUTPUT=$(bash ${BUILD_SCRIPT} part1)
# printf 'Build output:\n %s\n' "$OUTPUT"
# BUILD_RESULT=${OUTPUT: -1}
# printf 'Build result: %s\n' "$BUILD_RESULT"
# echo -e "-------------------------------------------------------------------------------\n"
# echo -e "\n"
