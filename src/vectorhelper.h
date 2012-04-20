/*
 * Copyright (C) 2012 Timothy Lin
 * This work is licensed under GPLv3 as published by the Free Software
 * Foundation. Please see http://www.gnu.org/licenses/gpl.html for details.
 * 
 * For more information, please visit the project homepage:
 * http://code.google.com/p/tx100
 */

#ifndef VECTORHELPER_H
#define	VECTORHELPER_H
#include <SFML/System/Vector2.hpp>
#include <cmath>
#include <algorithm>

template <class T>
float vector_length(const sf::Vector2<T>& v)
{
    return sqrt(v.x * v.x + v.y * v.y);
}

template <class T>
sf::Vector2<T> vector_normalize(const sf::Vector2<T>& v)
{
    return v / vector_length(v);
}

/**
 * Calculate the inner product of two vectors
 * @param v1
 * @param v2
 * @return 
 */
template <class T>
T vector_dot(const sf::Vector2<T>& v1, const sf::Vector2<T>& v2)
{
    return v1.x * v2.x + v1.y * v2.y;
}

/**
 * Project a vector to the direction of another vector
 * @param v the vector to be transformed
 * @param dir the direction to be projected on
 * @return the transformed vector
 */
template <class T>
sf::Vector2<T> vector_projection(const sf::Vector2<T>& v, const sf::Vector2<T>& dir)
{
    return vector_normalize(dir) * vector_dot(v, dir) / vector_length(dir);
}

template <class T>
bool vector_is_in_range(const sf::Vector2<T>& v, int x1, int y1, int x2, int y2)
{
    if (x1 > x2)
        std::swap(x1, x2);
    if (y1 > y2)
        std::swap(y1, y2);
    return v.x >= x1 && v.x <= x2 && v.y >= y1 && v.y <= y2;
}

template <class T>
float vector_correlation(const sf::Vector2<T>& v1, const sf::Vector2<T>& v2)
{
    float l1 = vector_length(v1), l2 = vector_length(v2);
    if (l1 * l2 == 0.0)
        return 0.0;
    return vector_dot(v1, v2) / (l1 * l2);
}

#endif	/* VECTORHELPER_H */

