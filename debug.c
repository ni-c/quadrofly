/**
 * Quadrofly Software (http://quadrofly.ni-c.de)
 *
 * @file 	debug.c
 * @brief 	Debug helper class
 * @author 	Willi Thiel (wthiel@quadrofly.ni-c.de)
 * @date 	Mar 7, 2012
 */
#include "main.h"

#ifdef SIMULAVR_AVAILABLE

/* This port correponds to the "-W 0x20,-" command line option. */
#define special_output_port (*((volatile char *)0x20))

#endif

/**
 * Poll the specified string out the debug port.
 *
 * @param str The specified string
 */
void debug_puts(const char *str) {

#ifdef SIMULAVR_AVAILABLE
  const char *c;

  for(c = str; *c; c++)
    special_output_port = *c;
#endif
}
