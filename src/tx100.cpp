#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include "game.h"

int event_loop(sf::RenderWindow& mainwindow);

int main(int argc, char** argv)
{
    sf::RenderWindow mainwindow(sf::VideoMode(800, 600), "TX100",
            sf::Style::Close);
    
    return event_loop(mainwindow);
}

int event_loop(sf::RenderWindow& mainwindow)
{
    Game game(mainwindow.GetWidth(), mainwindow.GetHeight());
    sf::Clock clock;
    while (mainwindow.IsOpened()) {
        sf::Event event;
        while (mainwindow.GetEvent(event)) { // window event handling
            switch (event.Type) {
                case sf::Event::Closed: // window close event
                    mainwindow.Close();
                    break;
            }
        }
        
        float interval = clock.GetElapsedTime();
        game.step(interval, mainwindow.GetInput());
        clock.Reset();
        mainwindow.Clear(sf::Color::Black);
        game.render(mainwindow);
        mainwindow.Display();
        //sf::Sleep(0.1);
    }
    return 0;
}