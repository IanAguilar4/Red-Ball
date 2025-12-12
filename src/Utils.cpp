#include "Utils.hpp"

void CenterTextX(sf::Text& text, float windowWidth) {
    sf::FloatRect b = text.getLocalBounds();
    text.setPosition((windowWidth - b.width) / 2.f - b.left, text.getPosition().y);
}
