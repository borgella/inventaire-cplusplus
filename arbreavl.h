/*
  INF3105 - Structures de données et algorithmes
  Squelette pour Lab5 + TP2
*/

#if !defined(__ARBREAVL_H__)
#define __ARBREAVL_H__

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include "pile.h"

template <class T> class ArbreAVL{
    private: class Noeud;
    private: Noeud* racine; // racine de l'arbreAvl
	public:
		ArbreAVL();
		ArbreAVL(const ArbreAVL&);
		~ArbreAVL();

		void inserer(const T&);
		void enlever(const T&);
		bool contient(const T&) const;
		bool vide() const;
		void vider();
		int  hauteur() const;
		ArbreAVL& operator = (const ArbreAVL&);
    
    private: // methode qui vont effectuer les opérations concrètes des méthodes public
        bool inserer(const T&, Noeud*&);
        bool enlever(const T& e, Noeud*& n);
        void rotationGaucheDroite(Noeud*&);
        void rotationDroiteGauche(Noeud*&);
        void vider(Noeud*&);
        void copier(const Noeud*, Noeud*&) const;
        const T& max(Noeud*) const;
		// Puisque les fonctions suivantes retournent des itérateurs, il faut *annoncer* la décalaration classe Iterateur
		// On ne peut pas déclarer Iterateur tout de suite, car Iterateur a besoin de Noeud.	
    public:
        class Iterateur;
		Iterateur debut() const;
		Iterateur fin() const;
		Iterateur rechercher(const T&) const;
		Iterateur rechercherEgalOuSuivant(const T&) const;
		Iterateur rechercherEgalOuPrecedent(const T&) const;
		const T& operator[](const Iterateur&) const;
		T& operator[](const Iterateur&);

	public:
		class Iterateur{
            private:
                const ArbreAVL& arbre_associe;
                Noeud* courant;
                Pile<Noeud*> chemin;
            public:
				Iterateur(const ArbreAVL& arbre);
				Iterateur(const Iterateur& iter);
				Iterateur(const ArbreAVL& arbre, Noeud* noeud);

				operator bool() const;
				bool operator!() const;
				bool operator==(const Iterateur&) const;
				bool operator!=(const Iterateur&) const;
            
				const T& operator*() const;
				Iterateur& operator++();
				Iterateur operator++(int);
				Iterateur& operator = (const Iterateur&);
            private:
				friend class ArbreAVL;
		};

        private:
            class Noeud{
                public:
                    Noeud(const T&);
                    Noeud *gauche;
                    Noeud *droite;
                    int equilibre;
                    T contenu;
            };
};

//-----------------------------------------------------------------------------

template <class T>
ArbreAVL<T>::Noeud::Noeud(const T& c):
    gauche(NULL), droite(NULL), equilibre(0), contenu(c)
{}

template <class T>
ArbreAVL<T>::ArbreAVL() : racine(NULL)
{}

template <class T>
ArbreAVL<T>::ArbreAVL(const ArbreAVL& autre) : racine(NULL){
    copier(autre.racine, racine);
}

template <class T>
ArbreAVL<T>::~ArbreAVL(){
    vider(racine);
}

template <class T>
void ArbreAVL<T>::inserer(const T& element){
    inserer(element, racine);
}

template <class T>
bool ArbreAVL<T>::inserer(const T& element, Noeud*& noeud){
    if(noeud==NULL)
    {
        noeud = new Noeud(element);
        return true;
    }
	
    if(element < noeud->contenu)
    {
        if(inserer(element, noeud->gauche))
        {
            noeud->equilibre++;
            if(noeud->equilibre == 0)
                return false;
            if(noeud->equilibre == 1)
                return true;
            assert(noeud->equilibre == 2);
            if(noeud->gauche->equilibre == -1)
                rotationDroiteGauche(noeud->gauche);
            rotationGaucheDroite(noeud);
        }
		
        return false;
    }
    else if(noeud->contenu < element)
    {
        if(inserer(element, noeud->droite))
        {
            noeud->equilibre--;
            if(noeud->equilibre == 0)
                return false;
            if(noeud->equilibre == -1)
                return true;
            assert(noeud->equilibre == -2);
            if(noeud->droite->equilibre == 1)
            {
                rotationGaucheDroite(noeud->droite);
            }
            rotationDroiteGauche(noeud);
        }
		
        return false;
    }
    // element == noeud->contenu
    noeud->contenu = element; // Mise à jour
	
    return false;
}

template <class T>
void ArbreAVL<T>::enlever(const T& element){
    enlever(element, racine);
}

