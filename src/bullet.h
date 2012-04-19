/*
 * Copyright (C) 2012 Timothy Lin
 * This work is licensed under GPLv3 as published by the Free Software
 * Foundation. Please see http://www.gnu.org/licenses/gpl.html for details.
 */

#ifndef BULLET_H
#define	BULLET_H

#include <SFML/System/Vector2.hpp>

#define BULLET_RADIUS 3

class Bullet {
public:
    
    Bullet();
    Bullet(int x, int y);
    Bullet(int x, int y, int vx, int vy);
    virtual ~Bullet();
    
    void step(float t, float speed, const sf::Vector2f& target);
    
    /* track the target */
    void step_track(float t, sf::Vector2f target, float speed);
    
    bool detectCollision(int x, int y, int rad) const;
    bool detectCollision(sf::Vector2f p, int rad) const;
    
    /**
     * Rebound with respect to the center point.
     * @param center
     */
    void rebound(sf::Vector2f center);
    
    sf::Vector2f pos; /* position */
    sf::Vector2f vel; /* velocity */
    
    enum BulletType {
        NORMAL,
        TRACKING,
        ACCELERATING
    };
    int type;
};

#endif	/* BULLET_H */

