#ifndef HEADER_HPP_INCLUDED
#define HEADER_HPP_INCLUDED

#include <SFML/Graphics.hpp>
#include <vector>
#include <fstream>
#include <memory>

class Box;

class Coin;

class Player;

class Background{

    std::shared_ptr <Player> player;

    std::vector <std::shared_ptr <Box>> box;
    std::vector <std::shared_ptr <Coin>> coin;

    std::shared_ptr <sf::RenderWindow> window;

    sf::Event event;

    std::shared_ptr <sf::RectangleShape> meta;

    int level = 1;

    bool theEnd = false;

    public:

    void gameLoop();

    Background();

    void add_boxes(std::ifstream &plik);
    void add_meta(std::ifstream &plik);
    void add_coins(std::ifstream &plik);
    void add_player(std::ifstream &plik);
    void start();

    void clearPlansza();

    void logikaPlanszy();

    void rysowanie();

    sf::RectangleShape &getMeta();

    bool win();
    bool lose();

    std::shared_ptr <Player> return_player();
    std::vector <std::shared_ptr <Box>> &return_box();
    std::vector <std::shared_ptr <Coin>> &return_coin();
};

class Player{

    sf::RectangleShape player;

    std::shared_ptr <sf::RenderWindow> window;

    std::shared_ptr <Background> bg;

    const int g = 10;

    sf::Clock t;

    float v_0;

    float v;

    bool up_pressed;

    bool fall;

    float grav_p;

    float grav_p_t;

    int collected_coins;

    public:

    Player(std::shared_ptr <sf::RenderWindow> window1, std::shared_ptr <Background> bg1);

    void jump();

    void jump_d();

    void player_moving(std::vector <std::shared_ptr <Box>> &box);

    void change_grav(std::vector <std::shared_ptr <Box>> &box);

    void collect_coin(std::vector <std::shared_ptr <Coin>> &coin);

    bool collision_l(std::shared_ptr <Box> box1);

    bool collision_r(std::shared_ptr <Box> box1);

    void logika();

    sf::RectangleShape &getPlayer();
};

class Box{

    sf::RectangleShape box;

    friend class Player;

    public:

    Box();

    sf::RectangleShape& getBox();
};

class Coin{

    sf::CircleShape coin;

    friend class Player;

    public:

    Coin();

    sf::CircleShape& getCoin();
};

#endif // HEADER_HPP_INCLUDED
