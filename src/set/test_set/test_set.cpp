#include "test_set.h"

int getThreadsNumber()
{
    return static_cast<int>(std::thread::hardware_concurrency());
}

TestResult::TestResult(int n, int ops_n, SetOperationsDistribution o, std::map<int, int> r)
    :test_number{n},ops_num{ops_n},op{o},results{r}{}

bool operator!= (const TestResult &a, const TestResult &b)
{
    return a.test_number != b.test_number;
}

TestSet::TestSet(int start, int end)
{
    if(1 <= start && start <= getThreadsNumber()) _start = start; else _start = 1;
    if(_start <= end && end <= getThreadsNumber()) _end = end; else _end = getThreadsNumber();
}

void TestSet::run(const std::vector<SetOperationsDistribution> &sod, 
                  const std::vector<int> &ops,
                  const std::string &output_path)
{
    _output_path = output_path;
    std::vector<TestResult> test_results;
    int test_number = 1;
    for(int operations_num : ops)
    {
        for(auto ops_distribution : sod)
        {
            auto results = _runTest(ops_distribution, operations_num);
            test_results.emplace_back(test_number, operations_num, ops_distribution, results);
            ++test_number;
        }
    }
    _writeResults(test_results);
}

std::map<int, int> TestSet::_runTest(SodLinkT p, int operations_num)
{
    std::map<int, int> results;
    LOGLN("operations: " << operations_num);
    for (int i = _start; i <= _end; ++i)
    {
        LOG(p << ": Running test with " << i << " number of threads (");
        int t = _testSet(i, p, operations_num);
        LOG("took: " << t << "ms)\n");
        results[i] = t;
    }
    return results;
}

int TestSet::_testSet(std::size_t threads_number, SodLinkT p, int operations_num) 
{
    std::vector<std::thread> threads;
    auto set = Set();
    std::atomic<int> ops; ops.store(operations_num);
    Timer<std::chrono::high_resolution_clock> t;
    for(int thread_num = 0; thread_num < threads_number; ++thread_num)
    {
        threads.emplace_back(_job, std::ref(set), std::ref(ops), std::ref(p));
    }
    std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));
    return t.elapsed_ms();
}

void TestSet::_job(Set &set, std::atomic<int> &operations_num, SodLinkT op)
{
    Rand rand;
    while(operations_num > 0)
    {
        char o = op.pickRandom();
        uint value = rand.range(0, 1023);
        _perform(o, set, value);
        --operations_num;
    }
}

bool TestSet::_perform(char op, Set &set, uint value)
{
    switch(op)
    {
        case 'a': return set.add(value);
        case 'c': return set.contains(value);
        case 'r': return set.remove(value);
        default: throw std::runtime_error("Unknown operation");
    }
    return false;
}

void TestSet::_writeResults(std::vector<TestResult> test_results)
{
    std::ofstream fout(_output_path, std::ios_base::trunc);
    for(const auto &t : test_results)
    {
        fout << 
        t.test_number << ":\n" <<
        "  operations: " << t.ops_num << '\n' <<
        "  distributions: \n" <<
        "    add: " << t.op.addPercent() << '\n' <<
        "    remove: " << t.op.removePercent() << '\n' <<
        "    contains: " << t.op.containsPercent() << '\n' <<
        "  results:\n";
        for(auto r : t.results)
        {
        fout <<
        "    " << r.first << ": " << r.second << '\n';
        }
    }
    fout.close();
}