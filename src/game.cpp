/* 
 * File:   scene.cpp
 * Author: timothy
 * 
 * Created on 2012年1月26日, 下午 10:35
 */

#include "game.h"
#include "player.h"
#include "bullet.h"
#include "vectorhelper.h"
#include <ctime>
#include <list>
#include <cmath>

#define BULLET_DIR_DEVIATION 10
#define BULLET_DIST_DEVIATION 30
#define BULLET_GEN_FREQ 2.0
#define BULLET_SPEED 130
#define DEFAULT_BULLET_COUNT 100
#define PI 3.1415926

struct Game::Private
{
    std::list<Bullet> bullet_list;
    Player player;
    sf::Shape bullet_shape;
    sf::String str_pause;
    sf::String str_autoplay;
    PlayerInput prev_input;
    sf::Vector2f center;
    float bullet_timer;
    float game_timer;
    bool pause, autoplay;
    int w, h;
    int bullet_count;
    Private(int width, int height) : w(width), h(height), pause(false), autoplay(false)
        , bullet_count(DEFAULT_BULLET_COUNT)
    {
        bullet_shape = sf::Shape::Circle(0, 0, BULLET_RADIUS, sf::Color::Yellow);
        center = sf::Vector2f(w/2, h/2);
        str_pause.SetFont(sf::Font::GetDefaultFont());
        str_pause.SetText("Pause");
        str_pause.Move((w - str_pause.GetRect().GetWidth())/2,
                (h - str_pause.GetRect().GetHeight())/2);
        str_pause.SetColor(sf::Color::Red);
        str_autoplay.SetText("Auto Play");
        str_autoplay.SetColor(sf::Color::Red);
        str_autoplay.Move(0, 0);
        player.stop();
    }
    void gameover_event()
    {
        player.stop();
    }
};

Game::Game(int width, int height) : p(new Private(width, height))
{

}

Game::~Game()
{
    delete p;
}

void Game::setBulletCount(unsigned int n)
{
    p->bullet_count = n;
}

void Game::setAutoPlay(bool flag)
{
    p->autoplay = flag;
}

void Game::restart()
{
    p->bullet_list.clear();
    p->player.setX(p->w/2);
    p->player.setY(p->h/2);
    p->player.start();
    generateBullets(p->bullet_count);
    p->pause = false;
    p->game_timer = 0;
}

void Game::render(sf::RenderWindow& w)
{
    /* draw player */
    p->player.render(w);
    
    /* draw bullets */
    std::list<Bullet>::iterator it;
    //bool draw_missile = (p->game_timer*3 - std::floor(p->game_timer*3)) > 0.3; /* blink */
    for (it=p->bullet_list.begin(); it!=p->bullet_list.end(); ++it) {
        p->bullet_shape.SetPosition(it->pos);
        switch (it->type) {
            case Bullet::NORMAL:
                p->bullet_shape.SetColor(sf::Color::Yellow); break;
            case Bullet::ACCELERATING:
                p->bullet_shape.SetColor(sf::Color::Green); break;
            case Bullet::TRACKING:
                p->bullet_shape.SetColor(sf::Color::Red); break;
        }
        //if (it->type == Bullet::TRACKING && !draw_missile)
        //    continue;
        w.Draw(p->bullet_shape);
    }
    
    if (p->pause) {
        w.Draw(p->str_pause);
    }
    if (p->autoplay) {
        w.Draw(p->str_autoplay);
    }
}

void Game::step(float t, const sf::Input& input)
{
    if (p->pause || isGameOver())
        return;
    
    /* move player */
    PlayerInput pi;
    pi.up = input.IsKeyDown(sf::Key::Up);
    pi.down = input.IsKeyDown(sf::Key::Down);
    pi.left = input.IsKeyDown(sf::Key::Left);
    pi.right = input.IsKeyDown(sf::Key::Right);
    //pi.rebound = input.IsKeyDown(sf::Key::Space);
    pi.rebound = false; /* disable rebound */
    
    if (p->autoplay)
        p->player.step(t, p->bullet_list, sf::Vector2f(p->w / 2, p->h / 2));
    else
        p->player.step(t, pi);
    
    p->player.constraint(p->w, p->h);
    
    /* move bullets */
    std::list<Bullet>::iterator bullet_it = p->bullet_list.begin();
    int player_radius = p->player.getCriticalRadius();
    int rebound_radius = p->player.getReboundRadius();
    sf::Vector2f player_pos = p->player.getPosition();
    while (bullet_it!=p->bullet_list.end()) {
        bullet_it->step(t, BULLET_SPEED, player_pos);
        
        if (p->player.isAlive()) {
            bool collision = bullet_it->detectCollision(player_pos, player_radius);

            if (collision) {
                p->player.stop();
                bullet_it = p->bullet_list.erase(bullet_it);
                continue;
//            } else if (pi.rebound && !p->prev_input.rebound) {
//                bool rebound = bullet_it->detectCollision(player_pos, rebound_radius);
//                if (rebound) {
//                    bullet_it->rebound(player_pos);
//                }
            } else if (!vector_is_in_range(bullet_it->pos, 0, 0, p->w, p->h)){
                /* if the product is negative, then the bullet is leaving the field */
                if (vector_dot(p->center - bullet_it->pos, bullet_it->vel) < 0) {
                    bullet_it = p->bullet_list.erase(bullet_it);
                    continue;
                }
            }
        }
        
        ++bullet_it;
    }
    
    p->bullet_timer += t;
    
    if (p->bullet_timer > BULLET_GEN_FREQ && p->bullet_list.size() < p->bullet_count) {
        generateBullets(rand() % p->bullet_count + 1);
        p->bullet_timer = 0;
    }
    
    p->prev_input = pi;
    p->game_timer += t;
    
    if (!p->player.isAlive())
        p->gameover_event();
}

void Game::generateBullets(int count)
{
    float start_distance = sqrt(p->w * p->w + p->h * p->h) / 2;
    for (int i=0; i<count; i++) {
        Bullet bullet;
        float theta = (rand() % 360) * 2 * PI / 360;
        float dist_deviation = 1 + (float)(rand() % BULLET_DIST_DEVIATION) / BULLET_DIST_DEVIATION;
        
        /* position */
        bullet.pos = p->center + dist_deviation * start_distance * sf::Vector2f(cos(theta), sin(theta));
        
        /* velocity and direction */
        sf::Vector2f velocity = vector_normalize(p->player.getPosition() - bullet.pos);
        sf::Vector2f dir_deviation = sf::Vector2f(-velocity.y, velocity.x); /* orthogonal to velocity */
        velocity *= (float)BULLET_SPEED;
        dir_deviation *= (float)(rand()%(BULLET_DIR_DEVIATION) - BULLET_DIR_DEVIATION);
        bullet.vel = velocity + dir_deviation;
        
        /* type */
        if (rand() % 500 == 0)
            bullet.type = Bullet::ACCELERATING;
        else if (rand() % 30 == 0)
            bullet.type = Bullet::TRACKING;
        
        p->bullet_list.push_back(bullet);
    }
}

void Game::pause(bool flag)
{
    p->pause = flag;
}

void Game::togglePause()
{
    p->pause = !p->pause;
}

float Game::getTime() const
{
    return p->game_timer;
}

bool Game::isGameOver() const
{
    return !p->player.isAlive();
}