#include "commons.h"
#include "audio.h"

double process_filter(struct filter *filt, double val) {
	double out = 0;
	filt->buffer[0] = val;
	for (int i=filt->size-1; i>-1; i--) {
		out = (out + filt->buffer[i]) * filt->coeffs[i];
		filt->buffer[i] = out;
	}
	return out;
	/*
	double out = 0;
	filt->buffer[filt->pos] = val;
	for (int i=0; i<filt->size; i++) {
		int j = filt->pos + i + 1;
		if (j >= filt->size) {
			j -= filt->size;
		}
		out += filt->buffer[j] * filt->coeffs[i];
	}
	if (filt->pos++ >= filt->size) {
		filt->pos -= filt->size;
	}
	return out;
	*/

};

double process_envelope(struct envelope *env) {
	if (env->state == ENV_NONE) {
		return 0.0;
	}
	env->time -= 1. / DEFAULT_RATE;
	if (env->time < 0) {
		switch (env->state) {
			case ENV_ATTACK:
				env->time = env->sustain;
				env->state = ENV_SUSTAIN;
				break;
			case ENV_SUSTAIN:
				env->time = env->decay;
				env->state = ENV_DECAY;
				break;
			case ENV_DECAY:
				env->state = ENV_NONE;
				break;
			default:
				break;				
		}
	}
	switch (env->state) {
		case ENV_ATTACK:
			return (env->attack - env->time) / env->attack;
		case ENV_SUSTAIN:
			return 1.0;
		case ENV_DECAY:
			return env->time / env->decay;
		default:
			return 0.0;
	}
}

void reset_envelope(void *data) {
	struct envelope *env = data;
	env->time = env->attack;
	env->state = ENV_ATTACK;
}




double process_signal_generator(struct signal_generator *gen) {
	switch(gen->type) {
		case SIGNAL_SINE:
			gen->phase += M_PI_M2 / DEFAULT_RATE;
			if (gen->phase >= M_PI_M2) {
				gen->phase -= M_PI_M2;
			}
			return sin(gen->phase * gen->freq);
		case SIGNAL_SQUARE:
			gen->phase += M_PI_M2 / DEFAULT_RATE;
			if (gen->phase >= M_PI_M2) {
				gen->phase -= M_PI_M2;
			}
			if (sin(gen->phase * gen->freq) < 0) {
				return -1;
			} else {
				return 1;
			}
		case SIGNAL_SAW:
			gen->phase += M_PI_M2 * gen->freq / DEFAULT_RATE;
			if (gen->phase >= M_PI_M2) {
				gen->phase -= M_PI_M2;
			}
			return gen->phase / M_PI_M2;
		default:
			return 0.0;
	}
}

void reset_generator(struct signal_generator *gen) {
	gen->phase = -M_PI_M2;
}

bool process_beat(struct beat *beat) {
	beat->time -= 1. / DEFAULT_RATE;
	if (beat-> time < 0) {
		beat->time = 60. / beat->bpm;
		return true;
	}
	return false;
}



double process_sequence(struct sequence* seq) {
	if (process_beat(seq->beat)) {
		reset_envelope(seq->env);
		reset_generator(seq->gen);
		seq->gen->freq = seq->notes[seq->pos]->freq;
		seq->pos++;
		if(seq->pos >= seq->max) {
			seq->pos = 0;
		}
	}
	return process_envelope(seq->env) * process_signal_generator(seq->gen) * seq->notes[seq->pos]->vol;
}
