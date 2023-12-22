#include "block.h"

#ifndef _DISQUEVIRTUEL__H
#define _DISQUEVIRTUEL__H

namespace TP3
{

#define N_INODE_ON_DISK 20  
#define N_BLOCK_ON_DISK 128 
#define FREE_BLOCK_BITMAP 2 
#define FREE_INODE_BITMAP 3 
#define BASE_BLOCK_INODE 4  
#define ROOT_INODE 1        

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

        int bd_FormatDisk();
        std::string bd_ls(const std::string &p_DirLocation);
        int bd_mkdir(const std::string &p_DirName);
        int bd_create(const std::string &p_FileName);
        int bd_rm(const std::string &p_Filename);

    private:
        std::vector<Block> m_blockDisque; 
        std::vector<bool> initFreeBlockBitmap();
        std::vector<bool> initFreeINodeBitmap();
        size_t findFirstFreeBlock(std::vector<bool> &fb_bitmap);
        size_t findFirstFreeINode(std::vector<bool> &fin_bitmap);
        size_t findRootINode();
        std::vector<std::string> seperatePathToVector(const std::string &path);
        void createRootDirectory();
        void createEmptyDirectory(size_t firstFreeINode, iNode *parentINode, string filename);
        void initAllBlocks();

        bool isDirEntryEmpty(const vector<dirEntry *> vecDirEntry);
        iNode *findNode(int currentINode, std::vector<std::string> path, COMMAND command);
        void updateParents(iNode *childInode, size_t childInodeIndex, COMMAND command);
        void printToConsole(size_t st_ino, size_t st_block, COMMAND command);
    };

} // Fin du namespace

#endif
