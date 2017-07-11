/*
 * aux.c
 *
 *  Created on: Jul 14, 2010
 *      Author: Rodrigo Barbosa
 */

#include "_aux.h"

/*======================================================================
   FUNCTION: freeAttributs
   ROLE:     Free the memory allocated to the attributs of the struct
   PARAM:    file - struct which contains the attributs that will be
				    freed
   RETURN:   void
======================================================================*/
void freeFileAttributs(dcs_file * file)
{
	int iIndex;

	for(iIndex = 2; iIndex < file->iDetectorCount; iIndex++)
        free(file->dCorrs[iIndex]);

    freeBestFitArray(file);
	free(file->dCorrs);
	free(file->dTaus);
	free(file->dTotalIntensities);
}

/*======================================================================
   FUNCTION: freeAllResources
   ROLE:     Free all the memory allocated by the program
   PARAM:    allFiles - struct which contains all resources used
   RETURN:   void
======================================================================*/
void freeAllResources(experiment * allFiles)
{
	int iIndexFile;

	for(iIndexFile = 0; iIndexFile < allFiles->iFileCount; iIndexFile++)
		freeFileAttributs(&(allFiles->files[iIndexFile]));

	free(allFiles->files);
	free(allFiles->dRhos);
	free(allFiles->iUsedDetectors);
}

/*======================================================================
   FUNCTION: freeExperimentInfo
   ROLE:     The same role of the function above, but considering only
             the experiment's information and one single file, instead
             of all the files
   PARAM:    file - struct that represents the file's informations
             averages - struct with the averages among detectors
             experimentInfo - experiment's informations
   RETURN:   void
======================================================================*/
void freeExperimentInfo(dcs_file * file, dcs_file * averages, exp_info * experimentInfo)
{
	freeFileAttributs(file);
	freeFileAttributs(averages);

	free(experimentInfo->dRhos);
	free(experimentInfo->iUsedDetectors);
}

/*======================================================================
   FUNCTION: setExperimentParameters
   ROLE:	 Set up all important parameters before getting the best fit.
             Get dimension, i.e. how many variable from [Db, beta, muA,
             muS] are being furnished
   PARAM:	 allFiles - Data read from all files generated during the
   	   	   	   	   	    experiment
   	   	   	 dimension - how many variable from [Db, beta, muA, muS] are
   	   	   	 	 	 	 being furnished
   RETURN:	 void
======================================================================*/
void setExperimentParameters(experiment * allFiles, int * dimension)
{
	/* TODO: ask user about all parameters */

	/* 1. DEFINING USED DETECTORS ====================================*/
	allFiles->iUsedDetectors = (int *)malloc(sizeof(int) * DETEC_COUNT);
	/* TODO: Free this vector of used detectors */

	allFiles->iUsedDetectors[0] = 1; /* 0 - unused */
	allFiles->iUsedDetectors[1] = 1;
	allFiles->iUsedDetectors[2] = 1; /* 1 - used */
	allFiles->iUsedDetectors[3] = 1;
	allFiles->iUsedDetectors[4] = 1;
	allFiles->iUsedDetectors[5] = 1;
	allFiles->iUsedDetectors[6] = 1;
	allFiles->iUsedDetectors[7] = 1;

	/* 2. DEFINING RHO ===============================================*/
	allFiles->dRhos = (value_t *)malloc(sizeof(value_t) * DETEC_COUNT);
	/* TODO: Free this vector of used detectors */

	allFiles->dRhos[0] = 2.5;
	allFiles->dRhos[1] = 2.5;
	allFiles->dRhos[2] = 2.5;
	allFiles->dRhos[3] = 2.5;
	allFiles->dRhos[4] = 2.5;
	allFiles->dRhos[5] = 2.5;
	allFiles->dRhos[6] = 2.5;
	allFiles->dRhos[7] = 2.5;

	/* 3. DEFINING LIMITS OF FITTING POINTS ==========================*/
	allFiles->iFirstFitPt = 1;
	allFiles->iLastFitPt  = 120;

	/* 4. DEFINING TYPE OF APPROXIMATION =============================*/
	allFiles->approximation = SI; /* Semi-infinite */

	/* 5. DEFINING THE STARTING POINT ================================*/
	allFiles->startPt[INDEX_DB]   = 1; /* 0.00000001; 1e-8*/
	allFiles->startPt[INDEX_BETA] = 1; /* 0.5; */
	allFiles->startPt[INDEX_MU_A] = 0; /* Constant */
	allFiles->startPt[INDEX_MU_S] = 0; /* Constant */

	/* 6. DEFINING THE DIMENSION =====================================*/
	/* Define dimension from the data furnished by the user */
	* dimension = allFiles->startPt[INDEX_DB]   +
				  allFiles->startPt[INDEX_BETA] +
				  allFiles->startPt[INDEX_MU_A] +
				  allFiles->startPt[INDEX_MU_S];

    /* 7. DEFINING THE NUMBER OF FILES CREATED FROM THE EXPERIMENT ==*/
    allFiles->iFileCount = 0; /* Get from LabVIEW */

    /* Memory allocation to the result array */
	allFiles->files = (dcs_file *)malloc(sizeof(dcs_file) * FILE_COUNT);
} /* setExperimentParameters */

