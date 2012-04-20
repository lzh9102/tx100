/*
 * Copyright (C) 2012 Timothy Lin
 * This work is licensed under GPLv3 as published by the Free Software
 * Foundation. Please see http://www.gnu.org/licenses/gpl.html for details.
 * 
 * For more information, please visit the project homepage:
 * http://code.google.com/p/tx100
 */

#include "game.h"
#include "player.h"
#include "bullet.h"
#include "vectorhelper.h"
#include <ctime>
#include <list>
#include <cmath>
#include <cstdio>
#include <algorithm>

#define BULLET_DIR_DEVIATION 10
#define BULLET_DIST_DEVIATION 30
#define BULLET_GEN_FREQ 2.0
#define BULLET_SPEED 130
#define DEFAULT_BULLET_COUNT 100
#define PI 3.1415926

#define FOREACH_PLAYER(iter) for (int iter=0; iter<PLAYER_COUNT; ++iter)

namespace {
    struct PlayerKeymap {
        sf::Key::Code up;
        sf::Key::Code down;
        sf::Key::Code left;
        sf::Key::Code right;
    };
    /* Keymap for the players */
    PlayerKeymap keymap[PLAYER_COUNT] = {
        /* {up, down, left, right} */
        {sf::Key::Up, sf::Key::Down, sf::Key::Left, sf::Key::Right},
        {sf::Key::R, sf::Key::F, sf::Key::D, sf::Key::G}
    };
}

struct Game::Private
{
    std::list<Bullet> bullet_list;
    Player player[PLAYER_COUNT];
    sf::Shape bullet_shape;
    sf::String str_pause;
    PlayerInput prev_input;
    sf::Vector2f center;
    float bullet_timer;
    float game_timer;
    bool pause;
    PlayerType player_type[PLAYER_COUNT];
    float player_timer[PLAYER_COUNT];
    int w, h;
    int bullet_count;
    bool gameover;
    Private(int width, int height) : w(width), h(height), pause(false)
        , bullet_count(DEFAULT_BULLET_COUNT), gameover(true)
    {
        bullet_shape = sf::Shape::Circle(0, 0, BULLET_RADIUS, sf::Color::Yellow);
        center = sf::Vector2f(w/2, h/2);
        str_pause.SetFont(sf::Font::GetDefaultFont());
        str_pause.SetText("Pause");
        str_pause.Move((w - str_pause.GetRect().GetWidth())/2,
                (h - str_pause.GetRect().GetHeight())/2);
        str_pause.SetColor(sf::Color::Red);
        FOREACH_PLAYER(i) {
            char filename[100];
            sprintf(filename, "player%d.png", i+1);
            player[i].setImage(filename);
            player[i].stop();
            player_type[i] = OFF;
        }
        player_type[0] = HUMAN;
    }
    void gameover_event()
    {
        FOREACH_PLAYER(i) player[i].stop();
        gameover = true;
    }
    int random_alive_index()
    {
        int a[PLAYER_COUNT];
        FOREACH_PLAYER(i)
            a[i] = i;
        FOREACH_PLAYER(i)
            std::swap(a[i], a[rand()%PLAYER_COUNT]);
        FOREACH_PLAYER(i) {
            if (player[a[i]].isAlive())
                return i;
        }
        return 0;
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

void Game::setPlayerType(unsigned int index, PlayerType type)
{
    if (index < PLAYER_COUNT)
        p->player_type[index] = type;
}

void Game::restart()
{
    p->bullet_list.clear();
    FOREACH_PLAYER(i) {
        const float theta = i * 2 * PI / PLAYER_COUNT;
        p->player[i].setX(p->w/2 + p->player[i].getWidth() * cos(theta));
        p->player[i].setY(p->h/2 + p->player[i].getWidth() * sin(theta));
        if (p->player_type[i] != OFF)
            p->player[i].start();
        else
            p->player[i].stop();
        p->player_timer[i] = 0.0;
    }
    generateBullets(p->bullet_count);
    p->pause = false;
    p->game_timer = 0;
    p->gameover = false;
}

void Game::render(sf::RenderWindow& w)
{
    /* draw player */
    FOREACH_PLAYER(i) {
        if (p->player[i].isAlive())
            p->player[i].render(w);
    }
    
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
}

void Game::step(float t, const sf::Input& input)
{
    if (p->pause || isGameOver())
        return;    
   
    /* move player */
    FOREACH_PLAYER(i) {
        if (!p->player[i].isAlive())
            continue;
        
        PlayerInput pi;
        PlayerKeymap *km = &keymap[i];
        pi.up = input.IsKeyDown(km->up);
        pi.down = input.IsKeyDown(km->down);
        pi.left = input.IsKeyDown(km->left);
        pi.right = input.IsKeyDown(km->right);
        
        Player *player = p->player;
        if (p->player_type[i] == COMPUTER)
            player[i].step(t, p->bullet_list, sf::Vector2f(p->w / 2, p->h / 2));
        else
            player[i].step(t, pi);
        player[i].constraint(p->w, p->h);
    }
    
    /* move bullets */
    std::list<Bullet>::iterator bullet_it = p->bullet_list.begin();

    while (bullet_it!=p->bullet_list.end()) {
        
        /* find the nearest alive player */
        float min_distance = 10000.0;
        int min_index = 0;
        FOREACH_PLAYER(i) {
            if (p->player[i].isAlive()) {
                float d = vector_length(bullet_it->pos - p->player[i].getPosition());
                if (min_distance > d)
                    min_distance = d, min_index = i;
            }
        }
        
        /* follow the nearest target */
        bullet_it->step(t, BULLET_SPEED, p->player[min_index].getPosition());
        
        FOREACH_PLAYER(index) {
            Player& player = p->player[index];
            const sf::Image& image = player.getImage();
            int player_radius = player.getCriticalRadius();
            sf::Vector2f player_pos = player.getPosition();
            if (player.isAlive()) {
                bool collision = bullet_it->detectCollision(player_pos, image);

                if (collision) {
                    player.stop();
                    bullet_it = p->bullet_list.erase(bullet_it);
                    continue;
                } else if (!vector_is_in_range(bullet_it->pos, 0, 0, p->w, p->h)){
                    /* if the product is negative, then the bullet is leaving the field */
                    if (vector_dot(p->center - bullet_it->pos, bullet_it->vel) < 0) {
                        bullet_it = p->bullet_list.erase(bullet_it);
                        continue;
                    }
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
    
    p->game_timer += t;
    
    int alive = 0;
    FOREACH_PLAYER(i) {
        if (p->player[i].isAlive()) {
            p->player_timer[i] += t;
            ++alive;
        }
    }
    if (!alive)
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
        sf::Vector2f velocity = vector_normalize(p->player[p->random_alive_index()].getPosition() - bullet.pos);
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

float Game::getPlayerTime(unsigned int n) const
{
    if (n < PLAYER_COUNT)
        return p->player_timer[n];
    else
        return 0.0;
}

bool Game::isGameOver() const
{
    return p->gameover;
}