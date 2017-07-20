#ifndef MAT_H
#define MAT_H

#include <math.h>
#include <stdlib.h>
#include "def.h"
#include "_aux.h"

void getBestFit(FILE ** reportFile, dcs_file * dataFile, dcs_file * averages, exp_info experimentInfo, value_t * var);

#endif // MAT_H