/*======================================================================
   FUNCTION: setExperimentInfo
   ROLE:	 The same idea of the function above, but this one sets up
             only the common informations and don't care about creating
             memory for all data files
   PARAM:	 info - Data read from all files generated during the
   	   	   	   	   	    experiment
             avg - Carry information about which detector is considered
                   in which average
             rho - Array of distances source-detector
             set - General settings: fittings boundaries, approximation,
                   variables that are being fitted (Db || Db and Beta)
             var - Value for Beta, Mua and Mus'
             iDetectorCount - number of detectors used (4 or 8)
   	   	   	 dimension - how many variable from [Db, beta, muA, muS] are
   	   	   	 	 	 	 being furnished
   RETURN:	 void
======================================================================*/
void setExperimentInfo(exp_info * info,
                       int * avg, value_t * rho, int * set,
                       int iDetectorCount, int * dimension, char * sFileName)
{
	/* TODO: ask user about all parameters */

    /* 0. DEFINING NUMBER OF DETECTORS ===============================*/
    info->iDetectorCount = iDetectorCount; /* 4 or 8 */

	/* 1. DEFINING RHO ===============================================*/
	info->dRhos = (value_t *)malloc(sizeof(value_t) * iDetectorCount);
	/* TODO: Free this vector of used detectors */

	/* 2.1 DEFINING USED DETECTORS ====================================*/
	info->iUsedDetectors = (int *)malloc(sizeof(int) * iDetectorCount);
	/* TODO: Free this vector of used detectors */

    info->dRhos[0] = rho[0];
    info->dRhos[1] = rho[1];
    info->dRhos[2] = rho[2];
    info->dRhos[3] = rho[3];

    info->iUsedDetectors[0] = 1; /* 0 - unused */
    info->iUsedDetectors[1] = 1;
    info->iUsedDetectors[2] = 1; /* 1 - used */
    info->iUsedDetectors[3] = 1;

    if(iDetectorCount == 8)
    {
        info->dRhos[4] = rho[4];
        info->dRhos[5] = rho[5];
        info->dRhos[6] = rho[6];
        info->dRhos[7] = rho[7];

        info->iUsedDetectors[4] = 1;
        info->iUsedDetectors[5] = 1;
        info->iUsedDetectors[6] = 1;
        info->iUsedDetectors[7] = 1;
    }

	/* 2.2 DEFINING USED AVERAGES ====================================*/
	int iIndexAvg,
        iIndexDet;

    for(iIndexAvg = 0; iIndexAvg < MAX_AVG; iIndexAvg++)
    {
        info->iUsedAvg[iIndexAvg] = FALSE;

        for(iIndexDet = 0; iIndexDet < iDetectorCount; iIndexDet++)
            if(avg[iIndexAvg * MAX_DETEC + iIndexDet])
            {
                info->iUsedAvg[iIndexAvg] = TRUE;

                /* For Rho, it doesn't matter if you take the one or
                   the other detector, because they should have the
                   same Rho value to be averaged */
                info->dRhosAvg[iIndexAvg] = info->dRhos[iIndexDet];
            }
    }

	/* 3. DEFINING LIMITS OF FITTING POINTS ==========================*/
	info->iFirstFitPt = set[0];
	info->iLastFitPt  = set[1];

	/* 4. DEFINING TYPE OF APPROXIMATION =============================*/
	info->approximation = set[2]; /* Semi-infinite */

	/* 5. DEFINING THE STARTING POINT ================================*/
	info->startPt[INDEX_DB]   = 1; /* 0.00000001; 1e-8*/ /* Always true */
	info->startPt[INDEX_BETA] = set[3]; /* 0: Db, 1: Db and Beta */
	info->startPt[INDEX_MU_A] = 0; /* Constant */
	info->startPt[INDEX_MU_S] = 0; /* Constant */

	/* 6. DEFINING THE DIMENSION =====================================*/
	/* Define dimension from the data furnished by the user */
	* dimension = info->startPt[INDEX_DB]   +
				  info->startPt[INDEX_BETA] +
				  info->startPt[INDEX_MU_A] +
				  info->startPt[INDEX_MU_S];

    /* 7. DEFINING THE NUMBER OF FILES CREATED FROM THE EXPERIMENT ==*/
    info->iFileCount = 0; /* Get from LabVIEW */

    /* 8. DEFINING THE NAME OF THE FILE WITH THE ANALYSIS RESULT */
    getFitFileName(sFileName, info->sFitFileName);
} /* setExperimentInfo */

