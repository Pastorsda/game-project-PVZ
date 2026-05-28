#include "Game.hpp"
#include "Plant.hpp"
#include "Zomb.hpp"
#include "Peashooter.hpp"
#include "Pea.hpp"
#include <iostream>
#include <random>
#include <algorithm>

Game::Game() : state(1), sunPool(150), sunTimer(0.0f) {
    window.create(sf::VideoMode({1200, 800}), "PVZ - test");
    window.setFramerateLimit(60);

    // Texture loading with fallback
    if (!plantTexture.loadFromFile("textures/plant.png")) {
        std::cout << "[WARN] plant.png not found. creating placeholder...\n";
        plantTexture = createColorPlaceholder(60, 60, sf::Color::Green);
    }

    if (!zombTexture.loadFromFile("textures/zombie.png")) {
        std::cout << "[WARN] zombie.png not found. creating placeholder...\n";
        zombTexture = createColorPlaceholder(60, 80, sf::Color(139, 69, 19));
    }

    // Grid initialization
    for (int r = 0; r < 5; ++r) {
        for (int c = 0; c < 9; ++c) {
            grid[r][c] = false;
        }
    }
}

void Game::run() {
    float zombtimer = 0.0f;
    while (window.isOpen()) {
        handleInput();

        // Frame time computing
        float dt = clock.restart().asSeconds();

        // Spawn countdown loop
        zombtimer += dt;
        if (zombtimer >= 6.0f) {
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

        // Mouse operations
        if (const auto* mouseClick = event->getIf<sf::Event::MouseButtonPressed>()) {
            if (mouseClick->button == sf::Mouse::Button::Left) {
                sf::Vector2i mousePos = mouseClick->position;

                // Change pixel into column
                int clickedCol = (mousePos.x - 200) / 90;
                int clickedRow = -1;

                // Change pixel into row
                for (int r = 0; r < 5; ++r) {
                    if (mousePos.y >= rowPositions[r] && mousePos.y <= rowPositions[r] + 80) {
                        clickedRow = r;
                        break;
                    }
                }

                // checking grid
                if (clickedRow >= 0 && clickedRow < 5 && clickedCol >= 0 && clickedCol < 9) {
                    if (!grid[clickedRow][clickedCol] && sunPool >= 100) {
                        // Centered pixel position
                        float plantX = 200.0f + clickedCol * 90.0f + 15.0f;
                        float plantY = rowPositions[clickedRow] + 10.0f;

                        // Create peashooter and buffor
                        auto newPlant = std::make_unique<Peashooter>(plantX, plantY, plantTexture, clickedRow, *this);
                        spawnNewObject(std::move(newPlant));

                        // Block spot and pay sun
                        grid[clickedRow][clickedCol] = true;
                        sunPool -= 100;

                        std::cout << "[GRID] Peashooter placed on space [" << clickedRow << "][" << clickedCol << "], Sun left: " << sunPool << "\n";
                    }
                }
            }
        }
    }
}

// Random number generator & zombie initiation
void Game::spawnZomb(float dt) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> rowDist(0, 4);

    int randomRow = rowDist(gen);
    float spawnY = rowPositions[randomRow];
    float spawnX = 1200.0f;

    // Create zombie object
    auto newZomb = std::make_unique<Zomb>(spawnX, spawnY, zombTexture, randomRow, 100, 40.0f, 30);
    spawnNewObject(std::move(newZomb));

    std::cout << "[SPAWNER TEST] Zombie in row: " << randomRow << "\n";
}

void Game::checkCollis(float dt) {
    std::vector<Plant*> activePlants;
    std::vector<Zomb*> activeZombs;
    std::vector<Pea*> activePeas;

    // Polymorphic cast safe filter for abstract base objects
    for (auto& obj : objects) {
        if (!obj->getIsActive()) continue;

        if (auto p = dynamic_cast<Plant*>(obj.get())) {
            activePlants.push_back(p);
        }
        else if (auto z = dynamic_cast<Zomb*>(obj.get())) {
            activeZombs.push_back(z);
        }
        else if (auto pea = dynamic_cast<Pea*>(obj.get())) {
            activePeas.push_back(pea);
        }
    }

    // Lane based collision (Zombie eats Plant)
    for (auto* z : activeZombs) {
        bool zombieEats = false;

        for (auto* p : activePlants) {
            if (z->getRow() == p->getRow() && z->getBounds().findIntersection(p->getBounds())) {
                z->eat(*p, dt);
                zombieEats = true;
                break;
            }
        }
        if (!zombieEats) {
            z->setEating(false);
        }
    }

    // Collision Zombie and Pea
    for (auto* pea : activePeas) {
        for (auto* z : activeZombs) {
            if (pea->getRow() == z->getRow() && pea->getBounds().findIntersection(z->getBounds())) {
                z->takeDamage(pea->getDamage());
                pea->destroy();
                break;
            }
        }
    }
}

void Game::update(float dt) {
    // passive sun
    sunTimer += dt;
    if (sunTimer >= 8.0f) {
        sunPool += 25;
        sunTimer = 0.0f;
        std::cout << "[SUN] Sun fell from the sky! Current sun: " << sunPool << "\n";
    }

    // Dynamic polymorphic updater
    for (auto& obj : objects) {
        obj->update(dt);
    }

    // Intersection evaluation
    checkCollis(dt);

    // Asset buffering conversion
    if (!creationBuffer.empty()) {
        for (auto& newObj : creationBuffer) {
            objects.push_back(std::move(newObj));
        }
        creationBuffer.clear();
    }

    // Freeing space before plant deletion
    for (auto& obj : objects) {
        if (!obj->getIsActive()) {
            if (auto p = dynamic_cast<Plant*>(obj.get())) {
                int col = static_cast<int>((p->getX() - 200) / 90);
                if (p->getRow() >= 0 && p->getRow() < 5 && col >= 0 && col < 9) {
                    grid[p->getRow()][col] = false;
                    std::cout << "[GRID] Plant died space [" << p->getRow() << "][" << col << "] is now free\n";
                }
            }
        }
    }

    // Memory cleanup
    objects.erase(std::remove_if(objects.begin(), objects.end(),
        [](const std::unique_ptr<GameObject>& obj) { return !obj->getIsActive(); }),
        objects.end());
}

void Game::render() {
    window.clear(sf::Color(34, 112, 34)); // Grass green background

    // Object rendering
    for (auto& obj : objects) {
        obj->draw(window);
    }

    window.display();
}

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

// chech line for zombs
bool Game::isZombieInRow(int row) const {
    for (const auto& obj : objects) {
        if (auto z = dynamic_cast<Zomb*>(obj.get())) {
            // check position
            if (z->getIsActive() && z->getRow() == row && z->getX() <= 1200.0f) {
                return true;
            }
        }
    }
    return false;
}