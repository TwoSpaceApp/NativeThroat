#include "AudioEngine.h"

AudioEngine::AudioEngine() {
    oboe::AudioStreamBuilder builder;
    builder.setDirection(oboe::Direction::Output)
           ->setPerformanceMode(oboe::PerformanceMode::LowLatency)
           ->setSharingMode(oboe::SharingMode::Exclusive)
           ->setFormat(oboe::AudioFormat::Float)
           ->setChannelCount(oboe::ChannelCount::Mono)
           ->setCallback(this)
           ->openManagedStream(mStream);
}

AudioEngine::~AudioEngine() {
    stop();
}

void AudioEngine::start() {
    if (mStream) {
        mStream->requestStart();
    }
}

void AudioEngine::stop() {
    if (mStream) {
        mStream->requestStop();
    }
}

oboe::DataCallbackResult AudioEngine::onAudioReady(oboe::AudioStream *oboeStream, void *audioData, int32_t numFrames) {
    memset(audioData, 0, numFrames * sizeof(float));
    return oboe::DataCallbackResult::Continue;
}
