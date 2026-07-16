#pragma once
#include <SFML/Graphics.hpp>

class Game {
private:
    sf::RenderWindow window;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;

    void initWindow();
    void initBackground();

public:
    Game();
    ~Game();

    void run();
    void processEvents();
    void update();
    void render();
};