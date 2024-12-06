#pragma once

#include <string>

// Константы окна
const unsigned int WINDOW_WIDTH = 1280;
const unsigned int WINDOW_HEIGHT = 720;
const std::string WINDOW_TITLE = "Vampire Survivors Clone";

// Константы карты и блоков
const int BLOCK_SIZE = 64;
const int MAP_WIDTH = WINDOW_WIDTH / BLOCK_SIZE;
const int MAP_HEIGHT = WINDOW_HEIGHT / BLOCK_SIZE;
const float TREE_SPAWN_CHANCE = 0.03f;
const int CHUNK_SIZE = 10;
const int VIEW_RADIUS = 5;

// Пути к текстурам
const std::string GRASS_TEXTURE = "Assets/BackGround/grass.png";
const std::string CACTUS_TEXTURE = "Assets/Blocks/flowers.png";
const std::string TREE_TEXTURE = "Assets/BackGround/Wall1.png";
const std::string PICKABLE_TEXTURE = "Assets/BackGround/ExpPotition.png";

// Константы анимации и кадров
const float FRAME_DURATION = 0.08f;

// Константы игрока
const std::string PLAYER_TEXTURE = "Assets/player_spritesheet.png";
const std::string GAME_FONT = "Assets/Font/Handjet-Regular.ttf";
const std::string GAME_MUSIC = "Assets/Music/music.mp3";
const float PLAYER_SPEED = 300.0f;
const float ATTACK_SPEED = 0.5f;
const float PLAYER_MAX_HEALTH = 1000000.0f;
const float PLAYER_HEALTH_REGEN = 1.5f;
const int SPRITESHEET_WIDTH = 512;
const int SPRITESHEET_HEIGHT = 256;
const int FRAME_COUNT = 8;
const int ROW_COUNT = 4;

const float EXPIRIENCE_AMOUNT = 10.0f;
const float POTION_IN_CHUNK_CHANCE = 0.3f;

// Константы врагов (Jumper)
const std::string JUMPER_TEXTURE_WALK = "Assets/Enemies/Zombie/Wild Zombie/Walk.png";
const int JUMPER_WALK_FRAME_COUNT = 10;
const int JUMPER_WALK_WIDTH = 640;
const int JUMPER_WALK_HEIGHT = 64;
const std::string JUMPER_TEXTURE_ATTACK = "Assets/Enemies/Zombie/Wild Zombie/Attack_1.png";
const int JUMPER_ATTACK_FRAME_COUNT = 4;
const int JUMPER_ATTACK_WIDTH = 256;
const int JUMPER_ATTACK_HEIGHT = 64;
const std::string JUMPER_TEXTURE_DEATH = "Assets/Enemies/Zombie/Wild Zombie/Dead.png";
const int JUMPER_DEATH_FRAME_COUNT = 5;
const int JUMPER_DEATH_WIDTH = 320;
const int JUMPER_DEATH_HEIGHT = 64;
const std::string JUMPER_TEXTURE_JUMP = "Assets/Enemies/Zombie/Wild Zombie/Jump.png";
const int JUMPER_JUMP_FRAME_COUNT = 6;
const int JUMPER_JUMP_WIDTH = 384;
const int JUMPER_JUMP_HEIGHT = 64;

const float JUMPER_SPEED = 100.0f;
const float JUMPER_HEALTH = 50.0f;
const float JUMPER_DAMAGE = 10.0f;
const float JUMPER_ATTACK_RANGE = 32.0f;
const float JUMPER_ATTACK_SPEED = 1.0f;
const float MIN_DISTANCE_BETWEEN_JUMPERS = 15.0f;
const float JUMPER_REMOVE_DELAY = 3.0f;

const float JUMPER_EXPERIENCE = 20.0f;

// Константы врагов (Runner)
const std::string RUNNER_TEXTURE_WALK = "Assets/Enemies/Zombie/Zombie Man/Run.png";
const int RUNNER_WALK_FRAME_COUNT = 7;
const int RUNNER_WALK_WIDTH = 448;
const int RUNNER_WALK_HEIGHT = 64;
const std::string RUNNER_TEXTURE_ATTACK = "Assets/Enemies/Zombie/Zombie Man/Attack_1.png";
const int RUNNER_ATTACK_FRAME_COUNT = 5;
const int RUNNER_ATTACK_WIDTH = 320;
const int RUNNER_ATTACK_HEIGHT = 64;
const std::string RUNNER_TEXTURE_DEATH = "Assets/Enemies/Zombie/Zombie Man/Dead.png";
const int RUNNER_DEATH_FRAME_COUNT = 5;
const int RUNNER_DEATH_WIDTH = 320;
const int RUNNER_DEATH_HEIGHT = 64;

const float RUNNER_REMOVE_DELAY = 3.0f;

const float RUNNER_SPEED = 200.0f;
const float RUNNER_HEALTH = 30.0f;
const float RUNNER_DAMAGE = 5.0f;
const float RUNNER_ATTACK_RANGE = 32.0f;
const float RUNNER_ATTACK_SPEED = 1.0f;
const float RUNNER_EXPERIENCE = 10.0f;

const float MIN_DISTANCE_BETWEEN_RUNNERS = 15.0f;

// Константы врагов (Boss)
const std::string BOSS_TEXTURE_WALK = "Assets/Enemies/Zombie/Boss/FLYING.png";
const int BOSS_WALK_FRAME_COUNT = 4;
const int BOSS_WALK_WIDTH = 316;
const int BOSS_WALK_HEIGHT = 69;
const std::string BOSS_TEXTURE_ATTACK = "Assets/Enemies/Zombie/Boss/ATTACK.png";
const int BOSS_ATTACK_FRAME_COUNT = 8;
const int BOSS_ATTACK_WIDTH = 632;
const int BOSS_ATTACK_HEIGHT = 69;
const std::string BOSS_TEXTURE_DEATH = "Assets/Enemies/Zombie/Boss/DEATH.png";
const int BOSS_DEATH_FRAME_COUNT = 7;
const int BOSS_DEATH_WIDTH = 553;
const int BOSS_DEATH_HEIGHT = 69;

const float BOSS_REMOVE_DELAY = 3.0f;

const float BOSS_SPEED = 200.0f;
const float BOSS_HEALTH = 100.0f;
const float BOSS_DAMAGE = 15.0f;
const float BOSS_ATTACK_RANGE = 32.0f;
const float BOSS_ATTACK_SPEED = 1.0f;
const float BOSS_EXPERIENCE = 50.0f;

const float MIN_DISTANCE_BETWEEN_ENEMIES = 15.0f;

// Константы снарядов
const float PROJECTILE_SPEED = 1000.0f;
const float PROJECTILE_DAMAGE = 1000.0f;
const float PROJECTILE_LIFETIME = 0.5f;

// Константы для Greg Star
const float GREG_STAR_SPAWN_CHANCE = 0.001f;
const std::string GREGSTAR_TEXTURE = "Assets/BackGround/greg.png";

// Константы для взрывов
const float EXPLOSION_CHANCE = 0.1f;          // 10% шанс на взрывной снаряд
const float EXPLOSION_DAMAGE = 15.0f;         // Урон за тик
const float EXPLOSION_TICK_INTERVAL = 0.5f;   // Интервал между тиками урона
const float EXPLOSION_DURATION = 1.5f;        // Общая длительность взрыва
const float EXPLOSION_RADIUS = 50.0f;         // Радиус области взрыва

// Константы для генерации мира
const float BIOME_SCALE = 0.005f;  // Было 0.02f, уменьшено для увеличения размера биомов

