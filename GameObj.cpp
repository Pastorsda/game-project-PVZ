#include "GameObj.hpp"

GameObject::GameObject(float startX, float startY, const sf::Texture& texture)
: x(startX), y(startY), width(0.0f), height(0.0f), isActive(true), sprite(texture) {}

sf::FloatRect GameObject::getBounds() const {
    return sf::FloatRect({x, y}, {width, height});
}

bool GameObject::getIsActive() const {
    return isActive;
}

void GameObject::destroy() {
    isActive = false;
}

float GameObject::getX() const {
    return x;
}

float GameObject::getY() const {
    return y;
}