/*======================================================================
   FUNCTION: initBestFitArray
   ROLE:     Allocate memory to the array which will contain the
   	   	     result of the minimization process for each detector of
   	   	     the file
   PARAM:	 file - where the pointer to the array is
   RETURN:	 void
======================================================================*/
void initBestFitArray(dcs_file * file)
{
	int iIndexDetector;

	file->bestFit = (value_t **)malloc(sizeof(value_t *) * file->iDetectorCount);

	for(iIndexDetector = 0; iIndexDetector < file->iDetectorCount; iIndexDetector++)
		file->bestFit[iIndexDetector] =
		 (value_t *)malloc(sizeof(value_t) * MAX_VAR);
}

/*======================================================================
   FUNCTION: freeBestFitArray
   ROLE:     Free memory from the array which contains the result of the
    		 minimization process for each detector of the file
   PARAM:	 file - where the pointer to the array is
   RETURN:	 void
======================================================================*/
void freeBestFitArray(dcs_file * file)
{
	int iIndexDetector;

	for(iIndexDetector = 0; iIndexDetector < file->iDetectorCount; iIndexDetector++)
		free(file->bestFit[iIndexDetector]);

	free(file->bestFit);
}

/*======================================================================
   FUNCTION: freeSimplex
   ROLE:     Free memory allocated to the simplex (in function minimize)
   PARAM:	 simplex - where the structures to be freed are
   RETURN:	 void
======================================================================*/
void freeSimplex(simplex_t * simplex)
{
	int index;

	/* Memory to the vertex coordinates. Each column represents one variable */
	for(index = 0; index <= simplex->n; index++)
		free(simplex->coordsVertex[index]);

	/* Memory to the vertex coordinates. Each row represents one vertex */
	free(simplex->coordsVertex);

	/* Memory to the array with the function evaluation in each vertex */
	free(simplex->fValueVertex);

	/* Memory to the vertex coordinates. Each column represents one variable */
	for(index = 0; index < MAX_CREC; index++)
		free(simplex->coordsCREC[index]);

	/* Memory to the Center, Reflection, Expansion and Contraction vertex
	 * Each row represents one vertex */
	free(simplex->coordsCREC);
}

/*======================================================================
   FUNCTION: printIteration
   ROLE:     Used to follow the minimization process, by printing the
   	   	     value of the function evaluation for each vertex
   PARAM:    simplex - Struct which contains the values to be printed
   	   	     iIteration - Number of the iteration
   RETURN:   void
======================================================================*/
void printIteration(simplex_t simplex, int iIteration)
{
	printf("%3d) 0: %.8f | 1: %.8f | 2: %.8f - ",
		   iIteration,
		   simplex.fValueVertex[simplex.smallestID],
		   simplex.fValueVertex[simplex.middleID],
		   simplex.fValueVertex[simplex.largestID]);
}

/*======================================================================
   FUNCTION: printCoordsBestFit
   ROLE:     Print the result of the minimization process for the
   	   	     specified detector
   PARAM:    file - Struct which contains the values to be printed
   RETURN:   void
======================================================================*/
void printCoordsBestFit(dcs_file file, int iIndexDetector)
{
	printf("Db: %.12f | Beta: %.8f | muA: %.8f | muB: %.8f\n",
		   file.bestFit[iIndexDetector][INDEX_DB],
		   file.bestFit[iIndexDetector][INDEX_BETA],
		   file.bestFit[iIndexDetector][INDEX_MU_A],
		   file.bestFit[iIndexDetector][INDEX_MU_S]);
}