template <class T>
bool ArbreAVL<T>::enlever(const T& e, Noeud*& noeud){
    if(noeud == NULL) return false;

    if(e < noeud->contenu)
    {
        if(enlever(e, noeud->gauche))
        {
            noeud->equilibre--;
            if(noeud->equilibre == -1) return false;
            if(noeud->equilibre == 0) return true;
				
            assert(noeud->equilibre == -2);
			
            if(noeud->droite->equilibre == 1)
                rotationGaucheDroite(noeud->droite);
            rotationDroiteGauche(noeud);
			
            return (noeud->equilibre==0);
        }
    }
    else if (noeud->contenu < e)
    {
        if(enlever(e, noeud->droite))
        {
            noeud->equilibre++;
            if(noeud->equilibre == 1) return false;
            if(noeud->equilibre == 0) return true;
				
            assert(noeud->equilibre == 2);
			
            if(noeud->gauche->equilibre == -1)
                rotationDroiteGauche(noeud->gauche);
            rotationGaucheDroite(noeud);
			
            return (noeud->equilibre==0);
        }
    }
    else
    {
        if(!(noeud->droite) && !(noeud->gauche))
        {
            delete noeud;
            noeud = NULL;
			
            return true;
        }
        else if (!noeud->droite && noeud->gauche)
        {
            Noeud *temp = noeud->gauche;
            delete noeud;
            noeud = temp;
			
            return true;
        }
        else if (noeud->droite && !noeud->gauche)
        {
            Noeud *temp = noeud->droite;
            delete noeud;
            noeud = temp;
			
            return true;
        }
        else
        {
            T temp = max(noeud->gauche);
            noeud->contenu = temp;

            if(enlever(temp, noeud->gauche))
            {
                noeud->equilibre--;
                if(noeud->equilibre == -1) return false;
                if(noeud->equilibre == 0) return true;
				
                assert(noeud->equilibre == -2);
				
                if(noeud->droite->equilibre == 1)
					rotationGaucheDroite(noeud->droite);
                rotationDroiteGauche(noeud);
				
                return (noeud->equilibre == 0);
            }
        }
    }
	
    return false;
}

template <class T>
void ArbreAVL<T>::rotationGaucheDroite(Noeud*& racinesousarbre){
    Noeud *temp = racinesousarbre->gauche;
    int ea = temp->equilibre;
    int eb = racinesousarbre->equilibre;
    int neb = -(ea>0 ? ea : 0) - 1 + eb;
    int nea = ea + (neb < 0 ? neb : 0) - 1;

    temp->equilibre = nea;
    racinesousarbre->equilibre = neb;
    racinesousarbre->gauche = temp->droite;
    temp->droite = racinesousarbre;
    racinesousarbre = temp;
}

template <class T>
void ArbreAVL<T>::rotationDroiteGauche(Noeud*& racinesousarbre){
    Noeud *temp = racinesousarbre->droite;
    int ea = temp->equilibre;
    int eb = racinesousarbre->equilibre;
    int neb = -(ea<0?ea:0) + 1 + eb;
    int nea = (neb>0?neb:0) + 1 + ea;

    temp->equilibre = nea;
    racinesousarbre->equilibre = neb;
    racinesousarbre->droite = temp->gauche;
    temp->gauche = racinesousarbre;
    racinesousarbre = temp;
}

template <class T>
bool ArbreAVL<T>::vide() const{
	return racine == NULL;
}

template <class T>
void ArbreAVL<T>::vider(){
    vider(racine);
    racine = NULL;
}

template <class T>
void ArbreAVL<T>::vider(Noeud*& noeud){
    if(noeud == NULL) return;
    vider(noeud->gauche);
    vider(noeud->droite);
    delete noeud;
}

template <class T>
void ArbreAVL<T>::copier(const Noeud* source, Noeud*& noeud) const{
    if(source == NULL){
        noeud = NULL;
        return;
    }
    noeud = new Noeud(source->contenu);
    noeud->equilibre = source->equilibre;
    
    if(source->gauche) copier(source->gauche, noeud->gauche);
    else noeud->gauche = NULL;
	
    if(source->droite) copier(source->droite, noeud->droite);
    else noeud->droite = NULL;
	
    return;
}

template <class T>
bool ArbreAVL<T>::contient(const T& element) const{
    Iterateur iter = rechercher(element);
	
    return iter;
}

template <class T>
int ArbreAVL<T>::hauteur() const{
    int hauteur = 0;
    Noeud* noeud = racine;
	
    while(noeud != NULL)
    {
        ++hauteur;
        if(noeud->equilibre == -1) noeud = noeud->droite;
        else noeud = noeud->gauche;
    }
	
    return hauteur;
}

template <class T>
const T& ArbreAVL<T>::max(Noeud* noeud) const{
    if(noeud->droite == NULL) return noeud->contenu;

    return max(noeud->droite);;
}

template <class T>
typename ArbreAVL<T>::Iterateur ArbreAVL<T>::debut() const{
    Iterateur iter(*this);
    iter.courant = racine;
	
    if(iter.courant != NULL)
        while(iter.courant->gauche != NULL)
        {
            iter.chemin.empiler(iter.courant);
            iter.courant = iter.courant->gauche;
        }

    return iter;
}

