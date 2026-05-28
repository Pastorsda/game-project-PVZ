#include "Game.hpp"
#include "Plant.hpp"
#include "Zomb.hpp"
#include <iostream>
#include <random>
#include <algorithm>

Game::Game() : state(1), sunPool(150) {
    window.create(sf::VideoMode({1200,800}), "PVZ - test");
    window.setFramerateLimit(60);
//grid inicialization
    for (int r = 0; r<5; ++r){
        for (int c = 0; c<9; ++c){
            grid[r][c] = false;
        }
    }
}

void Game::run() {
    float zombtimer = 0.0f;
    while (window.isOpen()) {
        handleInput();
//frame time computing
        float dt = clock.restart().asSeconds();
//spawn countdown loop
        zombtimer += dt;
        if (zombtimer >= 6.0f){
            spawnZomb(dt);
            zombtimer = 0.0f;
        }

        update(dt);
        render();
    }
}

void Game::handleInput() {
    while (const std::optional<sf::Event> event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }
    }
}
//random number generator
void Game::spawnZomb(float dt) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> rowDist(0, 4);

    int randomRow = rowDist(gen);
    float spawnY = rowPositions[randomRow];

    std::cout << "[SPAWNER TEST] Zombie in row: " << randomRow << "\n";
}

void Game::checkCollis(float dt) {
    std::vector<Plant*> activePlants;
    std::vector<Zomb*> activeZombs;

    //polimorphic cast safe filter for abstract base objects
    for (auto& obj : objects) {
        if (!obj->getIsActive()) continue;

        if (auto p = dynamic_cast<Plant*>(obj.get())) {
            activePlants.push_back(p);
        }
        else if (auto z = dynamic_cast<Zomb*>(obj.get())) {
            activeZombs.push_back(z);
        }
    }

    //lane based collision
    for (auto* z : activeZombs) {
        bool zombieEats = false;

        for (auto* p : activePlants) {
            //check if intersect on same row
            if (z->getRow() == p->getRow() && z->getBounds().findIntersection(p->getBounds())) {
                z->eat(*p, dt);
                zombieEats = true;
                break;
            }
        }
        // release eating state
        if (!zombieEats){
            z->setEating(false);
        }
    }
}

void Game::update(float dt) {
    //dynamic polymorphic updater
    for (auto& obj : objects) {
        obj->update(dt);
    }
//intersection evaluation
checkCollis(dt);
//asset buffering conversion
    if (!creationBuffer.empty()) {
        for (auto& newObj : creationBuffer) {
            objects.push_back(std::move(newObj));
        }
        creationBuffer.clear();
    }

    //memory cleanup
    objects.erase(std::remove_if(objects.begin(), objects.end(),
    [](const std::unique_ptr<GameObject>& obj) { return !obj->getIsActive(); }),
    objects.end());
}
//grass green background
void Game::render() {
    window.clear(sf::Color(34, 112, 34));

//object rendering
    for (auto& obj : objects) {
        obj->draw(window);
    }
    
    window.display();
}
//entity buffer to prevent altering vectors actively processed
void Game::spawnNewObject(std::unique_ptr<GameObject> newObj) {
    creationBuffer.push_back(std::move(newObj));
}