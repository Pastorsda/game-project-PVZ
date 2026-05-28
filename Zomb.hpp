#ifndef ZOMB_HPP
#define ZOMB_HPP

#include "GameObj.hpp"

class Plant;

class Zomb : public GameObject {
protected:
    int hp, dmg, row;
    float speed;
    bool isEating;
    float damageBuffer = 0.0f;
public:
    Zomb(float startX, float startY, const sf::Texture& texture, int rowNum, int health, float moveSpeed, int atkPower);

    virtual ~Zomb() = default;

    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;

    void eat(Plant& plant, float dt);
    void setEating(bool eating);
    int getRow() const;

    void takeDamage(int amount);
};

#endif