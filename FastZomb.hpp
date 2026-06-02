#ifndef FASTZOMB_HPP
#define FASTZOMB_HPP

#include "Zomb.hpp"
#include "Plant.hpp"

class FastZomb : public Zomb {
private:
    bool hasJumped = false;

    int currentFrame = 0;
    float animTimer = 0.0f;
    std::vector<sf::IntRect> moveFrames;
    std::vector<sf::IntRect> attackFrames;


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
        sprite.setOrigin({20.0f,20.0f});
        baseRowY = startY;

        int w = 100;
        int h = 95;

        for(int i = 0; i < 8; ++i) moveFrames.push_back(sf::IntRect({i * w, 0}, {w-10, h}));
    for(int i = 0; i < 4; ++i) attackFrames.push_back(sf::IntRect({i * w, h}, {w, h}));
    }

    void eat(Plant& plant, float dt) override;
    void update(float dt) override;
};

#endif