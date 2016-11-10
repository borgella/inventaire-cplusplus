/*
 INF3105 - Structures de données et algorithmes
 Session Automne 2015.
 BORGELLA JEAN MARY : BORJ15048201
 */

#include <iostream>
#include "inventaire.h"

using namespace std;

ListeIngredients& ListeIngredients::operator+=(const ListeIngredients& autre) {
    ListeIngredients temp = autre;
    ArbreMap <std::string, int>::Iterateur iterateurIngredients = temp.ingredients.debut();
    while(iterateurIngredients){
        ingredients[iterateurIngredients.cle()]+= temp.ingredients[iterateurIngredients.cle()];
        ++iterateurIngredients;
    }
    return *this;
}

ListeIngredients& ListeIngredients::operator*=(int facteur) {
    ArbreMap <std::string, int>::Iterateur iterateurIngredients = ingredients.debut();
    while(iterateurIngredients){
        ingredients[iterateurIngredients.cle()]*= facteur;
        ++iterateurIngredients;
    }
    return *this;
}

istream& operator >> (istream& is, ListeIngredients& liste) {
    liste.ingredients.vider();
    string chaine;
    is >> chaine;
    while(is && chaine!="---") {
        int quantite;
        is >> quantite;
        liste.ingredients[chaine] = quantite;
        is >> chaine;
    }
    return is;
}

Inventaire& Inventaire::operator+=(const Inventaire& autre) {
    Inventaire autre2 = autre;
    for (ArbreMap<string, ArbreMap<int, int> >::Iterateur iter = autre2.ingredientsInventaire.debut(); iter; ++iter) {
            ArbreMap<int, int>::Iterateur iter2 = iter.valeur().debut();
        for(ArbreMap<int, int>::Iterateur iter2 = iter.valeur().debut(); iter2; ++iter2) {
            ingredientsInventaire[iter.cle()][iter2.cle()] += autre2.ingredientsInventaire[iter.cle()][iter2.cle()];
        }
    }
    return *this;
}


// operateur a utiliser quand on recoit une commande
Inventaire& Inventaire::operator-=(const ListeIngredients& liste){
	ListeIngredients temp = liste;
	ArbreMap<std::string,int>::Iterateur iterliste = temp.ingredients.debut();
	while(iterliste){
		std::string st = iterliste.cle();
		int quantite = iterliste.valeur();
		ArbreMap<int,int>::Iterateur this_arbre = ingredientsInventaire[st];
		while(this_arbre){
			while(quantite > 0 &&(ingredientsInventaire[st][this_arbre.cle()] > 0)){
				--ingredientsInventaire[st][this_arbre.cle()];
				--quantite;
			}
			++this_arbre;
		}	
		++iterliste;
	}
    	return *this;
}

istream& operator >> (istream& is, Inventaire& inventaire) {
    inventaire.ingredientsInventaire.vider();
    string chaine ;
    is >> chaine;
    while(is && chaine!="---") {
        int quantite, expiration;
        is >> quantite >> expiration;
        if(inventaire.ingredientsInventaire.existe(chaine)){
            ArbreMap<int, int>::Iterateur iter = inventaire.ingredientsInventaire[chaine];
            quantite+=iter.valeur();
        }
        inventaire.ingredientsInventaire[chaine][expiration] = quantite;
        is >> chaine;
    }
    return is;
}
    


