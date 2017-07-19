#ifndef _MACROS_H_
#define _MACROS_H_


#define GRID_SIZE 512 
#define PLOT_RESOLUTION GRID_SIZE
#define SINGLE_PRECISION 0

#if SINGLE_PRECISION
#define Real float
#else
#define Real double
#endif

#define IJ(i, j) ((i) + GRID_SIZE*(j))

#endif