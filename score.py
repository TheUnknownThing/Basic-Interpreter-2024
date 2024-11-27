import subprocess
import os
from termcolor import colored

# Step 1: Compile the student's code
compile_command = "g++ -o testcode Basic/Basic.cpp Basic/evalstate.cpp Basic/exp.cpp Basic/parser.cpp Basic/program.cpp Basic/statement.cpp Basic/Utils/error.cpp Basic/Utils/error.hpp Basic/Utils/tokenScanner.cpp Basic/Utils/tokenScanner.hpp Basic/Utils/strlib.cpp"
subprocess.run(compile_command, shell=True, check=True)

# Constants
traceFolder = "Test/"
defaultStudentBasic = "./testcode"
defaultStandardBasic = "./Basic-Demo-64bit"
traceCount = 100

# Initialize counters and lists
total_tests = 0
failed_tests = 0
failed_test_cases = []

for i in range(traceCount):
    trace_file = os.path.join(traceFolder, f"trace{str(i).zfill(2)}.txt")
    
    # Check if the trace file exists
    if not os.path.exists(trace_file):
        print(f"{trace_file} does not exist.")
        continue

    total_tests += 1

    # Run student code
    with open(trace_file, 'r') as input_file:
        student_process = subprocess.run(defaultStudentBasic, stdin=input_file, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
    student_output = student_process.stdout

    # Run standard code
    with open(trace_file, 'r') as input_file:
        standard_process = subprocess.run(defaultStandardBasic, stdin=input_file, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
    standard_output = standard_process.stdout

    # Compare outputs
    if student_output != standard_output:
        failed_tests += 1
        failed_test_cases.append(trace_file)
        print(colored(f"Difference found in {trace_file}:", 'red'))
        print(colored("Input Data:", 'yellow'))
        with open(trace_file, 'r') as input_file:
            print(colored(input_file.read(), 'yellow'))
        print(colored("Student Output:", 'red'))
        print(colored(student_output, 'red'))
        print(colored("Expected Output:", 'green'))
        print(colored(standard_output, 'green'))

# Display final score
print(f"\nTotal Tests: {total_tests}")
print(f"Failed Tests: {failed_tests}")
print(f"Passed Tests: {total_tests - failed_tests}")
print(f"Score: {((total_tests - failed_tests) / total_tests) * 100:.2f}%")

# Display failed test cases
if failed_test_cases:
    print("\nFailed Test Cases:")
    for test_case in failed_test_cases:
        print(test_case)