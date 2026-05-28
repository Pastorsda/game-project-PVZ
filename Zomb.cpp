#include "Zomb.hpp"
#include "Plant.hpp"
#include <iostream>

Zomb::Zomb(float startX, float startY, const sf::Texture& texture, int rowNum, int health, float moveSpeed, int atkPower)
: GameObject(startX, startY, texture), hp(health), speed(moveSpeed), dmg(atkPower), row(rowNum), isEating(false) {

    width = 60.0f;
    height = 80.0f;

    sprite.setPosition({x, y});
}

void Zomb::update(float dt) {
    if (!isActive) return;

    if (!isEating) {
        x -= speed * dt;
    }

    sprite.setPosition({x, y});
}

void Zomb::draw(sf::RenderWindow& window) {
    if (isActive) {
        window.draw(sprite);
    }
}

void Zomb::eat(Plant& plant, float dt) {
    isEating = true;

    damageBuffer += dmg * dt;

    if (damageBuffer >= 1.0f) {
        int finalDmg = static_cast<int>(damageBuffer);
        plant.takeDamage(finalDmg);
        damageBuffer -= finalDmg;
    }
}

void Zomb::setEating(bool eating) {
    isEating = eating;
    if (!isEating) {
        damageBuffer = 0.0f;
    }
}

int Zomb::getRow() const {
    return row;
}

void Zomb::takeDamage(int amount) {
    hp -= amount;
    if (hp <= 0) {
        std::cout << "[LOG] Zombie in row " << row << " was defeated\n";
        destroy();
    }
}