/*======================================================================
   FUNCTION: printBestFit
   ROLE:     Print the result of the minimization process
   PARAM:    simplex - Struct which contains the values to be printed
   RETURN:   void
======================================================================*/
void printBestFit(simplex_t simplex)
{
	printf("Final: Db: %.12f, Beta: %f, muA: %f, muS: %f - %f\n",
		   simplex.coordsVertex[simplex.smallestID][INDEX_DB],
		   simplex.coordsVertex[simplex.smallestID][INDEX_BETA],
		   simplex.coordsVertex[simplex.smallestID][INDEX_MU_A],
		   simplex.coordsVertex[simplex.smallestID][INDEX_MU_S],
		   simplex.fValueVertex[simplex.smallestID]);
}

/*======================================================================
   FUNCTION: fprintfDate
   ROLE:     Print out the period of the experiment on the file
   PARAM:    file - file to be written, which is already opened
   	   	   	 firstFile - where is the date of the first file
   	   	   	 lastFile - where is the date of the last file
   RETURN:   void
======================================================================*/
void fprintfDate(FILE * file, dcs_file firstFile, dcs_file lastFile)
{
	fprintf(file, "DATE:  %s - %s\n", firstFile.sDate, lastFile.sDate);
}

/*======================================================================
   FUNCTION: fprintfConstants
   ROLE:     Print out the variables [Db, Beta, Mua and Mus] that were
   	   	     considered as a constant during the experiment
   PARAM:    file - file to be written, which is already opened
   	   	   	 var  - array that states if a variable is in fact a
   	   	   	 	 	variable (1) or not (0)
   RETURN:   void
======================================================================*/
void fprintfConstants(FILE * file, value_t * var)
{
	char * varNames[]     = VAR_NAMES;
	value_t constValues[] = {DB, BETA, MU_A, MU_S};
	int  iIndexVar;

	fprintf(file, "CONST: ");

	for(iIndexVar = 0; iIndexVar < MAX_VAR; iIndexVar++)
		if(var[iIndexVar] == 0) /* If it's a constant */
			fprintf(file, "%s: %f   ", varNames[iIndexVar], constValues[iIndexVar]);

	fprintf(file, "\n");
}

/*======================================================================
   FUNCTION: getVarHeader
   ROLE:     Build a string with the variables of the experiment
   PARAM:    var  - array that states if a variable is in fact a
   	   	   	 	 	variable (1) or not (0)
   	   	   	 sHeader - string built from the array
   	   	   	 separator - used to draw the table during the printing
   RETURN:   void
======================================================================*/
void getVarHeader(value_t * var, char * sHeader,
				  char * sSeparator, char * sBoldSeparator)
{
	/* Db is always a variable */
	strcpy(sHeader, "     Db     ");
	strcpy(sBoldSeparator, "============");
	strcpy(sSeparator, "------------");

	if(var[INDEX_BETA]) /* If Beta is a variable */
	{
		strcat(sHeader, " Beta   ");
		strcat(sBoldSeparator, "========");
		strcat(sSeparator, "--------");
	}

	if(var[INDEX_MU_A]) /* If Beta is a variable */
	{
		strcat(sHeader, "  mua   ");
		strcat(sBoldSeparator, "========");
		strcat(sSeparator, "--------");
	}

	if(var[INDEX_MU_S]) /* If Beta is a variable */
	{
		strcat(sHeader, "mus ");
		strcat(sBoldSeparator, "====");
		strcat(sSeparator, "----");
	}

	sHeader[strlen(sHeader) - 1]       = '|';
	sSeparator[strlen(sSeparator) - 1] = '|';
}

/*======================================================================
   FUNCTION: fprintfSmallsSeparator
   ROLE:	 Draw a line in the file
   PARAM:    file - file to be written, which is already opened
   	   	     sHeader - header for one single detector column
   	   	     info - experiment's auxiliary informations
   RETURN:   void
======================================================================*/
void fprintfSmallHeader(FILE * file, char * sHeader, exp_info info,
						char * margin)
{
	int iIndex;

	fprintf(file, "%s", margin);

	for(iIndex = 0; iIndex < info.iDetectorCount; iIndex++)
		if(info.iUsedDetectors[iIndex] == 1) /* If the detector is being used */
			fprintf(file, "%s", sHeader);

	fprintf(file, "\n");
}

