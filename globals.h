/**
 * \file globals.h
 **/
extern int32_t node_is_master;
extern int32_t current_master_id;
extern pthread_mutex_t mutex_master_params;

extern struct peer_net_params admin_net_params;
extern pthread_mutex_t mutex_adminp;

extern int32_t node_admin_offset;
extern pthread_mutex_t mutex_timeoffset;

extern struct network_params np;

extern bool running;
