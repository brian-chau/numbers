#!/bin/bash

# This should match the project name in the CMakeLists.txt file in the line "project(exe_test DESCRIPTION "Hello world" LANGUAGES CXX)"
test_exe_name="exe_test"
rm -rf build bin/$test_exe_name

echo Preparing to test...

# Run every test
mkdir build && cd build && cmake -Dtest=ON                 .. >/dev/null 2>&1 && make >/dev/null 2>&1

echo Testing...
make test > ../TestResults/gtest_results.txt
tests_failed=$(grep "The following tests FAILED:" ../TestResults/gtest_results.txt | wc -l)
if [[ $tests_failed -ge 1 ]]; then
    echo Tests failed!
    cat ../TestResults/gtest_results.txt
    exit 1
fi
echo Tests passed!

# Generate code coverage
echo Generating code coverage...
lcov --capture --directory ./CMakeFiles/$test_exe_name.dir/sources --output-file ../TestResults/main_coverage.info >/dev/null 2>&1
lcov --remove ../TestResults/main_coverage.info -o ../TestResults/main_filtered.info '/usr/include/*' '/usr/include/x86_64-linux-gnu/*' '/usr/lib/*' '/usr/local/*' '/11/*' >/dev/null 2>&1
genhtml ../TestResults/main_filtered.info --output-directory ../TestResults/CodeCoverage > ../TestResults/lcov_results.txt
mv ./Testing/Temporary/LastTest.log ../TestResults/LastTest.log

echo Cleaning up...
# Clean up
rm -rf ../build

echo Done!

