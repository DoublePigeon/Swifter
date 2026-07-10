#include "managers/AudioManager.h"

AudioManager& AudioManager::Instance() {
    static AudioManager instance;
    return instance;
}

AudioManager::AudioManager() = default;

void AudioManager::RegisterSfx(const std::string& name, const std::string& path) {
    sfxBindings[name] = path;
}

void AudioManager::PlaySfx(const std::string& name) {
    // 桩：待 ResourceManager 实现 SoundBuffer 加载后填充
}

void AudioManager::PlayMusic(const std::string& path, bool loop) {
    // 桩：待后续实现
}

void AudioManager::StopMusic() {
    music.stop();
}

void AudioManager::PauseMusic() {
    music.pause();
}

void AudioManager::ResumeMusic() {
    music.play();
}

void AudioManager::SetSfxVolume(float v) { sfxVolume = v; }
void AudioManager::SetMusicVolume(float v) { musicVolume = v; }

void AudioManager::Update() {
    // 清理已结束的 sfx 实例
    activeSounds.remove_if([](sf::Sound& s) {
        return s.getStatus() == sf::Sound::Status::Stopped;
    });
}
