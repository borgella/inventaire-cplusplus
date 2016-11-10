/*
  INF3105 - Structures de données et algorithmes
  Session Automne 2015.
  BORGELLA JEAN MARY : BORJ15048201
  TP2
*/

#include <fstream>
#include "inventaire.h"
#include "arbremap.h"

using namespace std;
void liquiderLivrasion(Inventaire& inventaire,int date_preparation);
bool estSuffisantIngredients(ListeIngredients&,Inventaire&);
bool estSuffisantIngredients(int quantiteIngredient,ArbreMap<string, ListeIngredients>&,string nomrecette);
void initialiserIngredientRequis(ListeIngredients&);
int estSuffisantIngredients(int entier,Inventaire&,ListeIngredients& ingredientsRequiss);

int main(int argc, const char** argv) {
    istream* entree;
    ifstream* entree_fichier = NULL;
    if(argc>1) {
        entree = entree_fichier = new ifstream(argv[1]);
        if(entree->fail())
            cerr << "Erreur d'ouverture du fichier '" << argv[1] << "'" << endl;
    } else
        entree = &std::cin;

    // Déclaration du dictionnaire de recettes.
    ArbreMap<string, ListeIngredients> allrecettes;
    ListeIngredients ingredientsRequis;
    Inventaire inventaire;
    int nocommande = 0;
    int date = -1;

    while(*entree) {
        string typeevenement;
        *entree >> typeevenement;
        if(!(*entree)) break;

        if(typeevenement=="recette") {
            string nomrecette;
            *entree >> nomrecette;
            ListeIngredients ingredients;
            *entree >> ingredients;
            allrecettes[nomrecette] = ingredients;
            ArbreMap<string, int>::Iterateur iter = allrecettes[nomrecette].ingredients.debut();
            while(iter){
                ingredientsRequis.ingredients[iter.cle()] = 0;
                ++iter;
            }
        } else if(typeevenement=="livraison") {
            int date_reception;
            *entree >> date_reception;
            if(date_reception <= date)
                cout << "Attention : ce programme supporte uniquement un ordre chronologique!" << endl;
            date = date_reception;
            Inventaire inventairerecu;
            *entree >> inventairerecu;
            inventaire += inventairerecu;
        } else if(typeevenement=="commande") {
            int date_preparation;
            *entree >> date_preparation;
            if(date_preparation <= date)
                std::cout << "Attention : ce programme supporte uniquement un ordre chronologique!" << std::endl;
            date = date_preparation;
            string nomrecette;
            *entree >> nomrecette;
            string nom = nomrecette;
            int ingredientTotaleCommande = 0;
            while(*entree && nomrecette!="---"){
                int quantite = 0;
                *entree >> quantite;
                // Calculer ingredientsRequis pour commande
                ArbreMap<string, int>::Iterateur iter = allrecettes[nomrecette].ingredients.debut();
                while(iter){
                    ingredientTotaleCommande = ingredientsRequis.ingredients[iter.cle()] += iter.valeur() * quantite;
                    ++iter;
                }
                *entree >> nomrecette;
            }
            liquiderLivrasion(inventaire,date_preparation);

            int toto = estSuffisantIngredients(ingredientTotaleCommande,inventaire,ingredientsRequis);

            if(estSuffisantIngredients(ingredientsRequis,inventaire)) {
                cout << nocommande << " : OK" << endl;
                inventaire -= ingredientsRequis;
            } else {
                cout << nocommande << " : Echec" << endl;

            }
            initialiserIngredientRequis(ingredientsRequis);
            nocommande++;
        } else {
            std::cout << "Type d'évènement '" << typeevenement << "' inconnu!" << std::endl;
            return 1;
        }
    }
    delete entree_fichier; // delete est sécuritaire même si entree_fichier==NULL
    return 0;
}

void liquiderLivrasion(Inventaire& inventaire,int date_preparation){
       ArbreMap<string, ArbreMap<int, int> >::Iterateur iter_inventaire = inventaire.ingredientsInventaire.debut();
       while(iter_inventaire) {
           ArbreMap<int, int>::Iterateur iter = iter_inventaire.valeur().debut();
           while (iter) {
               if (iter.cle() < date_preparation) {
                   inventaire.ingredientsInventaire[iter_inventaire.cle()][iter.cle()] = 0;
               }
               ++iter;
           }
           ++iter_inventaire;
       }
}

bool estSuffisantIngredients(ListeIngredients& ingredientsRequis,Inventaire& inventaire){
       ArbreMap<string, int>::Iterateur iter2 = ingredientsRequis.ingredients.debut();
       while(iter2) {
           int quantiteIngredient = 0;
           ArbreMap<int, int>::Iterateur iter3 = inventaire.ingredientsInventaire[iter2.cle()].debut();
           while(iter3) {
               quantiteIngredient += iter3.valeur();
               ++iter3;
           }
           if (quantiteIngredient < iter2.valeur()) {
               return false;
           }
       	++iter2;
       }
	return true;
}

void initialiserIngredientRequis(ListeIngredients& ingredientsRequis){
    ArbreMap<string, int>::Iterateur iterateur = ingredientsRequis.ingredients.debut();
    while(iterateur) {
        ingredientsRequis.ingredients[iterateur.cle()] = 0;
        ++iterateur;
    }
}


int estSuffisantIngredients(int entier,Inventaire& inventaire,ListeIngredients& ingredientsRequis){
    ArbreMap<string, ArbreMap<int, int> >::Iterateur iter_inventaire = inventaire.ingredientsInventaire.debut();
    int add = 0;
    while (iter_inventaire) {
        ArbreMap<int, int> temp = iter_inventaire.valeur();
        ArbreMap<int, int>::Iterateur itertemp = temp.debut();
        add += itertemp.cle();
        ++iter_inventaire;
    }
    return 0;
}
