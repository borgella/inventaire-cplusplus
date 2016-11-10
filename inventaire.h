/*
 INF3105 - Structures de données et algorithmes
 Session Automne 2015.
 BORGELLA JEAN MARY : BORJ15048201
 TP2
 */

#if !defined(__INVENTAIRE_H__)
#define __INVENTAIRE_H__
#include <iostream>
#include "arbremap.h"


class ListeIngredients{
    // Variable D'instance qui servirait a sauvegarder le nom et la quantite des ingredients par recette.
public: ArbreMap<std::string,int>  ingredients;
    
public:
    ListeIngredients& operator+=(const ListeIngredients& liste);
    ListeIngredients& operator*=(int quantite);
private:
    friend std::istream& operator >> (std::istream&, ListeIngredients&);
    friend class Inventaire;
};

class Inventaire {
public:
    ArbreMap<std::string,  ArbreMap<int, int> > ingredientsInventaire;
    Inventaire& operator+=(const Inventaire&);
    Inventaire& operator-=(const ListeIngredients&);
    
private:
    friend std::istream& operator >> (std::istream&, Inventaire&);
    
};

#endif

