#include "Game.hpp"
#include "Plant.hpp"
#include "Zomb.hpp"
#include <iostream>
#include <random>
#include <algorithm>

Game::Game() : state(1), sunPool(150) {
    window.create(sf::VideoMode({1200,800}), "PVZ - test");
    window.setFramerateLimit(60);

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

        float dt = clock.restart().asSeconds();

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
    std::vector<Enemy*> activeZombs;

    //sortowanie obiektów z głównego kontenera polimorficznego
    for (auto& obj : objects) {
        if (!obj->getIsActive()) continue;

        if (auto p = dynamic_cast<Plant*>(obj.get())) {
            activePlants.push_back(p);
        }
        else if (auto z = dynamic_cast<Enemy*>(obj.get())) {
            activeZombs.push_back(z);
        }
    }

    //lokalizacja kolizji interakcji
    for (auto* z : activeZombs) {
        bool zombieEats = false;

        for (auto* p : activePlants) {
            if (z->getRow() == p->getRow() && z->getBounds().findIntersection(p->getBounds())) {
                z->eat(*p, dt);
                zombieEats = true;
                break;
            }
        }
        if (!zombieEats){
            z->setEating(false);
        }
    }
}

void Game::update(float dt) {
    for (auto& obj : objects) {
        obj->update(dt);
    }

checkCollis(dt);

    if (!creationBuffer.empty()) {
        for (auto& newObj : creationBuffer) {
            objects.push_back(std::move(newObj));
        }
        creationBuffer.clear();
    }

    //usuwanie z pamięci
    objects.erase(std::remove_if(objects.begin(), objects.end(),
    [](const std::unique_ptr<GameObject>& obj) { return !obj->getIsActive(); }),
    objects.end());
}

void Game::render() {
    window.clear(sf::Color(34, 112, 34));

    //rysowanie obiektów z kontenera
    for (auto& obj : objects) {
        obj->draw(window);
    }
    
    window.display();
}

void Game::spawnNewObject(std::unique_ptr<GameObject> newObj) {
    creationBuffer.push_back(std::move(newObj));
}