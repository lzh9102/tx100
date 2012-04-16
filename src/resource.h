/*
 * Copyright (C) 2012 Timothy Lin
 * This work is licensed under GPLv3 as published by the Free Software
 * Foundation. Please see http://www.gnu.org/licenses/gpl.html for details.
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

