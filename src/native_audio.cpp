#include "AudioEngine.h"

extern "C" {
    __attribute__((visibility("default"))) __attribute__((used))
    AudioEngine* create_audio_engine() {
        return new AudioEngine();
    }

    __attribute__((visibility("default"))) __attribute__((used))
    void delete_audio_engine(AudioEngine* engine) {
        delete engine;
    }

    __attribute__((visibility("default"))) __attribute__((used))
    void start_audio(AudioEngine* engine) {
        engine->start();
    }

    __attribute__((visibility("default"))) __attribute__((used))
    void stop_audio(AudioEngine* engine) {
        engine->stop();
    }
}
