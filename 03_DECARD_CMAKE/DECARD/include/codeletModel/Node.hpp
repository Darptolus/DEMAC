/**
 * @file NCOM.hpp
 * @author Diego Roa
 * @date 31 Dec 2020
 * @brief Node Comunicator
 * @todo Add copyright
 *
 * Includes the definition of the Node Comunicator for each node of the cluster
 */

#ifndef NODE_H
#define NODE_H

#include <stdio.h>
#include <stdlib.h>
#include <NCOM.hpp>
#include <NMGR.hpp>
#include <eDARTS.hpp>

extern int world_size;
extern int world_rank;
extern int name_len;
extern char node_name[16];

int start_NODE();

#endif /* NODE_H */