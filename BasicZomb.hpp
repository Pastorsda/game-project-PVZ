#ifndef BASICZOMB_HPP
#define BASICZOMB_HPP

#include "Zomb.hpp"

class BasicZomb : public Zomb {
public:
    BasicZomb(float startX, float startY, const sf::Texture& texture, int rowNum)
        : Zomb(startX, startY, texture, rowNum, 100, 30.0f, 30) // HP, Speed, DMG
    {
        sprite.setColor(sf::Color::White);
    }
};

#endif