template <class T>
typename ArbreAVL<T>::Iterateur ArbreAVL<T>::fin() const{
    return Iterateur(*this);
}

template <class T>
typename ArbreAVL<T>::Iterateur ArbreAVL<T>::rechercher(const T& e) const{
    Iterateur iter(*this);
    Noeud *noeud = racine;
    while(noeud){
        if(e < noeud->contenu){
            iter.chemin.empiler(noeud);
            noeud = noeud->gauche;
        }else if (noeud->contenu < e)
                    noeud = noeud->droite;
        else{
            iter.courant = noeud;
            return iter;
        }
    }
    iter.chemin.vider();
    return iter;
}

template <class T>
typename ArbreAVL<T>::Iterateur ArbreAVL<T>::rechercherEgalOuSuivant
(const T& e) const{
    Noeud *noeud = racine;
    Pile<Noeud *> pile;	
    while(noeud)
    {
        pile.empiler(noeud);
		
        if(e < noeud->contenu) noeud = noeud->gauche;
        else if (noeud->contenu < e) noeud = noeud->droite;
        else break;
    }
	
    while(!pile.vide())
    {
        noeud = pile.depiler();
		
        if(!(noeud->contenu < e))
            return rechercher(noeud->contenu);
    }
	
    return Iterateur(*this);
}

template <class T>
typename ArbreAVL<T>::Iterateur ArbreAVL<T>::rechercherEgalOuPrecedent
(const T& e) const{
    Noeud *noeud = racine;
    Pile<Noeud *> pile;
	
    while(noeud)
    {
        pile.empiler(noeud);
		
        if(e < noeud->contenu) noeud = noeud->gauche;
        else if (noeud->contenu < e) noeud = noeud->droite;
        else break;
    }
	
    while(!pile.vide())
    {
        noeud = pile.depiler();
		
        if(!(e < noeud->contenu))
            return rechercher(noeud->contenu);
    }
	
    return Iterateur(*this);
}

template <class T>
const T& ArbreAVL<T>::operator[](const Iterateur& iterateur) const{
    assert(&iterateur.arbre_associe == this);
    assert(iterateur.courant);
    return iterateur.courant->contenu;
}

template <class T>
T& ArbreAVL<T>::operator[](const Iterateur& iterateur){
    assert(&iterateur.arbre_associe == this);
    assert(iterateur.courant);
    return iterateur.courant->contenu;
}

template <class T>
ArbreAVL<T>& ArbreAVL<T>::operator=(const ArbreAVL& autre){
    vider();
    copier(autre.racine, racine);
	
    return *this;
}

//-----------------------

template <class T>
ArbreAVL<T>::Iterateur::Iterateur(const ArbreAVL& a)
    : arbre_associe(a), courant(NULL)
{}

template <class T>
ArbreAVL<T>::Iterateur::Iterateur(const ArbreAVL<T>::Iterateur& a)
    : arbre_associe(a.arbre_associe){
    courant = a.courant;
    chemin = a.chemin;
}

template <class T> // Pré-incrément
typename ArbreAVL<T>::Iterateur& ArbreAVL<T>::Iterateur::operator++(){
    assert(courant);
    Noeud *suivant = courant->droite;
    while(suivant){
        chemin.empiler(suivant);
        suivant = suivant->gauche;
    }
    if(!chemin.vide())
        courant = chemin.depiler();
    else
        courant = NULL;
	return *this;
}

template <class T> // Post-incrément
typename ArbreAVL<T>::Iterateur ArbreAVL<T>::Iterateur::operator++(int){
    Iterateur copie(*this);
    operator++();
    return copie;
}

template <class T>
ArbreAVL<T>::Iterateur::operator bool() const{
    return courant != NULL;
}

template <class T>
bool ArbreAVL<T>::Iterateur::operator!() const{
    return courant == NULL;
}

template <class T>
bool ArbreAVL<T>::Iterateur::operator==(const Iterateur& o) const{
    assert(&arbre_associe==&o.arbre_associe);
    return courant==o.courant;
}

template <class T>
bool ArbreAVL<T>::Iterateur::operator!=(const Iterateur& o) const{
    assert(&arbre_associe==&o.arbre_associe);
    return courant!=o.courant;
}

template <class T>
const T& ArbreAVL<T>::Iterateur::operator *() const{ // Opérateur de déférencement
    assert(courant!=NULL);
    return courant->contenu;
}

template <class T>
typename ArbreAVL<T>::Iterateur& ArbreAVL<T>::Iterateur::operator = (const Iterateur& autre){
    assert(&arbre_associe==&autre.arbre_associe);
    courant = autre.courant;
    chemin = autre.chemin;
    return *this;
}

#endif
