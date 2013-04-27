/**
 * \file main.h
 **/
#ifndef MAIN_H
#define MAIN_H

/**
 * \brief Conditional compiler option for compilation on 64-bit systems 
 */
#ifndef __i386__
#include <stdint.h>
typedef int32 int;
#endif

#endif
