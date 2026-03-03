#include <iostream>
#include <fstream> //bibliotheque pour manipuler les fichiers
using namespace std;

//enumeration pour representer les differentes types de ressources
enum TypeRessource { VITAL = 0, TECHNIQUE = 1, CONSOMMABLE = 2};

//union permettant de stocker soit une pression soit une temperature(une seule valeur a la fois)
union SpecInfo {
    int pression; //valeur de pression(utiliser pour certains ressources)
    double temperature; // Valeur de temperature (utiliser pour les ressources techniques)
};

//structure representant une ressource complete
struct RessourceJB {
    string nom; //nom de la ressource
    int quantite; //quantite disponible
    TypeRessource type; //type de la ressource (Vital, technique ou consommable)
    SpecInfo spec; //information specifique (pression ou temperature)
};

const int NB_RESSOURCES = 5; //nombre fixe de ressources dans le tableau

//fonction qui permet d'enregistrer dans un fichier journal les differentes operations effectuer par l'utilisateur
void enregistrerLog(string message){
    ofstream fichier("journal_bord.txt", ios::app); //ouvre le fichier sans ecraser les donnees qui sont deja dedans
    if(!fichier.is_open()) //on verifie si le fichier s'est bien ouvert
        return;
    fichier << "[LOG] " << message << endl; //ecrit le message dans le fichier
    fichier.close(); //on ferme le fichier
}

//fonction qui permet d'ouvrir notre fichier base de donnees et de traiter les differentes resources qui sont disponibles
void chargerDonnees(RessourceJB* tableau, string nomFichier){
    ifstream fichier(nomFichier); //Ouvre le fichier en mode lecture seulement
    if(!fichier.is_open()) //on verifie que le fichier s'est bien ouvert
        throw string("Erreur ! le fichier ne s'est pas ouvert.");

    int i = 0;      //index pour remplir le tableau
    int typeInt;    //variable temporaire pour lire le type en entier
    double specVal; //variable temporaire pour lire la valeur specifique

    //chainage d'operateurs d'extraction
    while(fichier >> tableau[i].nom >> tableau[i].quantite >> typeInt >> specVal){
        tableau[i].type = (TypeRessource)typeInt; //on convertit l'entier en typeressource

        if(tableau[i].type == TECHNIQUE) //si notre ressource est de type technique
            tableau[i].spec.temperature = specVal; //on assigne la temperature
        else
            tableau[i].spec.pression = (int)specVal; //sinon on assigne la pression
        i++; //on passe a la ressource suivante
    }
    fichier.close(); //on ferme le fichier
    enregistrerLog("Chargement des donnees reussi."); //on met a jour notre journal
}

//fonction qui permet de mettre a jour l'etat des ressources que l'utilisateur a consommer
void mettreAJourStock(RessourceJB* ressource, int quantiteAconsommer){
    if((ressource->quantite - quantiteAconsommer)<0) //on verifie si la quantite restante serait negative
        throw string("Stock insuffisant !"); //on lance une exception si le stock est insuffisant
    else{
        ressource->quantite -= quantiteAconsommer; //sinon on soustrait la quantite consommee du stock
        string message = "Consommation de " + to_string(quantiteAconsommer) + " unites de " + ressource->nom;
        enregistrerLog(message); //on enregistre l'action dans le journal
    }
}

//fonction qui permet d'afficher l'etat des ressources disponibles
void afficherRessources(RessourceJB* tableau, int taille){
    cout << "NOM\t\tQTE\tTYPE\tSPEC" << endl; //on affiche l'entete du tableau
    for(int i = 0; i < taille; i++){ //parcourt tous les ressources
        cout << tableau[i].nom << "\t\t" << tableau[i].quantite << "\t"; //affiche nom et quantite
        if(tableau[i].type == TECHNIQUE) //si type technique
            cout << "Temp: " << tableau[i].spec.temperature; //on affiche la temperature
        else
            cout << "Pres: " << tableau[i].spec.pression; //sinon on affiche la pression

        cout << endl; //saut de ligne
    }
    enregistrerLog("Affichage des ressources effectue."); //on enregistre l'action dans notre journal
}

void trierRessources(RessourceJB* tableau, int taille){
    for(int i = 0; i < taille - 1; i++){
        for(int j = 0; j < taille - i - 1; j++){
            if(tableau[j].quantite > tableau[j+1].quantite){
                // On echange les deux ressources
                RessourceJB temp = tableau[j];
                tableau[j] = tableau[j+1];
                tableau[j+1] = temp;
            }
        }
    }
    enregistrerLog("Inventaire trie par quantite.");
}

int main(){
    RessourceJB tableau[NB_RESSOURCES];
    int choix;

    try{
        chargerDonnees(tableau, "database1.txt");
    } catch(string e){
        cerr << e << endl;
        return 1;
    }

    do{
        cout << "\n============================================\n\n";
        cout << "--- STATION ARCTIQUE : MENU DE GESTION ---" << endl;
        cout << "1. Afficher l'etat des ressources" << endl;
        cout << "2. Consommer une ressource" << endl;
        cout << "3. Trier par quantite (Croissant)" << endl;
        cout << "4. Quitter" << endl;
        cout << "Votre choix : ";
        cin >> choix;
        cout << "\n============================================\n";
        if(cin.fail()){
            cin.clear();
            cin.ignore(1000, '\n');
            enregistrerLog("\nOption invalide choisie.");
            cout << "\nOption invalide !" << endl;
            continue;
        }
        switch(choix){
            case 1:{
                afficherRessources(tableau, NB_RESSOURCES);
                break;
            }
            case 2: {
                int index, quantite;
                cout << "Index de la ressource (0-4) : ";
                cin >> index;
                cout << "Quantite a consommer : ";
                cin >> quantite;
                try {
                    if(index < 0 || index >= NB_RESSOURCES)
                        throw string("\nIndex invalide !");
                    mettreAJourStock(&tableau[index], quantite);
                    cout << "\nMise a jour effectue." << endl;
                } catch(string e){
                    cerr << e << endl;
                    enregistrerLog("\nErreur : " + e);
                }
                break;
            }
            case 3:{
                trierRessources(tableau, NB_RESSOURCES);
                afficherRessources(tableau, NB_RESSOURCES);
                break;
            }
            case 4:{
                cout << "\nFermeture du systeme..." << endl;
                break;
            }
            default:{
                enregistrerLog("\nOption invalide choisie.");
                cout << "\nOption invalide !" << endl;
                break;
            }
        }
    }while(choix!=4);

    return 0;
}
