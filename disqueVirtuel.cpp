/**
 * \file disqueVirtuel.cpp
 * \brief Implémentation d'un disque virtuel.
 * \author ?
 * \version 0.1
 * \date  2023
 *
 *  Travail pratique numéro 3
 *
 */
#include <stdio.h>
#include "disqueVirtuel.h"
#include <iostream>
#include <string.h>
#include <sstream>
#include <algorithm>
#include <numeric>
// vous pouvez inclure d'autres librairies si c'est nécessaire

namespace TP3
{
    //constructor
    DisqueVirtuel::DisqueVirtuel()
    {
        bd_FormatDisk();
    }

    //Destructor
    DisqueVirtuel::~DisqueVirtuel()
    {
        for (auto& block : m_blockDisque)
        {
            for (auto *dir : block.m_dirEntry)
            {
                dir->m_filename.clear();
                dir->m_iNode = 0;
                delete dir;
            }
            block.m_dirEntry.clear();
            delete block.m_inode;
            block.m_bitmap.clear();
            block.m_type_donnees = 0;
            delete &block;
        }
        m_blockDisque.clear();
    }

    //rm
    int DisqueVirtuel::bd_rm(const std::string &p_Filename)
    {
        std::vector<std::string> paths = seperatePathToVector(p_Filename);

        iNode *inode = findNode(ROOT_INODE, paths, rm);

        if (inode == nullptr)
            return 0;

        inode->st_nlink--;
        Block &block = m_blockDisque.at(inode->st_block);
        if (inode->st_mode == S_IFDIR && isDirEntryEmpty(block.m_dirEntry))
        {
            printToConsole(inode->st_ino, inode->st_block, rmd);
            updateParents(inode, inode->st_ino, rm);
        }

        if (inode->st_mode == S_IFREG && inode->st_nlink == 0)
        {
            printToConsole(inode->st_ino, 0, rmf);
            updateParents(inode, inode->st_ino, rm);
        }

        return 0;
    }

    //verifie si le dirEntry est vide
    bool DisqueVirtuel::isDirEntryEmpty(const vector<dirEntry *> vecDirEntry)
    {
        bool empty = true;

        for (size_t i = 2; i <= vecDirEntry.size() - 1; i++)
        {
            if (!vecDirEntry.at(i)->m_filename.empty())
            {
                empty = false;
            }
        }
        return empty;
    }

    //mkdir
    int DisqueVirtuel::bd_mkdir(const std::string &p_DirName)
    {
        if (p_DirName == "/")
        {
            return 0;
        }

        std::vector<std::string> path = seperatePathToVector(p_DirName);
        iNode *inode = m_blockDisque.at(BASE_BLOCK_INODE + ROOT_INODE).m_inode;
        string lastPath;
        for (auto p : path)
        {
            bool foundOne = false;
            size_t blocToFind = m_blockDisque.at(BASE_BLOCK_INODE + inode->st_ino).m_inode->st_block;
            Block block = m_blockDisque.at(blocToFind);

            for (auto val : block.m_dirEntry)
            {
                if (p == val->m_filename)
                {
                    foundOne = true;
                    vector<string> p2(1, p);
                    lastPath = p;
                    inode = findNode(val->m_iNode, p2, mkdir);
                    break;
                }
            }
            if (!foundOne)
            {
                size_t firstFreeINode = findFirstFreeINode(m_blockDisque[FREE_INODE_BITMAP].m_bitmap);
                m_blockDisque[inode->st_block].m_dirEntry.push_back(new dirEntry(firstFreeINode, p));

                size_t blockID = m_blockDisque[BASE_BLOCK_INODE + firstFreeINode].m_inode->st_block;
                Block &b = m_blockDisque[blockID];

                printToConsole(firstFreeINode, blockID, mkdir);

                b.m_type_donnees = S_IFDE;
                b.m_dirEntry.push_back(new dirEntry(firstFreeINode, "."));
                b.m_dirEntry.push_back(new dirEntry(inode->st_ino, ".."));

                m_blockDisque[BASE_BLOCK_INODE + firstFreeINode].m_inode->st_nlink++;
                m_blockDisque[BASE_BLOCK_INODE + firstFreeINode].m_inode->st_mode = S_IFDIR;
                m_blockDisque[BASE_BLOCK_INODE + firstFreeINode].m_inode->st_size = 28;
                m_blockDisque[FREE_INODE_BITMAP].m_bitmap[firstFreeINode] = false;

                inode = m_blockDisque[BASE_BLOCK_INODE + firstFreeINode].m_inode;
                updateParents(inode, inode->st_ino, mkdir);
            }
        }
        return 1;
    }

