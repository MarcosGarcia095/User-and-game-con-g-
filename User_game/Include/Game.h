#ifndef GAME_H
#define GAME_H

#include <string>

class Game {
public:
    int id;
    std::string name;

    Game(int id, const std::string& name) : id(id), name(name) {}
};

#endif
