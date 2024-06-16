#pragma once

struct Node {
	const double (*process)(void*, double);
	const void (*free_data)(void*);
	void* data;
};

double process_node(struct Node* node, double value);

void free_node(struct Node* node);
