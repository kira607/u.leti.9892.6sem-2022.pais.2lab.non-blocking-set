#include <thread>

#define DBG

#include "set.h"
#include "timer.h"
#include "log.h"
#include <atomic>
#include <random>
#include <map>
#include <algorithm>
#include <functional>
#include <sstream>

typedef unsigned int uint;

std::random_device rd;
std::mt19937 rnd(rd());

uint rand(uint start = 0, uint end = 9)
{
    return start + std::abs(static_cast<int>(rnd())) % (end + 1);
}

int getThreadsNumber()
{
    return static_cast<int>(std::thread::hardware_concurrency());
}

struct SetOperations
{
    SetOperations(uint a = 1, uint r = 1, uint c = 1)
    :add{a},remove{r},contains{c}{}

    uint total() const
    {
        return add + remove + contains;
    }

    char pickRandom() const
    {
        double y = (static_cast<double>(total()) / 100.0) * static_cast<double>(rand(0, 100));
        uint a = add;
        uint r = add + remove;
        uint c = r + contains;
        char op;
        if(0 <= y && y < a) {op = 'a';}
        else if(a <= y && y < r) {op = 'r';}
        else {op = 'c';}
        //DEBUGLN(y << " >> " << 0 << "-a-" << a << "-r-" << r << "-c-" << c << " --> " << op);
        return op;
    }

    bool perform(char op, Set &set, uint value) const
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

    float addPercent() const
    {
        return roundf((static_cast<float>(add) / total()) * 100.0 * 100.0) / 100;;
    }

    float removePercent() const
    {
        return roundf((static_cast<float>(remove) / total()) * 100.0 * 100.0) / 100;;
    }

    float containsPercent() const
    {
        return roundf((static_cast<float>(contains) / total()) * 100.0 * 100.0) / 100;;
    }

    std::string percentStr() const
    {
        float a = addPercent();
        float r = static_cast<float>(r) / total();
        float c = static_cast<float>(c) / total();
        std::stringstream ss;
        ss << "(add: " << addPercent() << "%,"
           << "remove: " << removePercent() << "%,"
           << "contains: " << containsPercent() << "%)";
        return ss.str();
    }

    uint add;
    uint remove;
    uint contains;
};

typedef const SetOperations& OpLinkT;

void writeResults(const std::map<int, int> &results)
{

}

class TestSet
{
public:
    TestSet(int start = 0, int end = 0)
    {
        if(1 <= start && start <= getThreadsNumber()) _start = start; else _start = 1;
        if(_start <= end && end <= getThreadsNumber()) _end = end; else _end = getThreadsNumber();
    }

    std::map<int, int> run(OpLinkT p)
    {
        std::map<int, int> results;
        for (int i = _start; i <= _end; ++i)
        {
            DEBUG(p.percentStr() << "; Running test with " << i << " number of threads (");
            int t = _testSet(i, p);
            DEBUG("took: " << t << "ms)\n");
            results[i] = t;
        }
        return results;
    }

private:
    int _start, _end;

    static void _job(Set &set, std::atomic<int> &operations_num, OpLinkT op)
    {
        while(operations_num > 0)
        {
            char o = op.pickRandom();
            uint value = rand(0, 1023);
            op.perform(o, set, value);
            --operations_num;
        }
    }

    int _testSet(std::size_t threads_number, OpLinkT p) 
    {
        std::vector<std::thread> threads;
        auto set = Set();
        std::atomic<int> ops; ops.store(1 << 20);
        Timer<std::chrono::high_resolution_clock> t;
        for(int thread_num = 0; thread_num < threads_number; ++thread_num)
        {
            threads.emplace_back(_job, std::ref(set), std::ref(ops), std::ref(p));
        }
        std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));
        return t.elapsed_ms();
    }
};

void testSet()
{
    TestSet tester = TestSet();
    std::vector<SetOperations> distributions{
        SetOperations(1,1,1),
        SetOperations(90,9,1),
        SetOperations(90,1,9),
        SetOperations(9,90,1),
        SetOperations(1,90,9),
        SetOperations(9,1,90),
        SetOperations(1,9,90)
    };
    for(auto d : distributions)
    {
        auto results = tester.run(d);
        writeResults(results);
    }
}

void testPickMethod(OpLinkT op)
{
    uint a{}, r{}, c{};
    for(int i = 0; i < 1 << 15; ++i){
        char m = op.pickRandom();
        switch(m)
        {
            case 'a': ++a; break;
            case 'r': ++r; break;
            case 'c': ++c; break;
        }
    }
    std::cout << '\n' <<
    "a: " << a << '\n' <<
    "r: " << r << '\n' <<
    "c: " << c << '\n';
}

int main()
{
    DEBUGLN("This computer has " << getThreadsNumber() << " threads");
    testSet();
    return 0;
}
