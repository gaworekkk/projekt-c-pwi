#ifndef RESOURCELOADER_H
#define RESOURCELOADER_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Button.h"
#include "Slider.h"
#include "Player.h"
#include "ObstacleManager.h"
#include "StatisticsManager.h"
#include "CoinManager.h"
#include "Difficulty.h"
#include "Utils.h"

// Deklaracje zasobów
// Tła
extern sf::Texture backgroundTexture;
extern sf::Sprite backgroundSprite;
extern sf::Texture storyBackgroundTexture;
extern sf::Sprite storyBackgroundSprite;
extern sf::Texture gameOverBackgroundTexture;
extern sf::Sprite gameOverBackgroundSprite;
extern sf::Texture statisticsBackgroundTexture;
extern sf::Sprite statisticsBackgroundSprite;
extern sf::Texture achievementsBackgroundTexture;
extern sf::Sprite achievementsBackgroundSprite;
extern sf::Texture optionsMenuBackgroundTexture;
extern sf::Sprite optionsMenuBackgroundSprite;
extern sf::Texture pauseBackgroundTexture;
extern sf::Sprite pauseBackgroundSprite;
extern sf::Sprite shopBackgroundSprite;

// Dino
extern sf::Texture dinoTexture;
extern sf::Sprite dinoSprite;
extern std::string dinoTexturePath[6];

// Czcionka
extern sf::Font font;

// Slidery
extern Slider *musicSlider;
extern Slider *soundSlider;

// Muzyka i dźwięki
extern sf::Music menuMusic;
extern sf::Music backgroundMusic;
extern sf::SoundBuffer deathBuffer;
extern sf::Sound deathSound;
extern sf::SoundBuffer jumpBuffer;
extern sf::Sound jumpSound;
extern sf::SoundBuffer buttonBuffer;
extern sf::Sound buttonSound;

// Przyciski
extern Button storyButton;
extern Button endlessButton;
extern Button optionsButton;
extern Button achievementsButton;
extern Button statisticsButton;
extern Button backButton;
extern Button exitButton;
extern Button pauseButton;
extern Button resumeButton;
extern Button restartButton;
extern Button restartButtonPause;
extern Button mainMenuButton;
extern Button easyButton;
extern Button normalButton;
extern Button hardButton;
extern Button shopButton;
extern Button* buyButton[6];
extern Button buyButtonSkin;
// Moneta
extern sf::Texture coinTexture;
extern sf::Sprite coinSprite;
extern sf::Sprite coinIconMainMenu;

// Gracz
extern Player player;

// Generatory
extern ObstacleManager cactusManager;
extern ObstacleManager birdManager;
extern CoinManager coinManager;

// Teksty
extern sf::Text nameText;
extern sf::Text optionsMenuText;
extern sf::Text pauseText;
extern sf::Text distanceText;
extern sf::Text coinCountText;
extern sf::Text coinCountMainMenuText;
extern sf::Text gameOverText;
extern sf::Text distanceText2;
extern sf::Text bestScoreText;
extern sf::Text yourScoreText;
extern sf::Text coinsText;

// Funkcje
void loadAllResources(sf::RenderWindow& window, int coinCount, float musicVolume, float soundVolume);
void loadOneBackground(sf::Texture& texture, sf::Sprite& sprite, const std::string& filePath, sf::RenderWindow& window);
void loadBackgrounds(sf::RenderWindow& window);
void loadDino();
void loadFont();
void loadSliders(float musicVolume, float soundVolume);
void loadMusic();
void loadOneMusic(sf::Music& music, const std::string& filePath);
void loadBuffers();
void loadOneBuffer(sf::SoundBuffer& buffer, sf::Sound& sound, const std::string& filePath);
void loadCoins();
void loadButtons();
void loadGenerators();
void loadTexts(int coinCount);


#endif // RESOURCELOADER_H