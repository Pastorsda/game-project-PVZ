#ifndef FASTZOMB_HPP
#define FASTZOMB_HPP

#include "Zomb.hpp"

class FastZomb : public Zomb {
public:
    FastZomb(float startX, float startY, const sf::Texture& texture, int rowNum)
        : Zomb(startX, startY, texture, rowNum, 65, 85.0f, 20) // HP, Speed, DMG
    {
        sprite.setColor(sf::Color(150, 150, 255));
    }
};

#endif