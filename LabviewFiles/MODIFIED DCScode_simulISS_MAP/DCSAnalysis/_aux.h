#ifndef _AUX_H
#define _AUX_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "def.h"

void setExperimentParameters(experiment * allFiles, int * dimension);
void freeAllResources(experiment * allFiles);

int fprintfHeader(FILE ** file, experiment allFiles, exp_info info);
void fprintfQuickFileBestFit(FILE ** file, dcs_file fileInfo, dcs_file averages, int iIndexFile, exp_info info);
void initBestFitArray(dcs_file * file);
void printCoordsBestFit(dcs_file file, int iIndexDetector);
void freeSimplex(simplex_t * simplex);
void freeBestFitArray(dcs_file * file);

void setExperimentInfo(exp_info * info, int * avg, value_t * rho, int * set, int iDetectorCount, int * dimension, char * sFitFileName);
void getFitFileName(char * sFileName, char * sFitFileName);
int  fprintfFootnote(FILE ** file);
int  fprintfHeaderAndClose(FILE ** file, exp_info experimentInfo);
void getFlowFromBestFit(exp_info info, dcs_file dataFile, dcs_file averages, int variable, value_t * result);
void freeExperimentInfo(dcs_file * file, dcs_file * averages, exp_info * experimentInfo);
int  fprintfQuickFileBestFitAndClose(FILE ** file, exp_info info, dcs_file dataFile, dcs_file averages);

#endif // _AUX_H