/*======================================================================
   FUNCTION: fprintfDetectorHeader
   ROLE:	 Print out the detectors' ID in the header of the file
   PARAM:	 file - file to be written, which is already opened
   	   	     iLength - length of the separator, which means the length
   	   	     	 	   of each detector's column
   	   	     info - experiment's auxiliary informations
   RETURN:   void
======================================================================*/
void fprintfDetectorHeader(FILE * file, int iLength, exp_info info)
{
	char sDetectorHeader[MAX_VAR_HEADER] = "";
	char sAux[DETEC_LABEL_LEN] = DETEC_LABEL;
	int iIndex,
		iAlignHeader;

	fprintf(file, "%s", MARGIN);

	iAlignHeader = (iLength - DETEC_LABEL_LEN)/2;

	for(iIndex = 0; iIndex < iAlignHeader; iIndex++)
		strcat(sDetectorHeader, " ");

	strcat(sDetectorHeader, sAux);

	for(iIndex = 0; iIndex < iAlignHeader; iIndex++)
		strcat(sDetectorHeader, " ");

	if(strlen(sDetectorHeader) < iLength)
		strcat(sDetectorHeader, " ");

	sDetectorHeader[strlen(sDetectorHeader) - 1] = '|';

	for(iIndex = 0; iIndex < info.iDetectorCount; iIndex++)
		if(info.iUsedDetectors[iIndex] == 1) /* If the detector is being considered */
		{
			sDetectorHeader[iAlignHeader + DETEC_LABEL_LEN - 1] = iIndex + 49;
			fprintf(file, "%s", sDetectorHeader);
		}

	fprintf(file, "\n");
}

/*======================================================================
   FUNCTION: fprintfHeader (unused)
   ROLE:     Print out the header of the file where is the result of
   	   	     the fitting process (getBestXFit)
   PARAM:    file - file to be written, with the fitting result
   	   	     allFiles - information to be written
   RETURN:   0 - error
   	   	     1 - success
======================================================================*/
int fprintfHeader(FILE ** file, experiment allFiles, exp_info info)
{
	char sFileName[MAX_FILE_NAME]   = "";
	char sVarHeader[MAX_VAR_HEADER] = "";
	char sSeparator[MAX_VAR_HEADER] = "";
	char sSeparatorBold[MAX_VAR_HEADER] = "";

	/* Construct the file's name */
	strcat(sFileName, PREFIX_FILE_NAME);
	strcat(sFileName, "FIT.dat");

	if((* file = fopen(sFileName, "w")) == NULL)
		return 0;

	fprintfDate(* file, allFiles.files[0], allFiles.files[allFiles.iFileCount - 1]);
	fprintfConstants(* file, allFiles.startPt);

	getVarHeader(allFiles.startPt, sVarHeader, sSeparator, sSeparatorBold);
	fprintfSmallHeader(* file, sSeparatorBold, info, MARGIN);

	fprintfDetectorHeader(* file, strlen(sSeparator), info);
	fprintfSmallHeader(* file, sSeparator, info, MARGIN);

	fprintfSmallHeader(* file, sVarHeader, info, MARGIN);
	fprintfSmallHeader(* file, sSeparatorBold, info, BORDER);

	return 1;
} /* fprintfHeader */

/*======================================================================
   FUNCTION: fprintfHeaderAndClose
   ROLE:     The same role of the function above, but this one close the
             file after the writing
   PARAM:    file - file to be written, with the fitting result
   	   	     info - information to be written
   RETURN:   0 - error
   	   	     1 - success
======================================================================*/
int fprintfHeaderAndClose(FILE ** file, exp_info info)
{
	char sVarHeader[MAX_VAR_HEADER]     = "";
	char sSeparator[MAX_VAR_HEADER]     = "";
	char sSeparatorBold[MAX_VAR_HEADER] = "";

	if((* file = fopen(info.sFitFileName, "w")) == NULL)
		return 0;

    // TODO: Print out the experiment's date
	fprintfConstants(* file, info.startPt);

	getVarHeader(info.startPt, sVarHeader, sSeparator, sSeparatorBold);
	fprintfSmallHeader(* file, sSeparatorBold, info, MARGIN);

	fprintfDetectorHeader(* file, strlen(sSeparator), info);
	fprintfSmallHeader(* file, sSeparator, info, MARGIN);

	fprintfSmallHeader(* file, sVarHeader, info, MARGIN);
	fprintfSmallHeader(* file, sSeparatorBold, info, BORDER);

    fclose(* file);

	return 1;
} /* fprintfHeaderAndClose */

