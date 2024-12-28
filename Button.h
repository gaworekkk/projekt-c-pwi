#ifndef BUTTON_H
#define BUTTON_H

#include <SFML/Graphics.hpp>

class Button {
public:
    Button(const sf::Vector2f& size, const sf::Vector2f& position, const sf::String& text, const sf::Font& font);

    void draw(sf::RenderWindow& window);
    void update(const sf::Vector2i& mousePos);
    bool isClicked(const sf::Vector2i& mousePos, const sf::Event::MouseButtonEvent& mouseEvent) const;

    std::string getText() const; // Pobranie tekstu z przycisku

private:
    sf::RectangleShape button; // Kształt przycisku
    sf::Text buttonText;       // Tekst wyświetlany na przycisku
    sf::Color normalColor;     // Kolor przycisku w normalnym stanie
    sf::Color hoverColor;      // Kolor przycisku w stanie "hover"
};

#endif

