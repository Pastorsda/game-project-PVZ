#include "Game.hpp"
#include "Plant.hpp"
#include "Zomb.hpp"
#include "Peashooter.hpp"
#include "Pea.hpp"
#include "Sunflower.hpp"
#include "Sun.hpp"
#include <iostream>
#include <random>
#include <algorithm>

Game::Game() : state(1), sunPool(150), sunTimer(0.0f), currentSelection(SelectedPlant::None), sunText(font) {
    window.create(sf::VideoMode({1200, 800}), "PVZ - test");
    window.setFramerateLimit(60);

    // Texture loading with fallback
    if (!plantTexture.loadFromFile("textures/peashooter.png")) {
        std::cout << "[WARN] peashooter.png not found. creating placeholder...\n";
        plantTexture = createColorPlaceholder(60, 60, sf::Color::Green);
    }

    if (!sunflowerTexture.loadFromFile("textures/sunflower.png")) {
        std::cout << "[WARN] sunflower.png not found. creating placeholder...\n";
        sunflowerTexture = createColorPlaceholder(60, 60, sf::Color::Yellow);
    }

    if (!zombTexture.loadFromFile("textures/zombie.png")) {
        std::cout << "[WARN] zombie.png not found. creating placeholder...\n";
        zombTexture = createColorPlaceholder(60, 80, sf::Color(139, 69, 19));
    }

    if (!peaTexture.loadFromFile("textures/pea.png")) {
        std::cout << "[WARN] pea.png not found. creating placeholder...\n";
        peaTexture = createColorPlaceholder(16, 16, sf::Color(50, 205, 50));
    }

    if (!sunTexture.loadFromFile("textures/sun.png")) {
        std::cout << "[WARN] sun.png not found. creating placeholder...\n";
        sunTexture = createColorPlaceholder(16, 16, sf::Color(255, 215, 0));
    }

    if (!font.openFromFile("C:/Windows/Fonts/arial.ttf")) {
        std::cout << "[Warn] arial.ttf not found. Sun not visible\n";
    }

    // peacard on ui
    peaCard.setSize({80.0f, 60.0f});
    peaCard.setPosition({200.0f, 10.0f});
    peaCard.setFillColor(sf::Color(0, 150, 0));
    peaCard.setOutlineThickness(2.0f);
    peaCard.setOutlineColor(sf::Color::White);

    // suncard on ui
    sunCard.setSize({80.0f, 60.0f});
    sunCard.setPosition({290.0f, 10.0f}); 
    sunCard.setFillColor(sf::Color(200, 200, 0)); 
    sunCard.setOutlineThickness(2.0f);
    sunCard.setOutlineColor(sf::Color::White);

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

        // 1. Zbieranie słońca przez najechanie myszką (Hover)
        if (const auto* mouseMove = event->getIf<sf::Event::MouseMoved>()) {
            sf::Vector2f mousePosF(static_cast<float>(mouseMove->position.x), static_cast<float>(mouseMove->position.y));

            for (auto& obj : objects) {
                if (!obj->getIsActive()) continue;

                if (auto sunObj = dynamic_cast<Sun*>(obj.get())) {
                    if (sunObj->getBounds().contains(mousePosF)) {
                        sunPool += 25;
                        sunObj->destroy();
                        std::cout << "[SUN] Hovered & Collected! Current pool: " << sunPool << "\n";
                    }
                }
            }
        }

        // 2. Kliknięcia myszy (Wybór kart i sadzenie)
        if (const auto* mouseClick = event->getIf<sf::Event::MouseButtonPressed>()) {
            if (mouseClick->button == sf::Mouse::Button::Left) {
                sf::Vector2i mousePos = mouseClick->position;
                sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
                
                // select peashooter
                if (peaCard.getGlobalBounds().contains(mousePosF)) {
                    currentSelection = SelectedPlant::Peashooter;
                    std::cout << "[UI] picked: peashooter\n";
                    continue;
                }
                // select sunflower
                else if (sunCard.getGlobalBounds().contains(mousePosF)) {
                    currentSelection = SelectedPlant::Sunflower;
                    std::cout << "[UI] picked: sunflower\n";
                    continue;
                }

                // check mouse on grid
                int clickedCol = (mousePos.x - 200) / 90;
                int clickedRow = -1;

                // Change pixel into row
                for (int r = 0; r < 5; ++r) {
                    if (mousePos.y >= rowPositions[r] && mousePos.y <= rowPositions[r] + 80) {
                        clickedRow = r;
                        break;
                    }
                }

                // checking grid i sadzenie
                if (clickedRow >= 0 && clickedRow < 5 && clickedCol >= 0 && clickedCol < 9) {
                    if (!grid[clickedRow][clickedCol]) {
                        // Centered pixel position
                        float plantX = 200.0f + clickedCol * 90.0f + 15.0f;
                        float plantY = rowPositions[clickedRow] + 10.0f;

                        // Create peashooter
                        if (currentSelection == SelectedPlant::Peashooter && sunPool >= 100) {
                            auto newPlant = std::make_unique<Peashooter>(plantX, plantY, plantTexture, clickedRow, *this);
                            spawnNewObject(std::move(newPlant));

                            grid[clickedRow][clickedCol] = true;
                            sunPool -= 100;
                            currentSelection = SelectedPlant::None;
                        }
                        // Create sunflower
                        else if (currentSelection == SelectedPlant::Sunflower && sunPool >= 50) {
                            auto newPlant = std::make_unique<Sunflower>(plantX, plantY, sunflowerTexture, clickedRow, *this);
                            spawnNewObject(std::move(newPlant));

                            grid[clickedRow][clickedCol] = true;
                            sunPool -= 50;
                            currentSelection = SelectedPlant::None;
                        }
                    }
                }
            }
        }
    } // end of loop pollEvent
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
    
    // delete zombs that left
    for (auto& obj : objects) {
        if (auto z = dynamic_cast<Zomb*>(obj.get())) {
            if (z->getX() < 0.0f) { 
                z->destroy();       
            }
        }
    }

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

    // highlight selected
    peaCard.setOutlineColor(currentSelection == SelectedPlant::Peashooter ? sf::Color::Red : sf::Color::White);
    sunCard.setOutlineColor(currentSelection == SelectedPlant::Sunflower ? sf::Color::Red : sf::Color::White);
    
    // draw interface
    window.draw(peaCard);
    window.draw(sunCard);
    
    // draw sun text
    sunText.setString("Sun: " + std::to_string(sunPool));
    window.draw(sunText);

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

// check line for zombs
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