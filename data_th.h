/**
 * \file data_th.h
 **/
#include "common.h"

#ifndef __DATA_TH_HEADER
#define __DATA_TH_HEADER

/** \struct temp_storage
 *
 *	Structure to temporary information before appending to temperature database.
 */
struct temp_storage {
	int32_t temperature; /*!< Temperature value */
	int32_t timestamp; /*!< Timestamp when temperature was taken */
	struct sockaddr_storage *node_addr; /*!< Node address of sensor */
};

extern node_sens *temp_database; /* Pointer to database of values */

/** 
 * \brief Data packet processing thread entry function.
 * 
 * \param con Connection information
 *
 * \return void
 **/
void *data_network_worker(void *sock);

/** 
 * \brief Data processing thread entry function.
 *
 * \return void
 **/
void *data_network_thread_entry();

/** 
 * \brief Report average temperature.
 * 
 * \param socket Socket to use when reporting temperature
 *
 * \return void
 **/
void report_average_temperature(int32_t socket);

/** 
 * \brief Update global master ID.
 * 
 * \param keydata Received data containing key
 *
 * \return void
 **/
void update_promo_key(char *keydata);

/** 
 * \brief Make node a master node
 *
 * \return void
 **/
void become_master();

/** 
 * \brief Append timestamp to temporary storage
 * 
 * \param temp_storage Pointer to temporary storage structure
 * 
 * \param operand Pointer containing timestamp
 *
 * \return void
 **/
void append_db_timestamp(struct temp_storage* temp_storage, char *operand);

/** 
 * \brief Append temperature to temporary storage
 * 
 * \param temp_storage Pointer to temporary storage structure
 * 
 * \param operand Pointer containing temperature
 * 
 * \param node_addr Pointer containing reporting node address
 *
 * \return void
 **/
void append_db_data(struct temp_storage *temp_data, char *operand , struct sockaddr_storage *node_addr);

/** 
 * \brief Report timestamps for offset calculations
 * 
 * \param socket Socket to use for communication
 * 
 * \param time_rx Timestamp of time when request was received
 *
 * \return void
 **/
void report_time_values(int32_t socket, int32_t time_rx);
#endif
