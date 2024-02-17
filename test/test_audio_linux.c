#include <stdio.h>
#include <stdlib.h>
#include <alsa/asoundlib.h>

#define PCM_DEVICE "default"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <wave_file>\n", argv[0]);
        return 1;
    }

    char *wave_file = argv[1];

    snd_pcm_t *pcm_handle;
    snd_pcm_hw_params_t *params;
    unsigned int pcm_access;
    int dir;
    snd_pcm_uframes_t frames;
    char *buffer;
    int rc;

    // Open PCM device for playback
    if ((rc = snd_pcm_open(&pcm_handle, PCM_DEVICE, SND_PCM_STREAM_PLAYBACK, 0)) < 0) {
        fprintf(stderr, "Unable to open PCM device: %s\n", snd_strerror(rc));
        return 1;
    }

    // Allocate parameters object and fill it with default values
    snd_pcm_hw_params_alloca(&params);
    snd_pcm_hw_params_any(pcm_handle, params);

    // Set parameters: interleaved mode, signed 16-bit little-endian format,
    // sample rate, number of channels
    snd_pcm_hw_params_set_access(pcm_handle, params, SND_PCM_ACCESS_RW_INTERLEAVED);
    snd_pcm_hw_params_set_format(pcm_handle, params, SND_PCM_FORMAT_S16_LE);
    snd_pcm_hw_params_set_channels(pcm_handle, params, 2);
    unsigned int rate = 44100;
    snd_pcm_hw_params_set_rate_near(pcm_handle, params, &rate, &dir);

    // Write parameters to the driver
    if ((rc = snd_pcm_hw_params(pcm_handle, params)) < 0) {
        fprintf(stderr, "Unable to set HW parameters: %s\n", snd_strerror(rc));
        return 1;
    }

    // Allocate buffer to hold the PCM data
    snd_pcm_hw_params_get_period_size(params, &frames, &dir);
    size_t buffer_size = frames * 4; // 2 bytes/sample, 2 channels
    buffer = (char *)malloc(buffer_size);

    // Open wave file
    FILE *wave_file_ptr = fopen(wave_file, "r");
    if (wave_file_ptr == NULL) {
        fprintf(stderr, "Unable to open wave file: %s\n", wave_file);
        return 1;
    }

    // Read data from wave file and write to PCM device
    while (fread(buffer, 1, buffer_size, wave_file_ptr) > 0) {
        rc = snd_pcm_writei(pcm_handle, buffer, frames);
        if (rc == -EPIPE) {
            fprintf(stderr, "Underrun occurred\n");
            snd_pcm_prepare(pcm_handle);
        } else if (rc < 0) {
            fprintf(stderr, "Error from writei: %s\n", snd_strerror(rc));
        } else if (rc != (int)frames) {
            fprintf(stderr, "Short write, write %d frames\n", rc);
        }
    }

    // Close wave file
    fclose(wave_file_ptr);

    // Close PCM device
    snd_pcm_drain(pcm_handle);
    snd_pcm_close(pcm_handle);
    free(buffer);

    return 0;
}
