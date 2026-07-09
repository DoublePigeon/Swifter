#include "core/ResourceManager.h"

// ===========================================================================
// 资源管理器（单例）：缓存贴图、字体、音效缓冲，避免重复加载。
// 返回 const 引用，保证资源不会被外部修改。
// ===========================================================================

ResourceManager& ResourceManager::Instance() {

}

const sf::Texture& ResourceManager::GetTexture(const std::string& path) {

}

const sf::Font& ResourceManager::GetFont(const std::string& path) {

}

const sf::SoundBuffer& ResourceManager::GetSoundBuffer(const std::string& path) {

}

void ResourceManager::Clear() {

} // 切关或退出时清空缓存


ResourceManager::ResourceManager() = default;
ResourceManager::ResourceManager(const ResourceManager&) = delete;
ResourceManager& ResourceManager::operator=(const ResourceManager&) = delete;