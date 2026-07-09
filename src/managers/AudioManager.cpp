#pragma once

#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/Music.hpp>
#include <list>
#include <unordered_map>
#include <string>

// ===========================================================================
// AudioManager（单例）：管理短音效（sfx）与背景音乐（BGM）。
// sfx 通过 ResourceManager 提供的 SoundBuffer 播放，用 list 维护活跃实例
// 以保证 SoundBuffer 生命周期；Update 清理已播放完的 sfx。
// 支持音量、暂停/恢复 BGM。
// ===========================================================================

class AudioManager {
public:
    static AudioManager& Instance();

    // 注册音效别名 -> 文件路径（可在初始化时批量注册）
    void RegisterSfx(const std::string& name, const std::string& path);
    void PlaySfx(const std::string& name);

    void PlayMusic(const std::string& path, bool loop = true);
    void StopMusic();
    void PauseMusic();
    void ResumeMusic();

    void SetSfxVolume(float v);    // 0..100
    void SetMusicVolume(float v);  // 0..100
    float GetSfxVolume() const { return sfxVolume; }
    float GetMusicVolume() const { return musicVolume; }

    void Update(); // 清理已结束的 sfx 实例

private:
    AudioManager();

    std::list<sf::Sound> activeSounds;
    std::unordered_map<std::string, std::string> sfxBindings; // name -> path
    sf::Music music;
    float sfxVolume = 80.0f;
    float musicVolume = 60.0f;
};
