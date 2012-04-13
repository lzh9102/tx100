/* 
 * File:   Bullet.h
 * Author: timothy
 *
 * Created on 2012年4月13日, 下午 1:49
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
    Bullet(const Bullet& orig);
    virtual ~Bullet();
    
    bool step(float t);
    
    bool detectCollision(int x, int y, int rad) const;
    bool detectCollision(sf::Vector2f p, int rad) const;
    
    /**
     * Rebound with respect to the center point.
     * @param center
     */
    void rebound(sf::Vector2f center);
    
    sf::Vector2f pos; /* position */
    sf::Vector2f vel; /* velocity */
};

#endif	/* BULLET_H */

