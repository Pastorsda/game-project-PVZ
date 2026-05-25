#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <random>

class Game;

// ============================================================================
// 1. KLASY BAZOWE (ZGODNIE Z TWÓIM SCHEMATEM KLAS)
// ============================================================================

class GameObject {
protected:
    float x, y;
    bool isActive = true;
public:
    GameObject(float startX, float startY) : x(startX), y(startY) {}
    virtual ~GameObject() = default;

    virtual void update(float dt, Game& game) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;
    
    bool getIsActive() const { return isActive; }
    void destroy() { isActive = false; }
    virtual sf::FloatRect getBounds() = 0;
    
    float getX() const { return x; }
    float getY() const { return y; }
};

class Plant : public GameObject {
protected:
    int hp;
    int cost;
    int row;
public:
    Plant(float startX, float startY, int r, int health, int price) 
        : GameObject(startX, startY), row(r), hp(health), cost(price) {}

    void takeDamage(int amount) {
        hp -= amount;
        if (hp <= 0) {
            destroy();
        }
    }
    int getRow() const { return row; }
};

class Enemy : public GameObject {
protected:
    int hp;
    float speed;
    int row;
    int attackDamage;
    bool isEating;
public:
    Enemy(float startX, float startY, int r, float s, int health, int dmg) 
        : GameObject(startX, startY), row(r), speed(s), hp(health), attackDamage(dmg), isEating(false) {}

    void takeDamage(int amount) {
        hp -= amount;
        if (hp <= 0) destroy();
    }
    
    void setEating(bool eating) { isEating = eating; }
    int getRow() const { return row; }
    int getDamage() const { return attackDamage; }
};

// ============================================================================
// 2. KONKRETNE OBIEKTY GRY
// ============================================================================

class Projectile : public GameObject {
private:
    float speed = 300.0f;
    int damage = 25;
    int row;
    sf::CircleShape shape;
public:
    Projectile(float startX, float startY, int r) : GameObject(startX, startY), row(r) {
        shape.setRadius(8.0f);
        shape.setFillColor(sf::Color::Yellow);
    }

    void update(float dt, Game& game) override {
        x += speed * dt;
        if (x > 1200) destroy();
    }

    void draw(sf::RenderWindow& window) override {
        shape.setPosition(sf::Vector2f({x, y}));
        window.draw(shape);
    }

    sf::FloatRect getBounds() override {
        shape.setPosition(sf::Vector2f({x, y}));
        return shape.getGlobalBounds();
    }

    int getDamage() const { return damage; }
    int getRow() const { return row; }
};

class SunPlant : public Plant {
private:
    float sunTimer = 0.0f;
    float sunInterval = 5.0f; // Słońce/Strzał co 5 sekund
    sf::RectangleShape shape;
public:
    SunPlant(float startX, float startY, int r) : Plant(startX, startY, r, 100, 50) {
        shape.setSize(sf::Vector2f({60.0f, 60.0f}));
        shape.setFillColor(sf::Color(255, 215, 0)); // Kolor złoty
    }

    void update(float dt, Game& game) override; // Implementacja pod klasą Game

    void draw(sf::RenderWindow& window) override {
        shape.setPosition(sf::Vector2f({x, y}));
        window.draw(shape);
    }

    sf::FloatRect getBounds() override {
        shape.setPosition(sf::Vector2f({x, y}));
        return shape.getGlobalBounds();
    }
};

class BasicZombie : public Enemy {
private:
    sf::RectangleShape shape;
public:
    BasicZombie(float startX, float startY, int r) : Enemy(startX, startY, r, 35.0f, 100, 30) {
        shape.setSize(sf::Vector2f({45.0f, 65.0f}));
        shape.setFillColor(sf::Color(46, 139, 87)); // Zielony zombie
    }

    void update(float dt, Game& game) override {
        if (!isEating) {
            x -= speed * dt;
        }
        if (x < 0) destroy();
        isEating = false; // Reset przed kolejną klatką i testem kolizji
    }

    void draw(sf::RenderWindow& window) override {
        shape.setPosition(sf::Vector2f({x, y}));
        window.draw(shape);
    }

    sf::FloatRect getBounds() override {
        shape.setPosition(sf::Vector2f({x, y}));
        return shape.getGlobalBounds();
    }
};

// ============================================================================
// 3. SEKCJA GLÓWNEGO SILNIKA GRY (Z BUFOREM NA OBIEKTY)
// ============================================================================

class Game {
public:
    int sunPool = 150;
    std::vector<std::unique_ptr<GameObject>> objects;
    
    // BEZPIECZNY BUFOR: Zapobiega inwalidacji iteratorów (i błędom Segmentation Fault)
    std::vector<std::unique_ptr<GameObject>> newObjectsBuffer;
    
    float rowPositions[5] = { 150.0f, 270.0f, 390.0f, 510.0f, 630.0f };

