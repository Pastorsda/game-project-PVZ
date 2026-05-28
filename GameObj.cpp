#include "GameObj.hpp"

GameObject::GameObject(float startX, float startY, float w, float h)
: x(startX), y(startY), width(w), height(h), isActive(true) {}

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