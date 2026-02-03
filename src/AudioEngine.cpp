#include "AudioEngine.h"
#include <android/log.h>

#define LOG_TAG "AudioEngine"
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

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
    cleanupOpus();
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

bool AudioEngine::loadOpusFile(const char *path) {
    cleanupOpus();
    int ret;
    OggOpusFile *opusFile = op_open_file(path, &ret);
    if (opusFile == nullptr) {
        LOGE("Failed to open opus file: %s, error: %d", path, ret);
        return false;
    }

    int channelCount = op_channel_count(opusFile, -1);
    if (channelCount > 1) {
        LOGE("Multi-channel opus files are not supported");
        op_free(opusFile);
        return false;
    }

    ogg_int64_t totalPcmFrames = op_pcm_total(opusFile, -1);
    mDecodedBuffer.resize(totalPcmFrames);

    float *buffer = mDecodedBuffer.data();
    int samplesRead;
    int totalSamplesRead = 0;

    while(totalSamplesRead < totalPcmFrames) {
        samplesRead = op_read_float(opusFile, buffer + totalSamplesRead, mDecodedBuffer.size() - totalSamplesRead, nullptr);
        if (samplesRead < 0) {
            LOGE("Opus read failed with error %d", samplesRead);
            op_free(opusFile);
            cleanupOpus();
            return false;
        }
        if (samplesRead == 0) {
            break;
        }
        totalSamplesRead += samplesRead;
    }

    op_free(opusFile);
    mPlaybackIndex = 0;
    return true;
}

void AudioEngine::cleanupOpus() {
    mDecodedBuffer.clear();
    mPlaybackIndex = 0;
}

oboe::DataCallbackResult AudioEngine::onAudioReady(oboe::AudioStream *oboeStream, void *audioData, int32_t numFrames) {
    if (mPlaybackIndex < mDecodedBuffer.size()) {
        int32_t framesToCopy = std::min(numFrames, (int32_t)(mDecodedBuffer.size() - mPlaybackIndex));
        memcpy(audioData, &mDecodedBuffer[mPlaybackIndex], framesToCopy * sizeof(float));
        mPlaybackIndex += framesToCopy;

        if (framesToCopy < numFrames) {
            // Fill the rest of the buffer with silence
            memset(static_cast<float*>(audioData) + framesToCopy, 0, (numFrames - framesToCopy) * sizeof(float));
        }
    } else {
        // Fill with silence if playback is finished
        memset(audioData, 0, numFrames * sizeof(float));
    }
    return oboe::DataCallbackResult::Continue;
}
