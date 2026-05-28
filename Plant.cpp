#include "Plant.hpp"
#include <iostream>

Plant::Plant(float startX, float startY, const sf::Texture& texture, int rowNum, int health, int price) 
: GameObject(startX, startY, 60.0f, 60.0f), sprite(texture), hp(health), cost(price), cooldown(0.0f), row(rowNum), isReady(true) {}

void Plant::takeDamage(int amount) {
    hp -= amount;
    if (hp <= 0) {
        std::cout << "[LOG] Plant in row: " << row << " was eaten\n";
        destroy();//flag for deletion
    }
}
//getters
int Plant::getRow() const {
    return row;
}

int Plant::getCost() const {
    return cost;
}