/*======================================================================
   FUNCTION: fprintfHeaderAndClose
   ROLE:     Print out a line in the end of the file to indicate that
             the analysis of all the experiment's files is done
   PARAM:    file - file to be written
   RETURN:   0 - error
   	   	     1 - success
======================================================================*/
int fprintfFootnote(FILE ** file)
{
    char sFileName[MAX_FILE_NAME] = "";

	/* Construct the file's name */
	strcat(sFileName, PREFIX_FILE_NAME);
	strcat(sFileName, "FIT.dat");

    if((* file = fopen(sFileName, "a")) == NULL)
		return 0;

    fprintf(* file, "%s END\n", BORDER);

    return 1;
} /* fprintfFootnote */

/*======================================================================
   FUNCTION: fprintfFileBestFit
   ROLE:	 Print out in the file the result of the fitting process for
   	   	     the specific file
   PARAM:    fileBestFit - file to be written
   	   	     fileInfo - Struct which contains the values to be printed
   RETURN:   void
======================================================================*/
void fprintfFileBestFit(FILE ** file, dcs_file fileInfo, int iIndexFile,
					    value_t * var)
{
	int iIndexDetector;

	fprintf(* file, "FILE %4d |", iIndexFile + 1);

	for(iIndexDetector = 0; iIndexDetector < fileInfo.iDetectorCount; iIndexDetector++)
	{
		fprintf(* file, " %.3e ", fileInfo.bestFit[iIndexDetector][INDEX_DB]);

		if(var[INDEX_BETA]) /* If Beta is a variable */
			fprintf(* file, "  %.3f ", fileInfo.bestFit[iIndexDetector][INDEX_BETA]);

		if(var[INDEX_MU_A]) /* If Beta is a variable */
			fprintf(* file, "  %.2f  ", fileInfo.bestFit[iIndexDetector][INDEX_MU_A]);

		if(var[INDEX_MU_S]) /* If Beta is a variable */
			fprintf(* file, " %.0f ", fileInfo.bestFit[iIndexDetector][INDEX_MU_S]);

		fprintf(* file, "|");
	}

	fprintf(* file, "\n");
}

/*======================================================================
   FUNCTION: fprintfQuickFileBestFit
   ROLE:	 This function does the same of the function above, but using
   	   	     a simple format, without separators
   PARAM:    fileBestFit - file to be written
   	   	     fileInfo - Struct which contains the values to be printed
   RETURN:   void
======================================================================*/
void fprintfQuickFileBestFit(FILE ** file, dcs_file fileInfo, dcs_file averages,
                             int iIndexFile, exp_info info)
{
	int iIndexDetector,
        iIndexAverage;

	fprintf(* file, "%s", fileInfo.sTime);

	for(iIndexDetector = 0; iIndexDetector < fileInfo.iDetectorCount; iIndexDetector++)
	{
		fprintf(* file, "\t%.3e", fileInfo.bestFit[iIndexDetector][INDEX_DB]);

		if(info.startPt[INDEX_BETA]) /* If Beta is a variable */
			fprintf(* file, "\t%.3f", fileInfo.bestFit[iIndexDetector][INDEX_BETA]);

		if(info.startPt[INDEX_MU_A]) /* If Beta is a variable */
			fprintf(* file, "\t%.2f  ", fileInfo.bestFit[iIndexDetector][INDEX_MU_A]);

		if(info.startPt[INDEX_MU_S]) /* If Beta is a variable */
			fprintf(* file, "\t%.0f ", fileInfo.bestFit[iIndexDetector][INDEX_MU_S]);
	}

	for(iIndexAverage = 0; iIndexAverage < MAX_AVG; iIndexAverage++)
	{
	    if(info.iUsedAvg[iIndexAverage])
	    {
            fprintf(* file, "\t%.3e", averages.bestFit[iIndexAverage][INDEX_DB]);

            if(info.startPt[INDEX_BETA]) /* If Beta is a variable */
                fprintf(* file, "\t%.3f", averages.bestFit[iIndexAverage][INDEX_BETA]);

            if(info.startPt[INDEX_MU_A]) /* If Beta is a variable */
                fprintf(* file, "\t%.2f  ", averages.bestFit[iIndexAverage][INDEX_MU_A]);

            if(info.startPt[INDEX_MU_S]) /* If Beta is a variable */
                fprintf(* file, "\t%.0f ", averages.bestFit[iIndexAverage][INDEX_MU_S]);
	    }
	}

	fprintf(* file, "\n");
}

