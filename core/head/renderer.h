#pragma once
#include <vector>
#include <iostream>

namespace FGames::core
{

class Renderer
{
public:
    Renderer(int width = 40, int height = 20);
    void clear();
    void draw(int x, int y, char symbol);
    void present();

private:
    int width_;
    int height_;
    std::vector<std::string> buffer_;
};

}