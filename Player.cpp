#include "Player.h"
#include <algorithm> // Do użycia std::max

// Konstruktor
Player::Player(const sf::Vector2f& size, const sf::Vector2f& position, const sf::Color& color) {
    player.setSize(size);
    player.setPosition(position);
    player.setFillColor(color);

    velocity = sf::Vector2f(0.f, 0.f);
    isJumping = false;
    isHoldingJump = false;
    jumpHoldTime = 0.f;
}

// Operator przypisania
Player& Player::operator=(const Player& other) {
    if (this != &other) {
        player = other.player;
        velocity = other.velocity;
        isJumping = other.isJumping;
        isHoldingJump = other.isHoldingJump;
        jumpHoldTime = other.jumpHoldTime;
    }
    return *this;
}

// Obsługa wejścia gracza
void Player::handleInput(float deltaTime) {
    // Ruch w lewo i prawo
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        velocity.x = -moveSpeed;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        velocity.x = moveSpeed;
    } else {
        velocity.x = 0.f;
    }

    // Skakanie
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

// Aktualizacja pozycji gracza
void Player::update(float deltaTime) {
    // Grawitacja
    velocity.y += gravity * deltaTime;

    // Poruszanie graczem
    player.move(velocity * deltaTime);

    // Ograniczenie pozycji gracza do "podłogi"
    if (player.getPosition().y + player.getSize().y >= groundHeight) {
        player.setPosition(player.getPosition().x, groundHeight - player.getSize().y);
        velocity.y = 0.f;
        isJumping = false;
    }
}

// Rysowanie gracza
void Player::draw(sf::RenderWindow& window) {
    window.draw(player);
}

