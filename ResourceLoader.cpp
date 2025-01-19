#include "ResourceLoader.h"
// Deklaracje zasobów
// Tła
sf::Texture backgroundTexture;
sf::Sprite backgroundSprite;
sf::Texture storyBackgroundTexture;
sf::Sprite storyBackgroundSprite;
sf::Texture gameOverBackgroundTexture;
sf::Sprite gameOverBackgroundSprite;
sf::Texture statisticsBackgroundTexture;
sf::Sprite statisticsBackgroundSprite;
sf::Texture achievementsBackgroundTexture;
sf::Sprite achievementsBackgroundSprite;
sf::Texture optionsMenuBackgroundTexture;
sf::Sprite optionsMenuBackgroundSprite;
sf::Texture pauseBackgroundTexture;
sf::Sprite pauseBackgroundSprite;
sf::Texture shopBackgroundTexture;
sf::Sprite shopBackgroundSprite;

// Dino
sf::Texture dinoTexture;
sf::Sprite dinoSprite;
std::string dinoTexturePath[6] = {"Tekstury/skórki dino/dino2(run).gif", "Tekstury/skórki dino/dino 4 (klatki)/dino_sprite_sheet.png", "Tekstury/skórki dino/dino6(run).gif", "Tekstury/skórki dino/dino6(run).gif", "Tekstury/skórki dino/dino7(run).gif", "Tekstury/skórki dino/dino8(run).gif"};

// Czcionka
sf::Font font;

// Slidery
Slider *musicSlider;
Slider *soundSlider;

// Muzyka i dźwięki
sf::Music menuMusic;
sf::Music backgroundMusic;
sf::SoundBuffer deathBuffer;
sf::Sound deathSound;
sf::SoundBuffer jumpBuffer;
sf::Sound jumpSound;
sf::SoundBuffer buttonBuffer;
sf::Sound buttonSound;

// Przyciski
Button storyButton(sf::Vector2f(324, 54), sf::Vector2f(250, 372), " ", font);
Button endlessButton(sf::Vector2f(324, 54), sf::Vector2f(626, 372), " ", font);
Button optionsButton(sf::Vector2f(54, 54), sf::Vector2f(209, 486), " ", font);
Button achievementsButton(sf::Vector2f(54, 54), sf::Vector2f(391, 486), " ", font);
Button statisticsButton(sf::Vector2f(54, 54), sf::Vector2f(573, 486), " ", font);
Button backButton(sf::Vector2f(200, 50), sf::Vector2f(500, 400), L"Powrót", font);
Button exitButton(sf::Vector2f(54, 54), sf::Vector2f(846, 486), " ", font);
Button pauseButton(sf::Vector2f(54, 54), sf::Vector2f(80, 40), " ", font);
Button resumeButton(sf::Vector2f(150, 142), sf::Vector2f(530, 252), " ", font);
Button restartButton(sf::Vector2f(200, 40), sf::Vector2f(375, 490), " ", font);
Button restartButtonPause(sf::Vector2f(200, 40), sf::Vector2f(375, 480), " ", font);
Button mainMenuButton(sf::Vector2f(200, 40), sf::Vector2f(625, 480), " ", font);
Button easyButton(sf::Vector2f(324, 54), sf::Vector2f(450, 120), " ", font);
Button normalButton(sf::Vector2f(324, 54), sf::Vector2f(450, 180), " ", font);
Button hardButton(sf::Vector2f(324, 54), sf::Vector2f(450, 240), " ", font);
Button shopButton(sf::Vector2f(54, 54), sf::Vector2f(755, 486), " ", font);
Button buyButtonSkin(sf::Vector2f(200, 40), sf::Vector2f(800, 400), "Kup", font);

// Moneta
sf::Texture coinTexture;
sf::Sprite coinSprite;
sf::Sprite coinIconMainMenu;

// Gracz
Player player(sf::Vector2f(80, 80), sf::Vector2f(100, 500), sf::Color::White, "Tekstury/skórki dino/dino_sprite_sheet.png", 3, 0.1f); // 3 klatki animacji, 0.1 sekundy na klatkę

// Generatory
ObstacleManager cactusManager(1, 1, "cactus");
ObstacleManager birdManager(1, 1, "bird");
CoinManager coinManager(2.0f, cactusManager.getInitialSpeed());

