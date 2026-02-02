#ifndef NATIVE_THROAT_AUDIOENGINE_H
#define NATIVE_THROAT_AUDIOENGINE_H

#include <oboe/Oboe.h>

class AudioEngine : public oboe::AudioStreamCallback {
public:
    AudioEngine();
    ~AudioEngine();

    void start();
    void stop();

    oboe::DataCallbackResult onAudioReady(oboe::AudioStream *oboeStream, void *audioData, int32_t numFrames) override;

private:
    oboe::ManagedStream mStream;
};

#endif //NATIVE_THROAT_AUDIOENGINE_H
