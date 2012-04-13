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
#include <list>
#include <cmath>

#define BULLET_DIR_DEVIATION 10
#define BULLET_DIST_DEVIATION 30
#define BULLET_GEN_FREQ 2.0
#define MIN_BULLET_COUNT 100
#define PI 3.1415926

struct Game::Private
{
    Player player;
    std::list<Bullet> bullet_list;
    sf::Shape bullet_shape;
    PlayerInput prev_input;
    sf::Vector2f center;
    float bullet_timer;
    int w, h;
    Private(int width, int height) : w(width), h(height) {
        bullet_shape = sf::Shape::Circle(0, 0, BULLET_RADIUS, sf::Color::Yellow);
        center = sf::Vector2f(w/2, h/2);
    }
};

Game::Game(int width, int height) : p(new Private(width, height))
{
    p->player.setX(width/2);
    p->player.setY(height/2);
    p->player.start();
    generateBullets(MIN_BULLET_COUNT);
}

Game::~Game()
{
    delete p;
}

void Game::render(sf::RenderWindow& w)
{
    /* draw player */
    p->player.render(w);
    
    /* draw bullets */
    std::list<Bullet>::iterator it;
    for (it=p->bullet_list.begin(); it!=p->bullet_list.end(); ++it) {
        p->bullet_shape.SetPosition(it->pos);
        w.Draw(p->bullet_shape);
    }
}

void Game::step(float t, const sf::Input& input)
{
    /* move player */
    PlayerInput pi;
    pi.up = input.IsKeyDown(sf::Key::Up);
    pi.down = input.IsKeyDown(sf::Key::Down);
    pi.left = input.IsKeyDown(sf::Key::Left);
    pi.right = input.IsKeyDown(sf::Key::Right);
    //pi.rebound = input.IsKeyDown(sf::Key::Space);
    pi.rebound = false; /* disable rebound */
    p->player.step(t, pi);
    
    /* move bullets */
    std::list<Bullet>::iterator bullet_it = p->bullet_list.begin();
    int player_radius = p->player.getCriticalRadius();
    int rebound_radius = p->player.getReboundRadius();
    while (bullet_it!=p->bullet_list.end()) {
        bullet_it->step(t);
        
        if (p->player.isAlive()) {
            bool collision = bullet_it->detectCollision(p->player.getPosition(), player_radius);

            if (collision) {
                p->player.stop();
                bullet_it = p->bullet_list.erase(bullet_it);
                continue;
            } else if (pi.rebound && !p->prev_input.rebound) {
                bool rebound = bullet_it->detectCollision(p->player.getPosition(), rebound_radius);
                if (rebound) {
                    bullet_it->rebound(p->player.getPosition());
                }
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
    
    if (p->bullet_timer > BULLET_GEN_FREQ && p->bullet_list.size() < MIN_BULLET_COUNT) {
        generateBullets(rand() % MIN_BULLET_COUNT + 1);
        p->bullet_timer = 0;
    }
    
    p->prev_input = pi;
}

void Game::generateBullets(int count)
{
    float start_distance = sqrt(p->w * p->w + p->h * p->h) / 2;
    for (int i=0; i<count; i++) {
        Bullet bullet;
        float theta = (rand() % 360) * 2 * PI / 360;
        float dist_deviation = 1 + (float)(rand() % BULLET_DIST_DEVIATION) / BULLET_DIST_DEVIATION;
        
        bullet.pos = p->center + dist_deviation * start_distance * sf::Vector2f(cos(theta), sin(theta));
        
        sf::Vector2f velocity = vector_normalize(p->player.getPosition() - bullet.pos);
        sf::Vector2f dir_deviation = sf::Vector2f(-velocity.y, velocity.x); /* orthogonal to velocity */
        velocity *= (float)100;
        dir_deviation *= (float)(rand()%(BULLET_DIR_DEVIATION) - BULLET_DIR_DEVIATION);
        bullet.vel = velocity + dir_deviation;
        
        p->bullet_list.push_back(bullet);
    }
}