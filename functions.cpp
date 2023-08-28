#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <fstream>
#include <conio.h>

#include "header.hpp"

using namespace std;
using namespace sf;

Background::Background(){

    window = make_shared <RenderWindow> ();

    window -> create(VideoMode(1500, 600), "Platformowka");

    window -> setFramerateLimit(30);

    start();
}

void Background::gameLoop(){

    while (window -> isOpen())
    {
        Event event;
        while (window -> pollEvent(event))
        {
            if (event.type == Event::Closed){

                window -> close();
            }

            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape){

                window -> close();
            }
        }

        window -> clear(Color(33, 130, 241));

        logikaPlanszy();

        window -> display();
    }
}

vector <shared_ptr <Box>> &Background::return_box(){

    return box;
}

vector <shared_ptr <Coin>> &Background::return_coin(){

    return coin;
}

RectangleShape &Background::getMeta(){

    return *meta;
}

sf::RectangleShape &Player::getPlayer(){

    return player;
}

shared_ptr <Player> Background::return_player(){

    return player;
}

void Background::start(){

    ifstream plik;

    plik.open("generowanie mapy/poziomy/Poziom " + to_string(level) +".txt");

    if(plik.is_open()){

        add_boxes(plik);

        add_meta(plik);

        add_coins(plik);

        add_player(plik);

        plik.close();
    }
    else{

        cout<< "Nie udalo sie wczytac pliku" <<endl;

        theEnd = true;
    }
}

void Background::add_meta(ifstream &plik){

    if(plik.is_open()){

        string odczyt;
        string odczyt1;

        meta = make_shared <RectangleShape> ();

        meta -> setFillColor(Color::Cyan);

        if(plik.eof() == false){

            getline(plik, odczyt);
            getline(plik, odczyt1);

            meta -> setPosition(atof(odczyt.c_str()), atof(odczyt1.c_str()));

            getline(plik, odczyt);
            getline(plik, odczyt1);

            meta -> setSize(Vector2f(atof(odczyt.c_str()), atof(odczyt1.c_str())));

            getline(plik, odczyt);
        }
    }
}

void Background::add_player(ifstream &plik){

    if(plik.is_open()){

        player = make_shared <Player> (window, make_shared <Background> (*this));

        string odczyt;
        string odczyt1;

        if(plik.eof() == false){

            getline(plik, odczyt);
            getline(plik, odczyt1);

            player -> getPlayer().setPosition(atof(odczyt.c_str()), atof(odczyt1.c_str()));

            getline(plik, odczyt);
            getline(plik, odczyt1);

            player -> getPlayer().setSize(Vector2f(atof(odczyt.c_str()), atof(odczyt1.c_str())));
        }
    }
}

void Background::add_boxes(ifstream &plik){

    if(plik.is_open()){

        string odczyt;
        string odczyt1;

        int n = 0;

        while(plik.eof() == false){

            getline(plik, odczyt);

            if(odczyt[1] != 'B'){

                break;
            }

            box.push_back(make_shared <Box> ());

            getline(plik, odczyt);
            getline(plik, odczyt1);

            box[n] -> getBox().setPosition(atof(odczyt.c_str()), atof(odczyt1.c_str()));

            getline(plik, odczyt);
            getline(plik, odczyt1);

            box[n] -> getBox().setSize(Vector2f(atof(odczyt.c_str()), atof(odczyt1.c_str())));

            n++;
        }
    }
}

void Background::add_coins(ifstream &plik){

    if(plik.is_open()){

        string odczyt;
        string odczyt1;

        int n = 0;

        while(plik.eof() == false){

            coin.push_back(make_shared <Coin> ());

            getline(plik, odczyt);

            getline(plik, odczyt1);

            coin[n] -> getCoin().setPosition(atof(odczyt.c_str()), atof(odczyt1.c_str()));

            getline(plik, odczyt);

            coin[n] -> getCoin().setRadius(atof(odczyt.c_str()));

            getline(plik, odczyt);

            if(odczyt[1] != 'C'){

                break;
            }

            n++;
        }
    }
}

void Background::clearPlansza(){

    player = nullptr;

    meta = nullptr;

    coin.clear();

    box.clear();
}

Player::Player(shared_ptr <RenderWindow> window1, std::shared_ptr <Background> bg1){

    window = window1;

    bg = bg1;

    player.setFillColor(Color(Color::Black));

    fall = true;

    v_0 = 8;

    v = v_0;

    up_pressed = false;

    grav_p = 0;

    grav_p_t = window -> getSize().y;

    collected_coins = 0;
}

void Background::logikaPlanszy(){

    if(theEnd == false){

        if(win() == false && lose() == false){

            player -> player_moving(box);

            player -> logika();

            player -> collect_coin(coin);

            rysowanie();
        }
    }
}

void Background::rysowanie(){

    for(unsigned int i=0; i < return_coin().size(); i++){

        window -> draw(coin.at(i) -> getCoin());
    }

    for(unsigned int i=0; i < return_box().size(); i++){

        window -> draw(box.at(i) -> getBox());
    }

    window -> draw(getMeta());

    window -> draw(player -> getPlayer());
}

void Player::jump(){

    if(v == v_0){

        t.restart();
    }

    if(v > 0 && player.getPosition().y > grav_p_t){

        v = v_0 - g*t.getElapsedTime().asSeconds();

        if(player.getPosition().y - v > grav_p_t){

            player.move(0, -v);
        }
        else{

            player.setPosition(player.getPosition().x, grav_p_t);
        }
    }
    else{

        up_pressed = false;

        fall = true;

        v = 0;

        t.restart();
    }
}