/*======================================================================
   FUNCTION: fprintfQuickFileBestFitAndClose
   ROLE:	 This function does the same of the function above, but this
             one considers the file is closed, open it, and close it in
             the end
   PARAM:    fileBestFit - file to be written
   	   	     fileInfo - Struct which contains the values to be printed
   RETURN:   0 - error
   	   	     1 - success
======================================================================*/
int fprintfQuickFileBestFitAndClose(FILE ** file, exp_info info, dcs_file dataFile,
                                    dcs_file averages)
{
	if((* file = fopen(info.sFitFileName, "a")) == NULL)
		return 0;

    // TODO: Print averages
    fprintfQuickFileBestFit(file, dataFile, averages, -1, info);

    fclose(* file);
    return 1;
}

/*======================================================================
   FUNCTION: getFitFileName
   ROLE:	 fileName_XX.dat -> fileName_FIT.dat
   PARAM:    sFileName - fileName_XX.dat
             sFitFileName - fileName_FIT.dat
   RETURN:   0 - error
   	   	     1 - success
======================================================================*/
void getFitFileName(char * sFileName, char * sFitFileName)
{
    int iLen = strlen(sFileName);
    int iIndex = iLen - 1;

    strcpy(sFitFileName, sFileName);

    while(iIndex >= 0 && sFitFileName[iIndex] != '_')
        iIndex--;

    if(iIndex >= 0)
        sFitFileName[iIndex + 1] = '\0';

    strcat(sFitFileName, "FIT.dat");
}

/*======================================================================
   FUNCTION: getVarFromBestFit
   ROLE:     Copy the value of the specified variable from the best fit
             array to the result array
   PARAM:    dataFile - struct that contains the file's best fit array
             averages - where the best fits for the averages are stored
             variable - which variable will be copied [Db, Beta, Mua, Mus]
             result   - array with the values of the variable for each
                        detector
   RETURN:   void
======================================================================*/
void getFlowFromBestFit(exp_info info, dcs_file dataFile, dcs_file averages,
                        int variable, value_t * result)
{
    int iIndexDetector = 0,
        iIndexAverages = 0,
        iIndex;

    for(iIndex = 0; iIndex < MAX_DETEC + MAX_AVG; iIndex++)
    {
        if(iIndex < dataFile.iDetectorCount)
            result[iIndex] = dataFile.bestFit[iIndexDetector++][variable];
        else
        {
            if(iIndex == 4) /* Jump 4 positions if the detectores 5-8 aren't being used */
                iIndex = 8;

            if(info.iUsedAvg[iIndexAverages])
                result[iIndex] = averages.bestFit[iIndexAverages++][variable];
            else
                result[iIndex] = -1; /* Not used */
        }
    }
}

