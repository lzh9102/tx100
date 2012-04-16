/*
 * Copyright (C) 2012 Timothy Lin
 * This work is licensed under GPLv3 as published by the Free Software
 * Foundation. Please see http://www.gnu.org/licenses/gpl.html for details.
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
