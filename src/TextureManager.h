#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <unordered_map>
#include <string>
#include <memory>

class TextureManager {
public:
    TextureManager();
    const sf::Texture& getTexture(const std::string& textureName) const;
    const sf::Font& getFont(const std::string& fontName) const;
    sf::Music& getMusic(const std::string& musicName);

private:
    void loadTexture(const std::string& textureName, const std::string& filePath);
    void loadFont(const std::string& fontName, const std::string& filePath);
    void loadMusic(const std::string& musicName, const std::string& filePath);
    std::unordered_map<std::string, sf::Texture> textures;
    std::unordered_map<std::string, sf::Font> fonts;
    std::unordered_map<std::string, std::unique_ptr<sf::Music>> music;
};
