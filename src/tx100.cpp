/*
 * Copyright (C) 2012 Timothy Lin
 * This work is licensed under GPLv3 as published by the Free Software
 * Foundation. Please see http://www.gnu.org/licenses/gpl.html for details.
 */

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include "game.h"
#include "menu.h"

#define RANDOM_SEED time(0)
#define BULLET_COUNT_DEFAULT 80
#define BULLET_COUNT_MIN 10
#define BULLET_COUNT_MAX 150

int event_loop(sf::RenderWindow& mainwindow);

enum {
    STATE_WAIT,
    STATE_INGAME,
    STATE_GAMEOVER
};

int bullet_count = BULLET_COUNT_DEFAULT;
int game_state = STATE_WAIT;

int main(int argc, char** argv)
{
    sf::RenderWindow mainwindow(sf::VideoMode(800, 600), "TX100 ("
            "http://code.google.com/p/tx100" ")",
            sf::Style::Close);
    
    srand(RANDOM_SEED);
    
    return event_loop(mainwindow);
}

void game_start(Game& game, Menu& mainmenu)
{
    switch (mainmenu.getSelection()) {
        case 0: /* play alone */
            game.setPlayerType(0, Game::HUMAN);
            game.setPlayerType(1, Game::OFF);
            break;
        case 1: /* play with computer */
            game.setPlayerType(0, Game::HUMAN);
            game.setPlayerType(1, Game::COMPUTER);
            break;
        case 2: /* play with human */
            game.setPlayerType(0, Game::HUMAN);
            game.setPlayerType(1, Game::HUMAN);
            break;
    }
    game.setBulletCount(bullet_count);
    game.restart();
    game_state = STATE_INGAME;
}

int window_keypress(int keycode, Game& game, sf::RenderWindow& mainwindow,
        Menu& mainmenu) {
    if (keycode == sf::Key::Return) {
        switch (game_state) {
            case STATE_WAIT:
                game_start(game, mainmenu);
                break;
            case STATE_INGAME:
                game.togglePause();
                break;
            case STATE_GAMEOVER:
                game_state = STATE_WAIT;
        }
    } else if (keycode == sf::Key::Escape) {
        if (game_state == STATE_INGAME)
            game_state = STATE_WAIT;
        else
            mainwindow.Close();
    } else if (game_state == STATE_WAIT) {
        switch (keycode) {
            case sf::Key::Up:
                mainmenu.up();
                break;
            case sf::Key::Down:
                mainmenu.down();
                break;
            case sf::Key::Left: /* decrement bullet count */
                --bullet_count;
                break;
            case sf::Key::Right: /* increment bullet count */
                ++bullet_count;
                break;
            case sf::Key::F1: /* autoplay */
                game.setBulletCount(bullet_count);
                game.setPlayerType(0, Game::COMPUTER);
                game.restart();
                game_state = STATE_INGAME;
                break;
        }
        if (bullet_count < BULLET_COUNT_MIN)
            bullet_count = BULLET_COUNT_MIN;
        else if (bullet_count > BULLET_COUNT_MAX)
            bullet_count = BULLET_COUNT_MAX;
        game.setBulletCount(bullet_count);
    }
}

std::string get_statistics(Game& game)
{
    std::ostringstream s;
    for (int i=0; i<PLAYER_COUNT; i++) {
        s << std::setiosflags(std::ios::fixed) << std::setprecision(2)
                << "Player " << (i+1) << " : " << game.getPlayerTime(i) << std::endl;
    }
    return s.str();
}

std::string get_bullet_count_text(int n)
{
    std::ostringstream s;
    s << "<- Bullet Count: " << n << " ->";
    return s.str();
}

void text_center(sf::String& text, sf::RenderWindow& window)
{
    const int width = window.GetWidth(), height = window.GetHeight();
    text.SetPosition((width - text.GetRect().GetWidth()) / 2,
        (height - text.GetRect().GetHeight()) / 2);
}

/* s.top = ref.bottom */
void text_align_bottom(sf::String& s, const sf::FloatRect& rect)
{
    s.SetY(rect.Bottom);
}

int event_loop(sf::RenderWindow& mainwindow)
{
    const int width = mainwindow.GetWidth(), height = mainwindow.GetHeight();
    int repeat = 0;
    Game game(width, height);
    Menu mainmenu(mainwindow);
    sf::Clock clock;
    sf::String bullet_count_text("");
    sf::String gameover_text("Game Over");
    sf::String stat_text("");
    
    /* center text */
    text_center(gameover_text, mainwindow);
    
    mainmenu.append("Play Alone");
    mainmenu.append("Play with Computer");
    mainmenu.append("Play with Human");
    
    while (mainwindow.IsOpened()) {
        sf::Event event;
        while (mainwindow.GetEvent(event)) { /* window event handling */
            switch (event.Type) {
                case sf::Event::KeyPressed:
                    window_keypress(event.Key.Code, game, mainwindow, mainmenu);
                    break;
                case sf::Event::Closed: /* window close event */
                    mainwindow.Close();
                    break;
                case sf::Event::LostFocus: /* window lost focus */
                    if (game_state == STATE_INGAME)
                        game.pause(true);
                    break;
            }
        }
        
        float interval = clock.GetElapsedTime();
        
        repeat = 1;
        
        if (mainwindow.GetInput().IsKeyDown(sf::Key::F2))
            interval /= 5;
        else if (mainwindow.GetInput().IsKeyDown(sf::Key::F3))
            repeat = 5;
        
        if (game_state == STATE_INGAME) {
            for (int i=0; i<repeat; i++)
                game.step(interval, mainwindow.GetInput());
            if (game.isGameOver()) {
                game_state = STATE_GAMEOVER;
                stat_text.SetText(get_statistics(game));
                text_center(stat_text, mainwindow);
                text_align_bottom(stat_text, gameover_text.GetRect());
            }
        }
        
        clock.Reset();
        mainwindow.Clear(sf::Color::Black);
        
        if (game_state == STATE_INGAME) {
            game.render(mainwindow);
        } else if (game_state == STATE_WAIT) {
            bullet_count_text.SetText(get_bullet_count_text(bullet_count));
            text_center(bullet_count_text, mainwindow);
            text_align_bottom(bullet_count_text, mainmenu.getRect());
            mainwindow.Draw(bullet_count_text);
            mainmenu.render();
        } else if (game_state == STATE_GAMEOVER) {
            game.render(mainwindow);
            mainwindow.Draw(gameover_text);
            mainwindow.Draw(stat_text);
        }
        
        mainwindow.Display();
        //sf::Sleep(0.1);
    }
    return 0;
}