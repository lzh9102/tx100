/* 
 * File:   menu.h
 * Author: timothy
 *
 * Created on 2012年4月20日, 上午 11:12
 */

#ifndef MENU_H
#define	MENU_H

#include <SFML/Graphics.hpp>
#include <vector>

class Menu {
public:
    Menu(sf::RenderWindow& window);
    virtual ~Menu();
    
    void render();
    void keypress(sf::Key::Code);
    
    void append(const char *s);
    
    void setSelection(int n);
    int getSelection() const;
    
    void up();
    void down();
    
    sf::FloatRect getRect() const;
    
private:
    Menu(const Menu& orig);
    std::vector<sf::String> m_list;
    int m_sel;
    int m_width, m_height;
    sf::RenderWindow *m_window;
    void adjust_layout();
    void text_center(sf::String& text, sf::RenderWindow& window);
    void text_align_bottom(const sf::String& ref, sf::String& s);
};

#endif	/* MENU_H */

