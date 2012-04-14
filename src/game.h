#ifndef GAME_H
#define	GAME_H

#include <SFML/Graphics.hpp>

class Game
{
public:
    Game(int width, int height);
    virtual ~Game();
    
    void restart();
    
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
    
    /**
     * @param flag If %flag is true, the game will be suspended. Otherwise, the game
     *       will be resumed.
     */
    void pause(bool flag);
    void togglePause();
    
    bool isGameOver() const;
    
private:
    // disable copy
    Game(const Game&);
    Game& operator=(const Game&);
    
    struct Private;
    Private *p;
};

#endif	/* SCENE_H */

