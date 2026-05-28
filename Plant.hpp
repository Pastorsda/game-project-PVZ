#ifndef PLANT_HPP
#define PLANT_HPP

#include "GameObj.hpp"

class Plant : public GameObject {
protected:
    int hp, cost, row;
    float cooldown;
    bool isReady;

public:
//constructor
    Plant(float startX, float startY, const sf::Texture& texture, int rowNum, int health, int price);
//virtual deconstructor: avoid memory leaks
    virtual ~Plant() = default;
//virtual function for attack behaviour
    virtual void attack() = 0;
//reduce health and handle destruction
    void takeDamage(int amount);
//getters
    int getRow() const;
    int getCost() const;
};

#endif