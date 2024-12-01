#include "TextureManager.h"
#include "Const.h"

TextureManager::TextureManager() {
    loadTexture("grass", GRASS_TEXTURE);
    loadTexture("tree", TREE_TEXTURE);
    loadTexture("cactus", CACTUS_TEXTURE);
    loadTexture("player", PLAYER_TEXTURE);
    loadTexture("pickable", PICKABLE_TEXTURE);
    loadFont("gameFont", GAME_FONT);
    loadMusic("gameMusic", GAME_MUSIC);
	loadTexture("jumperWalk", JUMPER_TEXTURE_WALK);
	loadTexture("jumperAttack", JUMPER_TEXTURE_ATTACK);
	loadTexture("jumperDeath", JUMPER_TEXTURE_DEATH);
	loadTexture("jumperJump", JUMPER_TEXTURE_JUMP);
	loadTexture("runnerWalk", RUNNER_TEXTURE_WALK);
	loadTexture("runnerAttack", RUNNER_TEXTURE_ATTACK);
	loadTexture("runnerDeath", RUNNER_TEXTURE_DEATH);
	loadTexture("bossWalk", BOSS_TEXTURE_WALK);
	loadTexture("bossAttack", BOSS_TEXTURE_ATTACK);
	loadTexture("bossDeath", BOSS_TEXTURE_DEATH);
	loadTexture("GregStar", GREGSTAR_TEXTURE);


}

void TextureManager::loadTexture(const std::string& textureName, const std::string& filePath) {
    sf::Texture texture;
    if (texture.loadFromFile(filePath)) {
        textures[textureName] = std::move(texture);
    }
    else {
        throw std::runtime_error("Failed to load texture: " + filePath);
    }
}

void TextureManager::loadFont(const std::string& fontName, const std::string& filePath) {
    sf::Font font;
    if (font.loadFromFile(filePath)) {
        fonts[fontName] = std::move(font);
    }
    else {
        throw std::runtime_error("Failed to load font: " + filePath);
    }
}

void TextureManager::loadMusic(const std::string& musicName, const std::string& filePath) {
    auto music = std::make_unique<sf::Music>();
    if (music->openFromFile(filePath)) {
        this->music[musicName] = std::move(music);
    }
    else {
        throw std::runtime_error("Failed to load music: " + filePath);
    }
}

const sf::Texture& TextureManager::getTexture(const std::string& textureName) const {
    return textures.at(textureName);
}

const sf::Font& TextureManager::getFont(const std::string& fontName) const {
    return fonts.at(fontName);
}

sf::Music& TextureManager::getMusic(const std::string& musicName) {
    return *music.at(musicName);
}
