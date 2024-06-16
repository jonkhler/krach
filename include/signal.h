enum signal_type
{
    SIGNAL_GENERATOR_SINE,
    SIGNAL_GENERATOR_SQUARE,
    SIGNAL_GENERATOR_SAW,
};

struct signal_generator
{
    enum signal_type type;
    double phase;
    double freq;
};

void free_signal_generator_data(void *data);

double process_sin_generator(struct signal_generator *gen);

double process_square_generator(struct signal_generator *gen);

double process_saw_generator(struct signal_generator *gen);

double process_signal_generator(void *data);

struct graph_node *create_signal_generator_node(struct signal_generator *data);
