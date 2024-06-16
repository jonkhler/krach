#pragma once

#include "graph.h"

struct MixData {
	struct Node** children;
	double* weights;
	int32_t num_children;
};

struct MixData* create_mix_data(int32_t num_children, struct Node** children, double* weights);

void free_mix_data(void* data);

double process_mix(void* data, double sample);

struct Node* create_mix_node(struct MixData* data);
