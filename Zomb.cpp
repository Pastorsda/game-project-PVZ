#include "Zomb.hpp"
#include "Plant.hpp"
#include <iostream>

Zomb::Zomb(float startX, float startY, const sf::Texture& texture, int rowNum, int maxHp, float moveSpeed, int attackDmg)
    : GameObject(startX, startY), hp(maxHp), dmg(attackDmg), row(rowNum), speed(moveSpeed), isEating(false), damageBuffer(0.0f), sprite(texture)
    {
    sprite.setTexture(texture);

    int w = 85;
    int h = 95;

    for(int i = 0; i < 8; ++i) moveFrames.push_back(sf::IntRect({i * w, 0}, {w, h}));
    for(int i = 0; i < 4; ++i) attackFrames.push_back(sf::IntRect({i * w, h}, {w, h}));

    sprite.setTextureRect(moveFrames[0]);
    sprite.setOrigin({40.0f,30.0f});
    sprite.setScale({-1.8f, 1.8f});
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

    animTimer += dt;
    if (animTimer >= 0.15f) { // Animation Speed
        animTimer = 0.0f;
        currentFrame++;

        if (isEating) {
            if (currentFrame >= attackFrames.size()) currentFrame = 0;
            sprite.setTextureRect(attackFrames[currentFrame]);
        } else {
            if (currentFrame >= moveFrames.size()) currentFrame = 0;
            sprite.setTextureRect(moveFrames[currentFrame]);
        }
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