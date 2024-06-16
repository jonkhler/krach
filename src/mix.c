#include <stdlib.h>

#include "graph.h"
#include "mix.h"

struct mix_data *create_mix_data(int32_t num_children, struct graph_node **children,
                                 double *weights)
{
    struct mix_data *mix = (struct mix_data *)malloc(sizeof(struct mix_data));
    mix->num_children = num_children;
    mix->children = children;
    mix->weights = weights;
    return mix;
}

void free_mix_data(void *data)
{
    struct mix_data *mix = (struct mix_data *)data;
    for (int32_t i = 0; i < mix->num_children; ++i)
    {
        free_node(mix->children[i]);
    }
    free(mix);
}

double process_mix(void *data)
{
    struct mix_data *mix = (struct mix_data *)data;
    double agg = 0.0;
    for (int32_t i = 0; i < mix->num_children; ++i)
    {
        agg += mix->weights[i] * process_node(mix->children[i]);
    }
    return agg;
}

struct graph_node *create_mix_node(struct mix_data *data)
{
    struct graph_node *node = (struct graph_node *)malloc(sizeof(struct graph_node));
    node->data = data;
    node->process = process_mix;
    node->free_data = free_mix_data;
    return node;
}