// Teksty
sf::Text nameText;
sf::Text optionsMenuText;
sf::Text pauseText;
sf::Text distanceText;
sf::Text coinCountText;
sf::Text coinCountMainMenuText;
sf::Text gameOverText;
sf::Text distanceText2;
sf::Text bestScoreText;
sf::Text yourScoreText;
sf::Text coinsText;

void loadOneBackground(sf::Texture& texture, sf::Sprite& sprite, const std::string& filePath, sf::RenderWindow& window) {
    // Załaduj teksturę tła
    if (!texture.loadFromFile(filePath)) {
        throw std::runtime_error("Nie udało się załadować tekstury: " + filePath);
    }
    
    // Sprite z tekstury tła
    sprite.setTexture(texture);
    
    // Oblicz skalę dla tła
    float scaleX = static_cast<float>(window.getSize().x) / texture.getSize().x;
    float scaleY = static_cast<float>(window.getSize().y) / texture.getSize().y;
    sprite.setScale(scaleX, scaleY);

}

void loadBackgrounds(sf::RenderWindow& window){
    loadOneBackground(backgroundTexture, backgroundSprite, "Tekstury/menu.png", window);
    loadOneBackground(storyBackgroundTexture, storyBackgroundSprite, "Tekstury/background-dino.png", window);
    loadOneBackground(gameOverBackgroundTexture, gameOverBackgroundSprite, "Tekstury/GAMEOVER-background.png", window);
    loadOneBackground(statisticsBackgroundTexture, statisticsBackgroundSprite, "Tekstury/statistic-menu.png", window);
    loadOneBackground(achievementsBackgroundTexture, achievementsBackgroundSprite, "Tekstury/archivment-menu.png", window);
    loadOneBackground(optionsMenuBackgroundTexture, optionsMenuBackgroundSprite, "Tekstury/settings-menu.png", window);
    loadOneBackground(pauseBackgroundTexture, pauseBackgroundSprite, "Tekstury/pause-background.png", window);
    loadOneBackground(shopBackgroundTexture, shopBackgroundSprite, "Tekstury/shop-menu.png", window);
}

void loadDino(){
    // Załaduj teksturę dino
    if (!dinoTexture.loadFromFile("Tekstury/skórki dino/dino2.png")) {
        throw std::runtime_error("Nie udało się załadować tekstury dino2!");
    }
    
    // Utwórz sprite z tekstury dino
    dinoSprite.setTexture(dinoTexture);
    dinoSprite.setPosition(20, 104); // Przeniesienie dino do lewej krawędzi na wysokość 104 pikseli
    dinoSprite.setScale(1.4f, 1.4f); // Powiększenie grafiki 1.256-krotnie
}

void loadFont(){
    // Załaduj czcionkę
    if (!font.loadFromFile("Pi.ttf")) {
        throw std::runtime_error("Nie udało się załadować czcionki!");
    }
}

void loadSliders(float musicVolume, float soundVolume){
    // Ustawienia dla sliderów
    musicSlider = new Slider(545, 313, 110, 20, musicVolume, font);
    soundSlider = new Slider(545, 351, 110, 20, soundVolume, font);
}

void loadOneMusic(sf::Music& music, const std::string& filePath){
    // Załaduj muzykę
    if (!music.openFromFile(filePath)) {
        throw std::runtime_error("Nie udało się załadować muzyki: " + filePath);
    }
    music.setLoop(true); // Ustawienie muzyki na pętlę
}

void loadMusic(){
    loadOneMusic(menuMusic, "muzyka/muzka-do-menu1.wav");
    loadOneMusic(backgroundMusic, "muzyka/muzykadogry.wav");
}

void loadOneBuffer(sf::SoundBuffer& buffer, sf::Sound& sound, const std::string& filePath){
    // Załaduj dźwięk
    if (!buffer.loadFromFile(filePath)) {
        throw std::runtime_error("Nie udało się załadować dźwięku: " + filePath);
    }
    sound.setBuffer(buffer);
}

void loadBuffers(){
    loadOneBuffer(deathBuffer, deathSound, "muzyka/smierc2.ogg");
    loadOneBuffer(jumpBuffer, jumpSound, "muzyka/skok.ogg");
    loadOneBuffer(buttonBuffer, buttonSound, "muzyka/przycisk2.ogg");
}

Button* buyButton[6];

