/*
 * Copyright (C) 2012 Timothy Lin
 * This work is licensed under GPLv3 as published by the Free Software
 * Foundation. Please see http://www.gnu.org/licenses/gpl.html for details.
 */

#define PLAYER_SPEED (float)100

#include "player.h"
#include "vectorhelper.h"
#include <cmath>

#define SQRT_2 1.414

struct Player::Private
{
    sf::Image image;
    sf::Sprite sprite;
    sf::Vector2f pos; /* position */
    bool alive;

    Private() : alive(false)
    {
        image.LoadFromFile("player.png");
        sprite.SetImage(image);
    }
    
    void render(sf::RenderWindow& w)
    {
        const int x = pos.x - image.GetWidth() / 2;
        const int y = pos.y - image.GetHeight() / 2;
        sprite.SetPosition(x, y);
        w.Draw(sprite);
    }
};

Player::Player() : p(new Private)
{
}

Player::~Player()
{
    delete p;
}

void Player::start()
{
    p->alive = true;
}

void Player::stop()
{
    p->alive = false;
}

void Player::render(sf::RenderWindow& w)
{
    if (p->alive)
        p->render(w);
}

void Player::step(float t, const PlayerInput& input)
{
    sf::Vector2f delta;
    if (input.left)
        delta.x -= 1;
    if (input.right)
        delta.x += 1;
    if (input.up)
        delta.y -= 1;
    if (input.down)
        delta.y += 1;
    if (vector_length(delta) >= 0.5) // the player is moving
        p->pos += vector_normalize(delta) * (t * PLAYER_SPEED);
}

void Player::step(float t, const std::list<Bullet>& bullet_list,
        const sf::Vector2f& center)
{
    sf::Vector2f v;
    std::list<Bullet>::const_iterator it;

    for (it=bullet_list.begin(); it!=bullet_list.end(); ++it) {
        const Bullet& bullet = *it;
        const sf::Vector2f difference = p->pos - bullet.pos;
        const float distance = vector_length(difference);
        sf::Vector2f normal = vector_normalize(sf::Vector2f(-bullet.vel.y, bullet.vel.x));
        //if (distance > 10 * getWidth())
        //    normal = sf::Vector2f(0, 0);
        normal /= (float)0.05 * distance;
        if (vector_dot(difference, normal) < 0)
            normal = -normal;
        if (vector_dot(difference, bullet.vel) > 0)
            v += normal;
        //v += - (difference * (float)0.01);
    }
    
    sf::Vector2f deviation = center - p->pos;
    if (vector_length(deviation) >= 100)
        v += vector_normalize(deviation);
    
    if (vector_length(v) >= t)
        p->pos += vector_normalize(v) * PLAYER_SPEED * t;
}

float Player::getX() const
{
    return p->pos.x;
}

float Player::getY() const
{
    return p->pos.y;
}

int Player::getWidth() const
{
    return p->image.GetWidth();
}

int Player::getHeight() const
{
    return p->image.GetHeight();
}

sf::Vector2f Player::getPosition() const
{
    return p->pos;
}

float Player::setX(float x)
{
    return p->pos.x = x;
}

float Player::setY(float y)
{
    return p->pos.y = y;
}

void Player::constraint(int w, int h)
{
    const int player_width = p->image.GetWidth(), player_height = p->image.GetHeight();
    const int top_limit = player_height/2, bottom_limit = h - player_height/2;
    const int left_limit = player_width/2, right_limit = w - player_width/2;
    const int x = p->pos.x, y = p->pos.y;

    if (x < left_limit)
        p->pos.x = left_limit;
    else if (x >= right_limit)
        p->pos.x = right_limit;
    if (y < top_limit)
        p->pos.y = top_limit;
    else if (y >= bottom_limit)
        p->pos.y = bottom_limit;
}

int Player::getCriticalRadius() const
{
    /* be more permissive */
    return std::min(p->image.GetWidth(), p->image.GetHeight()) / (SQRT_2 * 2);
}

int Player::getReboundRadius() const
{
    return std::min((float)p->image.GetWidth(), (float)(p->image.GetHeight() / 2));
}

bool Player::isAlive() const
{
    return p->alive;
}