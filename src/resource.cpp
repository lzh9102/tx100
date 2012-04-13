/* 
 * File:   resource.cpp
 * Author: timothy
 * 
 * Created on 2012年1月26日, 下午 11:07
 */

#include "resource.h"
#include <string>
#include <exception>

#define MAX_PLAYER 4

namespace
{
    sf::Image img_player;
}

void Resource::loadAll()
{
    img_player.LoadFromFile("player.png");
}

const sf::Image& Resource::getPlayerImage(int index)
{
    if (index >= MAX_PLAYER)
        throw std::exception();
    return img_player;
}
