#include <stdlib.h>

#include "commons.h"
#include "graph.h"
#include "signal.h"

void free_signal_generator_data(void *data)
{
    struct signal_generator *gen = (struct signal_generator *)data;
    free(gen);
}

double process_sin_generator(struct signal_generator *gen)
{
    gen->phase += M_PI_M2 / DEFAULT_RATE;
    if (gen->phase >= M_PI_M2)
    {
        gen->phase -= M_PI_M2;
    }
    return sin(gen->phase * gen->freq);
}

double process_square_generator(struct signal_generator *gen)
{
    gen->phase += M_PI_M2 / DEFAULT_RATE;
    if (gen->phase >= M_PI_M2)
    {
        gen->phase -= M_PI_M2;
    }
    double result = 1;
    if (sin(gen->phase * gen->freq) < 0)
    {
        result = -1;
    }
    return result;
}

double process_saw_generator(struct signal_generator *gen)
{
    gen->phase += M_PI_M2 * gen->freq / DEFAULT_RATE;
    if (gen->phase >= M_PI_M2)
    {
        gen->phase -= M_PI_M2;
    }
    return gen->phase / M_PI_M2;
}

double process_signal_generator(void *data)
{
    struct signal_generator *gen = (struct signal_generator *)data;
    switch (gen->type)
    {
    case SIGNAL_GENERATOR_SINE:
        return process_sin_generator(gen);
    case SIGNAL_GENERATOR_SQUARE:
        return process_square_generator(gen);
    case SIGNAL_GENERATOR_SAW:
        return process_saw_generator(gen);
    }
}

struct graph_node *create_signal_generator_node(struct signal_generator *data)
{
    struct graph_node *node = (struct graph_node *)malloc(sizeof(struct graph_node));
    node->data = data;
    node->process = process_signal_generator;
    node->free_data = free_signal_generator_data;
    return node;
}