    //update les node parent
    void DisqueVirtuel::updateParents(iNode *childInode, size_t childInodeIndex, COMMAND command)
    {
        size_t blocToFind = m_blockDisque.at(BASE_BLOCK_INODE + childInode->st_ino).m_inode->st_block;
        Block &block = m_blockDisque.at(blocToFind);

        iNode *parentInode = m_blockDisque.at(BASE_BLOCK_INODE + block.m_dirEntry.at(1)->m_iNode).m_inode;
        if (command == rm)
        {
            if (childInode->st_ino == childInodeIndex)
            {
                for (size_t i = 0; i <= m_blockDisque[parentInode->st_block].m_dirEntry.size() - 1; i++)
                {
                    if (m_blockDisque[parentInode->st_block].m_dirEntry.at(i)->m_iNode == childInodeIndex)
                    {
                        m_blockDisque[parentInode->st_block].m_dirEntry.erase(m_blockDisque[parentInode->st_block].m_dirEntry.begin() + i);
                    }
                }
                m_blockDisque[FREE_INODE_BITMAP].m_bitmap[childInodeIndex] = true;
                block.m_dirEntry.clear();
                block.m_type_donnees = 0;
                childInode->st_mode = 0;
                childInode->st_nlink = 0;
            }
            parentInode->st_nlink--;
            if (childInode->st_mode == S_IFDIR)
                parentInode->st_size -= 28;
        }
        else
        {
            parentInode->st_nlink++;
            if (childInode->st_mode == S_IFDIR)
                parentInode->st_size += 28;
        }

        if (childInode->st_ino != ROOT_INODE && childInode->st_mode != S_IFREG)
            updateParents(parentInode, childInodeIndex, command);
        return;
    }

    //trouve une node
    iNode *DisqueVirtuel::findNode(int currentINode, std::vector<std::string> path, COMMAND command)
    {
        size_t inode = currentINode;
        for (int i = 0; i < path.size(); i++)
        {
            bool isExist = false;
            size_t curentBlockID = m_blockDisque.at(BASE_BLOCK_INODE + inode).m_inode->st_block;
            for (auto dir : m_blockDisque.at(curentBlockID).m_dirEntry)
            {
                if (dir->m_filename == path[i])
                {
                    isExist = true;
                    inode = dir->m_iNode;
                    break;
                }
            }
            if (command == rm && !isExist)
                return nullptr;
            if (i == path.size() - 1 && m_blockDisque[BASE_BLOCK_INODE + inode].m_type_donnees == S_IFIN)
                return m_blockDisque[BASE_BLOCK_INODE + inode].m_inode;
        }
        return m_blockDisque.at(BASE_BLOCK_INODE + ROOT_INODE).m_inode;
    }

    //format disk
    int DisqueVirtuel::bd_FormatDisk()
    {
        for (auto block : m_blockDisque)
        {
            for (auto *dir : block.m_dirEntry)
            {
                dir->m_filename.clear();
                dir->m_iNode = 0;
                delete dir;
            }
            block.m_dirEntry.clear();
            delete block.m_inode;
            block.m_bitmap.clear();
            block.m_type_donnees = 0;
        }
        initAllBlocks();
        createRootDirectory();

        return 0;
    }

    //ls
    std::string DisqueVirtuel::bd_ls(const std::string &p_DirLocation)
    {
        std::vector<std::string> path = seperatePathToVector(p_DirLocation);
        iNode *inode = findNode(ROOT_INODE, path, ls);
        if ((inode == nullptr && p_DirLocation != "/") || inode->st_mode == S_IFREG)
        {
            return "Directory doesn't exist";
        }
        size_t blocToFind = m_blockDisque.at(BASE_BLOCK_INODE + inode->st_ino).m_inode->st_block;
        Block &block = m_blockDisque.at(blocToFind);
        std::stringstream output;
        output << p_DirLocation << std::endl;
        if (block.m_type_donnees == S_IFDE)
        {
            for (auto dir : block.m_dirEntry)
            {
                std::string dirOrFile = "";
                if (m_blockDisque.at(BASE_BLOCK_INODE + dir->m_iNode).m_inode->st_mode == S_IFDIR)
                    dirOrFile = "d ";
                else if (m_blockDisque.at(BASE_BLOCK_INODE + dir->m_iNode).m_inode->st_mode == S_IFREG)
                    dirOrFile = "- ";

                output
                    << dirOrFile
                    << dir->m_filename
                    << "\t\tSize: "
                    << m_blockDisque.at(BASE_BLOCK_INODE + dir->m_iNode).m_inode->st_size
                    << "\t\tiNode: "
                    << m_blockDisque.at(BASE_BLOCK_INODE + dir->m_iNode).m_inode->st_ino
                    << "\t\tnlink: "
                    << m_blockDisque.at(BASE_BLOCK_INODE + dir->m_iNode).m_inode->st_nlink << std::endl;
            }
        }
        return output.str();
    }

