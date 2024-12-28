#include <SFML/Graphics.hpp>
#include <iostream>

enum GameState { MainMenu, OptionsMenu, Gameplay, Pause };
class Player {
public:
    Player(const sf::Vector2f& size, const sf::Vector2f& position, const sf::Color& color) {
        player.setSize(size);
        player.setPosition(position);
        player.setFillColor(color);
        velocity = sf::Vector2f(0.f, 0.f);
        isJumping = false;
        isHoldingJump = false;
        jumpHoldTime = 0.f;
    }

    // Copy assignment operator
    Player& operator=(const Player& other) {
        if (this != &other) {
            player = other.player;
            velocity = other.velocity;
            isJumping = other.isJumping;
            isHoldingJump = other.isHoldingJump;
            jumpHoldTime = other.jumpHoldTime;
        }
        return *this;
    }

    void handleInput(float deltaTime) {
        // Ruch w lewo i prawo
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            velocity.x = -moveSpeed;
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            velocity.x = moveSpeed;
        } else {
            velocity.x = 0.f;
        }

        // Skakanie - wykrywanie przytrzymania klawisza
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            if (!isJumping) {
                velocity.y = -baseJumpSpeed;
                isJumping = true;
                isHoldingJump = true;
                jumpHoldTime = 0.f;
            } else if (isHoldingJump && jumpHoldTime < maxHoldTime) {
                jumpHoldTime += deltaTime;
                velocity.y = std::max(velocity.y - jumpBoost * deltaTime, -maxJumpSpeed);
            }
        } else {
            isHoldingJump = false;
        }
    }

    void update(float deltaTime) {
        // Grawitacja
        velocity.y += gravity * deltaTime;

        // Poruszanie graczem
        player.move(velocity * deltaTime);

        // Ograniczenie pozycji do "podłogi"
        if (player.getPosition().y + player.getSize().y >= groundHeight) {
            player.setPosition(player.getPosition().x, groundHeight - player.getSize().y);
            velocity.y = 0.f;
            isJumping = false;
        }
    }

    void draw(sf::RenderWindow& window) {
        window.draw(player);
    }

private:
    sf::RectangleShape player;
    sf::Vector2f velocity;
    bool isJumping;
    bool isHoldingJump;
    float jumpHoldTime;

    const float moveSpeed = 200.f;
    const float baseJumpSpeed = 250.f; // Bazowa prędkość skoku
    const float maxJumpSpeed = 500.f; // Maksymalna prędkość w górę
    const float gravity = 981.f;
    const float groundHeight = 600.f;
    const float maxHoldTime = 1.0f; // Maksymalny czas trzymania klawisza (sekundy)
    const float jumpBoost = 500.f; // Dodatkowe przyspieszenie przy trzymaniu klawisza
};

class Button {
public:
    Button(const sf::Vector2f& size, const sf::Vector2f& position, const sf::String& text, const sf::Font& font) {
        button.setSize(size);
        button.setPosition(position);
        button.setFillColor(normalColor);

        buttonText.setFont(font); 
        buttonText.setString(text);
        buttonText.setCharacterSize(50);
        buttonText.setFillColor(sf::Color::White);
        buttonText.setPosition(
            position.x + (size.x - buttonText.getLocalBounds().width) / 2,
            position.y + (size.y - buttonText.getLocalBounds().height) / 2
        );
    }

    void draw(sf::RenderWindow& window) {
        window.draw(button);
        window.draw(buttonText);
    }

    void update(const sf::Vector2i& mousePos) {
        if (button.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
            button.setFillColor(hoverColor);
        } else {
            button.setFillColor(normalColor);
        }
    }

    bool isClicked(const sf::Vector2i& mousePos, sf::Event::MouseButtonEvent mouseEvent) {
        return button.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos)) &&
               mouseEvent.button == sf::Mouse::Left;
    }

private:
    sf::RectangleShape button;
    sf::Text buttonText;
    sf::Color normalColor = sf::Color::Blue;
    sf::Color hoverColor = sf::Color::Green;
};

