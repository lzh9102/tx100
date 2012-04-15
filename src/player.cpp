/* 
 * File:   player.cpp
 * Author: timothy
 * 
 * Created on 2012年1月26日, 下午 10:58
 */

#define PLAYER_SPEED 100

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