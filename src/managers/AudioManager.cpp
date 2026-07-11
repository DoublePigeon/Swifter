#include "managers/AudioManager.h"
#include "core/ResourceManager.h"

AudioManager& AudioManager::Instance() {
    static AudioManager instance;
    return instance;
}

AudioManager::AudioManager() = default;

void AudioManager::RegisterSfx(const std::string& name, const std::string& path) {
    sfxBindings[name] = path;
}

void AudioManager::PlaySfx(const std::string& name) {
    auto it = sfxBindings.find(name);
    if (it == sfxBindings.end()) {
        return;
    }

    auto& buffer = ResourceManager::Instance().GetSoundBuffer(it->second);
    auto& sound = activeSounds.emplace_back(buffer);
    sound.setVolume(sfxVolume);
    sound.play();
}

void AudioManager::PlayMusic(const std::string& path, bool loop) {
    if (!music.openFromFile(path)) {
        return;
    }
    music.setLooping(loop);
    music.setVolume(musicVolume);
    music.play();
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
