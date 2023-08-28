#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <memory>

#include "header.hpp"

using namespace std;
using namespace sf;

int main()
{

    shared_ptr <Background> bg = make_shared <Background> ();

    bg -> programLoop();

    return 0;
}
