#include "Obstacle.h"

Obstacle::Obstacle(float x, float y, float width, float height) {
    obstacleShape.setSize(sf::Vector2f(width, height));
    obstacleShape.setPosition(x, y);
    obstacleShape.setFillColor(sf::Color::Green);  // Można zmienić na odpowiednią teksturę
}

void Obstacle::update(float deltaTime, float speed) {
    // Ruch w lewo
    obstacleShape.move(-speed * deltaTime, 0.f);
}

void Obstacle::draw(sf::RenderWindow& window) {
    window.draw(obstacleShape);
}

sf::FloatRect Obstacle::getBounds() const {
    return obstacleShape.getGlobalBounds();
}