/*======================================================================
   FUNCTION: printFileReport
   ROLE:     Auxiliary function for debuggin, which prints all the
			 informations read in the file and that were stored in the
			 struct
   PARAM:    fileInfo - struct that contains all file's informations
   RETURN:   void
======================================================================*/
void printFileReport(dcs_file fileInfo)
{
	printf("|========================================== FILE REPORT ===========================================|\n");
	printf("|                                                                                                  |\n");
	printf("|NAME: %s .... DATE: %s ... DURATION: %.6f ...|\n",
			fileInfo.sFileName, fileInfo.sDate, fileInfo.fDuration);
	printf("|                                                                                                  |\n");
	printf("|NUMBER OF DETECTORS: %4d ......................... NUMBER OF TAUS: %4d .........................|\n",
			fileInfo.iDetectorCount, fileInfo.iTausCount);
	printf("|                                                                                                  |\n");
	printf("|INTENSITIES:   %.6f|%.6f|%.6f|%.6f|%.6f|%.6f|%.6f|%.6f|\n",
			fileInfo.dTotalIntensities[0], fileInfo.dTotalIntensities[1], fileInfo.dTotalIntensities[2],
			fileInfo.dTotalIntensities[3], fileInfo.dTotalIntensities[4], fileInfo.dTotalIntensities[5],
			fileInfo.dTotalIntensities[6], fileInfo.dTotalIntensities[7]);
	printf("|                                                                                                  |\n");
	printf("|DELAY(TAU)|    D1    |    D2    |    D3    |    D4    |    D5    |    D6    |    D7    |    D8    |\n");
	printf("|%1.8f|%1.8f|%1.8f|%1.8f|%1.8f|%1.8f|%1.8f|%1.8f|%1.8f|\n",
			fileInfo.dTaus[0], fileInfo.dCorrs[0][0], fileInfo.dCorrs[1][0], fileInfo.dCorrs[2][0],
							   fileInfo.dCorrs[3][0], fileInfo.dCorrs[4][0], fileInfo.dCorrs[5][0],
							   fileInfo.dCorrs[6][0], fileInfo.dCorrs[7][0]);
	printf("|%1.8f|%1.8f|%1.8f|%1.8f|%1.8f|%1.8f|%1.8f|%1.8f|%1.8f|\n",
			fileInfo.dTaus[1], fileInfo.dCorrs[0][1], fileInfo.dCorrs[1][1], fileInfo.dCorrs[2][1],
							   fileInfo.dCorrs[3][1], fileInfo.dCorrs[4][1], fileInfo.dCorrs[5][1],
							   fileInfo.dCorrs[6][1], fileInfo.dCorrs[7][1]);
	printf("|   ...    |   ...    |   ...    |   ...    |   ...    |   ...    |   ...    |   ...    |   ...    |\n");
	printf("|%1.8f|%1.8f|%1.8f|%1.8f|%1.8f|%1.8f|%1.8f|%1.8f|%1.8f|\n",
			fileInfo.dTaus[fileInfo.iTausCount - 3], fileInfo.dCorrs[0][fileInfo.iTausCount - 3],
													 fileInfo.dCorrs[1][fileInfo.iTausCount - 3],
													 fileInfo.dCorrs[2][fileInfo.iTausCount - 3],
													 fileInfo.dCorrs[3][fileInfo.iTausCount - 3],
													 fileInfo.dCorrs[4][fileInfo.iTausCount - 3],
													 fileInfo.dCorrs[5][fileInfo.iTausCount - 3],
													 fileInfo.dCorrs[6][fileInfo.iTausCount - 3],
													 fileInfo.dCorrs[7][fileInfo.iTausCount - 3]);
	printf("|%1.8f|%1.8f|%1.8f|%1.8f|%1.8f|%1.8f|%1.8f|%1.8f|%1.8f|\n",
			fileInfo.dTaus[fileInfo.iTausCount - 2], fileInfo.dCorrs[0][fileInfo.iTausCount - 2],
													 fileInfo.dCorrs[1][fileInfo.iTausCount - 2],
													 fileInfo.dCorrs[2][fileInfo.iTausCount - 2],
													 fileInfo.dCorrs[3][fileInfo.iTausCount - 2],
													 fileInfo.dCorrs[4][fileInfo.iTausCount - 2],
													 fileInfo.dCorrs[5][fileInfo.iTausCount - 2],
													 fileInfo.dCorrs[6][fileInfo.iTausCount - 2],
													 fileInfo.dCorrs[7][fileInfo.iTausCount - 2]);
	printf("|-------------------.-------------------------------------------------------------------------------|\n");
	printf("|%1.0f|%1.0f|%1.0f|%1.0f|%1.0f|%1.0f|%1.0f|%1.0f|%1.0f|\n",
			fileInfo.dTaus[fileInfo.iTausCount - 1], fileInfo.dCorrs[0][fileInfo.iTausCount - 1],
													 fileInfo.dCorrs[1][fileInfo.iTausCount - 1],
													 fileInfo.dCorrs[2][fileInfo.iTausCount - 1],
													 fileInfo.dCorrs[3][fileInfo.iTausCount - 1],
													 fileInfo.dCorrs[4][fileInfo.iTausCount - 1],
													 fileInfo.dCorrs[5][fileInfo.iTausCount - 1],
													 fileInfo.dCorrs[6][fileInfo.iTausCount - 1],
													 fileInfo.dCorrs[7][fileInfo.iTausCount - 1]);
	printf("|==================================================================================================|\n");
} /* printFileReport */

/*======================================================================
   FUNCTION: printFileReport
   ROLE:     Auxiliary function for debuggin, which prints all the
			 data obtained from the minimization process
   PARAM:
   RETURN:   void
======================================================================*/
void printFinalReport(experiment allFiles)
{
	printf("Final Report\n");
}

/*======================================================================
   FUNCTION: getNumberOfFiles (unused)
   ROLE:     Return the number of files in the directory 'data'
   PARAM:
   RETURN:	 Number of files
   STATUS:   Unused
   TODO: Verify if this function doesn't affect the performance
======================================================================*/
/*int getNumberOfFiles()
{
	char * sResult = NULL;

	FILE * command = popen("ls ../data | wc -l", "r");
	getdelim(&sResult, NULL, '\0', command);
	pclose(command);
	printf("%s", sResult);

	return atoi(sResult);
}*/
