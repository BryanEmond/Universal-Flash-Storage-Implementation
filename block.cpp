/**
 * \file block.cpp
 * \brief Implémentation d'un bloc.
 * \author ?
 * \version 0.1
 * \date  2023
 *
 *  Travail pratique numéro 3
 *
 */

#include "block.h"
// vous pouvez inclure d'autres librairies si c'est nécessaire

namespace TP3
{
    // constructor vide
    Block::Block() : m_type_donnees(0), m_bitmap({}), m_inode(nullptr), m_dirEntry({}){};
    // constructor avec un param
    Block::Block(size_t td) : m_type_donnees(td), m_bitmap({}), m_inode(nullptr), m_dirEntry({}){};
    // destructor
    Block::~Block(){};

} // Fin du namespace
