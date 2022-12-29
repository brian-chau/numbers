#include <memory>

#include "Solution.h"

int main(int argc, char **argv) {
    std::unique_ptr<Solution> ptr(std::make_unique<Solution>(argc, argv));
    ptr->Solve();
    return 0;
}