int main() {
    // Tworzenie okna
    sf::RenderWindow window(sf::VideoMode(1200, 640), "Dino Game");
    window.setFramerateLimit(60); // Ograniczenie FPS do 60

    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Nie udało się załadować czcionki!" << std::endl;
        return -1;
    }

    // Ustawienie domyślnego stanu gry
    GameState gameState = MainMenu;

    // Tworzenie przycisków
    Button playButton(sf::Vector2f(200, 50), sf::Vector2f(500, 250), "Graj", font);
    Button optionsButton(sf::Vector2f(200, 50), sf::Vector2f(500, 320), "Opcje", font);
    Button backButton(sf::Vector2f(200, 50), sf::Vector2f(500, 400), "Powrót", font);
    Button exitButton(sf::Vector2f(200, 50), sf::Vector2f(500, 470), "Wyjście", font);
    Button pauseButton(sf::Vector2f(200, 50), sf::Vector2f(10, 10), "Pause", font);
    Button resumeButton(sf::Vector2f(200, 50), sf::Vector2f(10, 10), "Wznów", font);
    Button restartButton(sf::Vector2f(200, 50), sf::Vector2f(500, 320), "Restart", font);
    Button mainMenuButton(sf::Vector2f(200, 50), sf::Vector2f(500, 390), "Menu Główne", font);

    // Tworzenie gracza
    Player player(sf::Vector2f(50.f, 50.f), sf::Vector2f(100.f, 500.f), sf::Color::Cyan);

    // Teksty do wyświetlania
    sf::Text optionsMenuText("Menu Opcji", font, 50);
    optionsMenuText.setPosition(500, 100);

    sf::Text gameplayText("Gejmplej!", font, 50);
    gameplayText.setPosition(500, 200);

    sf::Text pauseText("Pause", font, 50);
    pauseText.setPosition(500, 200);

    sf::Text nameText("Dino Game", font, 50);
    nameText.setPosition(500, 50);

    float deltaTime;
    sf::Clock clock;

    // Główna pętla gry
    while (window.isOpen()) {
        deltaTime = clock.restart().asSeconds();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            // Obsługa kliknięć w różnych stanach
            if (event.type == sf::Event::MouseButtonPressed) {
                if (gameState == MainMenu) {
                    if (playButton.isClicked(sf::Mouse::getPosition(window), event.mouseButton)) {
                        gameState = Gameplay; // Przejście do Gameplay
                    }
                    if (optionsButton.isClicked(sf::Mouse::getPosition(window), event.mouseButton)) {
                        gameState = OptionsMenu; // Przejście do OptionsMenu
                    }
                    if (exitButton.isClicked(sf::Mouse::getPosition(window), event.mouseButton)) {
                        window.close(); // Zamknięcie okna
                    }
                } else if (gameState == OptionsMenu) {
                    if (backButton.isClicked(sf::Mouse::getPosition(window), event.mouseButton)) {
                        gameState = MainMenu; // Powrót do MainMenu
                    }
                } else if (gameState == Gameplay) {
                    if (pauseButton.isClicked(sf::Mouse::getPosition(window), event.mouseButton)) {
                        gameState = Pause; // Przejście do Pause
                    }
                } else if (gameState == Pause) {
                    if (resumeButton.isClicked(sf::Mouse::getPosition(window), event.mouseButton)) {
                        gameState = Gameplay; // Powrót do Gameplay
                    }
                    if (restartButton.isClicked(sf::Mouse::getPosition(window), event.mouseButton)) {
                        player = Player(sf::Vector2f(50.f, 50.f), sf::Vector2f(100.f, 500.f), sf::Color::Cyan);
                        gameState = Gameplay; // Restart gry
                    }
                    if (mainMenuButton.isClicked(sf::Mouse::getPosition(window), event.mouseButton)) {
                        gameState = MainMenu; // Powrót do MainMenu
                    }
                }
            }

            // Obsługa klawiszy w różnych stanach
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    if (gameState == OptionsMenu) {
                        gameState = MainMenu; // Powrót do MainMenu
                    }
                    if (gameState == Gameplay) {
                        gameState = Pause; // Przejście do Pause
                    } else if (gameState == Pause) {
                        gameState = Gameplay; // Powrót do Gameplay
                    }
                }
                if (event.key.code == sf::Keyboard::Q) {
                    window.close(); // Zamknięcie okna
                }
                if (event.key.code == sf::Keyboard::R) {
                    player = Player(sf::Vector2f(50.f, 50.f), sf::Vector2f(100.f, 500.f), sf::Color::Cyan);
                    gameState = Gameplay; // Restart gry
                }
                if (event.key.code == sf::Keyboard::P) {
                    gameState = Pause; // Przejście do Pause
                }
            }
        }

        // Logika w zależności od stanu gry
        if (gameState == Gameplay) {
            player.handleInput(deltaTime);
            player.update(deltaTime);
        }

        // Aktualizacja przycisków zależnie od stanu
        if (gameState == MainMenu) {
            playButton.update(sf::Mouse::getPosition(window));
            optionsButton.update(sf::Mouse::getPosition(window));
            exitButton.update(sf::Mouse::getPosition(window));
        } else if (gameState == OptionsMenu) {
            backButton.update(sf::Mouse::getPosition(window));
        } else if (gameState == Gameplay) {
            pauseButton.update(sf::Mouse::getPosition(window));
        } else if (gameState == Pause) {
            resumeButton.update(sf::Mouse::getPosition(window));
            restartButton.update(sf::Mouse::getPosition(window));
            mainMenuButton.update(sf::Mouse::getPosition(window));
        }

        // Rysowanie
        window.clear(sf::Color::Black);

        if (gameState == MainMenu) {
            playButton.draw(window);
            optionsButton.draw(window);
            exitButton.draw(window);
            window.draw(nameText);
        } else if (gameState == OptionsMenu) {
            window.draw(optionsMenuText);
            backButton.draw(window);
        } else if (gameState == Gameplay) {
            player.draw(window); // Rysowanie gracza
            window.draw(gameplayText);
            pauseButton.draw(window);
        } else if (gameState == Pause) {
            window.draw(pauseText);
            resumeButton.draw(window);
            restartButton.draw(window);
            mainMenuButton.draw(window);
        }

        window.display();
    }

    return 0;
}


