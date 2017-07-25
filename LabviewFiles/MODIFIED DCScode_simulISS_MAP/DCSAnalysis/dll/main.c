/*
 * main.c
 *
 *  Created on: Jul 14, 2010
 *      Author: Rodrigo Barbosa
 */

#include "main.h"

/*======================================================================
   FUNCTION: getFlow
   ROLE:     Read and analyse the datas from the specified file,
             returning the Db (flow) value founded
   PARAM:    iIndexFile - File's ID
             rho - Array of distances source-detector
             flow - array with 8 positions (one for each detector), that
                    will keep the result
   RETURN:   > 0 - Succeeded
             < 0 - End of the process
======================================================================*/
int getFlow(int iFileIndex, char * sFileName, int * avg, value_t * rho, int * set, value_t * var, int iDetectorCount, double * flow)
{
    FILE *   reportFile;
	exp_info experimentInfo;
	dcs_file dataFile,
             averages;
    int      dimension,
             bThereIsFile = TRUE;

    // Set experiment's parameters
    setExperimentInfo(&experimentInfo, avg, rho, set, iDetectorCount, &dimension, sFileName);

    // Read file created from the experiment (LabVIEW)
    dataFile = readNextFile(&bThereIsFile, sFileName, &averages, avg, experimentInfo);

    // Minimization
    if(!bThereIsFile)
    {
        fprintfFootnote(&reportFile);
        return -1;
    }

    if(iFileIndex == 0)
        // Header for the report file
        fprintfHeaderAndClose(&reportFile, experimentInfo);

    // Figure out the best values for [Db, Beta, Mua, Mus']
    getBestFit(&reportFile, &dataFile, &averages, experimentInfo, var);

    // From the array of variables [Db, Beta, Mua, Mus'], pick up only the flow (Db)
    getFlowFromBestFit(experimentInfo, dataFile, averages, INDEX_DB, flow);

    // Free memory
    freeExperimentInfo(&dataFile, &averages, &experimentInfo); // Free allFiles.files

	return 1;
} /* getFlow */

void initAvg(int * avg)
{
    int index;

    for(index = 0; index < 32; index++)
        avg[index] = 0;

    avg[0] = 1;
    avg[1] = 0;
    avg[2] = 0;
    avg[3] = 1;
    avg[4] = 0;
    avg[5] = 0;
    avg[6] = 0;
    avg[7] = 0;

    avg[8] = 0;
    avg[9] = 1;
    avg[10] = 1;
    avg[11] = 0;
    avg[12] = 0;
    avg[13] = 0;
    avg[14] = 0;
    avg[15] = 0;

    avg[16] = 0;
    avg[17] = 0;
    avg[18] = 0;
    avg[19] = 0;
    avg[20] = 0;
    avg[21] = 0;
    avg[22] = 0;
    avg[23] = 0;

    avg[24] = 0;
    avg[25] = 0;
    avg[26] = 0;
    avg[27] = 0;
    avg[28] = 0;
    avg[29] = 0;
    avg[30] = 0;
    avg[31] = 0;
}

void initRho(value_t * rho)
{
    rho[0] = 2.5;
    rho[1] = 2.5;
    rho[2] = 2.5;
    rho[3] = 2.5;
    rho[4] = 2.5;
    rho[5] = 2.5;
    rho[6] = 2.5;
    rho[7] = 2.5;
}

void initFlow(double * flow)
{
    int index;

    for(index = 0; index < 12; index++)
        flow[index] = -1;
}

void initVarValues(value_t * var)
{
    var[INDEX_DB]   = 0.00000001; /* 1e-8*/
    var[INDEX_BETA] = 0.5;
    var[INDEX_MU_A] = 0.05;
    var[INDEX_MU_S] = 10.0;
}

void initGeneralSettings(int * set)
{
    set[0] = 1;   /* Start */
    set[1] = 120; /* End */
    set[2] = SI;  /* Approximation: Semi-infinite */
    set[3] = 1;   /* For Db and Beta */
}

int main(void)
{
    int iFileIndex = 0,
        avg[32];
    char sFileName[MAX_FILE_NAME];
    double flow[12];
    value_t rho[8];
    int     set[4];
    value_t var[4];

    getFileName(iFileIndex, sFileName);
    initFlow(flow);
    initAvg(avg);
    initRho(rho);
    initGeneralSettings(set);
    initVarValues(var);

    getFlow(iFileIndex, sFileName, avg, rho, set, var, 8, flow);

    return 0;
}
