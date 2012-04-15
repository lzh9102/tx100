/* 
 * File:   player.h
 * Author: timothy
 *
 * Created on 2012年1月26日, 下午 10:58
 */

#ifndef PLAYER_H
#define	PLAYER_H

#include <SFML/Graphics.hpp>
#include <list>
#include "bullet.h"

struct PlayerInput
{
    bool up, down, left, right;
    bool rebound;
};

class Player
{
public:
    Player();
    virtual ~Player();
    
    virtual void start();
    virtual void stop();
    
    virtual void render(sf::RenderWindow& w);
    
    virtual void step(float t, const PlayerInput& input);
    
    /* autoplay */
    void step(float t, const std::list<Bullet>& bullet_list, const sf::Vector2f& center);
    
    float getX() const;
    float getY() const;
    int getWidth() const;
    int getHeight() const;
    sf::Vector2f getPosition() const;
    float setX(float x);
    float setY(float y);
    
    void constraint(int w, int h);
    
    int getCriticalRadius() const;
    int getReboundRadius() const;
    
    bool isAlive() const;
    
private:
    // disable copy
    Player(const Player&);
    Player& operator=(const Player&);
    
    struct Private;
    Private *p;
};

#endif	/* PLAYER_H */

