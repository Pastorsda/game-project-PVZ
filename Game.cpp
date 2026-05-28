#include "Game.hpp"
#include "Plant.hpp"
#include "Zomb.hpp"
#include "Peashooter.hpp"
#include <iostream>
#include <random>
#include <algorithm>

Game::Game() : state(1), sunPool(150) {
    window.create(sf::VideoMode({1200,800}), "PVZ - test");
    window.setFramerateLimit(60);
//texture loading with fallback
if (!plantTexture.loadFromFile("textures/plant.png")) {
    std::cout << "[WARN] plant.png not found. creating placeholder...\n";
    plantTexture = createColorPlaceholder(60, 60, sf::Color::Green);
}

if (!zombTexture.loadFromFile("textures/zombie.png")) {
    std::cout << "[WARN] zombie.png not found. creating placeholder...\n";
    zombTexture = createColorPlaceholder(60, 80, sf::Color(139, 69, 19));
}

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
        //mouse operations
        if (const auto* mouseClick = event->getIf<sf::Event::MouseButtonPressed>()) {
            if (mouseClick->button == sf::Mouse::Button::Left) {
                sf::Vector2i mousePos = mouseClick->position;
                //change pixel into column
                int clickedCol = (mousePos.x - 200) / 90;
                int clickedRow = -1;
                //change pixel into row
                for (int r = 0; r < 5; ++r) {
                    if (mousePos.y >= rowPositions[r] && mousePos.y <= rowPositions[r] + 80) {
                        clickedRow = r;
                        break;
                    }
                }
                if (clickedRow >= 0 && clickedRow < 5 && clickedCol >= 0 && clickedCol < 9) {
                    if (!grid[clickedRow][clickedCol] && sunPool >= 100) {
                        //centered pixel position
                        float plantX = 200.0f + clickedCol * 90.0f + 15.0f;
                        float plantY = rowPositions[clickedRow] + 10.0f;
                        //create peashooter
                        auto newPlant = std::make_unique<Peashooter>(plantX, plantY, plantTexture, clickedRow);
                        spawnNewObject(std::move(newPlant));
                        //block spot and pay sun
                        grid[clickedRow][clickedCol] = true;
                        sunPool -= 100;

                        std::cout << "[GRID] peashooter placed on space [" << clickedRow <<"]["<< clickedCol << "], Sun left: " << sunPool <<"\n";
                    }
                }
            }
        }
    }
}
//random number generator & zomb iniciation
void Game::spawnZomb(float dt) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> rowDist(0, 4);

    int randomRow = rowDist(gen);
    float spawnY = rowPositions[randomRow];
    float spawnX = 1200.0f;
//create zombie object with texture x, y, texture, row, hp, speed, damage
    auto newZomb = std::make_unique<Zomb>(spawnX, spawnY, zombTexture, randomRow, 100, 40.0f, 30);
//push into safe buffer
    spawnNewObject(std::move(newZomb));

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

    //freeing space before plant deletion
    for (auto& obj : objects) {
        if (!obj->getIsActive()) {
            if (auto p = dynamic_cast<Plant*>(obj.get())) {
                int col = static_cast<int>((p->getX() - 200) / 90);
                if (p->getRow() >= 0 && p->getRow() < 5 && col >= 0 && col < 9) {
                    grid[p->getRow()][col] = false;
                    std::cout << "[GRID] plant died space [" << p->getRow() << "][" << col << "] is now free\n";
                }
            }
        }
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

sf::Texture Game::createColorPlaceholder(unsigned int width, unsigned int height, sf::Color color) {
    sf::Image image({width, height}, color);
    sf::Texture tex;
    if (!tex.loadFromImage(image)) {
        std::cerr << "[ERROR] Failed to load from placeholder\n";
    }
    return tex;
}
