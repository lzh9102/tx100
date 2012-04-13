#ifndef GAME_H
#define	GAME_H

#include <SFML/Graphics.hpp>

class Game
{
public:
    Game(int width, int height);
    virtual ~Game();
    
    /**
     * Draw the game scene on the window.
     * @param w
     */
    void render(sf::RenderWindow& w);
    
    /**
     * Run the game for t seconds.
     * @param t
     */
    void step(float t, const sf::Input& input);
    
    void generateBullets(int count);
    
private:
    // disable copy
    Game(const Game&);
    Game& operator=(const Game&);
    
    struct Private;
    Private *p;
};

#endif	/* SCENE_H */

