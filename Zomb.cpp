#include "Zomb.hpp"
#include "Plant.hpp"
#include <iostream>

Zomb::Zomb(float startX, float startY, const sf::Texture& texture, int rowNum, int health, float moveSpeed, int atkPower)
: GameObject(startX, startY, texture), hp(health), speed(moveSpeed), dmg(atkPower), row(rowNum), isEating(false) {
//collision boundaries
    width = 60.0f;
    height = 80.0f;
//sprite underlying
    sprite.setPosition({x, y});
}

void Zomb::update(float dt) {
    if (!isActive) return;
//continous motion until obstructed
    if (!isEating) {
        x -= speed * dt;
    }
//Sync physical coordinates to graphic sprite for rendering
    sprite.setPosition({x, y});
}

void Zomb::draw(sf::RenderWindow& window) {
    if (isActive) {
        window.draw(sprite); //direct rendering call
    }
}

void Zomb::eat(Plant& plant, float dt) {
    isEating = true;
//delta time accumulation prevents frame updates to operate bellow 0
    damageBuffer += dmg * dt;

    if (damageBuffer >= 1.0f) {
        int finalDmg = static_cast<int>(damageBuffer);
        plant.takeDamage(finalDmg);
        damageBuffer -= finalDmg; //deduct the whole int damage retain remainder
    }
}

void Zomb::setEating(bool eating) {
    isEating = eating;
    if (!isEating) {
        damageBuffer = 0.0f; // reset float accumulation
    }
}

int Zomb::getRow() const {
    return row;
}

void Zomb::takeDamage(int amount) {
    hp -= amount;
    if (hp <= 0) {
        std::cout << "[LOG] Zombie in row " << row << " was defeated\n";
        destroy(); //mark for deletion
    }
}