#ifndef POTS_H
#define POTS_H

#include "parameters.h"
#include "pots_lag_types.h"

const byte NUM_POTS  = sizeof(POTS) / sizeof(*POTS);
#define DEBUG_POTS        (D_L_POTS        == (DEBUG & D_L_POTS))


#endif
