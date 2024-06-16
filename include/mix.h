#ifndef MIX_H
#define MIX_H

#include <stdint.h>

#include "graph.h"

struct mix_data
{
    struct graph_node **children;
    double *weights;
    int32_t num_children;
} __attribute__((packed)) __attribute__((aligned(32)));

struct mix_data *create_mix_data(int32_t num_children, struct graph_node **children,
                                 double *weights);

void free_mix_data(void *data);

double process_mix(void *data);

struct graph_node *create_mix_node(struct mix_data *data);

#endif
