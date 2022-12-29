#include "Solution.h"

#include <fmt/format.h>
#include <stdio.h>
#include <stdlib.h>

#include <cstdio>
#include <cstdlib>
#include <stack>
#include <string>

Solution::Solution() {
}

Solution::Solution(int argc, char **argv) {
    int i;

    target = 10;
    --argc;
    ++argv;

    int max_nums = argc;
    int max_ops = 2 * max_nums - 1;
    int max_stack = max_nums;
    if (!(st.nums = (long *)malloc(sizeof(num_t) * max_nums))) {
        LogDie("out of memory");
    }
    if (!(st.ops = (struct op *)malloc(sizeof(struct op) * max_ops))) {
        LogDie("out of memory");
    }
    if (!(st.stack = (long *)malloc(sizeof(num_t) * max_stack))) {
        LogDie("out of memory");
    }
    for (i = 0; i < argc; ++i) {
        st.nums[i] = ParseArg(argv[i], MAX_NUMBER);
    }
    st.num_nums = argc;
    st.num_ops = 0;
    st.num_stack = 0;
    Solve();
    fflush(stdout);
}

Solution::~Solution() {
}

void Solution::LogDie(const char *s) {
    fprintf(stderr, "%s\n", s);
    fflush(stderr);
    exit(1);
}

num_t Solution::ParseArg(const char *s, num_t limit) {
    num_t ans = 0;
    if (!*s) {
        LogDie("Failed to parse arguments");
    }
    while (*s) {
        if (*s < '0' || *s > '9') {
            LogDie("Failed to parse arguments");
        }
        ans = 10 * ans + (*s - '0');
        if (ans > limit) {
            LogDie("Argument too big");
        }
        ++s;
    }

    return ans;
}

void Solution::ShowInfix(const char *n) {
    for (int i = 0; i < st.num_ops; ++i) {
        if (st.ops[i].is_number) {
            printf(" " num_format, st.ops[i].operand.n);
        } else {
            printf(" %c", st.ops[i].operand.o);
        }
    }
    printf("\n");
}

void Solution::ShowPostfix(const char *n) {
    std::stack<std::string> nums;

    for (int i = 0; i < st.num_ops; ++i) {
        if (st.ops[i].is_number) {
            nums.push(std::to_string(st.ops[i].operand.n));
        } else {
            std::string a(nums.top());
            nums.pop();
            std::string b(nums.top());
            nums.pop();

            char buf[1024];
            memset(&buf, 0, 1024);
            sprintf(buf, "(%s %c %s)", a.c_str(), st.ops[i].operand.o, b.c_str());
            nums.push(std::string(buf));
        }
    }

    fmt::print("{} = {}\n", nums.top(), target);
}

void Solution::Leaf() {
    num_t diff = abs(st.stack[0] - target);
    if (best_diff < 0 || diff <= best_diff) {
        if (st.num_nums == 0 && st.stack[0] == target) {
            ShowPostfix("leaf");
            best_diff = diff;
        }
    }
}

void Solution::Solve() {
    if (st.num_stack == 1) {
        Leaf();
    } else if (st.num_stack >= 2) {
        num_t x = st.stack[st.num_stack - 1];
        num_t y = st.stack[st.num_stack - 2];

        st.num_ops++;
        st.num_stack--;
        st.ops[st.num_ops - 1].is_number = 0;

        if (x >= y) {  // shortcut
            st.ops[st.num_ops - 1].operand.o = '+';
            st.stack[st.num_stack - 1] = x + y;
            Solve();
        }

        if (x > y) {  // avoid making 0
            st.ops[st.num_ops - 1].operand.o = '-';
            st.stack[st.num_stack - 1] = x - y;
            Solve();
        }

        if (x >= y && x > 1 && y > 1) {  // shortcut, and avoid *1
            st.ops[st.num_ops - 1].operand.o = '*';
            st.stack[st.num_stack - 1] = x * y;
            Solve();
        }

        if (y > 1 && (x % y) == 0) {  // avoid /1
            st.ops[st.num_ops - 1].operand.o = '/';
            st.stack[st.num_stack - 1] = x / y;
            Solve();
        }

        st.num_ops--;
        st.num_stack++;

        st.stack[st.num_stack - 1] = x;
        st.stack[st.num_stack - 2] = y;
    }

    if (st.num_nums) {
        int i, j;

        // push a number
        st.num_nums--;
        num_t top_num = st.nums[st.num_nums];
        st.num_ops++;
        st.ops[st.num_ops - 1].is_number = 1;
        st.num_stack++;

        for (i = 0; i <= st.num_nums; ++i) {
            int doneit = 0;
            for (j = 0; j < i; ++j) {
                if (st.nums[i] == st.nums[j]) {
                    doneit = 1;
                    break;
                }
            }
            if (doneit) {
                continue;
            }

            num_t thisnum = st.nums[i];

            st.nums[i] = top_num;
            st.ops[st.num_ops - 1].operand.n = thisnum;
            st.stack[st.num_stack - 1] = thisnum;

            Solve();

            st.nums[st.num_nums] = top_num;
            st.nums[i] = thisnum;
        }

        st.num_nums++;
        st.num_ops--;
        st.num_stack--;
    }
}