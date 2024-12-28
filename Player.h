#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>

class Player {
public:
    Player(const sf::Vector2f& size, const sf::Vector2f& position, const sf::Color& color);
    Player& operator=(const Player& other);

    void handleInput(float deltaTime);
    void update(float deltaTime);
    void draw(sf::RenderWindow& window);

private:
    sf::RectangleShape player;
    sf::Vector2f velocity;
    bool isJumping;
    bool isHoldingJump;
    float jumpHoldTime;

    const float moveSpeed = 200.f;
    const float baseJumpSpeed = 250.f;
    const float maxJumpSpeed = 500.f;
    const float gravity = 981.f;
    const float groundHeight = 600.f;
    const float maxHoldTime = 1.0f;
    const float jumpBoost = 500.f;
};

#endif

