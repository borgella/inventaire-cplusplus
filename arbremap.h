/*
  INF3105 - Structures de données et algorithmes
  Squelette pour Lab8 + TP2
*/

#if !defined(__ARBRE_MAP_H__)
#define __ARBRE_MAP_H__
#include "arbreavl.h"

template <class K, class V> class ArbreMap {
    class Entree {
        public:
            Entree() {}
            Entree(const K& c):cle(c) {}
            K cle;
            V valeur;
            bool operator < (const Entree& e) const {
                return cle < e.cle;
            }
        };
    ArbreAVL<Entree> avlEntrees;
    
    public: class Iterateur {
        // variable d'instance de la classe iterateur
        private: typename ArbreAVL<Entree>::Iterateur iterateur;
        
        //premier constructeur avec un objet de type AvlMap
        public: Iterateur(ArbreMap& a) : iterateur(a.avlEntrees.debut()) {
    
        }
        
        //deuxieme constructeur avec un objet de type Iterateur d'un Arbre AVL
        public: Iterateur(typename ArbreAVL<Entree>::Iterateur iterator) : iterateur(iterator) {
    
        }
        //definition des méthodes
        public: operator bool() const {
            return iterateur.operator bool();
        };
        
        public: Iterateur& operator++() {
            ++iterateur;
            return *this;
        }
        
        public: const K& cle() const {
            return (*iterateur).cle;
        }
        
        public: V& valeur() {
            return (V&) (*iterateur).valeur;
        }
    };// fin de la classe Iterateur
    
    // Comme pour un Abre AVL l'abre map offre aussi les métodes comme debut, fin, rechercher qui retourne des objets Iterateurs
    public: Iterateur debut() {
        return Iterateur(*this);
    }
    public: Iterateur fin() {
        return Iterateur(avlEntrees.fin());
    }
    public: Iterateur rechercher(const K& cle) {
        return Iterateur(avlEntrees.rechercher(cle));
    }
    public: Iterateur rechercherEgalOuSuivant(const K& cle) {
        return Iterateur(avlEntrees.rechercherEgalOuSuivant(cle));
    }
    public: Iterateur rechercherEgalOuPrecedent(const K& cle) {
        return Iterateur(avlEntrees.rechercherEgalOuPrecedent(cle));
    }

public:
    bool contient(const K&) const;
    void vider();
    const V& operator[] (const K&) const;
    V& operator[] (const K&);
    
    
    bool existe(const K&);
    V& retourneNoeud(const K&);

};

template <class K, class V>
void ArbreMap<K,V>::vider() {
    avlEntrees.vider();
}

template <class K, class V>
bool ArbreMap<K,V>::contient(const K& c) const {
    return avlEntrees.rechercher(c) != NULL;
}

template <class K, class V>
const V& ArbreMap<K,V>::operator[] (const K& c)  const {
    Entree* entreetrouvee = avlEntrees.rechercher(c);
    return entreetrouvee->valeur;
}

template <class K, class V>
V& ArbreMap<K,V>::operator[] (const K& c) {
    typename ArbreAVL<Entree>::Iterateur iter = avlEntrees.rechercher(Entree(c));
    if(!iter) {
        avlEntrees.inserer(Entree(c));
        iter = avlEntrees.rechercher(c);
    }
    return avlEntrees[iter].valeur;
}


 
template <class K, class V>
bool ArbreMap<K,V>::existe(const K& c) {
     typename ArbreAVL<Entree>::Iterateur iter = avlEntrees.rechercher(Entree(c));
     if(iter) {
         return true;
     }
     return false;
}

template <class K, class V>
V& ArbreMap<K,V>::retourneNoeud(const K& c) {
    typename ArbreAVL<Entree>::Iterateur iter = avlEntrees.rechercher(Entree(c));
    if(!iter) {
        avlEntrees.inserer(Entree(c));
        iter = avlEntrees.rechercher(c);
    }
    return avlEntrees[iter].valeur;
}

#endif

