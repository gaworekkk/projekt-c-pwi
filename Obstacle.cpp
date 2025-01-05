#include "Obstacle.h"

Obstacle::Obstacle(float x, float y, float width, float height, float obstacleSpeed) {
    obstacleShape.setSize(sf::Vector2f(width, height));
    obstacleShape.setPosition(x, y);
    obstacleShape.setFillColor(sf::Color::Blue);  // do zmiany na teksturę
    speed = obstacleSpeed;
}

void Obstacle::update(float deltaTime) {
    // Ruch w lewo
    obstacleShape.move(-speed * deltaTime, 0.f);
}

void Obstacle::draw(sf::RenderWindow& window) {
    window.draw(obstacleShape);
}

sf::FloatRect Obstacle::getBounds() const {
    return obstacleShape.getGlobalBounds();
}

float Obstacle::getSpeed() const {
    return speed;
}

void Obstacle::setSpeed(float newSpeed) {
    speed = newSpeed;
}