    //create
    int DisqueVirtuel::bd_create(const std::string &p_FileName)
    {

        std::vector<std::string> path = seperatePathToVector(p_FileName);

        if (path.size() > 1)
        {
            string newPath = "/" + std::accumulate(
                                       std::next(path.begin()),
                                       path.end() - 1,
                                       path[0],
                                       [](std::string a, std::string b)
                                       {
                                           return a + "/" + b;
                                       });
            bd_mkdir(newPath);
        }

        iNode *inode = findNode(ROOT_INODE, path, create);

        size_t firstFreeINode = findFirstFreeINode(m_blockDisque[FREE_INODE_BITMAP].m_bitmap);
        m_blockDisque[inode->st_block].m_dirEntry.push_back(new dirEntry(firstFreeINode, path.at(path.size() - 1)));

        printToConsole(firstFreeINode, 0, create);

        size_t blockID = m_blockDisque[BASE_BLOCK_INODE + firstFreeINode].m_inode->st_block;
        Block &b = m_blockDisque[blockID];

        b.m_type_donnees = S_IFIN;

        b.m_dirEntry.push_back(new dirEntry(firstFreeINode, "."));
        b.m_dirEntry.push_back(new dirEntry(inode->st_ino, ".."));
        m_blockDisque[BASE_BLOCK_INODE + firstFreeINode].m_inode->st_nlink++;
        m_blockDisque[BASE_BLOCK_INODE + firstFreeINode].m_inode->st_mode = S_IFREG;
        m_blockDisque[BASE_BLOCK_INODE + firstFreeINode].m_inode->st_size = 0;
        m_blockDisque[FREE_INODE_BITMAP].m_bitmap[firstFreeINode] = false;

        inode = m_blockDisque[BASE_BLOCK_INODE + firstFreeINode].m_inode;
        updateParents(inode, inode->st_ino, create);

        return 1;
    }

    //trouve la root i-node
    size_t DisqueVirtuel::findRootINode()
    {
        size_t root_iNode;
        for (int i = 0; i < 128; i++)
        {
            if (m_blockDisque.at(i).m_type_donnees == S_IFIN)
            {
                if (m_blockDisque.at(i).m_inode->st_ino == 1)
                {
                    root_iNode = m_blockDisque.at(i).m_inode->st_ino;
                    break;
                }
            }
        }
        return root_iNode;
    }

    //initialise tout les blocks
    void DisqueVirtuel::initAllBlocks()
    {

        // Clear disk
        if (m_blockDisque.size() > 0)
        {
            m_blockDisque.clear();
        }

        for (int i = 0; i < N_BLOCK_ON_DISK; i++)
        {
            if (i == FREE_BLOCK_BITMAP)
            {
                Block fb_bitmap = Block(S_IFBL);
                fb_bitmap.m_bitmap = initFreeBlockBitmap();
                m_blockDisque.push_back(fb_bitmap);
            }
            else if (i == FREE_INODE_BITMAP)
            {
                Block fin_bitmap = Block(S_IFIL);
                fin_bitmap.m_bitmap = initFreeINodeBitmap();
                m_blockDisque.push_back(Block(fin_bitmap));
            }
            else if (BASE_BLOCK_INODE <= i && i <= 23)
            {
                Block newBlock(S_IFIN);
                size_t firstFreeBlock = findFirstFreeBlock(m_blockDisque.at(FREE_BLOCK_BITMAP).m_bitmap);

                iNode inode = iNode(i - BASE_BLOCK_INODE, S_IFREG, 0, 0, firstFreeBlock);
                newBlock.m_inode = new iNode(inode);
                m_blockDisque.push_back(newBlock);

                // Set block to taken
                m_blockDisque.at(FREE_BLOCK_BITMAP).m_bitmap.at(firstFreeBlock) = false;
            }
            else
            {
                m_blockDisque.push_back(Block());
            }
        }
    }

