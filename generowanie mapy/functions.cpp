#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <windows.h>
#include <math.h>
#include <vector>
#include <string>
#include <fstream>

#include "header.hpp"

using namespace std;
using namespace sf;

Background::Background(){

    window = make_shared <RenderWindow> ();

    window -> create(VideoMode(1500, 600), "SFML works!");

    window -> setFramerateLimit(30);

    add_player();
}

vector <shared_ptr <Box>> &Background::return_box(){

    return box;
}

vector <shared_ptr <Coin>> &Background::return_coin(){

    return coin;
}

void Background::events(){

    if (event.type == Event::Closed){

        //bg -> zapisanieDoPliku();

        window -> close();
    }

    if(event.type == sf::Event::KeyPressed){

        if(event.key.code == sf::Keyboard::B){

            add_boxes();
        }

        if(event.key.code == sf::Keyboard::C){

            add_coins();
        }

        if(event.key.code == sf::Keyboard::D){

            zdarzenieUsuwania();
        }

        if(event.key.code == sf::Keyboard::Up){

            zdarzenieZwiekszeniaY();
        }

        if(event.key.code == sf::Keyboard::Down){

            zdarzenieZmniejszeniaY();
        }

        if(event.key.code == sf::Keyboard::Left){

            zdarzenieZmniejszeniaX();
        }

        if(event.key.code == sf::Keyboard::Right){

            zdarzenieZwiekszeniaX();
        }

        if(event.key.code == sf::Keyboard::R){

            restart();
        }

        if(event.key.code == sf::Keyboard::S){

            zapisanieDoPliku();
        }
    }

    if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left){

        zdarzeniePierwszegoKlikniecia();
    }
}

void Background::drawAll(){

    window -> draw(player -> getBox());

    for(unsigned int i=0; i < box.size(); i++){

        window -> draw(box.at(i) -> getBox());
    }

    for(unsigned int i=0; i < coin.size(); i++){

        window -> draw(coin.at(i) -> getCoin());
    }
}

void Background::programLoop(){

    while (window -> isOpen()){

        while (window -> pollEvent(event)){

            events();
        }

        window -> clear(Color(33, 130, 241));

        poruszanieKliknietym();

        drawAll();

        window -> display();
    }
}

void Background::add_player(){

    player = make_shared <Player> ();
}

void Background::add_boxes(){

    box.push_back(make_shared <Box> ());

    if(box.size() == 1){

        box[0] -> box.setFillColor(Color::Blue);
    }
    else if(box.size() > 1){

        box.back() -> box.setFillColor(Color::Cyan);

        if(box.size() > 2){

            box[box.size() - 2] -> box.setFillColor(Color::Green);
        }
    }
}

void Background::add_coins(){

    coin.push_back(make_shared <Coin> ());
}

void Background::restart(){

    player = nullptr;

    box.clear();

    coin.clear();

    add_player();
}

string Grafika::getName(){

    return nazwa;
}

shared_ptr <Player> Background::return_player(){

    return player;
}

std::shared_ptr <Grafika> Background::sprawdzanieCzyKliknietyObiekt(){

    for(unsigned int i=0; i < box.size(); i++){

        if(Mouse::getPosition(*window).x > box[i] -> box.getPosition().x &&
           Mouse::getPosition(*window).x <  box[i] -> box.getPosition().x +  box[i] -> box.getSize().x &&
            Mouse::getPosition(*window).y >  box[i] -> box.getPosition().y &&
            Mouse::getPosition(*window).y <  box[i] -> box.getPosition().y +  box[i] -> box.getSize().y){

                keyPressed = true;

                return box[i];
        }
    }

    for(unsigned int i=0; i < coin.size(); i++){

        if(Mouse::getPosition(*window).x > coin[i] -> circle.getPosition().x &&
           Mouse::getPosition(*window).x < coin[i] -> circle.getPosition().x + coin[i] -> circle.getRadius()*2 &&
            Mouse::getPosition(*window).y > coin[i] -> circle.getPosition().y &&
            Mouse::getPosition(*window).y < coin[i] -> circle.getPosition().y + coin[i] -> circle.getRadius()*2){

                keyPressed = true;

                return coin[i];
        }
    }

    if(Mouse::getPosition(*window).x > player -> getBox().getPosition().x &&
        Mouse::getPosition(*window).x < player -> getBox().getPosition().x + player -> getBox().getSize().x &&
        Mouse::getPosition(*window).y > player -> getBox().getPosition().y &&
        Mouse::getPosition(*window).y < player -> getBox().getPosition().y + player -> getBox().getSize().y){

            keyPressed = true;

            return player;
    }

    return nullptr;

}

void Background::zdarzeniePierwszegoKlikniecia(){

    if(keyPressed == false && cel == nullptr){

        cel = sprawdzanieCzyKliknietyObiekt();
    }
    else{

        cel = nullptr;

        keyPressed = false;
    }
}

void Background::poruszanieKliknietym(){

    if(keyPressed == true){

        if(cel -> getName() == "box"){

            cel -> box.setPosition(Mouse::getPosition(*window).x, Mouse::getPosition(*window).y);
        }
        else if(cel -> getName() == "coin"){

            cel -> circle.setPosition(Mouse::getPosition(*window).x, Mouse::getPosition(*window).y);
        }
        else if(cel -> getName() == "player"){

            cel -> box.setPosition(Mouse::getPosition(*window).x, Mouse::getPosition(*window).y);
        }
    }
}

