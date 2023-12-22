/**
 * \file main.cpp
 * \brief Fichier pour les différents tests
 * \author GLO-2001
 * \version 0.1
 * \date  2023
 *
 */

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include "disqueVirtuel.h"
#include <algorithm>
#include <numeric>

using namespace std;
using namespace TP3;

typedef struct
{
    string StrCommande;
    size_t nArguments;
} EntreeCommande;

vector<EntreeCommande> TableDesFonctions = {
    {.StrCommande = "mkdir", .nArguments = 1},
    {.StrCommande = "ls", .nArguments = 1},
    {.StrCommande = "rm", .nArguments = 1},
    {.StrCommande = "create", .nArguments = 1},
    {.StrCommande = "format", .nArguments = 0}};

int fonctionExecute(const string &p_commande, DisqueVirtuel *p_disqueVirtuel);

int main(int argc, char **argv)
{
    int choix = -1;
    DisqueVirtuel mon_disque;

    while (choix != 0)
    {
        cout << endl;
        cout << "------------------ Menu --------------------------------" << endl;
        cout << "1 - Executer une commande (format, ls, mkdir, create, rm)." << endl;
        cout << "2 - Executer plusieurs commandes se trouvant dans un fichier texte." << endl;
        cout << "0 - Quitter." << endl;
        cout << "--------------------------------------------------------" << endl
             << endl;

        cout << "Entrer s.v.p. votre choix (0 a 2):? ";
        cin >> choix;
        if ((choix < 0) || (choix > 2))
        {
            cout << "***Option invalide!***" << endl;
        }
        else
        {
            cin.ignore();
            if ((choix >= 1) && (choix <= 2))
            {
                string commande;
                if (choix == 2)
                {
                    string nameFile;
                    cout << "Entrez le nom du fichier: ";
                    getline(cin, nameFile);
                    ifstream file(nameFile, std::ios::in);
                    if (file)
                    {
                        while (getline(file, commande))
                        {
                            commande.erase(remove_if(commande.begin(), commande.end(),
                                                  [](unsigned char c)
                                                  {
                                                      return !std::isprint(c);
                                                  }),
                                        commande.end());
                            fonctionExecute(commande, &mon_disque);
                        }
                    }
                    else
                    {
                        cout << "Impossible d’ouvrir le fichier!!" << endl;
                    }
                }
                else
                {
                    cout << "Entrez la commande (exemple: mkdir /home): ";
                    getline(cin, commande);
                    fonctionExecute(commande, &mon_disque);
                }
            }
        }
    }
    return 0;
}

int fonctionExecute(const string &p_commande, DisqueVirtuel *p_disqueVirtuel)
{

    int retour = -1;
    vector<string> v_splitCommande;
    std::string chaine = p_commande;
    size_t found = chaine.find_first_of(" ");
    while (found != -1)
    {
        v_splitCommande.push_back(chaine.substr(0, found));
        chaine = chaine.substr(found + 1);
        found = chaine.find_first_of(" ");
    }
    v_splitCommande.push_back(chaine);

    for (size_t index = 0; index < 5; index++)
    {
        if (TableDesFonctions[index].StrCommande == v_splitCommande[0])
        {
            if (TableDesFonctions[index].nArguments != (v_splitCommande.size() - 1))
            {
                cout << "La commande " << v_splitCommande[0] << " demande " << TableDesFonctions[index].nArguments << " arguments" << endl;
            }
            else
            {
                if (index < 4)
                {
                    cout << endl
                         << "===== Commande " << v_splitCommande[0] << " " << v_splitCommande[1] << " =====" << endl;
                }
                else
                {
                    cout << endl
                         << "===== Commande " << v_splitCommande[0] << " =====" << endl;
                }
                switch (index)
                {
                case 0:
                    retour = (*p_disqueVirtuel).bd_mkdir(v_splitCommande[1]);
                    break;
                case 1:
                    cout << (*p_disqueVirtuel).bd_ls(v_splitCommande[1]);
                    break;
                case 2:
                    retour = (*p_disqueVirtuel).bd_rm(v_splitCommande[1]);
                    break;
                case 3:
                    retour = (*p_disqueVirtuel).bd_create(v_splitCommande[1]);
                    break;
                case 4:
                    retour = (*p_disqueVirtuel).bd_FormatDisk();
                    break;
                }
            }
        }
    }
    return retour;
}
