#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "GameObj.hpp"

class Game {
    private:
    sf::RenderWindow window;
    sf::Clock clock;

    int state;
    int sunPool;

    std::vector<std::unique_ptr<GameObject>> objects;

    std::vector<std::unique_ptr<GameObject>> creationBuffer;

    bool grid[5][9];

    float rowPositions[5] = {150.0f, 270.0f, 390.0f, 510.0f, 630.0f};

    void handleInput();
    void update(float dt);
    void render();
    void spawnZomb(float dt);
    void checkCollis(float dt);

    public:
    Game();
    void run();
    void spawnNewObject(std::unique_ptr<GameObject> newObj);
};

#endif