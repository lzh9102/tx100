/* 
 * File:   menu.cpp
 * Author: timothy
 * 
 * Created on 2012年4月20日, 上午 11:12
 */

#include "menu.h"

#define BORDER_WIDTH 3

Menu::Menu(sf::RenderWindow& window) : m_window(&window) , m_sel(0),
        m_width(0), m_height(0)
{
    
}

Menu::~Menu() {
}

void Menu::render()
{
    for (int i=0; i<m_list.size(); i++) {
        m_window->Draw(m_list[i]);
        if (m_sel == i) {
            sf::FloatRect rect = m_list[i].GetRect();
            sf::Shape polygon;
            polygon.AddPoint(rect.Left, rect.Top, sf::Color::White, sf::Color::White);
            polygon.AddPoint(rect.Right, rect.Top, sf::Color::White, sf::Color::White);
            polygon.AddPoint(rect.Right, rect.Bottom, sf::Color::White, sf::Color::White);
            polygon.AddPoint(rect.Left, rect.Bottom, sf::Color::White, sf::Color::White);
            polygon.EnableFill(false);
            polygon.EnableOutline(true);
            polygon.SetOutlineWidth(BORDER_WIDTH);
            m_window->Draw(polygon);
        }
    }
}

void Menu::keypress(sf::Key::Code keycode)
{
    
}

void Menu::append(const char* s)
{
    m_list.push_back(sf::String(s));
    adjust_layout();
}

void Menu::setSelection(int n)
{
    if (n >= 0 && n < m_list.size())
        m_sel = n;
}

int Menu::getSelection() const
{
    return m_sel;
}

void Menu::up()
{
    if (!m_list.empty())
        m_sel = (m_sel + m_list.size() - 1) % m_list.size();
}

void Menu::down()
{
    if (!m_list.empty())
        m_sel = (m_sel + 1) % m_list.size();
}

sf::FloatRect Menu::getRect() const
{
    sf::FloatRect rect;
    if (!m_list.empty()) {
        sf::FloatRect rect0 = m_list[0].GetRect();
        rect.Top = rect0.Top;
        rect.Bottom = m_list[m_list.size()-1].GetRect().Bottom;
        rect.Left = rect0.Left + (rect0.GetWidth() - m_width) / 2;
        rect.Right = rect0.Left + (rect0.GetWidth() + m_width) / 2;
    }
    return rect;
}

void Menu::adjust_layout()
{
    if (m_list.empty())
        return;
    
    int h = 0, w = 0;
    for (int i=0; i<m_list.size(); i++) {
        h += m_list[i].GetRect().GetHeight();
        if (m_list[i].GetRect().GetWidth() > w)
            w = m_list[i].GetRect().GetWidth();
        text_center(m_list[i], *m_window);
    }
    
    m_list[0].SetY((m_window->GetHeight() - h) / 2);
    for (int i=1; i<m_list.size(); i++) {
        text_align_bottom(m_list[i-1], m_list[i]);
    }
    
    m_width = w;
    m_height = h;
}

void Menu::text_center(sf::String& text, sf::RenderWindow& window)
{
    const int width = window.GetWidth(), height = window.GetHeight();
    text.SetPosition((width - text.GetRect().GetWidth()) / 2,
        (height - text.GetRect().GetHeight()) / 2);
}

/* s.top = ref.bottom */
void Menu::text_align_bottom(const sf::String& ref, sf::String& s)
{
    s.SetY(ref.GetRect().Bottom);
}