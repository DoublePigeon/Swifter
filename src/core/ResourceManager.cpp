#include "core/ResourceManager.h"

ResourceManager& ResourceManager::Instance() {
    static ResourceManager instance;
    return instance;
}

const sf::Texture& ResourceManager::GetTexture(const std::string& path) {
    auto it = textures.find(path);
    if (it != textures.end()) return *it->second;

    auto tex = std::make_unique<sf::Texture>();
    if (!tex->loadFromFile(path)) {
        tex->resize({ 1u, 1u });
    }
    auto& ref = *tex;
    textures[path] = std::move(tex);
    return ref;
}

const sf::Font& ResourceManager::GetFont(const std::string& path) {
    auto it = fonts.find(path);
    if (it != fonts.end()) return *it->second;

    auto font = std::make_unique<sf::Font>();
    if (!font->openFromFile(path)) {
        // 加载失败：尝试系统默认字体回退，或保持空字体
    }
    auto& ref = *font;
    fonts[path] = std::move(font);
    return ref;
}

const sf::SoundBuffer& ResourceManager::GetSoundBuffer(const std::string& path) {
    auto it = sounds.find(path);
    if (it != sounds.end()) return *it->second;

    auto buf = std::make_unique<sf::SoundBuffer>();
    if (!buf->loadFromFile(path)) {
        // 加载失败：返回空缓冲
    }
    auto& ref = *buf;
    sounds[path] = std::move(buf);
    return ref;
}

void ResourceManager::Clear() {
    textures.clear();
    fonts.clear();
    sounds.clear();
}