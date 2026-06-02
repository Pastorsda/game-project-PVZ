#include "FastZomb.hpp"
#include "Plant.hpp"
#include <iostream>
#include <cmath>

void FastZomb::eat(Plant& plant, float dt) {
    if (!hasJumped && !isJumpingAnimation) {
        isJumpingAnimation = true;
        jumpTimer = 0.0f;
        jumpStartX = this->getX();

        jumpTargetX = jumpStartX - 230.0f;
        hasJumped = true;
        float currentX = this->getX();
        this->setEating(false);
        this->isEating = false;
        sprite.setColor(sf::Color(100, 100, 180));

        std::cout << "[FAST ZOMBIE] HOP! Jumped over a plant!\n";
    }
    else if (!isJumpingAnimation) {
        Zomb::eat(plant, dt);
    }
}

void FastZomb::update(float dt) {
    if (isJumpingAnimation) {
        jumpTimer += dt;

        float progress = jumpTimer / jumpDuration;
        if (progress > 1.0f) progress = 1.0f;
        float currentX = jumpStartX + (jumpTargetX - jumpStartX) * progress;

        float maxheight = 50.0f;
        float currentY = baseRowY - (sinf(progress * 3.14159f) * maxheight);
        this->setX(currentX);
        y = currentY;
        sprite.setPosition({currentX, currentY});

        float rotationDegrees = sinf(progress * 3.14159f) * 15.0f;
        sprite.setRotation(sf::degrees(rotationDegrees));

        if (progress >= 1.0f) {
            isJumpingAnimation = false;
            sprite.setRotation(sf::degrees(0.0f));
            sprite.setColor(sf::Color(120, 120, 220));
            sprite.setColor(sf::Color(100, 100, 200));
            std::cout << "[FAST ZOMBIE] Landed successfully!\n";
        }
    }
    else {
        y = baseRowY;
        Zomb::update(dt);
    }
}