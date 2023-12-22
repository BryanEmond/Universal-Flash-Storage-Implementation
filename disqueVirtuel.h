/**
 * \file disqueVirtuel.h
 * \brief Gestion d'un disque virtuel.
 * \author GLO-2001
 * \version 0.1
 * \date  2023
 *
 *  Travail pratique numéro 3
 *
 */

#include "block.h"

#ifndef _DISQUEVIRTUEL__H
#define _DISQUEVIRTUEL__H

namespace TP3
{

#define N_INODE_ON_DISK 20  // nombre maximal d'i-nodes (donc de fichiers) sur votre disque
#define N_BLOCK_ON_DISK 128 // nombre de blocs sur le disque au complet
#define FREE_BLOCK_BITMAP 2 // numero du bloc contenant le bitmap des block libres
#define FREE_INODE_BITMAP 3 // numero du bloc contenant le bitmap des i-nodes libres
#define BASE_BLOCK_INODE 4  // bloc de depart ou les i-nodes sont stockes sur disque
#define ROOT_INODE 1        // numero du i-node correspondant au repertoire racine

    //enum de commande
    enum COMMAND
    {
        rm,
        create,
        ls,
        mkdir,
        format,
        rmd,
        rmf,
    };

    class DisqueVirtuel
    {
    public:
        DisqueVirtuel();
        ~DisqueVirtuel();

        // Méthodes demandées
        int bd_FormatDisk();
        std::string bd_ls(const std::string &p_DirLocation);
        int bd_mkdir(const std::string &p_DirName);
        int bd_create(const std::string &p_FileName);
        int bd_rm(const std::string &p_Filename);

        // Vous pouvez ajouter ici d'autres méthodes publiques

    private:
        // Il est interdit de modifier ce modèle d'implémentation (i.e le type de m_blockDisque)!
        std::vector<Block> m_blockDisque; // Un vecteur de blocs représentant le disque virtuel

        // Vous pouvez ajouter ici des méthodes privées
        std::vector<bool> initFreeBlockBitmap();
        std::vector<bool> initFreeINodeBitmap();
        size_t findFirstFreeBlock(std::vector<bool> &fb_bitmap);
        size_t findFirstFreeINode(std::vector<bool> &fin_bitmap);
        size_t findRootINode();
        std::vector<std::string> seperatePathToVector(const std::string &path);
        void createRootDirectory();
        void initAllBlocks();

        bool isDirEntryEmpty(const vector<dirEntry *> vecDirEntry);
        iNode *findNode(int currentINode, std::vector<std::string> path, COMMAND command);
        void updateParents(iNode *childInode, size_t childInodeIndex, COMMAND command);
        void printToConsole(size_t st_ino, size_t st_block, COMMAND command);
    };

} // Fin du namespace

#endif
