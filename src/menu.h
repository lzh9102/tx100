/*
 * Copyright (C) 2012 Timothy Lin
 * This work is licensed under GPLv3 as published by the Free Software
 * Foundation. Please see http://www.gnu.org/licenses/gpl.html for details.
 * 
 * For more information, please visit the project homepage:
 * http://code.google.com/p/tx100
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