void Player::jump_d(){

    if(v == 0){

        t.restart();
    }

    if(player.getPosition().y + player.getSize().y < grav_p){

        v = g*t.getElapsedTime().asSeconds();

        if(player.getPosition().y + player.getSize().y + v < grav_p){

            player.move(0, v);
        }
        else{

            player.setPosition(player.getPosition().x, grav_p - player.getSize().y);
        }
    }
    else{

        fall = false;

        t.restart();

        v = v_0;
    }
}
void Player::player_moving(std::vector <shared_ptr <Box>> &box){

    if(Keyboard::isKeyPressed(Keyboard::Up) && up_pressed == false && fall == false){

        up_pressed = true;
    }

    if(Keyboard::isKeyPressed(Keyboard::Left) && player.getPosition().x > 0){

        bool do_move = false;

        for(unsigned int i=0; i < box.size(); i++){

            if(collision_l(box[i]) == false){

                do_move = true;
            }
            else{

                do_move = false;

                player.setPosition(box[i] -> getBox().getPosition().x + box[i] -> getBox().getSize().x, player.getPosition().y);

                break;
            }
        }

        if(do_move == true){

            player.move(-v_0, 0);

            change_grav(box);
        }
    }

    if(Keyboard::isKeyPressed(Keyboard::Right) && player.getPosition().x + player.getSize().x < window -> getSize().x){

        bool do_move = false;

        for(unsigned int i=0; i < box.size(); i++){

            if(collision_r(box[i]) == false){

                do_move = true;
            }
            else{

                do_move = false;

                player.setPosition(box[i] -> getBox().getPosition().x - player.getSize().x, player.getPosition().y);

                break;
            }
        }

        if(do_move == true){

            player.move(v_0, 0);

            change_grav(box);
        }
    }
}

void Player::logika(){

    if(up_pressed == true){

        jump();

        change_grav(bg -> return_box());
    }

    if(fall == true){

        jump_d();

        change_grav(bg -> return_box());
    }

    if(up_pressed == false && fall == false){

        t.restart();
    }
}

Box::Box(){

    box.setFillColor(Color(Color::Green));
}

void Player::change_grav(vector <shared_ptr<Box>> &box1){

    bool change_p = false;

    bool change_p_t = false;

    float check_grav_p_t = 0;

    float check_grav_p = window -> getSize().y;

    for(unsigned int i=0; i < box1.size(); i++){

        if((player.getPosition().x < box1[i] -> box.getPosition().x + box1[i] -> box.getSize().x &&
            player.getPosition().x + player.getSize().x > box1[i] -> box.getPosition().x)||
            (player.getPosition().x == box1[i] -> box.getPosition().x &&
           player.getPosition().x + player.getSize().x == box1[i] -> box.getPosition().x + box1[i] -> box.getSize().x)){

            if(player.getPosition().y >= box1[i] -> box.getPosition().y + box1[i] -> box.getSize().y){

                if(box1[i] -> box.getPosition().y + box1[i] -> box.getSize().y > check_grav_p_t || check_grav_p_t == 0){

                    check_grav_p_t = box1[i] -> box.getPosition().y + box1[i] -> box.getSize().y;
                }

                change_p_t = true;
            }

            if(player.getPosition().y + player.getSize().y <= box1[i] -> getBox().getPosition().y){

                if(box1[i] -> box.getPosition().y < check_grav_p){

                    check_grav_p = box1[i] -> box.getPosition().y;
                }

                change_p = true;
            }
        }
    }

    if(change_p == false){

        grav_p = window -> getSize().y;
    }
    else{

        grav_p = check_grav_p;
    }

    if(change_p_t == false){

        grav_p_t = 0;
    }
    else{

        grav_p_t = check_grav_p_t;
    }

    if(fall == false && up_pressed == false && player.getPosition().y + player.getSize().y < grav_p){

        fall = true;

        v = 0;
    }
}

bool Player::collision_l(shared_ptr <Box> box1){

    if(player.getPosition().x > box1 -> box.getPosition().x &&
       player.getPosition().x - v_0 < box1 -> box.getPosition().x + box1 -> box.getSize().x &&
       player.getPosition().y < box1 -> box.getPosition().y + box1 -> box.getSize().y &&
       player.getPosition().y + player.getSize().y > box1 -> box.getPosition().y){

        return true;
    }
    else{

        return false;
    }
}

bool Player::collision_r(shared_ptr <Box> box1){

    if(player.getPosition().x + player.getSize().x + v_0 > box1 -> box.getPosition().x &&
       player.getPosition().x < box1 -> box.getPosition().x &&
       player.getPosition().y < box1 -> box.getPosition().y + box1 -> box.getSize().y &&
       player.getPosition().y + player.getSize().y > box1 -> box.getPosition().y){

        return true;
    }
    else{

        return false;
    }
}

RectangleShape& Box::getBox(){

    return box;
}

Coin::Coin(){

    coin.setFillColor(Color(Color::Yellow));
}

CircleShape& Coin::getCoin(){

    return coin;
}

void Player::collect_coin(vector <shared_ptr<Coin>> &coin){

    for(unsigned int i=0; i < coin.size(); i++){

        if(coin[i] -> coin.getGlobalBounds().intersects(player.getGlobalBounds())){

            coin.erase(coin.begin() + i);

            collected_coins++;
        }

    }
}

bool Background::lose(){

    if(player -> getPlayer().getPosition().y + player -> getPlayer().getSize().y >= window -> getSize().y){

        clearPlansza();

        theEnd = true;

        return true;
    }
    else{

        return false;
    }
}

bool Background::win(){

    if(player -> getPlayer().getGlobalBounds().intersects(getMeta().getGlobalBounds())){

        level++;

        clearPlansza();

        start();

        return true;
    }
    else{

        return false;
    }
}
