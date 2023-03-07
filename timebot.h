// (c) 2023 Florian Giest
// This code is licensed under MIT license (see LICENSE.txt for details)


#ifndef _TIMEBOT_H
#define _TIMEBOT_H

#include <stdio.h>
#include <string.h>
#include <time.h>

typedef struct timebot {
    const char *description[10];
    const char *name[10];
    void (*function[10])(void*);
    void *arg[10];
    double result[10];
    int n;
    double timesum;
    double max;
    const char *max_function;
    int max_func_name;
} timebot;

/* runs function with a argumenet and returns runtime */
static double _time_check(void (*func)(void*), void *arg)
{
    clock_t start = clock();
    func(arg);
    return (double) (clock() - start) / CLOCKS_PER_SEC;
}

static void tb_append_(timebot *tb, void (*func)(void *), void *arg, const char *name, const char *description)
{
    size_t fname_len = strlen(name);
    tb->name[tb->n] = name;
    if(fname_len > tb->max_func_name) {
        tb->max_func_name = fname_len;
    }
    tb->function[tb->n] = func;
    tb->arg[tb->n] = arg;
    tb->description[tb->n] = description;
    tb->n++;
}

/* append to timebot's execution list */
#define timebot_append(tb, func, arg, desc) tb_append_(tb, func, arg, #func, desc)

/* runs all appended functions */
static void timebot_run(timebot *tb) {
    double tmp;
    for(size_t i = 0; i < tb->n; i++) {
        tb->result[i] = _time_check(tb->function[i], tb->arg[i]);
        if(tb->result[i] > tb->max) {
            tb->max = tb->result[i];
            tb->max_function = tb->name[i];
        }
    }
}

/* compare functions execution time */
static void timebot_compare(timebot *tb)
{
    printf("%*s\tTime\n", -tb->max_func_name, "Function");
    for(size_t i = 0; i < tb->n; i++) {
        printf("%*s\t%f sec (faster than %s by %f%%)\n", -tb->max_func_name, tb->name[i], tb->result[i], tb->max_function,  100 - (tb->result[i] / tb->max * 100));
    }
}

/* prints a summery of all executions */
static void timebot_print_summery(timebot *tb)
{
    printf("%*s | Time\n", -tb->max_func_name, "Function");
    for(size_t i = 0; i < tb->n; i++) {
        printf("%*s | %f sec (%s)\n", -tb->max_func_name, tb->name[i], tb->result[i], tb->description[i]);
    }
}

#endif