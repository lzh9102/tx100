/*
 * Copyright (C) 2012 Timothy Lin
 * This work is licensed under GPLv3 as published by the Free Software
 * Foundation. Please see http://www.gnu.org/licenses/gpl.html for details.
 * 
 * For more information, please visit the project homepage:
 * http://code.google.com/p/tx100
 */

#ifndef PLAYER_H
#define	PLAYER_H

#include <SFML/Graphics.hpp>
#include <list>
#include "bullet.h"

struct PlayerInput
{
    bool up, down, left, right;
};

class Player
{
public:
    Player();
    virtual ~Player();
    
    virtual void start();
    virtual void stop();
    
    virtual void render(sf::RenderWindow& w);
    
    /* render the player at position (x,y) */
    virtual void render(sf::RenderWindow& w, int x, int y);
    
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
    void setPosition(float x, float y);
    
    void constraint(int w, int h);
    
    bool isAlive() const;
    
    bool setImage(const char *filename);
    
    const sf::Sprite& getSprite() const;
    
private:
    // disable copy
    Player(const Player&);
    Player& operator=(const Player&);
    
    struct Private;
    Private *p;
};

#endif	/* PLAYER_H */

