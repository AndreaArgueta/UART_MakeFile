#ifndef INCLUDE_H_
#define INCLUDE_H_

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "TM4C1294NCPDT.h"

#define MINT    96
#define MFRAC   0
#define N       4
#define Q       0
#define PSYSDIV 11

#define FBCHT   0x6
#define EBCHT   0x6
#define FBCE    0
#define EBCE    0
#define FWS     0x5
#define EWS     0x5
#define MB1     0x00100010

volatile char Dato;
#endif /* INCLUDE_H_ */
