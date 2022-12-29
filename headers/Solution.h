#ifndef SOLUTION_H
#define SOLUTION_H
#include "Types.h"

class Solution {
public:
    Solution();
    Solution(int argc, char** argv);
    ~Solution();
    void Solve();
    void Leaf();
    void ShowInfix(const char* n);
    void ShowPostfix(const char* n);

private:
    num_t target = 0;
    num_t best_diff = -1;

    void LogDie(const char* s);
    num_t ParseArg(const char* s, num_t limit);

    struct state st;
};

#endif