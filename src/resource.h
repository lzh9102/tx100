/* 
 * File:   resource.h
 * Author: timothy
 *
 * Created on 2012年1月26日, 下午 11:07
 */

#ifndef RESOURCE_H
#define	RESOURCE_H

#include <SFML/Graphics.hpp>
#include <exception>

class ResourceNotFoundException : std::exception
{
    const char *what()
    {
        return "ResourceNotFound";
    }
};

class Resource {
public:
    static void loadAll();
    static const sf::Image& getPlayerImage(int index);
private:
    Resource();
    ~Resource();
};

#endif	/* RESOURCE_H */

