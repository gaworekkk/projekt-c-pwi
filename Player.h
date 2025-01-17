#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>

class Player {
public:
    Player(const sf::Vector2f& size, const sf::Vector2f& position, const sf::Color& color, const std::string& textureFile, int frameCount, float frameDuration);
    Player& operator=(const Player& other);
    void handleInput(float deltaTime);
    void update(float deltaTime);
    void draw(sf::RenderWindow& window);
    void resetPosition(); // Nowa metoda
	void turnSkyLevelOn();
	void turnSkyLevelOff();
    void setPosition(const sf::Vector2f& position);
    void setSize(const sf::Vector2f& size);
    void resetSize();
    void updateAnimation(float deltaTime);
    void setFrameDuration(float newFrameDuration);
    void setTexture(const std::string& textureFile);
    void setTexture(sf::Texture texture);
    sf::Texture getTexture();

   sf::FloatRect getGlobalBounds();
   sf::Vector2f getVelocity() const;
private:
    bool isSkyLevelOn;
    sf::RectangleShape player;
    sf::Texture playerTexture;
    sf::IntRect frameRect;
    int frameCount;
    float frameDuration;
    float currentFrameTime;
    int currentFrame;
    
    sf::Vector2f velocity;
    bool isJumping;
    bool isHoldingJump;
    float jumpHoldTime;
    bool isCrouching;               
    sf::Vector2f originalSize;       
    sf::Vector2f crouchSize;          

    const float crouchSpeedFactor = 5.f;
    const float moveSpeed = 200.f;
    const float baseJumpSpeed = 250.f;
    const float maxJumpSpeed = 500.f;
    const float gravity = 981.f;
    const float groundHeight = 589.f;
	const float skyHeight = 0.f;
    const float screenWidth = 1200;
    const float maxHoldTime = 1.0f;
    const float jumpBoost = 800.f;
    sf::Vector2f initialPosition; // Nowa zmienna
};

#endif

