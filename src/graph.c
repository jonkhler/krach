#include <stdlib.h>

#include "graph.h"

double process_node(struct graph_node *node) { return node->process(node->data); }

void free_node(struct graph_node *node)
{
    node->free_data(node);
    free(node);
}
