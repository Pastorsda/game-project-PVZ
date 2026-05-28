#ifndef PLANT_HPP
#define PLANT_HPP

#include "GameObj.hpp"

class Plant : public GameObject {
protected:
    int hp, cost, row;
    float cooldown;
    bool isReady;

public:
    Plant(float startX, float startY, const sf::Texture& texture, int rowNum, int health, int price);

    virtual ~Plant() = default;

    virtual void attack() = 0;

    void takeDamage(int amount);

    int getRow() const;
    int getCost() const;
};

#endif