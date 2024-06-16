#ifndef MIX_H
#define MIX_H

#include <stdint.h>

#include "graph.h"

struct MixData
{
    struct Node **children;
    double *weights;
    int32_t num_children;
} __attribute__((packed)) __attribute__((aligned(32)));

struct MixData *create_mix_data(int32_t num_children, struct Node **children, double *weights);

void free_mix_data(void *data);

double process_mix(void *data, double sample);

struct Node *create_mix_node(struct MixData *data);

#endif