    void addSun(int amount) {
        sunPool += amount;
        std::cout << "Zasoby slonca: " << sunPool << "\n";
    }

    // Nowe pociski trafiają najpierw do bufora bezpieczeństwa
    void addProjectile(float startX, float startY, int row) {
        newObjectsBuffer.push_back(std::make_unique<Projectile>(startX, startY, row));
    }

    void handleCollisions(float dt) {
        std::vector<Plant*> plants;
        std::vector<Enemy*> zombies;
        std::vector<Projectile*> projectiles;

        for (auto& obj : objects) {
            if (!obj->getIsActive()) continue;
            if (auto p = dynamic_cast<Plant*>(obj.get())) plants.push_back(p);
            else if (auto z = dynamic_cast<Enemy*>(obj.get())) zombies.push_back(z);
            else if (auto proj = dynamic_cast<Projectile*>(obj.get())) projectiles.push_back(proj);
        }

        // Kolizja 1: Pocisk vs Zombie
        for (auto* proj : projectiles) {
            for (auto* z : zombies) {
                if (proj->getRow() == z->getRow() && proj->getBounds().findIntersection(z->getBounds())) {
                    z->takeDamage(proj->getDamage());
                    proj->destroy();
                    break; 
                }
            }
        }

        // Kolizja 2: Zombie vs Roślina
        for (auto* z : zombies) {
            for (auto* p : plants) {
                if (z->getRow() == p->getRow() && z->getBounds().findIntersection(p->getBounds())) {
                    z->setEating(true);
                    p->takeDamage(z->getDamage() * dt);
                }
            }
        }
    }
};

// Spóźniona definicja logiczna dla słonecznika
void SunPlant::update(float dt, Game& game) {
    sunTimer += dt;
    if (sunTimer >= sunInterval) {
        game.addSun(25);
        game.addProjectile(x + 60, y + 20, row); // Bezpiecznie dodaje do bufora
        sunTimer = 0.0f;
    }
}

// ============================================================================
// 4. GŁÓWNA PĘTLA GRY
// ============================================================================

int main() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> rowDist(0, 4);

    int width = 1200;
    int height = 800;
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u({static_cast<unsigned int>(width), static_cast<unsigned int>(height)})), "Plants vs Zombies - Poprawiona wersja");
    window.setFramerateLimit(60);

    Game game;

    // Postawienie 3 startowych roślin testowych
    game.objects.push_back(std::make_unique<SunPlant>(150.0f, game.rowPositions[0], 0));
    game.objects.push_back(std::make_unique<SunPlant>(150.0f, game.rowPositions[2], 2));
    game.objects.push_back(std::make_unique<SunPlant>(150.0f, game.rowPositions[4], 4));

    sf::Clock clock;
    float zombieSpawnTimer = 0.0f;
    float difficultyFactor = 1.0f;

    std::cout << "Kompilacja udana! Gra dziala stabilnie pod SFML 3.\n";

    while (window.isOpen()) {
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        float dt = clock.restart().asSeconds();

        // --- TIMER GENEROWANIA PRZECIWNIKÓW ---
        zombieSpawnTimer += dt;
        if (zombieSpawnTimer >= (5.0f / difficultyFactor)) {
            int randomRow = rowDist(gen);
            float spawnY = game.rowPositions[randomRow] + 5.0f;
            
            game.objects.push_back(std::make_unique<BasicZombie>(1200.0f, spawnY, randomRow));
            
            zombieSpawnTimer = 0.0f;
            difficultyFactor += 0.04f;
            std::cout << "Spawn Zombie! Trudnosc: " << difficultyFactor << "\n";
        }

        // --- 1. AKTUALIZACJA AKTUALNYCH OBIEKTÓW ---
        for (auto& obj : game.objects) {
            obj->update(dt, game);
        }

        // --- 2. BEZPIECZNE PRZEPIĘCIE ELEMENTÓW Z BUFORA GRY ---
        if (!game.newObjectsBuffer.empty()) {
            for (auto& newObj : game.newObjectsBuffer) {
                game.objects.push_back(std::move(newObj));
            }
            game.newObjectsBuffer.clear(); // Oczyszczenie bufora na następną klatkę
        }

        // --- 3. SPRAWDZANIE KOLIZJI ORAZ CZYSZCZENIE ---
        game.handleCollisions(dt);

        game.objects.erase(std::remove_if(game.objects.begin(), game.objects.end(),
            [](const std::unique_ptr<GameObject>& obj) { return !obj->getIsActive(); }),
            game.objects.end());

        // --- 4. RYSOWANIE PLANSZY ---
        window.clear(sf::Color(34, 112, 34)); // Trawa

        for (auto& obj : game.objects) {
            obj->draw(window);
        }

        window.display();
    }

    return 0;
}