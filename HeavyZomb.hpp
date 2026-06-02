#ifndef HEAVYZOMB_HPP
#define HEAVYZOMB_HPP

#include "Zomb.hpp"

class HeavyZomb : public Zomb {
public:
    HeavyZomb(float startX, float startY, const sf::Texture& texture, int rowNum)
        : Zomb(startX, startY, texture, rowNum, 260, 13.0f, 80) // HP, Speed, DMG
    {
        sprite.setScale({-2.3f, 2.3f});
        sprite.setOrigin({40.0f,45.0f});
        sprite.setColor(sf::Color(255, 100, 100));
    }
};

#endif