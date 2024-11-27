#include <iostream>
#include <cstdlib>
#include <unistd.h>

using namespace std;

const string traceFolder = "Test/";
const string defaultStudentBasic = "./testcode";
const string defaultStanderBasic = "./Basic-Demo-64bit";

int traceCount = 10; // 默认评测个数
string studentBasic = "";
string standerBasic = "";
string traceFile = "";
int runTraces = traceCount, currentTrace = 0;
bool silent = false, firstFail = false, hideError = false, useColor = true;

int correct = 0, wrong = 0, total = 0;

void usage(const char *progname) {
    cout
            << progname << " [-h] [-e <your_exec>] [-s <stander_exec>] [-t <trace_file>] [-f] [-m] [-q] [-n <trace_count>]" << endl
            << "    -h  Show this message and quit" << endl
            << "    -e  Specify your executable file, default value: " << defaultStudentBasic << endl
            << "    -s  Specify demo executable file, default value: " << defaultStanderBasic << endl
            << "    -t  Run specified trace file" << endl
            << "    -f  Stop at first failed test" << endl
            << "    -m  Hide error message" << endl
            << "    -q  Show final score only, cannot use with -t or -f, include -m" << endl
            << "    -n  Specify number of traces to run, default value: 10" << endl;
    exit(1);
}

string color(string ce) {
    if (useColor) return ce;
    return "";
}

void parseArguments(int argc, char **argv) {
    int c;
    opterr = 0;
    while ((c = getopt(argc, argv, "e:s:t:fmqhn:")) != -1) {
        switch (c) {
            case 'e':
                if (studentBasic.size()) usage(argv[0]);
                studentBasic = optarg;
                break;
            case 's':
                if (standerBasic.size()) usage(argv[0]);
                standerBasic = optarg;
                break;
            case 't':
                if (traceFile.size()) usage(argv[0]);
                traceFile = optarg;
                break;
            case 'f':
                if (firstFail) usage(argv[0]);
                firstFail = true;
                break;
            case 'm':
                if (hideError) usage(argv[0]);
                hideError = true;
                break;
            case 'q':
                if (silent) usage(argv[0]);
                silent = true;
                break;
            case 'n':
                traceCount = atoi(optarg);
                if (traceCount <= 0) usage(argv[0]);
                break;
            case 'h':
                usage(argv[0]);
                break;
            default:
                usage(argv[0]);
                break;
        }
    }
    if (silent && (traceFile.size() || firstFail)) usage(argv[0]);
    if (silent) hideError = true;
    if (studentBasic.size() == 0) studentBasic = defaultStudentBasic;
    if (standerBasic.size() == 0) standerBasic = defaultStanderBasic;
}

void clearTempFiles() {
    int r = system("rm test_ans test_out -f");
    (void) r;
}

int testTrace(const char *trace) {
    clearTempFiles();
    if (system((string() + "cat " + trace + " | timeout 1 " + standerBasic + " > test_ans 2> /dev/null").c_str()) !=
        0)
        return 1;
    if (system((string() + "cat " + trace + " | timeout 1 " + studentBasic + " > test_out 2> /dev/null").c_str()) !=
        0)
        return 2;
    if (system("diff test_ans test_out > /dev/null 2> /dev/null")) return 4;
    if (system(
            (string() + "cat " + trace + " | timeout 5 valgrind --error-exitcode=2 --leak-check=full " + studentBasic +
             " > /dev/null 2> /dev/null").c_str()) != 0)
        return 3;
    clearTempFiles();
    return 0;
}

void runTest(const string currentTrace) {
    if (!silent) cout << "Trace \"" << currentTrace << "\" ... ";
    cout.flush();
    int error = testTrace(currentTrace.c_str());
    total++;
    if (!error) {
        if (!silent) cout << color("\x1b[32;1m") << "Pass" << color("\x1b[0m") << endl;
        correct++;
    } else {
        wrong++;
        if (!silent) {
            cout << color("\x1b[31;1m") << "Fail" << color("\x1b[0m") << endl;
            if (!hideError) {
                cout << "Trace file: " << endl << color("\x1b[35m");
                cout.flush();
                int r0 = system((string() + "cat " + currentTrace).c_str());
                (void) r0;
                cout << color("\x1b[0m") << endl;
                if (error == 1)
                    cout << color("\x1b[31m") << "Error occurred while running demo program" << color("\x1b[0m")
                         << endl;
                if (error == 2)
                    cout << color("\x1b[31m") << "Error occurred while running your program" << color("\x1b[0m")
                         << endl;
                if (error == 3) cout << color("\x1b[31m") << "Memory leak" << color("\x1b[0m") << endl;
                if (error == 4) {
                    cout << "Demo output: " << endl << color("\x1b[36m");
                    cout.flush();
                    int r1 = system("cat test_ans");
                    (void) r1;
                    cout << color("\x1b[0m") << endl;
                    cout << "Your output: " << endl << color("\x1b[33m");
                    cout.flush();
                    int r2 = system("cat test_out");
                    (void) r2;
                    cout << color("\x1b[0m") << endl;
                }
            }
        }
        clearTempFiles();
        if (firstFail) throw exception();
    }
}

void showScore() {
    int score = correct / 5 * 5;
    if (!silent)
        cout << correct << " / " << total << " trace(s) passed." << endl;
    if (total != traceCount) return;
    cout << "Final Score: " << (score / 10) << "." << (score % 10) << endl;
}

int main(int argc, char **argv) {
    parseArguments(argc, argv);
    try {
        cout << "Compiling code ..." << endl;
        /**************************************************************
         if you modify the structure of the files, you should modify the file paths here.
         **************************************************************/
        system("g++ -std=c++17 -o testcode Basic/Basic.cpp Basic/evalstate.cpp Basic/exp.cpp Basic/parser.cpp Basic/program.cpp Basic/statement.cpp Basic/Utils/error.cpp Basic/Utils/tokenScanner.cpp Basic/Utils/strlib.cpp");
        system("chmod a+rwx Basic-Demo-64bit");
        if (traceFile.size()) runTest(traceFile);
        else {
            for (int i = 0; i < traceCount; i++) {
                string traceName = traceFolder + "trace" + (i < 10 ? "0" : "") + to_string(i) + ".txt";
                runTest(traceName);
            }
        }
    } catch (...) {}
    system("rm testcode -f");
    showScore();
    return 0;
}