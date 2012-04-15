#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <sstream>
#include <iostream>
#include <iomanip>
#include "game.h"

int event_loop(sf::RenderWindow& mainwindow);

enum {
    STATE_WAIT,
    STATE_INGAME,
    STATE_GAMEOVER,
    STATE_COUNT
};

int main(int argc, char** argv)
{
    sf::RenderWindow mainwindow(sf::VideoMode(800, 600), "TX100",
            sf::Style::Close);
    
    return event_loop(mainwindow);
}

int window_keypress(int keycode, Game& game, int& game_state,
        sf::RenderWindow& mainwindow) {
    if (keycode == sf::Key::Return) {
        switch (game_state) {
            case STATE_WAIT:
                game.restart();
                game_state = STATE_INGAME;
                break;
            case STATE_INGAME:
                game.togglePause();
                break;
            case STATE_GAMEOVER:
                game_state = STATE_WAIT;
        }
    } else if (keycode == sf::Key::Escape) {
        mainwindow.Close();
    }
}

sf::String get_statistics(Game& game)
{
    std::ostringstream s;
    s << std::setiosflags(std::ios::fixed) << std::setprecision(2)
            << "Time: " << game.getTime();
    return sf::String(s.str());
}

void text_center(sf::String& text, sf::RenderWindow& window)
{
    const int width = window.GetWidth(), height = window.GetHeight();
    text.SetPosition((width - text.GetRect().GetWidth()) / 2,
        (height - text.GetRect().GetHeight()) / 2);
}

int event_loop(sf::RenderWindow& mainwindow)
{
    const int width = mainwindow.GetWidth(), height = mainwindow.GetHeight();
    Game game(width, height);
    sf::Clock clock;
    int game_state = STATE_WAIT;
    sf::String wait_text("Press \"Enter\" to start the game.");
    sf::String gameover_text("Game Over");
    sf::String stat_text("");
    
    /* center text */
    text_center(wait_text, mainwindow);
    text_center(gameover_text, mainwindow);
    
    while (mainwindow.IsOpened()) {
        sf::Event event;
        while (mainwindow.GetEvent(event)) { // window event handling
            switch (event.Type) {
                case sf::Event::KeyPressed:
                    window_keypress(event.Key.Code, game, game_state, mainwindow);
                    break;
                case sf::Event::Closed: // window close event
                    mainwindow.Close();
                    break;
            }
        }
        
        float interval = clock.GetElapsedTime();
        
        if (game_state == STATE_INGAME) {
            game.step(interval, mainwindow.GetInput());
            if (game.isGameOver()) {
                game_state = STATE_GAMEOVER;
                stat_text = get_statistics(game);
                text_center(stat_text, mainwindow);
                stat_text.SetY(gameover_text.GetRect().Bottom);
            }
        }
        
        clock.Reset();
        mainwindow.Clear(sf::Color::Black);
        game.render(mainwindow);
        
        if (game_state == STATE_WAIT) {
            mainwindow.Draw(wait_text);
        } else if (game_state == STATE_GAMEOVER) {
            mainwindow.Draw(gameover_text);
            mainwindow.Draw(stat_text);
        }
        
        mainwindow.Display();
        //sf::Sleep(0.1);
    }
    return 0;
}