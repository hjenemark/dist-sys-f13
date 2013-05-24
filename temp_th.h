/**
 * \file temp_th.h
 * Headerfile for temp_th.c
 **/
#include "common.h"

#ifndef __TEMP_TH_HEADER
#define __TEMP_TH_HEADER

#define SENSOR_SLEEP_INTERVAL 15

/** 
 * \brief Entry function for temperature monitoring thread.
 *
 * \param np A struct holding local network parameters.
 **/
void *temperature_thread_entry();

/** 
 * \brief Reads temperature of sensor. Random number based.
 *
 * \return Temperature value.
 **/
int8_t get_temperature();

/** 
 * \brief Send temperature to master node.
 *
 * \param np A struct storing local network settings.
 * 
 * \param pnp A struct storing peer (admin node) network settings.
 *
 * \param temperature An integer storing temperature.
 **/
void send_temperature(struct peer_net_params *pnp, int8_t temperature);

/** 
 * \brief Get current admin node network parameters.
 *
 * \param pnp A struct where network parameters will be held.
 * 
 * \return 0: on success.
 **/
int8_t get_admin_params(struct peer_net_params *pnp);


/** 
 * \brief Get current admin node to sensor time offset.
 * 
 * \return time offset.
 **/
int32_t get_time_offset();

#endif