void loadButtons(){
    // Ustawienia przycisków
    storyButton = Button(sf::Vector2f(324, 54), sf::Vector2f(250, 372), " ", font);
    storyButton.setTexture("Tekstury/przyciskSTART.png", "Tekstury/kliknięte przyciski/clicked-przyciskSTART.png");
    
    endlessButton = Button(sf::Vector2f(324, 54), sf::Vector2f(626, 372), " ", font);
    endlessButton.setTexture("Tekstury/przyciskENDLESS.png", "Tekstury/kliknięte przyciski/clicked-przyciskENDLESS.png");
    
    optionsButton = Button(sf::Vector2f(54, 54), sf::Vector2f(209, 486), " ", font);
    optionsButton.setTexture("Tekstury/settings.png", "Tekstury/kliknięte przyciski/clicked-settings.png");
    
    achievementsButton = Button(sf::Vector2f(54, 54), sf::Vector2f(391, 486), " ", font);
    achievementsButton.setTexture("Tekstury/archievment.png", "Tekstury/kliknięte przyciski/clicked-archievment.png");
    
    statisticsButton = Button(sf::Vector2f(54, 54), sf::Vector2f(573, 486), " ", font);
    statisticsButton.setTexture("Tekstury/statistic.png", "Tekstury/kliknięte przyciski/clicked-statistic.png");
    
    backButton = Button(sf::Vector2f(200, 40), sf::Vector2f(500, 400), " ", font);
    backButton.setTexture("Tekstury/przyciskPowrot.png", "Tekstury/kliknięte przyciski/clicked-przyciskPowrot.png");
    
    exitButton = Button(sf::Vector2f(54, 54), sf::Vector2f(937, 486), " ", font);
    exitButton.setTexture("Tekstury/exit.png", "Tekstury/kliknięte przyciski/clicked-exit.png");
    
    pauseButton = Button(sf::Vector2f(54, 54), sf::Vector2f(80, 40), " ", font);
    pauseButton.setTexture("Tekstury/PAUSE.png", "Tekstury/kliknięte przyciski/clicked-PAUSE.png" );
    
    resumeButton = Button(sf::Vector2f(150, 142), sf::Vector2f(530, 252), " ", font);
    resumeButton.setTexture("Tekstury/PAUSE.png", "Tekstury/kliknięte przyciski/clicked-PAUSE.png");
    
    restartButton = Button(sf::Vector2f(200, 40), sf::Vector2f(375, 490), " ", font);
    restartButton.setTexture("Tekstury/przyciskRESTART.png", "Tekstury/kliknięte przyciski/clicked-przyciskRESTART.png");
    
    restartButtonPause = Button(sf::Vector2f(200, 40), sf::Vector2f(375, 480), " ", font);
    restartButtonPause.setTexture("Tekstury/przyciskRESTART.png", "Tekstury/kliknięte przyciski/clicked-przyciskRESTART.png");
    
    mainMenuButton = Button(sf::Vector2f(200, 40), sf::Vector2f(625, 480), " ", font);
    mainMenuButton.setTexture("Tekstury/przyciskMenuGlowne.png", "Tekstury/kliknięte przyciski/clicked-przyciskMenuGlowne.png");
    
    easyButton = Button(sf::Vector2f(324, 54), sf::Vector2f(450, 120), " ", font);
    easyButton.setTexture("Tekstury/przyciskLatwy.png", "Tekstury/kliknięte przyciski/clicked-przyciskLatwy.png");
    
    normalButton = Button(sf::Vector2f(324, 54), sf::Vector2f(450, 180), " ", font);
    normalButton.setTexture("Tekstury/przyciskSREDNI.png", "Tekstury/kliknięte przyciski/clicked-przyciskSREDNI.png");
    
    hardButton = Button(sf::Vector2f(324, 54), sf::Vector2f(450, 240), " ", font);
    hardButton.setTexture("Tekstury/przyciskTRUDNY.png", "Tekstury/kliknięte przyciski/clicked-przyciskTRUDNY.png");

    shopButton = Button(sf::Vector2f(54, 54), sf::Vector2f(755, 486), " ", font);
    shopButton.setTexture("Tekstury/shop.png", "Tekstury/kliknięte przyciski/clicked-shop.png");

    buyButtonSkin = Button(sf::Vector2f(200, 40), sf::Vector2f(800, 400), "Kup", font);

    sf::Vector2f sizes[6] = {
        {200, 50}, {200, 50}, {200, 50}, {200, 50}, {200, 50}, {200, 50}
    };
    sf::Vector2f positions[6] = {
        {200, 120}, {200, 220}, {200, 320}, {500, 120}, {500, 220}, {500, 320}
    };

    std::wstring labels[6] = {
        L"Skin1", L"Skin2", L"Skin3", L"Skin4", L"Skin5", L"Skin6"
    };

    for (int i = 0; i < 6; ++i) {
        buyButton[i] =new Button(sizes[i], positions[i], labels[i], font);
    }
}

