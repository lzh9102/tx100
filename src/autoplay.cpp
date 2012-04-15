/* 
 * File:   autoplay.cpp
 * Author: timothy
 * 
 * Created on 2012年4月15日, 下午 3:04
 */

#include "autoplay.h"
#include "vectorhelper.h"
#include <SFML/System/Vector2.hpp>

AutoPlay::AutoPlay(Player& player, std::list<Bullet>& bullet_list)
        : m_player(&player), m_list(&bullet_list)
{
}

AutoPlay::~AutoPlay()
{
}

void AutoPlay::step(float t)
{
    sf::Vector2f v;
    std::list<Bullet>::iterator it;
    PlayerInput input = {};
    for (it=m_list->begin(); it!=m_list->end(); ++it) {
        Bullet& bullet = *it;
        const sf::Vector2f difference = m_player->getPosition() - bullet.pos;
        const float distance = vector_length(difference);
        sf::Vector2f normal = sf::Vector2f(-bullet.vel.y, bullet.vel.x);
        normal = vector_normalize(normal) * (distance * distance);
        if (vector_dot(difference, bullet.vel) > 0)
            v += normal;
        //v += - (difference * (float)0.01);
    }
    
    input.up = v.y < 0;
    input.down = v.y > 0;
    input.left = v.x < 0;
    input.right = v.x > 0;
    
    m_player->step(t, input);
}

