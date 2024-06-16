#ifndef GRAPH_H
#define GRAPH_H

struct graph_node
{
    double (*process)(void *);
    void (*free_data)(void *);
    void *data;
} __attribute__((aligned(32)));

double process_node(struct graph_node *node);

void free_node(struct graph_node *node);

#endif
