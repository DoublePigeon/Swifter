#pragma once

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <unordered_map>
#include <string>
#include <memory>

// ===========================================================================
// 资源管理器（单例）：缓存贴图、字体、音效缓冲，避免重复加载。
// 返回 const 引用，保证资源不会被外部修改。
// ===========================================================================

class ResourceManager {
public:
    static ResourceManager& Instance();

    const sf::Texture&     GetTexture(const std::string& path);
    const sf::Font&        GetFont(const std::string& path);
    const sf::SoundBuffer& GetSoundBuffer(const std::string& path);

    void Clear(); // 切关或退出时清空缓存

private:
    ResourceManager() = default;
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

    std::unordered_map<std::string, std::unique_ptr<sf::Texture>>     textures;
    std::unordered_map<std::string, std::unique_ptr<sf::Font>>        fonts;
    std::unordered_map<std::string, std::unique_ptr<sf::SoundBuffer>> sounds;
};
