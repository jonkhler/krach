#include <stdlib.h>

#include "graph.h"
#include "mix.h"


struct MixData* create_mix_data(int32_t num_children, struct Node** children, double* weights) {
	struct MixData* mix = (struct MixData*) malloc(sizeof(struct MixData));
	mix->num_children = num_children;
	mix->children = children;
	mix->weights = weights;
	return mix;
}


void free_mix_data(void* data) {
	struct MixData* mix = (struct MixData*) data;
	for (int32_t i=0; i < mix->num_children; ++i) {
		free_node(mix->children[i]);
	}
	free(mix);
}


double process_mix(void* data, double sample) {
	struct MixData* mix = (struct MixData*) data;
	double agg = 0.0;
	for (int32_t i=0; i < mix->num_children; ++i) {
		agg += mix->weights[i] * process_node(mix->children[i], sample);
	}
	return agg;
}


struct Node* create_mix_node(struct MixData* data) {
	struct Node* node = (struct Node*) malloc(sizeof(struct Node));
	node->data = data;
	node->process = process_mix;
	node->free_data = free_mix_data;
	return node;
}
