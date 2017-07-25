#ifndef MAIN_H
#define MAIN_H

#include <windows.h> // DLL
#include <stdio.h>
#include "_aux.h"
#include "read.h"
#include "mat.h"

/*  To use this exported function of dll, include this header
 *  in the project. */

#ifdef BUILD_DLL
    #define DLL_EXPORT __declspec(dllexport)
#else
    #define DLL_EXPORT __declspec(dllimport)
#endif

int DLL_EXPORT getFlow(int iFileIndex, char * sFileName, int * avg, value_t * rho, int * set, value_t * var, int iDetectorCount, double * flow);

#endif // MAIN_H
