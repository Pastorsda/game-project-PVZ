#ifndef FASTZOMB_HPP
#define FASTZOMB_HPP

#include "Zomb.hpp"
#include "Plant.hpp"

class FastZomb : public Zomb {
private:
    bool hasJumped = false;

    // jump animation
    bool isJumpingAnimation = false;
    float jumpTimer = 0.0f;
    float jumpDuration = 0.6f;
    float jumpStartX = 0.0f;
    float jumpTargetX = 0.0f;
    float baseRowY = 0.0f;
public:
    FastZomb(float startX, float startY, const sf::Texture& texture, int rowNum)
        : Zomb(startX, startY, texture, rowNum, 65, 85.0f, 20) // HP, Speed, DMG
    {
        sprite.setColor(sf::Color(150, 150, 255));
        baseRowY = startY;
    }

    void eat(Plant& plant, float dt) override;
    void update(float dt) override;
};

#endif