#pragma once

#include <math.h>
#include <stdbool.h>
#define M_PI_M2 ( M_PI + M_PI )

#define DEFAULT_RATE            44100
#define DEFAULT_CHANNELS        2
#define DEFAULT_VOLUME          0.2

enum envelope_state {
	ENV_ATTACK,
	ENV_SUSTAIN,
	ENV_DECAY,
	ENV_NONE,
};

struct envelope {
	double time;

	enum envelope_state state;

	double attack;
	double sustain;
	double decay;
};

struct filter {
	double *buffer;
	double *coeffs;
	int pos;
	int size;
};

enum signal_type {
	SIGNAL_SINE,
	SIGNAL_SQUARE,
	SIGNAL_SAW,
};

struct signal_generator {
	enum signal_type type;
	double phase;
	double freq;
};

struct beat {
	double time;
	double bpm;
};

struct note {
	double freq;
	double vol;
};

struct sequence {
	struct beat* beat;
	struct envelope *env;
	struct signal_generator *gen;
	struct note **notes;
	int pos;
	int max;
};

double process_filter(struct filter *filt, double val);

double process_envelope(struct envelope *env);

void reset_envelope(void *data);

double process_signal_generator(struct signal_generator *gen);

void reset_generator(struct signal_generator *gen);

double process_sequence(struct sequence* seq);
