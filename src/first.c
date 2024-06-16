#include <math.h>

#include <spa/param/audio/format-utils.h>

#include <pipewire/pipewire.h>

#include "audio.h"

#define M_PI_M2 ( M_PI + M_PI )

#define DEFAULT_RATE            44100
#define DEFAULT_CHANNELS        2
#define DEFAULT_VOLUME          0.2

struct data {
        struct pw_main_loop *loop;
        struct pw_stream *stream;

		struct sequence** seqs;
		int num_seqs;

		struct filter* filt;
};



/* [on_process] */
static void on_process(void *userdata)
{
        struct data *data = userdata;
        struct pw_buffer *b;
        struct spa_buffer *buf;
        int i, c, n_frames, stride;
        int16_t *dst;

        if ((b = pw_stream_dequeue_buffer(data->stream)) == NULL) {
                pw_log_warn("out of buffers: %m");
                return;
        }

        buf = b->buffer;
        if ((dst = buf->datas[0].data) == NULL)
                return;

        stride = sizeof(int16_t) * DEFAULT_CHANNELS;
        n_frames = buf->datas[0].maxsize / stride;
        if (b->requested)
                n_frames = SPA_MIN(b->requested, n_frames);

        for (i = 0; i < n_frames; i++) {
				double val = 0;
				for (int j=0; j < data->num_seqs; j++) {
					val += process_sequence(data->seqs[j]);
				}
				val = process_filter(data->filt, val);
				int16_t out = (val * DEFAULT_VOLUME) * 32767.0;
                for (c = 0; c < DEFAULT_CHANNELS; c++)
                        *dst++ = out;
        }

        buf->datas[0].chunk->offset = 0;
        buf->datas[0].chunk->stride = stride;
        buf->datas[0].chunk->size = n_frames * stride;

        pw_stream_queue_buffer(data->stream, b);
}
/* [on_process] */

static const struct pw_stream_events stream_events = {
        PW_VERSION_STREAM_EVENTS,
        .process = on_process,
};

int main(int argc, char *argv[])
{
		struct sequence **seqs = malloc(sizeof(struct sequence*) * 2);

		struct signal_generator gen1 = {.type=SIGNAL_SQUARE};
		reset_generator(&gen1);
		struct envelope env1 = {.attack=.001, .sustain=0.1, .decay=0.0};
		reset_envelope(&env1);
		struct beat beat1 = {.time=0., .bpm=480.};

		struct note notes[4];
		notes[0].freq=100.;
		notes[0].vol=0.9;
		notes[1].freq=100.;
		notes[1].vol=0.9;
		notes[2].freq=200.;
		notes[2].vol=0.9;
		notes[3].freq=120.;
		notes[3].vol=0.9;
		struct note **note_ptrs = malloc(sizeof(struct note*) * 4);
		note_ptrs[0] = &notes[0];
		note_ptrs[1] = &notes[1];
		note_ptrs[2] = &notes[2];
		note_ptrs[3] = &notes[3];

		struct sequence seq = {
			.beat=&beat1, .gen=&gen1, .env=&env1, .pos=0, .max=4, .notes=note_ptrs
		};
		seqs[0] = &seq;

		struct signal_generator gen2 = {.type=SIGNAL_SQUARE};
		reset_generator(&gen2);
		struct envelope env2 = {.attack=.1, .sustain=0.1, .decay=0.1};
		reset_envelope(&env2);
		struct beat beat2 = {.time=0., .bpm=80.};

		struct note notes2[4];
		notes2[0].freq=900.;
		notes2[0].vol=0.9;
		notes2[1].freq=840.;
		notes2[1].vol=0.7;
		notes2[2].freq=800.;
		notes2[2].vol=0.9;
		notes2[3].freq=1300.;
		notes2[3].vol=0.9;
		struct note **note_ptrs2 = malloc(sizeof(struct note*) * 4);
		note_ptrs2[0] = &notes2[0];
		note_ptrs2[1] = &notes2[1];
		note_ptrs2[2] = &notes2[2];
		note_ptrs2[3] = &notes2[3];

		struct sequence seq2 = {
			.beat=&beat2, .gen=&gen2, .env=&env2, .pos=0, .max=4, .notes=note_ptrs2
		};
		seqs[1] = &seq2;

		int N = 32;
		double *filter_buffer = malloc(sizeof(double) * N);
		double *filter_coeffs = malloc(sizeof(double) * N);
		double Z = 0;
		for (int i=0; i<N; i++) {
			filter_buffer[i] = 0;
			double e = (30 * i/N);
			double c = exp(-e * e );
			filter_coeffs[i] = c;
			Z += c;
		}
		for (int i=0; i<N; i++) {
			filter_coeffs[i] /= Z;
		}
		struct filter filt = {
			.buffer=filter_buffer, .size=N, .coeffs=filter_coeffs
		};


		struct data data;
		data.filt = &filt;
		data.seqs = seqs;
		data.num_seqs = 2;

        const struct spa_pod *params[1];
        uint8_t buffer[1024];
        struct spa_pod_builder b = SPA_POD_BUILDER_INIT(buffer, sizeof(buffer));

        pw_init(&argc, &argv);

        data.loop = pw_main_loop_new(NULL);

        data.stream = pw_stream_new_simple(
                        pw_main_loop_get_loop(data.loop),
                        "audio-src",
                        pw_properties_new(
                                PW_KEY_MEDIA_TYPE, "Audio",
                                PW_KEY_MEDIA_CATEGORY, "Playback",
                                PW_KEY_MEDIA_ROLE, "Music",
                                NULL),
                        &stream_events,
                        &data);

        params[0] = spa_format_audio_raw_build(&b, SPA_PARAM_EnumFormat,
                        &SPA_AUDIO_INFO_RAW_INIT(
                                .format = SPA_AUDIO_FORMAT_S16,
                                .channels = DEFAULT_CHANNELS,
                                .rate = DEFAULT_RATE ));

        pw_stream_connect(data.stream,
                          PW_DIRECTION_OUTPUT,
                          PW_ID_ANY,
                          PW_STREAM_FLAG_AUTOCONNECT |
                          PW_STREAM_FLAG_MAP_BUFFERS |
                          PW_STREAM_FLAG_RT_PROCESS,
                          params, 1);

        pw_main_loop_run(data.loop);

        pw_stream_destroy(data.stream);
        pw_main_loop_destroy(data.loop);

		free(filter_buffer);
		free(filter_coeffs);
		free(seqs);

        return 0;
}
