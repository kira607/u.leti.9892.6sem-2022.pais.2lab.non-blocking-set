#include <vector>
#include "set/test_set/test_set.h"
#include "set/test_set/set_operations_distribution.h"
#include "log.h"

int main()
{
    DEBUGLN("This computer has " << getThreadsNumber() << " threads");
    std::vector<SetOperationsDistribution> distributions{
        SetOperationsDistribution(1,1,1),
        SetOperationsDistribution(90,9,1),
        SetOperationsDistribution(90,1,9),
        SetOperationsDistribution(9,90,1),
        SetOperationsDistribution(1,90,9),
        SetOperationsDistribution(9,1,90),
        SetOperationsDistribution(1,9,90)
    };
    std::vector<int> operations_num{1 << 5, 1 << 7, 1 << 10, 1 << 15, 1 << 17};
    TestSet tester = TestSet();
    tester.run(distributions, operations_num, "../test_data.yaml");
    return 0;
}
