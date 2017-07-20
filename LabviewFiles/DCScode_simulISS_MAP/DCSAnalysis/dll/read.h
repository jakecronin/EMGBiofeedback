#ifndef READ_H
#define READ_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // sleep
#include <time.h>
#include "def.h"

void getFileName(int iFileNumber, char * sFileName);
dcs_file readNextFile(int * bThereIsFile, char * sFileName, dcs_file * averages, int * avg, exp_info info);
int test(char * sFileName);

#endif // READ_H
