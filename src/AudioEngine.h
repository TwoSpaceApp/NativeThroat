#ifndef NATIVE_THROAT_AUDIOENGINE_H
#define NATIVE_THROAT_AUDIOENGINE_H

#include <oboe/Oboe.h>
#include <string>
#include <vector>
#include <opusfile.h>

class AudioEngine : public oboe::AudioStreamCallback {
public:
    AudioEngine();
    ~AudioEngine();

    void start();
    void stop();
    bool loadOpusFile(const char *path);

    oboe::DataCallbackResult onAudioReady(oboe::AudioStream *oboeStream, void *audioData, int32_t numFrames) override;

private:
    void cleanupOpus();

    oboe::ManagedStream mStream;

    // OPUS related members
    std::vector<float> mDecodedBuffer;
    int32_t mPlaybackIndex = 0;
};

#endif //NATIVE_THROAT_AUDIOENGINE_H
