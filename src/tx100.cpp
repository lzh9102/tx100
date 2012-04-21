/*
 * Copyright (C) 2012 Timothy Lin
 * This work is licensed under GPLv3 as published by the Free Software
 * Foundation. Please see http://www.gnu.org/licenses/gpl.html for details.
 * 
 * For more information, please visit the project homepage:
 * http://code.google.com/p/tx100
 */

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <cstdio>
#include "game.h"
#include "menu.h"

#define RANDOM_SEED time(0)
#define BULLET_COUNT_DEFAULT 80
#define BULLET_COUNT_MIN 10
#define BULLET_COUNT_MAX 150

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

enum {
    STATE_WAIT,
    STATE_INGAME,
    STATE_GAMEOVER
};

int bullet_count = BULLET_COUNT_DEFAULT;
int game_state = STATE_WAIT;

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
                if (mainmenu.getSelection() == 3) /* quit */
                    mainwindow.Close();
                else
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
    
    /* sort player by score */
    int player[PLAYER_COUNT];
    for (int i=0; i<PLAYER_COUNT; i++)
        player[i] = i;
    for (int i=0; i<PLAYER_COUNT; i++) {
        for (int j=0; j<PLAYER_COUNT; j++) {
            if (i!=j && game.getPlayerTime(player[i]) > game.getPlayerTime(player[j]))
                std::swap(player[i], player[j]);
        }
    }    
    
    for (int i=0; i<PLAYER_COUNT; i++) {
        const int index = player[i];
        s << std::setiosflags(std::ios::fixed) << std::setprecision(2)
                << "Player " << (index+1) << " : " << game.getPlayerTime(index) 
                << " sec" << std::endl;
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
    mainmenu.append("Play with Human (P2: RDFG)");
    mainmenu.append("Quit");
    
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
            bullet_count_text.SetY(mainmenu.getRect().Bottom + 20);
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

#define SIM_STEP 0.01
bool simulation(int n_bullet, int time_limit, int width=WINDOW_WIDTH,
        int height=WINDOW_HEIGHT)
{
    Game game(width, height);
    game.setBulletCount(n_bullet);
    game.setPlayerType(0, Game::COMPUTER);
    game.setPlayerType(1, Game::OFF);
    
    game.restart();
    for (float t=0; t<time_limit; t+=SIM_STEP) {
        game.step(SIM_STEP, sf::Input());
        if (game.isGameOver())
            break;
    }
    
    std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(2) <<
            game.getPlayerTime(0) << std::endl;
    
    return game.isGameOver();
}

int main(int argc, char** argv)
{
    if (argc > 1) {
        int nbullets = BULLET_COUNT_DEFAULT;
        int seed = RANDOM_SEED;
        int time_limit = 1000;
        if (std::string(argv[1]) != "-s")
            return -1;
        if (argc >= 3)
            nbullets = std::atoi(argv[2]);
        if (argc >= 4)
            seed = std::atoi(argv[3]);
        
        fprintf(stderr, "n_bullet=%d, time_limit=%d, ", nbullets, time_limit);
        fprintf(stderr, "seed=%d\n", seed);
        
        srand(seed);
        simulation(nbullets, time_limit);
    } else {
        sf::RenderWindow mainwindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT),
                "TX100 (http://code.google.com/p/tx100)",
                sf::Style::Close);

        srand(RANDOM_SEED);

        return event_loop(mainwindow);
    }
}