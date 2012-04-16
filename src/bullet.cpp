/*
 * Copyright (C) 2012 Timothy Lin
 * This work is licensed under GPLv3 as published by the Free Software
 * Foundation. Please see http://www.gnu.org/licenses/gpl.html for details.
 */

#include "bullet.h"
#include "vectorhelper.h"
#include <cmath>

#define ACCEL_RATE 1.5
#define TRACK_PARAM 120

Bullet::Bullet() : type(0) { }

Bullet::Bullet(int x, int y) : pos(x, y), type(0) { }

Bullet::Bullet(int x, int y, int vx, int vy) : pos(x, y), vel(vx, vy), type(0) { }

Bullet::~Bullet() { }

void Bullet::step(float t, float speed, sf::Vector2f& target)
{
    pos += vel * t;
    switch (type) {
        case ACCELERATING:
            vel *= (float)pow(ACCEL_RATE, t);
            break;
        case TRACKING:
            vel += vector_normalize(target - pos) * (float)(TRACK_PARAM * t);
            vel = vector_normalize(vel) * speed;
            break;
    }
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