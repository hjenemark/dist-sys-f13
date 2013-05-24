/**
 * \file sensor.h
 **/
#include "common.h"
#include "temp_th.h"
#include "data_th.h"
#include "admin_th.h"


/** 
 * \brief Checks validity of startup parameters
 *
 * \param argc Integer indicating number of parameters.
 * 
 * \param argv[] Pointer to a lits of passed parameters
 *
 * \return void
 *
 **/
void check_startup_params(int argc, char *argv[]);

/** 
 * \brief Ensures prompt removal of dead processes
 *
 * \param s Unused
 *
 * \return void
 *
 **/
void sigchld_handler(int s);

/** 
 * \brief Main entry function
 *
 * \return EXIT code
 *
 **/
int main (int argc, char *argv[]);