#ifndef HEADER_HPP_INCLUDED
#define HEADER_HPP_INCLUDED

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

class Box;

class Coin;

class Player;

class Grafika;

class Background{

    std::shared_ptr <sf::RenderWindow> window;

    sf::Event event;

    std::shared_ptr <Player> player;

    std::vector <std::shared_ptr <Box>> box;
    std::vector <std::shared_ptr <Coin>> coin;

    bool keyPressed = false;

    int poziom = 1;

    std::shared_ptr <Grafika> cel = nullptr;

    public:

    Background();

    void events();

    void programLoop();

    void drawAll();

    void add_boxes();
    void add_coins();
    void add_player();

    std::vector <std::shared_ptr <Box>> &return_box();
    std::vector <std::shared_ptr <Coin>> &return_coin();
    std::shared_ptr <Player> return_player();

    std::shared_ptr <Grafika> sprawdzanieCzyKliknietyObiekt();
    void zdarzeniePierwszegoKlikniecia();
    void poruszanieKliknietym();

    void zdarzenieZwiekszeniaY();
    void zdarzenieZmniejszeniaY();
    void zdarzenieZwiekszeniaX();
    void zdarzenieZmniejszeniaX();

    void zdarzenieUsuwania();

    void zapisanieDoPliku();

    void restart();

};

class Grafika{

    protected:

    std::string nazwa;

    public:

    sf::RectangleShape box;

    sf::CircleShape circle;

    public:

    virtual sf::RectangleShape getBox() = 0;

    std::string getName();

    virtual ~Grafika(){}
};

class Player :public Grafika{

    public:

    Player();

    sf::RectangleShape getBox();
};

class Box :public Grafika{

    public:

    Box();

    sf::RectangleShape getBox();
};

class Coin :public Grafika{

    public:

    Coin();

    sf::CircleShape getCoin();

    sf::RectangleShape getBox(){

        return box;
    }
};

#endif // HEADER_HPP_INCLUDED
