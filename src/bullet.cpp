/* 
 * File:   Bullet.cpp
 * Author: timothy
 * 
 * Created on 2012年4月13日, 下午 1:49
 */

#include "bullet.h"
#include "vectorhelper.h"
#include <cmath>

Bullet::Bullet() { }

Bullet::Bullet(int x, int y) : pos(x, y) { }

Bullet::Bullet(int x, int y, int vx, int vy) : pos(x, y), vel(vx, vy) { }

Bullet::Bullet(const Bullet& orig) : pos(orig.pos), vel(orig.vel) { }

Bullet::~Bullet() { }

void Bullet::step(float t)
{
    pos += vel * t;
}

bool Bullet::detectCollision(int x, int y, int rad) const
{
    int dx = std::abs(x - pos.x), dy = std::abs(y - pos.y);
    int mindist = rad + BULLET_RADIUS;
    return dx*dx + dy*dy <= mindist*mindist;
}

bool Bullet::detectCollision(sf::Vector2f p, int rad) const
{
    detectCollision(p.x, p.y, rad);
}

void Bullet::rebound(sf::Vector2f center)
{
    sf::Vector2f diff = pos - center;
    sf::Vector2f proj = vector_projection(vel, diff);
    vel -= (float)2.0 * proj;
}