#include <stdlib.h>

#include "graph.h"

double process_node(struct Node *node, double value) { return node->process(node->data, value); }

void free_node(struct Node *node)
{
    node->free_data(node);
    free(node);
}