void Background::zdarzenieZwiekszeniaX(){

    if(cel != nullptr){

        if(cel -> getName() == "box"){

            cel -> box.setSize(Vector2f(cel -> box.getSize().x + 10, cel -> box.getSize().y));
        }
        else if(cel -> getName() == "coin"){

            cel -> circle.setRadius(cel -> circle.getRadius() + 10);
        }
        if(cel -> getName() == "player"){

            cel -> box.setSize(Vector2f(cel -> box.getSize().x + 10, cel -> box.getSize().y));
        }
    }
}

void Background::zdarzenieZmniejszeniaX(){

    if(cel != nullptr){

        if(cel -> getName() == "box"){

            cel -> box.setSize(Vector2f(cel -> box.getSize().x - 10, cel -> box.getSize().y));
        }
        else if(cel -> getName() == "coin"){

            cel -> circle.setRadius(cel -> circle.getRadius() - 10);
        }
        if(cel -> getName() == "player"){

            cel -> box.setSize(Vector2f(cel -> box.getSize().x - 10, cel -> box.getSize().y));
        }
    }
}

void Background::zdarzenieZwiekszeniaY(){

    if(cel != nullptr){

        if(cel -> getName() == "box"){

            cel -> box.setSize(Vector2f(cel -> box.getSize().x, cel -> box.getSize().y + 10));
        }
        else if(cel -> getName() == "coin"){

            cel -> circle.setRadius(cel -> circle.getRadius() + 10);
        }
        if(cel -> getName() == "player"){

            cel -> box.setSize(Vector2f(cel -> box.getSize().x, cel -> box.getSize().y + 10));
        }
    }
}

void Background::zdarzenieZmniejszeniaY(){

    if(cel != nullptr){

        if(cel -> getName() == "box"){

            cel -> box.setSize(Vector2f(cel -> box.getSize().x, cel -> box.getSize().y - 10));
        }
        else if(cel -> getName() == "coin"){

            cel -> circle.setRadius(cel -> circle.getRadius() - 10);
        }
        if(cel -> getName() == "player"){

            cel -> box.setSize(Vector2f(cel -> box.getSize().x, cel -> box.getSize().y - 10));
        }
    }
}

void Background::zdarzenieUsuwania(){

    if(cel != nullptr){

        for(unsigned int i=0; i < box.size(); i++){

            if(cel == box[i]){

                box.erase(box.begin() + i);

                keyPressed = false;

                cel = nullptr;

                break; //?
            }
        }

        for(unsigned int i=0; i < coin.size(); i++){

            if(cel == coin[i]){

                coin.erase(coin.begin() + i);

                keyPressed = false;

                cel = nullptr;

                break; //?
            }
        }
    }
}

void Background::zapisanieDoPliku(){

    ofstream plik( "poziomy/Poziom " +  to_string(poziom) + ".txt");

    if(plik.good() == true){

        if(box.size() > 2){

            for(unsigned int i=0; i < box.size() - 1; i++){

                plik << "*Box:" << i + 1 << ":" <<endl;

                plik << box[i] -> box.getPosition().x <<endl;
                plik << box[i] -> box.getPosition().y <<endl;

                plik << box[i] -> box.getSize().x <<endl;
                plik << box[i] -> box.getSize().y <<endl;
            }
        }

        if(box.size() > 1){

            plik << "*Meta:" <<endl;

            plik << box[box.size()-1] -> box.getPosition().x <<endl;
            plik << box[box.size()-1] -> box.getPosition().y <<endl;

            plik << box[box.size()-1] -> box.getSize().x <<endl;
            plik << box[box.size()-1] -> box.getSize().y <<endl;

        }

        for(unsigned int i=0; i < coin.size(); i++){

            plik << "*Coin:" << i + 1 << ":" <<endl;

            plik << coin[i] -> circle.getPosition().x <<endl;
            plik << coin[i] -> circle.getPosition().y <<endl;

            plik << coin[i] -> circle.getRadius() <<endl;
        }

        plik << "*Player:" <<endl;

        plik << player -> box.getPosition().x <<endl;
        plik << player -> box.getPosition().y <<endl;

        plik << player -> box.getSize().x <<endl;
        plik << player -> box.getSize().y <<endl;

        plik.close();
    }

    poziom++;
}

Player::Player(){

    nazwa = "player";

    box.setSize(Vector2f(50, 50));
    box.setPosition(200, 200);
    box.setFillColor(Color(Color::Black));

}

Box::Box(){

    nazwa = "box";

    box.setSize(Vector2f(50, 50));
    box.setPosition(100, 100);
    box.setFillColor(Color(Color::Green));
}

RectangleShape Box::getBox(){

    return box;
}

RectangleShape Player::getBox(){

    return box;
}

Coin::Coin(){

    nazwa = "coin";

    circle.setRadius(25);
    circle.setPosition(300, 100);
    circle.setFillColor(Color(Color::Yellow));
}

CircleShape Coin::getCoin(){

    return circle;
}

