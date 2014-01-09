/* 
 * Copyright (C) <2012> <EDF-R&D> <FRANCE>
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/ 
 
// ********** DO NOT REMOVE THIS BANNER **********
// ORG      : LJLL Universite Pierre et Marie Curie, Paris,  FRANCE
// author Projet_Tympan

#ifndef R2_HPP
#define  R2_HPP
#include <cmath>
#include <cstdlib>
#include <iostream>
#include "../AcousticRaytracer/Geometry/mathlib.h"

// Definition de la class R2
//  sans compilation separe toute les fonctions
// sont definies dans ce R2.hpp avec des inline
//
// definition R (les nombres reals)
// remarque la fonction abort est definie dans
// #include <cstdlib>

typedef decimal R;
typedef vec2 R2;

/*
// The class R2
class R2
{
public:
    //  typedef double R;
    static const int d = 2;

    R x, y; // declaration de membre
    // les 3 constructeurs ---
    R2() : x(0.), y(0.) {} // rappel : x(0), y(0)  sont initialise via le constructeur de double
    R2(const R& a, const R& b): x(a), y(b)  {}
    R2(const R2& a, const R2& b): x(b.x - a.x), y(b.y - a.y)  {}
    // le constucteur par defaut est inutile
    R2(const R2& a) : x(a.x), y(a.y) {}

    // rappel: les operateurs definis dans une classe ont un parametre cache qui est la classe elle meme (*this)

    // les operateurs affectation
    //  operateur affection (*this) = P est inutile par defaut il fait le travail correctement
    R2&   operator=(const R2& P)  {x = P.x; y = P.y; return *this;}
    // les autres operateur affectations
    R2&   operator+=(const R2& P)  {x += P.x; y += P.y; return *this;}
    R2&   operator-=(const R2& P) {x -= P.x; y -= P.y; return *this;}
    // operateur binaire + - * , ^ /
    R2   operator+(const R2& P)const   {return R2(x + P.x, y + P.y);}
    R2   operator-(const R2& P)const   {return R2(x - P.x, y - P.y);}
    R    operator, (const R2& P)const  {return  x * P.x + y * P.y;} // produit scalaire
    R    operator^(const R2& P)const {return  x * P.y - y * P.x;} // produit mixte
    R2   operator*(R c)const {return R2(x * c, y * c);}
    R2   operator/(R c)const {return R2(x / c, y / c);}
    // operateur unaire
    R2   operator-()const  {return R2(-x, -y);}
    R2   operator+()const  {return *this;}
    // une methode
    R2   perp() const {return R2(-y, x);} // la perpendiculaire
    // les operators  tableau
    // version qui peut modifier la classe via l'adresse de x ou y
    R&    operator[](int i) { return (&x)[i];}
    const R&    operator[](int i) const { return (&x)[i];}


    R norme() const { return std::sqrt(x * x + y * y);}
    R norme2() const { return (x * x + y * y);}

    friend  R2 operator*(const R& c, const R2& P) {return P * c;}
    friend  R2 perp(const R2& P) { return R2(-P.y, P.x) ; }
    //inline R2 Perp(const R2 & P) { return P.perp(); }  // autre ecriture  de la fonction perp
    friend R  det(const R2& A, const R2& B, const R2& C) { return R2(A, B) ^ R2(A, C);}
    friend R  area(const R2& A, const R2& B, const R2& C) { return R2(A, B) ^ R2(A, C) * 0.5;}

    friend  std::ostream& operator <<(std::ostream& f, const R2& P)
    { f << P.x << ' ' << P.y   ; return f; }
    friend  std::istream& operator >>(std::istream& f,  R2& P)
    { f >>  P.x >>  P.y  ; return f; }
};
*/
#endif