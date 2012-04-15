/* 
 * File:   autoplay.h
 * Author: timothy
 *
 * Created on 2012年4月15日, 下午 3:04
 */

#ifndef AUTOPLAY_H
#define	AUTOPLAY_H
#include "player.h"
#include "bullet.h"
#include <list>

class AutoPlay {
public:
    AutoPlay(Player& player, std::list<Bullet>& bullet_list);
    virtual ~AutoPlay();
    
    void step(float t);
    
private:
    AutoPlay(const AutoPlay&);
    AutoPlay& operator=(const AutoPlay&);
    Player *m_player;
    std::list<Bullet> *m_list;
    
};

#endif	/* AUTOPLAY_H */

