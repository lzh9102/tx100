/*
 * Copyright (C) 2012 Timothy Lin
 * This work is licensed under GPLv3 as published by the Free Software
 * Foundation. Please see http://www.gnu.org/licenses/gpl.html for details.
 * 
 * For more information, please visit the project homepage:
 * http://code.google.com/p/tx100
 */

#ifndef GAME_H
#define	GAME_H

#include <SFML/Graphics.hpp>
#include "player.h"

#define PLAYER_COUNT 2

class Game
{
public:
    Game(int width, int height);
    virtual ~Game();
    
    enum PlayerType {
        OFF,
        HUMAN,
        COMPUTER
    };
    
    void setBulletCount(unsigned int n);
    void setPlayerType(unsigned int index, PlayerType type);
    
    void restart();
    
    /**
     * Draw the game scene on the window.
     * @param w
     */
    void render(sf::RenderWindow& w);
    
    void setPlayerInput(unsigned int n, const PlayerInput& input);
    
    /**
     * Run the game for t seconds.
     * @param t
     */
    void step(float t);
    
    void generateBullets(int count);
    
    /**
     * @param flag If %flag is true, the game will be suspended. Otherwise, the game
     *       will be resumed.
     */
    void pause(bool flag);
    void togglePause();
    
    /**
     * Return the time since the game starts (in seconds).
     */
    float getTime() const;
    float getPlayerTime(unsigned int n) const;
    bool isGameOver() const;
    
    /* Write the encoded game status to the bytearray for transmission.
     * The buffer v must be large enough to hold the game status.
     * The maxlen parameter is just for debugging, the function does not check
     * whether data length exceed maxlen.
     * @return the length of the returned data
     */
    int encodeStatus(unsigned char *v, int maxlen) const;
    
    void renderStatus(const unsigned char *v, int len, sf::RenderWindow& w);
    
private:
    // disable copy
    Game(const Game&);
    Game& operator=(const Game&);
    
    struct Private;
    Private *p;
};

#endif	/* SCENE_H */