void loadCoins(){
    // Załaduj teksturę monety
    if (!coinTexture.loadFromFile("Tekstury/coin.png")) {
        throw std::runtime_error("Nie udało się załadować tekstury monety!");
    }

    // Sprite z tekstury monety
    coinSprite.setTexture(coinTexture);
    coinSprite.setPosition(20, 104 + dinoSprite.getGlobalBounds().height + 20); // Przeniesienie monety poniżej dino
    coinSprite.setScale(3.15f, 3.15f); // Zmniejszenie grafiki o 10 procent

    // Sprite z tekstury monety dla menu
    coinIconMainMenu = coinSprite;
    coinIconMainMenu.setScale(coinSprite.getScale().x * 1.5, coinSprite.getScale().y * 1.5);
    coinIconMainMenu.setPosition(20, 20);

}

void loadGenerators(sf::RenderWindow& window){
    // Tworzenie generatora przeszkód
    cactusManager = ObstacleManager(window.getSize().x, window.getSize().y, "cactus");
    birdManager = ObstacleManager(window.getSize().x, window.getSize().y, "bird");
    cactusManager.setDifficulty(Normal);
    birdManager.setDifficulty(Normal);
    // Tworzenie menedzera monet
    coinManager = CoinManager(2.0f, cactusManager.getInitialSpeed());
}

void loadTexts(int coinCount){
    // Ustawienia tekstu
    nameText = sf::Text(L" ", font, 70);

    // Wyśrodkowanie tekstu
    centerText(nameText, 1200, 640, 60);  // Wyśrodkowanie w poziomie, wysokość 60

    optionsMenuText = sf::Text(L"Menu Opcji", font, 50);
    centerText(optionsMenuText, 1200, 640, 100);

    pauseText = sf::Text(L"Pause", font, 50);
    centerText(pauseText, 1200, 640, 200);
    
    distanceText = sf::Text(L"0", font, 50);
    distanceText.setFillColor(sf::Color::White);
    distanceText.setCharacterSize(distanceText.getCharacterSize() + 20);
    distanceText.setPosition(40 + dinoSprite.getGlobalBounds().width, 104 + (dinoSprite.getGlobalBounds().height / 2) - (distanceText.getGlobalBounds().height / 2) - 20);

    coinCountText = sf::Text(L"0", font, 50);
    coinCountText.setFillColor(sf::Color::White);
    coinCountText.setCharacterSize(distanceText.getCharacterSize());
    coinCountText.setPosition(49 + coinSprite.getGlobalBounds().width, 99 + dinoSprite.getGlobalBounds().height + 20 + (coinSprite.getGlobalBounds().height / 2) - (coinCountText.getGlobalBounds().height / 2) - 20);

    coinCountMainMenuText = sf::Text(L"0", font, 80);
    coinCountMainMenuText.setCharacterSize(coinCountMainMenuText.getCharacterSize() );
    coinCountMainMenuText.setPosition(110, 0);
    coinCountMainMenuText.setString(std::to_wstring(coinCount));

    gameOverText = sf::Text(L"Game Over", font, 70);
    centerText(gameOverText, 1200, 640, 100);

    distanceText2 = distanceText;
    distanceText2.setFillColor(sf::Color::Black);

    bestScoreText = sf::Text(L"NAJLEPSZY WYNIK:", font, 70);
    bestScoreText.setFillColor(sf::Color::White);

    yourScoreText = sf::Text(L"TWÓJ WYNIK:", font, 70);
    yourScoreText.setFillColor(sf::Color::White);

    coinsText = sf::Text(L"MONETY:", font, 70);
    coinsText.setFillColor(sf::Color::White);
}

void loadAllResources(sf::RenderWindow& window, int coinCount, float musicVolume, float soundVolume) {
    loadBackgrounds(window);
    loadDino();
    loadFont();
    loadSliders(musicVolume, soundVolume);
    loadMusic();
    menuMusic.play();
    loadBuffers();
    loadCoins();
    loadButtons();
    loadGenerators(window);
    loadTexts(coinCount);
}
