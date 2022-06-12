#ifndef INC_TEST_SET_H
#define INC_TEST_SET_H

#include <vector>
#include <map>
#include <fstream>
#include <thread>
#include <atomic>
#include <algorithm>
#include <functional>

#include "set_operations_distribution.h"
#include "../set.h"
#include "../../log.h"
#include "../../timer.h"
#include "../../rand/rand.h"


int getThreadsNumber();

struct TestResult
{
    TestResult(int n, int ops_n, SetOperationsDistribution o, std::map<int, int> r);
    friend bool operator!= (const TestResult &a, const TestResult &b);

    int test_number;
    int ops_num;
    SetOperationsDistribution op;
    std::map<int, int> results;
};

class TestSet
{
public:
    TestSet(int start = 0, int end = 0);
    void run(const std::vector<SetOperationsDistribution> &sod, 
             const std::vector<int> &ops,
             const std::string &output_path);

private:
    int _start, _end;
    std::string _output_path;

    std::map<int, int> _runTest(SodLinkT p, int operations_num);
    int _testSet(std::size_t threads_number, SodLinkT p, int operations_num);
    static void _job(Set &set, std::atomic<int> &operations_num, SodLinkT op);
    static bool _perform(char op, Set &set, uint value);
    void _writeResults(std::vector<TestResult> test_results);
};


#endif //INC_TEST_SET_H