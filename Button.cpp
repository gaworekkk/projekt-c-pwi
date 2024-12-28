#include "Button.h"

Button::Button(const sf::Vector2f& size, const sf::Vector2f& position, const sf::String& text, const sf::Font& font) {
    // Inicjalizacja kształtu przycisku
    button.setSize(size);
    button.setPosition(position);
    normalColor = sf::Color::Blue;  // Domyślny kolor
    hoverColor = sf::Color::Green; // Kolor podczas najechania
    button.setFillColor(normalColor);

    // Inicjalizacja tekstu
    buttonText.setFont(font);
    buttonText.setString(text);
    buttonText.setCharacterSize(50);
    buttonText.setFillColor(sf::Color::White);

    // Centrowanie tekstu na przycisku
    sf::FloatRect textBounds = buttonText.getLocalBounds();
    buttonText.setPosition(
        position.x + (size.x - textBounds.width) / 2,
        position.y + (size.y - textBounds.height) / 2 - textBounds.top
    );
}

void Button::draw(sf::RenderWindow& window) {
    window.draw(button);
    window.draw(buttonText);
}

void Button::update(const sf::Vector2i& mousePos) {
    if (button.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
        button.setFillColor(hoverColor); // Zmiana koloru na hover
    } else {
        button.setFillColor(normalColor); // Powrót do normalnego koloru
    }
}

bool Button::isClicked(const sf::Vector2i& mousePos, const sf::Event::MouseButtonEvent& mouseEvent) const {
    return button.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos)) &&
           mouseEvent.button == sf::Mouse::Left;
}

std::string Button::getText() const {
    return buttonText.getString(); // Zwraca tekst przycisku jako std::string
}

