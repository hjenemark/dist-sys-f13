/**
 * \file user.h
 * Headerfile for user.c
 **/
#include "common.h"

#ifndef __USER_HEADER
#define __USER_HEADER

/** 
 * \brief Connects provided socket to node.
 * 
 * \param pnp A struct indicating peer network parameters.
 *
 * \param socket Pointer to socket that is to be connected.
 *
 * \return 0: upon success, other upon failure.
 **/
uint32_t connect_to_node(struct peer_net_params *pnp, uint32_t *socket);

/** 
 * \brief Promotes node to be master.
 *
 * \param socket A socket connected to node that is to become master.
 * 
 * \param promokey A pointer to a next promotion key.
 *
 * \param socket_type Enumerator to indicate requred socket type.
 *
 * \return 0: upon success, other upon failure.
 **/
uint32_t promote_node(uint32_t socket, uint32_t *promokey);

/** 
 * \brief Prints average temperature of nodes.
 *
 * \param socket A socket connected to a master node.
 *
 * \return 0: upon success.
 **/
uint32_t get_avg_temp(uint32_t socket);

/** 
 * \brief Provides user interface to communicate with nodes.
 * 
 * \param none.
 * 
 * \return 0: upon success.
 *
 **/
int32_t main();

#endif
