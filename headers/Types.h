#ifndef TYPES_H
#define TYPES_H
#define MAX_TARGET 1000000
#define MAX_NUMBER 1000
#define num_t long
#define num_format "%ld"

union operand {
    num_t n;
    char o;
};

struct op {
    int is_number;
    union operand operand;
};

struct state {
    int num_nums;
    num_t* nums;
    int num_ops;
    struct op* ops;
    int num_stack;
    num_t* stack;
};

#endif