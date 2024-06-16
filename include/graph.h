#ifndef GRAPH_H
#define GRAPH_H

struct Node
{
    double (*process)(void *, double);
    void (*free_data)(void *);
    void *data;
} __attribute__((aligned(32)));

double process_node(struct Node *node, double value);

void free_node(struct Node *node);

#endif
