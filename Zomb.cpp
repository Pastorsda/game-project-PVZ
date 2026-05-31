#include "Zomb.hpp"
#include "Plant.hpp"
#include <iostream>

Zomb::Zomb(float startX, float startY, const sf::Texture& texture, int rowNum, int maxHp, float moveSpeed, int attackDmg)
    : GameObject(startX, startY), hp(maxHp), dmg(attackDmg), row(rowNum), speed(moveSpeed), isEating(false), damageBuffer(0.0f), sprite(texture)
    {
    sprite.setTexture(texture);
    // starting position
    sprite.setPosition({x, y});
    }

void Zomb::update(float dt) {
    if (!getIsActive()) return;

    // Movement when not eating
    if (!isEating) {
        x -= speed * dt;
        sprite.setPosition({x, y});
    }
}

void Zomb::draw(sf::RenderWindow& window) {
    if (getIsActive()) {
        window.draw(sprite);
    }
}

void Zomb::eat(Plant& plant, float dt) {
    isEating = true;
    
    // dmg to buffor
    damageBuffer += static_cast<float>(dmg) * dt;
    
    // dmg buffor for elimination calculation bugs
    if (damageBuffer >= 1.0f) {
        int damageToSend = static_cast<int>(damageBuffer);
        plant.takeDamage(damageToSend);
        damageBuffer -= static_cast<float>(damageToSend);
    }
}

void Zomb::setEating(bool eating) {
    isEating = eating;
}

int Zomb::getRow() const {
    return row;
}

sf::FloatRect Zomb::getBounds() const {
    // hitbox
    return sprite.getGlobalBounds();
}

void Zomb::takeDamage(int amount) {
    hp -= amount;
    if (hp <= 0) {
        destroy();
        std::cout << "[ZOMBIE] Died in row " << row << "\n";
    }
}