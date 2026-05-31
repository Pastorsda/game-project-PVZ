#ifndef HEAVYZOMB_HPP
#define HEAVYZOMB_HPP

#include "Zomb.hpp"

class HeavyZomb : public Zomb {
public:
    HeavyZomb(float startX, float startY, const sf::Texture& texture, int rowNum)
        : Zomb(startX, startY, texture, rowNum, 260, 18.0f, 55) // HP, Speed, DMG
    {
        sprite.setColor(sf::Color(255, 100, 100));
        sprite.setScale({1.25f, 1.25f}); //Bigger then regular by 25%
    }
};

#endif