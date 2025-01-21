#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Player.h"
#include "Button.h"
#include "ObstacleManager.h"
#include "Utils.h"
#include "CoinManager.h"
#include "StatisticsManager.h"
#include "Difficulty.h"
#include "Slider.h"
#include "ResourceLoader.h"
#include "Achivements.h"
#include <stdexcept>
#include <iostream>
#include <algorithm> 
#include <vector> 

enum GameState { MainMenu, OptionsMenu, Achievements, Statistics, Gameplay, Pause, GameOver, Shop };

// Ustawienie domyślnego stanu gry
GameState gameState = MainMenu;
// Poziom trudności
Difficulty difficulty;

int main() {
    // Ustawienia okna
    sf::VideoMode videoMode(1200, 640);
    sf::Uint32 style = sf::Style::Titlebar | sf::Style::Close;

    // Tworzenie okna
    sf::RenderWindow window(videoMode, "Dino Game", style);
    window.setFramerateLimit(60); // Ograniczenie FPS do 60
    
    // Zmienne stanu gry
    int coinCount = 0, currentCoinCount, cactusCount = 0, birdCount = 0;
    float distance;

    // Zaladowanie statystyk
    int bestDistance = 0, totalDistance = 0, jumpCount = 0, deathCount = 0, gamesPlayed = 0;
    int skinState[6];
	int prices[6] = {50, 100, 200, 500, 1000, 10000};
    float musicVolume, soundVolume;
    StatisticsManager::loadStatistics(coinCount, bestDistance, totalDistance, jumpCount, deathCount, gamesPlayed, cactusCount, birdCount, skinState, musicVolume, soundVolume, difficulty);
    // Załaduj wszystkie zasoby
    try {
        loadAllResources(window, coinCount, musicVolume, soundVolume);
    } catch (const std::runtime_error& e) {
        // W przypadku błędu wyświetl komunikat i zakończ program
        std::cerr << e.what() << std::endl;
        return -1;
    }
    cactusManager.setDifficulty(difficulty);
    birdManager.setDifficulty(difficulty);
    menuMusic.setVolume(musicVolume);
    backgroundMusic.setVolume(musicVolume);
    buttonSound.setVolume(soundVolume);
    jumpSound.setVolume(soundVolume);
    deathSound.setVolume(soundVolume);

    // Teksty do wyświetlania
    std::vector<std::wstring> achievements;
    achievements.push_back(L"Biegacz (" + std::to_wstring(totalDistance) + L"/" + std::to_wstring(range_distance(totalDistance)) + L")");
    achievements.push_back(L"Jump man (" + std::to_wstring(jumpCount) + L"/" + std::to_wstring(range_jump(jumpCount)) + L")");
    achievements.push_back(L"Pora umierać (" + std::to_wstring(deathCount) + L"/" + std::to_wstring(range_death(deathCount)) + L")");
    achievements.push_back(L"Ja to zawsze miałem łeb do interesów (" + std::to_wstring(coinCount) + L"/" + std::to_wstring(range_coins(coinCount)) + L")");

    std::vector<std::wstring> statisticsLabels = {
        L"Zdobyte monety: ",
        L"Najlepszy wynik: ",
        L"Całkowity dystans: ",
        L"Liczba skoków: ",
        L"Liczba śmierci: ",
        L"Liczba gier: ",
        L"Liczba kaktusów: ",
        L"Liczba ptaków: "
    };
    std::vector<std::wstring> statisticsCounters = {
        std::to_wstring(coinCount),
        std::to_wstring(bestDistance),
        std::to_wstring(totalDistance),
        std::to_wstring(jumpCount),
        std::to_wstring(deathCount),
        std::to_wstring(gamesPlayed),
        std::to_wstring(cactusCount),
        std::to_wstring(birdCount)
    };


    
    float deltaTime;
    sf::Clock clock;

    // Zmienne "zapadki"

    bool if_changed_speed = false;
    bool isJumping = false;
    const float standardLevel = 500.0f; // Standardowy poziom gracza
    sf::Texture aktualna;
    int pressed = -5;
    for(int i=0;i<6;i++){
        if (skinState[i]==0){
            pressed=i;

            //buyButton[i]->setText("Wybrano");
            buyButton[i]->setTexture("Tekstury/extinguished-przyciskUBRANO(small).png", "Tekstury/extinguished-przyciskUBRANO(small).png");
            player.setTexture(dinoTexturePath[i]);
            //buyButtonSkin.setText("Wybrano");
            buyButtonSkin.setTexture("Tekstury/extinguished-przyciskUBRANO(small).png", "Tekstury/extinguished-przyciskUBRANO(small).png");

        }
         else {
        //buyButton[i]->setText("Skin" + std::to_string(i + 1));
        buyButton[i]->setTexture("Tekstury/przyciskUBIERZ(small).png", "Tekstury/extinguished-przyciskUBIERZ(small).png");
    }
    }

    // Zmienne do śledzenia miniętych przeszkód
    std::vector<bool> passedCacti; 
    std::vector<bool> passedBirds; 

    // Load textures for achievements
    sf::Texture bronzeMedalTexture, silverMedalTexture, goldMedalTexture, achievementFrameTexture;
    if (!bronzeMedalTexture.loadFromFile("Tekstury/bronze-medal.png") ||
        !silverMedalTexture.loadFromFile("Tekstury/silver-medal.png") ||
        !goldMedalTexture.loadFromFile("Tekstury/gold-medal.png") ||
        !achievementFrameTexture.loadFromFile("Tekstury/archievment-frame.png")) {
        std::cerr << "Error loading achievement textures" << std::endl;
        return -1;
    }

    sf::Sprite achievementSprite;
    achievementSprite.setScale(2.0f, 2.0f); // Scale the achievement sprite to 2 times its size

    sf::Sprite achievementFrameSprite;
    achievementFrameSprite.setTexture(achievementFrameTexture);
    achievementFrameSprite.setScale(2.0f, 2.0f); // Scale the achievement frame sprite to 2 times its size

    // Główna pętla gry
    while (window.isOpen()) {
        deltaTime = clock.restart().asSeconds();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed){
                totalDistance += static_cast<int>(distance);
                StatisticsManager::saveStatistics(coinCount, bestDistance, totalDistance, jumpCount, deathCount, gamesPlayed, cactusCount, birdCount, skinState, musicVolume, soundVolume, difficulty);
                window.close();
            }
                 

            // Obsługa kliknięć w różnych stanach
            if (event.type == sf::Event::MouseButtonPressed) {
                if (gameState == MainMenu) {
                    //Aktualizacja statystyk
                    statisticsCounters[0] = std::to_wstring(coinCount);
                    statisticsCounters[1] = std::to_wstring(bestDistance);
                    statisticsCounters[2] = std::to_wstring(totalDistance);
                    statisticsCounters[3] = std::to_wstring(jumpCount);
                    statisticsCounters[4] = std::to_wstring(deathCount);
                    statisticsCounters[5] = std::to_wstring(gamesPlayed);
                    statisticsCounters[6] = std::to_wstring(cactusCount);
                    statisticsCounters[7] = std::to_wstring(birdCount);
                    if (storyButton.isClicked(sf::Mouse::getPosition(window), event.mouseButton)) {
						player.resetPosition();
                        Player player(sf::Vector2f(80, 80), sf::Vector2f(100, 500), sf::Color::White, "Tekstury/skórki dino/dino_sprite_sheet.png", 3, 0.1f); // 3 klatki animacji, 0.1 sekundy na klatkę

                        buttonSound.play(); // Odtwarzanie dźwięku przycisku
                        distance = 0.0f;
                        currentCoinCount = 0.0f;
                        cactusManager.restart();
			            birdManager.restart();
                        coinManager.restart();
                        passedCacti.clear();  
                        passedBirds.clear();  
                        player.resetPosition();
                        gameState = Gameplay; // Przejście do Gameplay
                        gamesPlayed++;
                        menuMusic.stop(); // Zatrzymanie muzyki tła dla menu
                        backgroundMusic.play(); // Odtwarzanie muzyki tła dla gry
                    }
					/*
                    if (endlessButton.isClicked(sf::Mouse::getPosition(window), event.mouseButton)) {
                        buttonSound.play(); // Odtwarzanie dźwięku przycisku
                        distance = 0.0f;
                        currentCoinCount = 0.0f;
			            cactusManager.restart();
                        birdManager.restart();
                        coinManager.restart();
                        passedCacti.clear(); 
                        passedBirds.clear();  
                        player.resetPosition();
                        gameState = Gameplay; // Przejście do Gameplay
                        gamesPlayed++;
                        menuMusic.stop(); // Zatrzymanie muzyki tła dla menu
                        backgroundMusic.play(); // Odtwarzanie muzyki tła dla gry
                    }
					*/
                    if (optionsButton.isClicked(sf::Mouse::getPosition(window), event.mouseButton)) {
                        buttonSound.play(); // Odtwarzanie dźwięku przycisku
                        gameState = OptionsMenu; // Przejście do OptionsMenu
                    }
                    if (achievementsButton.isClicked(sf::Mouse::getPosition(window), event.mouseButton)) {
                        buttonSound.play(); // Odtwarzanie dźwięku przycisku
                        gameState = Achievements; // Przejście do Achievements
                    }
                    if (statisticsButton.isClicked(sf::Mouse::getPosition(window), event.mouseButton)) {
                        buttonSound.play(); // Odtwarzanie dźwięku przycisku
                        gameState = Statistics; // Przejście do Statistics
                    }
                    if (exitButton.isClicked(sf::Mouse::getPosition(window), event.mouseButton)) {
                        buttonSound.play(); // Odtwarzanie dźwięku przycisku
                        totalDistance += static_cast<int>(distance);
                        StatisticsManager::saveStatistics(coinCount, bestDistance, totalDistance, jumpCount, deathCount, gamesPlayed, cactusCount, birdCount, skinState, musicVolume, soundVolume, difficulty);
                        window.close(); // Zamknięcie okna
                    }
                    if (shopButton.isClicked(sf::Mouse::getPosition(window), event.mouseButton)) {
                        buttonSound.play(); // Odtwarzanie dźwięku przycisku
                        gameState = Shop; // Przejście do Shop
                        aktualna=player.getTexture();
                        player.setPosition(sf::Vector2f(800, 160));
                        player.setSize(sf::Vector2f(200, 200));
                        StatisticsManager::loadStatistics(coinCount, bestDistance, totalDistance, jumpCount, deathCount, gamesPlayed, cactusCount, birdCount, skinState, musicVolume, soundVolume, difficulty);
                    }

                } else if (gameState == OptionsMenu) {
                    if (easyButton.isClicked(sf::Mouse::getPosition(window), event.mouseButton)) {
                        buttonSound.play();
                        difficulty = Difficulty::Easy;
                    }
                    if (normalButton.isClicked(sf::Mouse::getPosition(window), event.mouseButton)) {
                        buttonSound.play();
                        difficulty = Difficulty::Normal;
                    }
                    if (hardButton.isClicked(sf::Mouse::getPosition(window), event.mouseButton)) {
                        buttonSound.play();
                        difficulty = Difficulty::Hard;
                    }
                    if (backButton.isClicked(sf::Mouse::getPosition(window), event.mouseButton)) {
                        buttonSound.play(); // Odtwarzanie dźwięku przycisku
                        cactusManager.setDifficulty(difficulty);
                        birdManager.setDifficulty(difficulty);
                        musicSlider->reset();
                        soundSlider->reset();
                        musicVolume = musicSlider->getValue();
                        soundVolume = soundSlider->getValue();
                        StatisticsManager::saveStatistics(coinCount, bestDistance, totalDistance, jumpCount, deathCount, gamesPlayed, cactusCount, birdCount, skinState, musicVolume, soundVolume, difficulty);
                        gameState = MainMenu; // Powrót do MainMenu
                    }

                } else if (gameState == Achievements) {
                    if (backButton.isClicked(sf::Mouse::getPosition(window), event.mouseButton)) {
                        buttonSound.play(); // Odtwarzanie dźwięku przycisku
                        gameState = MainMenu; // Powrót do MainMenu
                    }

                } else if (gameState == Statistics) {
                    if (backButton.isClicked(sf::Mouse::getPosition(window), event.mouseButton)) {
                        buttonSound.play(); // Odtwarzanie dźwięku przycisku
                        gameState = MainMenu; // Powrót do MainMenu
                    }

                } else if (gameState == Gameplay) {
                    if (pauseButton.isClicked(sf::Mouse::getPosition(window), event.mouseButton)) {
                        buttonSound.play(); // Odtwarzanie dźwięku przycisku
                        gameState = Pause; // Przejście do Pause
                        backgroundMusic.pause(); // Zatrzymanie muzyki tła dla gry
                    }

                } else if (gameState == Pause) {
                    if (resumeButton.isClicked(sf::Mouse::getPosition(window), event.mouseButton)) {
                        buttonSound.play(); // Odtwarzanie dźwięku przycisku
                        gameState = Gameplay; // Powrót do Gameplay
                        backgroundMusic.play(); // Wznowienie muzyki tła dla gry
                        // Reset dino icon position
                        dinoSprite.setPosition(20, 104);
                        // Reset coin icon position
                        coinSprite.setPosition(20, 104 + dinoSprite.getGlobalBounds().height + 20);
                        // Reset coin counter position
                        coinCountText.setPosition(49 + coinSprite.getGlobalBounds().width, 99 + dinoSprite.getGlobalBounds().height + 20 + (coinSprite.getGlobalBounds().height / 2) - (coinCountText.getGlobalBounds().height / 2) - 20);
                        // Reset distance counter position
                        distanceText.setPosition(40 + dinoSprite.getGlobalBounds().width, 104 + (dinoSprite.getGlobalBounds().height / 2) - (distanceText.getGlobalBounds().height / 2) - 20);
                    }
                    if (restartButtonPause.isClicked(sf::Mouse::getPosition(window), event.mouseButton)) {
                        buttonSound.play(); // Odtwarzanie dźwięku przycisku
                        totalDistance += static_cast<int>(distance);
                        player.resetPosition();
                        Player player(sf::Vector2f(80, 80), sf::Vector2f(100, 500), sf::Color::White, "Tekstury/skórki dino/dino_sprite_sheet.png", 3, 0.1f); // 3 klatki animacji, 0.1 sekundy na klatkę
                        distance = 0.0f;
                        currentCoinCount = 0.0f;
   			            cactusManager.restart();
                        birdManager.restart();
                        coinManager.restart();
                        distanceText.setPosition(40 + dinoSprite.getGlobalBounds().width, 104 + (dinoSprite.getGlobalBounds().height / 2) - (distanceText.getGlobalBounds().height / 2) - 20);
                        coinCountText.setPosition(49 + coinSprite.getGlobalBounds().width, 99 + dinoSprite.getGlobalBounds().height + 20 + (coinSprite.getGlobalBounds().height / 2) - (coinCountText.getGlobalBounds().height / 2) - 20);
                        gameState = Gameplay; // Restart gry
                        gamesPlayed++;
                        backgroundMusic.play(); // Odtwarzanie muzyki tła od nowa
                        // Reset dino icon position
                        dinoSprite.setPosition(20, 104);
                        // Reset coin icon position
                        coinSprite.setPosition(20, 104 + dinoSprite.getGlobalBounds().height + 20);
                        // Reset coin counter position
                        coinCountText.setPosition(49 + coinSprite.getGlobalBounds().width, 99 + dinoSprite.getGlobalBounds().height + 20 + (coinSprite.getGlobalBounds().height / 2) - (coinCountText.getGlobalBounds().height / 2) - 20);
                        // Reset distance counter position
                        distanceText.setPosition(40 + dinoSprite.getGlobalBounds().width, 104 + (dinoSprite.getGlobalBounds().height / 2) - (distanceText.getGlobalBounds().height / 2) - 20);
                    }
                    if (mainMenuButton.isClicked(sf::Mouse::getPosition(window), event.mouseButton)) {
                        totalDistance += static_cast<int>(distance);
                        StatisticsManager::saveStatistics(coinCount, bestDistance, totalDistance, jumpCount, deathCount, gamesPlayed, cactusCount, birdCount, skinState, musicVolume, soundVolume, difficulty);
                        buttonSound.play(); // Odtwarzanie dźwięku przycisku
                        gameState = MainMenu; // Powrót do MainMenu
                        backgroundMusic.stop(); // Zatrzymanie muzyki tła dla gry
                        menuMusic.play(); // Odtwarzanie muzyki tła dla menu
                        // Reset dino icon position
                        dinoSprite.setPosition(20, 104);
                        // Reset coin icon position
                        coinSprite.setPosition(20, 104 + dinoSprite.getGlobalBounds().height + 20);
                        // Reset coin counter position
                        coinCountText.setPosition(49 + coinSprite.getGlobalBounds().width, 99 + dinoSprite.getGlobalBounds().height + 20 + (coinSprite.getGlobalBounds().height / 2) - (coinCountText.getGlobalBounds().height / 2) - 20);
                        // Reset distance counter position
                        distanceText.setPosition(40 + dinoSprite.getGlobalBounds().width, 104 + (dinoSprite.getGlobalBounds().height / 2) - (distanceText.getGlobalBounds().height / 2) - 20);
                    }

                } else if (gameState == GameOver) {
                    bestDistance = std::max(bestDistance, static_cast<int>(distance));
                    coinCount += currentCoinCount;
                    StatisticsManager::saveStatistics(coinCount, bestDistance, totalDistance, jumpCount, deathCount, gamesPlayed, cactusCount, birdCount, skinState, musicVolume, soundVolume, difficulty);
                    currentCoinCount = 0.0f;
                    coinCountMainMenuText.setString(std::to_wstring(coinCount));
					if (mainMenuButtonOver.isClicked(sf::Mouse::getPosition(window), event.mouseButton)) {
                        totalDistance += static_cast<int>(distance);
                        buttonSound.play(); // Odtwarzanie dźwięku przycisku
                        gameState = MainMenu; // Powrót do MainMenu
                        backgroundMusic.stop(); // Zatrzymanie muzyki tła dla gry
                        menuMusic.play(); // Odtwarzanie muzyki tła dla menu
                        // Reset dino icon position
                        dinoSprite.setPosition(20, 104);
                        // Reset coin icon position
                        coinSprite.setPosition(20, 104 + dinoSprite.getGlobalBounds().height + 20);
                        // Reset coin counter position
                        coinCountText.setPosition(49 + coinSprite.getGlobalBounds().width, 99 + dinoSprite.getGlobalBounds().height + 20 + (coinSprite.getGlobalBounds().height / 2) - (coinCountText.getGlobalBounds().height / 2) - 20);
                        // Reset distance counter position
                        distanceText.setPosition(40 + dinoSprite.getGlobalBounds().width, 104 + (dinoSprite.getGlobalBounds().height / 2) - (distanceText.getGlobalBounds().height / 2) - 20);
                    }
                    if (restartButton.isClicked(sf::Mouse::getPosition(window), event.mouseButton)) {
                        buttonSound.play(); // Odtwarzanie dźwięku przycisku
                        player.resetPosition();
                        Player player(sf::Vector2f(80, 80), sf::Vector2f(100, 500), sf::Color::White, "Tekstury/skórki dino/dino_sprite_sheet.png", 3, 0.1f); // 3 klatki animacji, 0.1 sekundy na klatkę
                        distance = 0.0f;
    			        cactusManager.restart();
                        birdManager.restart(); 
                        coinManager.restart();  
                        passedCacti.clear();  
                        passedBirds.clear();
			            currentCoinCount = 0.0f;
                        coinCountMainMenuText.setString(std::to_wstring(coinCount));
                        distanceText.setPosition(40 + dinoSprite.getGlobalBounds().width, 104 + (dinoSprite.getGlobalBounds().height / 2) - (distanceText.getGlobalBounds().height / 2) - 20);
                        coinCountText.setPosition(49 + coinSprite.getGlobalBounds().width, 99 + dinoSprite.getGlobalBounds().height + 20 + (coinSprite.getGlobalBounds().height / 2) - (coinCountText.getGlobalBounds().height / 2) - 20);
                        gameState = Gameplay; // Restart gry
                        gamesPlayed++;
                        backgroundMusic.play(); // Odtwarzanie muzyki tła od nowa
                    }
                    // Remove the exit button functionality from the GameOver screen
                    // if (exitButton.isClicked(sf::Mouse::getPosition(window), event.mouseButton)) {
                    //     buttonSound.play(); // Odtwarzanie dźwięku przycisku
                    //     window.close(); // Zamknięcie okna
                    // }
                } else if (gameState == Shop) {
                    
                    if (backButton.isClicked(sf::Mouse::getPosition(window), event.mouseButton)) {
                        buttonSound.play(); // Odtwarzanie dźwięku przycisku
                        gameState = MainMenu; // Powrót do MainMenu
                        player.resetPosition();
                        player.resetSize();
                        player.setFrameDuration(0.1f);
                        player.setTexture(aktualna);
                        StatisticsManager::saveStatistics(coinCount, bestDistance, totalDistance, jumpCount, deathCount, gamesPlayed, cactusCount, birdCount, skinState, musicVolume, soundVolume, difficulty);
                        for (int i = 0;i<6;i++){
                            if (skinState[i] == 0){

                                //buyButton[i]->setText("Wybrano");
                                buyButton[i]->setTexture("Tekstury/extinguished-przyciskUBRANO(small).png", "Tekstury/extinguished-przyciskUBRANO(small).png");
                            } else {
                                //buyButton[i]->setText("Skin" + std::to_string(i+1));
                                buyButton[i]->setTexture("Tekstury/przyciskUBIERZ(small).png", "Tekstury/extinguished-przyciskUBIERZ(small).png");

                            }
                           
                        }
                        
                    }
                    
                    for (int i=0;i<6;i++){

                        if (buyButton[i]->isClicked(sf::Mouse::getPosition(window), event.mouseButton)) {
                            buttonSound.play(); // Odtwarzanie dźwięku przycisku
                            player.setTexture(dinoTexturePath[i]); 
                            pressed = i;

                            //buyButton[i]->setText("Wybrano");
                            buyButton[i]->setTexture("Tekstury/extinguished-przyciskUBRANO(small).png", "Tekstury/extinguished-przyciskUBRANO(small).png");
                            for (int j=0;j<6;j++){
                                if (j!=i){
                                    //buyButton[j]->setText("Skin" + std::to_string(j+1));
                                    buyButton[j]->setTexture("Tekstury/przyciskUBIERZ(small).png", "Tekstury/extinguished-przyciskUBIERZ(small).png");
                                }
                            }
                            if (skinState[i] == 0){
                                //buyButtonSkin.setText("Wybrano");
                                buyButtonSkin.setTexture("Tekstury/extinguished-przyciskUBRANO(small).png", "Tekstury/extinguished-przyciskUBRANO(small).png");
                            } else if (skinState[i] == 2){
                                //buyButtonSkin.setText("Kup");
                                 buyButtonSkin.setTexture("Tekstury/przyciskKUP(small).png", "Tekstury/extinguished-przyciskKUP(small).png");
                            } else {
                                //buyButtonSkin.setText("Wybierz");
                                buyButtonSkin.setTexture("Tekstury/przyciskUBIERZ(small).png", "Tekstury/extinguished-przyciskUBIERZ(small).png");

                            }
                        }
                    }
                    if (buyButtonSkin.isClicked(sf::Mouse::getPosition(window), event.mouseButton)) {
						

                        if (skinState[pressed] == 2){
                            if (coinCount >= 1){
                                coinCount -= 1;
                                coinCountMainMenuText.setString(std::to_wstring(coinCount));
                                skinState[pressed] = 0;
                                aktualna=player.getTexture();
                                //buyButtonSkin.setText("Wybrano");
                                buyButtonSkin.setTexture("Tekstury/extinguished-przyciskUBRANO(small).png", "Tekstury/extinguished-przyciskUBRANO(small).png");
                                for (int j=0;j<6;j++){
                                    if (j!=pressed && skinState[j] == 0){
                                        skinState[j] = 1;
                                        buyButton[j]->setTexture("Tekstury/przyciskUBIERZ(small).png", "Tekstury/extinguished-przyciskUBIERZ(small).png");
                                    }
                                }
                            }
                        } else if (skinState[pressed] == 1){
                            skinState[pressed] = 0;
                            //buyButtonSkin.setText("Wybrano");
                            buyButtonSkin.setTexture("Tekstury/extinguished-przyciskUBRANO(small).png", "Tekstury/extinguished-przyciskUBRANO(small).png");
                            aktualna=player.getTexture();
                            for (int j=0;j<6;j++){
                                if (j!=pressed && skinState[j] != 2){
                                    skinState[j] = 1;
                                }
                            }
                        }

                    }
                }
            }

            // Obsługa klawiszy w różnych stanach
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    if (gameState == OptionsMenu) {
                        cactusManager.setDifficulty(difficulty);
                        birdManager.setDifficulty(difficulty);
                        coinManager.setObstacleSpawnSpeed(cactusManager.getInitialSpeed());
                        musicSlider->reset();
                        soundSlider->reset();
                        musicVolume = musicSlider->getValue();
                        soundVolume = soundSlider->getValue();
                        StatisticsManager::saveStatistics(coinCount, bestDistance, totalDistance, jumpCount, deathCount, gamesPlayed, cactusCount, birdCount, skinState, musicVolume, soundVolume, difficulty);
                        gameState = MainMenu; // Powrót do MainMenu
                    }
                    if (gameState == Achievements) {
                        gameState = MainMenu; // Powrót do MainMenu
                    }
                    if (gameState == Statistics) {
                        gameState = MainMenu; // Powrót do MainMenu
                    }
                    if (gameState == Gameplay) {
                        gameState = Pause; // Przejście do Pause
                        backgroundMusic.pause(); // Zatrzymanie muzyki tła dla gry
                    } 
                    else if (gameState == Pause) {
                        gameState = Gameplay; // Powrót do Gameplay
                        backgroundMusic.play(); // Wznowienie muzyki tła dla gry
                        // Reset dino icon position
                        dinoSprite.setPosition(20, 104);
                        // Reset coin icon position
                        coinSprite.setPosition(20, 104 + dinoSprite.getGlobalBounds().height + 20);
                        // Reset coin counter position
                        coinCountText.setPosition(49 + coinSprite.getGlobalBounds().width, 99 + dinoSprite.getGlobalBounds().height + 20 + (coinSprite.getGlobalBounds().height / 2) - (coinCountText.getGlobalBounds().height / 2) - 20);
                        // Reset distance counter position
                        distanceText.setPosition(40 + dinoSprite.getGlobalBounds().width, 104 + (dinoSprite.getGlobalBounds().height / 2) - (distanceText.getGlobalBounds().height / 2) - 20);
                    }
                    else if (gameState == Shop) {
                        gameState = MainMenu; // Powrót do MainMenu
                        player.resetPosition();
                        player.resetSize();
                        player.setFrameDuration(0.1f);
                        player.setTexture(aktualna);
                        StatisticsManager::saveStatistics(coinCount, bestDistance, totalDistance, jumpCount, deathCount, gamesPlayed, cactusCount, birdCount, skinState, musicVolume, soundVolume, difficulty);
                        for (int i = 0;i<6;i++){
                            if (skinState[i] == 0){

                                //buyButton[i]->setText("Wybrano");
                                buyButtonSkin.setTexture("Tekstury/extinguished-przyciskUBRANO(small).png", "Tekstury/extinguished-przyciskUBRANO(small).png");
                            } else {
                                //buyButton[i]->setText("Skin" + std::to_string(i+1));
                                buyButtonSkin.setTexture("Tekstury/przyciskUBIERZ(small).png", "Tekstury/extinguished-przyciskUBIERZ(small).png");

                            }
                        }
                    }
                }
                if (event.key.code == sf::Keyboard::Q) {
                    window.close(); // Zamknięcie okna
                }

                if (gameState != MainMenu && gameState != OptionsMenu && gameState != Statistics && gameState != Achievements && gameState != Shop) { // Block 'R' key in MainMenu, OptionsMenu, Statistics, Achievements, and Shop
                    if (event.key.code == sf::Keyboard::R) {
                      player.resetPosition();
                      Player player(sf::Vector2f(80, 80), sf::Vector2f(100, 500), sf::Color::White, "Tekstury/skórki dino/dino_sprite_sheet.png", 3, 0.1f);
                        coinManager.restart();
                        cactusManager.restart();
                        birdManager.restart();
                        distance = 0.0f;
                        coinCount = 0.0f;
                        currentCoinCount = 0.0f; // Reset coin counter
                        gameState = Gameplay; // Restart gry
                        gamesPlayed++;
                        backgroundMusic.stop(); // Zatrzymanie muzyki tła
                        backgroundMusic.play(); // Odtwarzanie muzyki tła od nowa
                        // Reset dino icon position
                        dinoSprite.setPosition(20, 104);
                        // Reset coin icon position
                        coinSprite.setPosition(20, 104 + dinoSprite.getGlobalBounds().height + 20);
                        // Reset coin counter position
                        coinCountText.setPosition(49 + coinSprite.getGlobalBounds().width, 99 + dinoSprite.getGlobalBounds().height + 20 + (coinSprite.getGlobalBounds().height / 2) - (coinCountText.getGlobalBounds().height / 2) - 20);
                        // Reset distance counter position
                        distanceText.setPosition(40 + dinoSprite.getGlobalBounds().width, 104 + (dinoSprite.getGlobalBounds().height / 2) - (distanceText.getGlobalBounds().height / 2) - 20);
                    }
                }
                if (event.key.code == sf::Keyboard::Space || event.key.code == sf::Keyboard::Up) {
                    if (!isJumping && player.getGlobalBounds().top >= standardLevel && gameState == Gameplay) {
                        jumpSound.play(); // Odtwarzanie dźwięku skoku
                        jumpCount++;
                        isJumping = true;
                    }
                }
            }

            // Obsługa klawiszy w różnych stanach
            if (event.type == sf::Event::KeyReleased) {
                if (event.key.code == sf::Keyboard::Space || event.key.code == sf::Keyboard::Up) {
                    isJumping = false;
                }
            }

            // Obsługa przesuwania sliderów
            if (gameState == OptionsMenu) {
                musicSlider->update(event, window);
                soundSlider->update(event, window);
                musicVolume = musicSlider->getValue();
                soundVolume = soundSlider->getValue();
                menuMusic.setVolume(musicVolume);
                backgroundMusic.setVolume(musicVolume);
                buttonSound.setVolume(soundVolume);
                jumpSound.setVolume(soundVolume);
                deathSound.setVolume(soundVolume);
            }
        }

        // Logika w zależności od stanu gry
        if (gameState == Gameplay) {
            player.handleInput(deltaTime, cactusManager.getSpeed());
            player.update(deltaTime);
	        cactusManager.update(deltaTime);
	        birdManager.update(deltaTime);
	        std::vector<sf::FloatRect> allObstacles;
	        auto cactusBounds = cactusManager.getObstacleBounds();
	        auto birdBounds = birdManager.getObstacleBounds();
	        allObstacles.insert(allObstacles.end(), cactusBounds.begin(), cactusBounds.end());
            allObstacles.insert(allObstacles.end(), birdBounds.begin(), birdBounds.end());
	        coinManager.update(deltaTime, player.getGlobalBounds(), currentCoinCount, allObstacles);

            // Obliczanie odległości
            float cactusSpeed = cactusManager.getSpeed();
	        float birdSpeed = birdManager.getSpeed();
            float obstacleInitialSpeed = cactusManager.getInitialSpeed();
            float playerVelocity = player.getVelocity().x;
            distance += (deltaTime * (cactusSpeed + playerVelocity)) / obstacleInitialSpeed;
            
            // Zmiana prędkości co 50 jednostek
            int distanceInt = static_cast<int>(distance);
            if(distanceInt % 50 == 0 && distanceInt <= 1000 && if_changed_speed == false){
                cactusManager.setSpeed(cactusSpeed + 20);
		        birdManager.setSpeed(birdSpeed + 20);
                coinManager.setObstacleSpawnSpeed(cactusSpeed + 20);
                coinManager.setSpeed(cactusSpeed + 20);
                if_changed_speed = true;
            } else if(distanceInt % 50 != 0) {
                if_changed_speed = false;
            }
			// Zmiana rodzaju poziomu ze zwykłego na powietrzny i odwrotnie
			if(distanceInt % 25 == 0 && distanceInt != 0){
				if(distanceInt % 50 != 0){
					cactusManager.turnSkyLevelOn();
					birdManager.turnSkyLevelOn();
					coinManager.turnSkyLevelOn();
					player.turnSkyLevelOn();
				}else{
					cactusManager.turnSkyLevelOff();
					birdManager.turnSkyLevelOff();
					coinManager.turnSkyLevelOff();
				}
			}
			int playerFallDelay = 4; //do wyczucia
			if(((distanceInt-playerFallDelay) % 25 == 0) && ((distanceInt - playerFallDelay)% 50 == 0) && ((distanceInt- playerFallDelay)!=0)){
				player.turnSkyLevelOff();
			}

            distanceText.setString(L"" + std::to_wstring(distanceInt)); // Ustawienie tekstu odległości
            coinCountText.setString(L"" + std::to_wstring(currentCoinCount)); // Usunięcie napisu "Monety:"

            // Sprawdzenie kolizji i koniec gry lub restart
            if ((cactusManager.checkCollisions(player.getGlobalBounds())) || (birdManager.checkCollisions(player.getGlobalBounds()))) {
                deathCount++;
                totalDistance += static_cast<int>(distance); 
                gameState = GameOver;
                backgroundMusic.stop(); // Zatrzymanie muzyki tła dla gry
                deathSound.play(); // Odtwarzanie dźwięku śmierci
            }

            // Zliczanie miniętych kaktusów i ptaków
            float playerRight = player.getGlobalBounds().left + player.getGlobalBounds().width;

            if (passedCacti.size() > cactusBounds.size()) {
                passedCacti.assign(cactusBounds.size(), false);  
            } else if (passedCacti.size() < cactusBounds.size()){
                passedCacti.resize(cactusBounds.size(), false);
            }

            if (passedBirds.size() > birdBounds.size()) {
                passedBirds.assign(birdBounds.size(), false);  
            } else if (passedBirds.size() < birdBounds.size()){
                passedBirds.resize(birdBounds.size(), false);
            }

            for (size_t i = 0; i < cactusBounds.size(); ++i) {
                if (!passedCacti[i] && playerRight > cactusBounds[i].left + cactusBounds[i].width) {
                    cactusCount++;
                    passedCacti[i] = true;  
                }
            }

            // Zliczanie miniętych ptaków
            for (size_t i = 0; i < birdBounds.size(); ++i) {
                if (!passedBirds[i] && playerRight > birdBounds[i].left + birdBounds[i].width) {
                    birdCount++;
                    passedBirds[i] = true;  
                }
            }

        } else if (gameState == Shop) {
            player.updateAnimation(deltaTime);
            player.setFrameDuration(0.5f);
        }

        // Aktualizacja przycisków i sliderów zależnie od stanu
        if (gameState == MainMenu) {
            if (menuMusic.getStatus() != sf::Music::Playing) {
                menuMusic.play(); // Odtwarzanie muzyki tła dla menu
            }
            storyButton.update(sf::Mouse::getPosition(window));
            //endlessButton.update(sf::Mouse::getPosition(window));
            optionsButton.update(sf::Mouse::getPosition(window));
            achievementsButton.update(sf::Mouse::getPosition(window));
            statisticsButton.update(sf::Mouse::getPosition(window));
            exitButton.update(sf::Mouse::getPosition(window));
            shopButton.update(sf::Mouse::getPosition(window));
        } else if (gameState == OptionsMenu) {
            easyButton.update(sf::Mouse::getPosition(window));
            normalButton.update(sf::Mouse::getPosition(window));
            hardButton.update(sf::Mouse::getPosition(window));
            backButton.update(sf::Mouse::getPosition(window));
        } else if (gameState == Achievements) {
            backButton.update(sf::Mouse::getPosition(window));
        } else if (gameState == Statistics) {
            window.draw(statisticsBackgroundSprite); // Rysowanie tła trybu Statistics
            float leftMargin = 160.0f; // Distance from the left edge of the screen
            float rightMargin = 165.0f; // Distance from the right edge of the screen
            float yPosition = 100.0f; // Starting y position
            unsigned int fontSize = 40; // Reduced the font size by 5

            // Draw the statistics labels and counters
            for (size_t i = 0; i < statisticsLabels.size(); ++i) {
                sf::Text labelText(statisticsLabels[i], font, fontSize);
                labelText.setFillColor(sf::Color::Black); // Set the color of labelText to black
                labelText.setPosition(leftMargin, yPosition);
                window.draw(labelText);

                sf::Text counterText(statisticsCounters[i], font, fontSize);
                counterText.setFillColor(sf::Color::Black); // Set the color of counterText to black
                counterText.setPosition(window.getSize().x - rightMargin - counterText.getGlobalBounds().width, yPosition);
                window.draw(counterText);

                yPosition += labelText.getGlobalBounds().height + 10.0f; // Move to the next line
            }
            backButton.draw(window);
        } else if (gameState == Gameplay) {
            pauseButton.update(sf::Mouse::getPosition(window));
        } else if (gameState == Pause) {
            resumeButton.update(sf::Mouse::getPosition(window));
            restartButtonPause.update(sf::Mouse::getPosition(window));
            mainMenuButton.update(sf::Mouse::getPosition(window));
        } else if (gameState == GameOver) {

            window.clear(sf::Color::Black);
            window.draw(gameOverBackgroundSprite); // Rysowanie tła trybu GameOver
            restartButton.draw(window); // Draw the restart button for GameOver

  
            // Remove the exit button from the GameOver screen
            // exitButton.draw(window);

            // Center the distance and coin counters under the "Game Over" text
            bestScoreText.setString(L"Najlepszy wynik: " + std::to_wstring(bestDistance)); // Set the best score text
            float totalWidthBestScore = bestScoreText.getGlobalBounds().width + distanceText.getGlobalBounds().width + 20;
            float startXBestScore = (window.getSize().x - totalWidthBestScore) / 2;
            bestScoreText.setPosition(startXBestScore, gameOverText.getPosition().y + gameOverText.getGlobalBounds().height + 150);
            distanceText.setPosition(startXBestScore + bestScoreText.getGlobalBounds().width + 20, bestScoreText.getPosition().y);

            yourScoreText.setString(L"Twój wynik:"); // Change text to "Twój wynik:"
            float totalWidthYourScore = yourScoreText.getGlobalBounds().width + distanceText.getGlobalBounds().width + 20;
            float startXYourScore = (window.getSize().x - totalWidthYourScore) / 2 - 20; // Shift 20 pixels to the left
            yourScoreText.setPosition(startXYourScore, bestScoreText.getPosition().y + bestScoreText.getGlobalBounds().height + 10);
            distanceText.setPosition(startXYourScore + yourScoreText.getGlobalBounds().width + 20, yourScoreText.getPosition().y);

            coinsText.setString(L"Monety:"); // Change text to "Monety:"
            coinsText.setPosition((window.getSize().x - coinsText.getGlobalBounds().width) / 2 - 45, distanceText.getPosition().y + distanceText.getGlobalBounds().height + 8); // Podniesienie o 2 piksele
            coinCountText.setPosition(coinsText.getPosition().x + coinsText.getGlobalBounds().width + 20, coinsText.getPosition().y);

            distanceText.setFillColor(sf::Color::White); // Set the color of distanceText to white
            coinCountText.setFillColor(sf::Color::White); // Set the color of coinCountText to white

            window.draw(yourScoreText); // Draw the "Twój wynik:" text
            window.draw(distanceText); // Draw the distance counter
            window.draw(coinsText); // Draw the "Monety:" text
            window.draw(coinCountText); // Draw the coin counter
            window.draw(bestScoreText); // Draw the best score text
        } else if (gameState == Shop) {
            backButton.update(sf::Mouse::getPosition(window));
        }

        // Rysowanie
        window.clear(sf::Color::Black);

        if (gameState == MainMenu) {
            window.draw(backgroundSprite); // Rysowanie tła
            storyButton.draw(window);
            //endlessButton.draw(window);
            optionsButton.draw(window);
            achievementsButton.draw(window);
            statisticsButton.draw(window);
            exitButton.draw(window);
            shopButton.draw(window);
            window.draw(nameText);
            window.draw(coinIconMainMenu); // Draw the coin icon
            window.draw(coinCountMainMenuText); // Draw the coin count text
        } else if (gameState == OptionsMenu) {
            window.draw(optionsMenuText);
            window.draw(optionsMenuBackgroundSprite); // Rysowanie tła trybu OptionsMenu
            easyButton.draw(window);
            normalButton.draw(window);
            hardButton.draw(window);
            musicSlider->draw(window);
            soundSlider->draw(window);
            backButton.draw(window);
        } else if (gameState == Achievements) {
            window.draw(achievementsBackgroundSprite); // Rysowanie tła trybu Achievements
            float yPosition = 150.0f; // Starting y position for the frames, lowered by 50 pixels
            float xCenter = window.getSize().x / 2.0f; // Center of the window
            for (size_t i = 0; i < achievements.size(); ++i) {
                // Draw the achievement frame
                achievementFrameSprite.setPosition(xCenter - achievementFrameSprite.getGlobalBounds().width / 2, yPosition);
                window.draw(achievementFrameSprite);

                // Draw the achievement text
                sf::Text achievementText(achievements[i], font, 30);
                achievementText.setFillColor(sf::Color::Black);
                achievementText.setPosition(xCenter - achievementText.getGlobalBounds().width / 2, yPosition + 40); // Adjust position for scaled frame
                window.draw(achievementText);

                // Determine which texture to use based on the achievement range
                if (range_distance(totalDistance) == 1000) {
                    achievementSprite.setTexture(bronzeMedalTexture);
                } else if (range_distance(totalDistance) == 10000) {
                    achievementSprite.setTexture(silverMedalTexture);
                } else if (range_distance(totalDistance) == 100000) {
                    achievementSprite.setTexture(goldMedalTexture);
                }

                // Scale the achievement sprite to 1.2 times its size
                achievementSprite.setScale(1.2f, 1.2f);

                // Draw the achievement sprite at the same position as the achievement frame
                yPosition += achievementFrameSprite.getGlobalBounds().height + 5; // Move to the next frame position with 5 pixels gap
            }
            backButton.draw(window);
        } else if (gameState == Statistics) {
            window.draw(statisticsBackgroundSprite); // Rysowanie tła trybu Statistics
            float leftMargin = 160.0f; // Distance from the left edge of the screen
            float rightMargin = 165.0f; // Distance from the right edge of the screen
            float yPosition = 100.0f; // Starting y position
            unsigned int fontSize = 40; // Reduced the font size by 5

            // Draw the statistics labels and counters
            for (size_t i = 0; i < statisticsLabels.size(); ++i) {
                sf::Text labelText(statisticsLabels[i], font, fontSize);
                labelText.setFillColor(sf::Color::Black); // Set the color of labelText to black
                labelText.setPosition(leftMargin, yPosition);
                window.draw(labelText);

                sf::Text counterText(statisticsCounters[i], font, fontSize);
                counterText.setFillColor(sf::Color::Black); // Set the color of counterText to black
                counterText.setPosition(window.getSize().x - rightMargin - counterText.getGlobalBounds().width, yPosition);
                window.draw(counterText);

                yPosition += labelText.getGlobalBounds().height + 10.0f; // Move to the next line
            }
            backButton.draw(window);
        } else if (gameState == Gameplay) {
            window.draw(storyBackgroundSprite); // Rysowanie tła trybu fabularnego
            player.draw(window); // Rysowanie gracza
	        cactusManager.draw(window);  // Rysowanie przeszkód
            coinManager.draw(window);
            birdManager.draw(window);
            // window.draw(gameplayText); // Usunięcie napisu "Gejmplej!"
            pauseButton.draw(window);
            window.draw(dinoSprite); // Rysowanie dino2
            window.draw(distanceText); // Rysowanie tekstu odległości
            window.draw(coinSprite); // Rysowanie monety
            window.draw(coinCountText); // Rysowanie tekstu obok monety
        } else if (gameState == Pause) {
            window.draw(pauseText);
            window.draw(pauseBackgroundSprite); // Rysowanie tła trybu Pause
            resumeButton.draw(window);
            restartButtonPause.draw(window); // Draw the restart button for Pause
            mainMenuButton.draw(window); // Draw the main menu button for Pause

            // Move the dino icon to coordinates (375, 400) on the pause screen
            dinoSprite.setPosition(375, 400);

            // Move the distance counter to coordinates (445, 385) on the pause screen
            distanceText.setPosition(445, 385);

            // Move the coin icon to coordinates (625, 409) on the pause screen
            coinSprite.setPosition(625, 409);

            // Move the coin counter to coordinates (695, 385) on the pause screen
            coinCountText.setPosition(695, 385);

            window.draw(dinoSprite); // Draw the dino icon
            window.draw(distanceText); // Draw the distance counter
            window.draw(coinSprite); // Draw the coin icon
            window.draw(coinCountText); // Draw the coin counter
        } else if (gameState == GameOver) {
            window.clear(sf::Color::Black);
            window.draw(gameOverBackgroundSprite); // Rysowanie tła trybu GameOver
            restartButton.draw(window); // Draw the restart button for GameOver
			mainMenuButtonOver.draw(window);
            // Remove the exit button from the GameOver screen
            // exitButton.draw(window);

            // Center the distance and coin counters under the "Game Over" text
            bestScoreText.setString(L"Najlepszy wynik: " + std::to_wstring(bestDistance)); // Set the best score text
            float totalWidthBestScore = bestScoreText.getGlobalBounds().width + distanceText.getGlobalBounds().width + 20;
            float startXBestScore = (window.getSize().x - totalWidthBestScore) / 2;
            bestScoreText.setPosition(startXBestScore, gameOverText.getPosition().y + gameOverText.getGlobalBounds().height + 150);
            distanceText.setPosition(startXBestScore + bestScoreText.getGlobalBounds().width + 20, bestScoreText.getPosition().y);

            yourScoreText.setString(L"Twój wynik:"); // Change text to "Twój wynik:"
            float totalWidthYourScore = yourScoreText.getGlobalBounds().width + distanceText.getGlobalBounds().width + 20;
            float startXYourScore = (window.getSize().x - totalWidthYourScore) / 2 - 20; // Shift 20 pixels to the left
            yourScoreText.setPosition(startXYourScore, bestScoreText.getPosition().y + bestScoreText.getGlobalBounds().height + 10);
            distanceText.setPosition(startXYourScore + yourScoreText.getGlobalBounds().width + 20, yourScoreText.getPosition().y);

            coinsText.setString(L"Monety:"); // Change text to "Monety:"
            coinsText.setPosition((window.getSize().x - coinsText.getGlobalBounds().width) / 2 - 45, distanceText.getPosition().y + distanceText.getGlobalBounds().height + 8); // Podniesienie o 2 piksele
            coinCountText.setPosition(coinsText.getPosition().x + coinsText.getGlobalBounds().width + 20, coinsText.getPosition().y);

            distanceText.setFillColor(sf::Color::White); // Set the color of distanceText to white
            coinCountText.setFillColor(sf::Color::White); // Set the color of coinCountText to white

            window.draw(yourScoreText); // Draw the "Twój wynik:" text
            window.draw(distanceText); // Draw the distance counter
            window.draw(coinsText); // Draw the "Monety:" text
            window.draw(coinCountText); // Draw the coin counter
            window.draw(bestScoreText); // Draw the best score text
        } else if (gameState == Shop) {
            window.draw(shopBackgroundSprite); // Rysowanie tła trybu Shop
            window.draw(coinIconMainMenu); // Draw the coin icon
            window.draw(coinCountMainMenuText); // Draw the coin count text
            backButton.draw(window);
            buyButtonSkin.draw(window);
            for (int i = 0; i < 6; i++) {
                buyButton[i]->draw(window);
            }
            window.draw(dinoFrameSprite); // Rysowanie tekstury dino-frame(big) pod player
            player.draw(window);
        }
        window.display();
    }

    // sound.stop();
    // music.stop();

    for (int i = 0; i < 6; i++) {
        delete buyButton[i];
    }
    delete musicSlider;
    delete soundSlider;
    return 0;
}