    //create le root directory
    void DisqueVirtuel::createRootDirectory()
    {
        size_t rootBlock = m_blockDisque.at(BASE_BLOCK_INODE + 1).m_inode->st_block;
        size_t rootBlockINode = m_blockDisque.at(BASE_BLOCK_INODE + 1).m_inode->st_ino;
        printToConsole(rootBlockINode, rootBlock, format);

        // m_blockDisque.at(rootBlock).m_dirEntry.reserve(2);
        m_blockDisque.at(rootBlock).m_dirEntry.push_back(new dirEntry(1, "."));
        m_blockDisque.at(rootBlock).m_dirEntry.push_back(new dirEntry(1, ".."));
        m_blockDisque.at(rootBlock).m_type_donnees = S_IFDE;
        m_blockDisque.at(BASE_BLOCK_INODE + 1).m_inode->st_nlink += 2;
        m_blockDisque.at(BASE_BLOCK_INODE + 1).m_inode->st_mode = S_IFDIR;
        m_blockDisque.at(BASE_BLOCK_INODE + 1).m_inode->st_size = 56;
        m_blockDisque.at(FREE_INODE_BITMAP).m_bitmap.at(rootBlockINode) = false;
    }

    //separe la path en vector
    std::vector<std::string> DisqueVirtuel::seperatePathToVector(const std::string &path)
    {
        std::istringstream ss(path);
        std::vector<std::string> tokens;
        std::string token;

        while (std::getline(ss, token, '/'))
        {

            token.erase(remove_if(token.begin(), token.end(),
                                  [](unsigned char c)
                                  {
                                      return !std::isprint(c);
                                  }),
                        token.end());
            if (!token.empty())
            {
                tokens.push_back(token);
            }
        }
        return tokens;
    }

    //initialise les block du bitmap
    std::vector<bool> DisqueVirtuel::initFreeBlockBitmap()
    {

        std::vector<bool> fbbitmap(N_BLOCK_ON_DISK, true);

        for (int i = 0; i < N_BLOCK_ON_DISK; i++)
        {

            if (i <= 23)
            {
                fbbitmap.at(i) = false;
            }
        }

        return fbbitmap;
    }

    //trouve le premier block vide
    size_t DisqueVirtuel::findFirstFreeBlock(std::vector<bool> &fb_bitmap)
    {

        for (int i = 0; i < N_BLOCK_ON_DISK; i++)
        {

            if (fb_bitmap.at(i))
            {
                return i;
            }
        }

        return -1;
    }

    //trouve le premier inode bitmap
    std::vector<bool> DisqueVirtuel::initFreeINodeBitmap()
    {

        std::vector<bool> finbitmap(N_INODE_ON_DISK, true);

        for (int i = 0; i < N_INODE_ON_DISK; i++)
        {

            if (i == 0)
                finbitmap.at(i) = false;
        }

        return finbitmap;
    }

    //trouve le premier inode vide
    size_t DisqueVirtuel::findFirstFreeINode(std::vector<bool> &fin_bitmap)
    {
        for (int i = 0; i < N_INODE_ON_DISK; i++)
        {
            if (fin_bitmap.at(i))
            {
                return i;
            }
        }
        return -1;
    }

    //print to console
    void DisqueVirtuel::printToConsole(size_t st_ino, size_t st_block, COMMAND command)
    {
        std::stringstream output;
        switch (command)
        {
        case create:
            output
                << "UFS: saisir i-node "
                << st_ino
                << std::endl;
            break;
        case rmd:
            output
                << "UFS: Relache i-node "
                << st_ino
                << std::endl
                << "UFS: Relache bloc   "
                << st_block
                << std::endl;
            break;

        case rmf:
            output
                << "UFS: Relache i-node "
                << st_ino
                << std::endl;
            break;
        case mkdir:
            output
                << "UFS: saisir i-node "
                << st_ino
                << std::endl
                << "UFS: saisir bloc   "
                << st_block
                << std::endl;
            break;
        case format:
            output
                << "UFS: saisir i-node "
                << st_ino
                << std::endl
                << "UFS: saisir bloc   "
                << st_block
                << std::endl;
            break;
        }
        cout << output.str();
    }
}
