/*
 * read.c
 *
 *  Created on: Jul 9, 2010
 *      Author: Rodrigo Barbosa
 */

#include "read.h"

/*======================================================================
   FUNCTION: uploadFileToMemory
   ROLE:     Read the file and upload his content to the string
   PARAM:    sFileName - Name of the file to be uploaded
			 sFileContent - Dynamically allocated buffer (entire file)
   RETURN:   -1 - File doesn't exist
			 -2 - Insufficient memory to read file
			  1 - The file has been uploaded succesfully
======================================================================*/
int uploadFileToMemory(char * sFileName, char ** sFileContent)
{
	FILE * inputFile;
	long   lFileLength;

	if((inputFile = fopen(sFileName, "r")) == NULL)
	{
		/* printf("ERROR: The file \'%s\' doesn't exist!\n", sFileName); */
		return -1;
	}

	fseek(inputFile, 0L, SEEK_END); /* Position to end of file */
	lFileLength = ftell(inputFile); /* Get file length 		   */
	rewind(inputFile);              /* Back to start of file   */

	/* Memory allocation to read the entire file */
    //* sFileContent = calloc(lFileLength + 1, sizeof(char));
    * sFileContent = (char *)calloc(lFileLength + 1, sizeof(char));

	if(* sFileContent == NULL)
	{
    	/* printf("ERROR: Insufficient memory to read file.\n"); */
    	return -2;
	}

	/* Read the entire file into sFileContent */
	fread(* sFileContent, lFileLength, 1, inputFile);

    fclose(inputFile);

	return 1; /* Success */
}

/*======================================================================
   FUNCTION: initFileAttributs
   ROLE:     Initialize the attributs of the struct result, by
			 allocating memory to the pointers
   PARAM:    result - struct that will have your pointers with
			 		  memory allocated
			 cFileContent - string with the content of the file,
			 				used to estimate the number of rows
							in the matrix
   RETURN:   void
======================================================================*/
void initFileAttributs(dcs_file * result, char * sFileContent,
                       int iDetectorCount)
{
	int iIndex; /* Used in the memory allocation of the matrix */

	/* Memory allocation to the array of total intensities
	   detected in each detector */
	result->dTotalIntensities =
		(value_t *)malloc(sizeof(value_t) * result->iDetectorCount);

	/* Estimate the number of taus values (rows number) */
	/* TODO: Ensure that it's a good estimate */
	if(iDetectorCount == 8)
		result->iTausCountEstim = strlen(sFileContent)/100;
    else
        result->iTausCountEstim = 600;

	/* Memory allocation to the array of Taus */
	result->dTaus =
		(value_t *)malloc(sizeof(value_t) * result->iTausCountEstim);

	/* Memory allocation to the matrix (rows - detectors) of g2 values */
	result->dCorrs =
		(value_t **)malloc(sizeof(value_t *) * result->iDetectorCount);

	/* Memory allocation to the matrix (columns - taus) of g2 values */
	for(iIndex = 0; iIndex < result->iDetectorCount; iIndex++)
		result->dCorrs[iIndex] =
			(value_t *)malloc(sizeof(value_t) * result->iTausCountEstim);

	/* Memory allocation to the matrix (rows) of g2 values */
	//result->dCorrs =
	//	(value_t **)malloc(sizeof(value_t *) * result->iTausCountEstim);
	/* Memory allocation to the matrix (columns) of g2 values */
	//for(iIndex = 0; iIndex < result->iTausCountEstim; iIndex++)
	//	result->dCorrs[iIndex] =
	//		(value_t *)malloc(sizeof(value_t) * result->iDetectorCount);
}

/*======================================================================
   FUNCTION: skipLine
   ROLE:     Considering the current line of the character, skip the
   	   	   	 number of lines specified
   PARAM:	 cCurrentChar - Pointer to the current character
   	   	     iNumberLines  - Number of lines to be skipped
   RETURN:	 void
======================================================================*/
void skipLine(char ** cCurrentChar, int iNumberLines)
{
	int iCountLine = 0;

	/* If it's already in the end of the line */
	if(** cCurrentChar == CR || ** cCurrentChar == LF)
		while(** cCurrentChar == CR || ** cCurrentChar == LF)
			(* cCurrentChar)++; /* Position to next character */

	while(iCountLine < iNumberLines && ** cCurrentChar != '\0')
	{
		while(** cCurrentChar != CR && ** cCurrentChar != LF)
			(* cCurrentChar)++;

		if(** cCurrentChar == CR || ** cCurrentChar == LF)
			while(** cCurrentChar == CR || ** cCurrentChar == LF)
				(* cCurrentChar)++; /* Position to next character */

		iCountLine++; /* Next line */
	}
}

/*======================================================================
   FUNCTION: getStringValue
   ROLE:     Read from the file one field that ends with TAB or in
			 the end of the line
   PARAM:    cCurrentChar - Pointer to the current character
			 sValue - result
			 iNext - Distance from the next field, if applicable
   RETURN:   void
======================================================================*/
void getStringValue(char ** cCurrentChar, char * sValue, int iNext)
{
	int iIndex = 0;

	/* Read until reaching end of the field */
	while(** cCurrentChar != TB && ** cCurrentChar != '\n' && ** cCurrentChar != '\0')
	{
		sValue[iIndex++] = (** cCurrentChar); /* Get the character */
		(* cCurrentChar)++; /* Position to next character */
	}

	sValue[iIndex] = '\0';
	/* Position to the beginning of the next field */
	(* cCurrentChar) += iNext;
}

/*======================================================================
   FUNCTION: getDoubleValue
   ROLE:     Read from the file one field that ends with TAB or in
			 the end of the line, converting the result to value_t
   PARAM:    cCurrentChar - Pointer to the current character
			 fValue - result
			 iNext - Distance from the next field, if applicable
   RETURN:   void
======================================================================*/
void getDoubleValue(char ** cCurrentChar, value_t * dValue, int iNext)
{
	char sValue[MAX_FIELD_LEN];

	getStringValue(cCurrentChar, sValue, iNext);

	* dValue = atof(sValue);
}

/*======================================================================
   FUNCTION: getRestOfLine
   ROLE:     Read the rest of the line, from the current char, storing
			 the result in the array.
   PARAM:    cCurrentChar - Pointer to the current character
			 fArray - array where the result will be stored, which can
					  be either the array of total intensities, or one
					  line of the matrix of correlations
             info - where is the number of detectors
   RETURN:   void
======================================================================*/
void getRestOfLine(char ** cCurrentChar, value_t * fArray,
				   value_t ** fMatrix, int iIndexTaus, exp_info info)
{
	//int iIndexDetector = 0;
	int iIndexDet = 0;

    iIndexDet = 0;
	/* Read until reaching end of the line */
	//while(** cCurrentChar != CR && ** cCurrentChar != LF)
	while(** cCurrentChar != '\n' && iIndexDet < info.iDetectorCount)
	{
		if(iIndexTaus >= 0) /* Reading taus and correlations */
			getDoubleValue(cCurrentChar,
						   &(fMatrix[iIndexDet++][iIndexTaus]), 0);
		else /* Reading total intensities */
			getDoubleValue(cCurrentChar,
						   &(fArray[iIndexDet++]), 0);

        if(** cCurrentChar != '\n')
                (* cCurrentChar) += 1;
	}
}

/*======================================================================
   FUNCTION: copyCorrsLine
   ROLE:     Make a copy of one column of the matrix 'from' in the
             matrix 'to'
   PARAM:    iTau - Number of the column that will be copied
             to - matrix destination
             from - matrix source
             avg - array that states which detector is being considered
                   in each average
             info - where is the number of detectors
   RETURN:   void
======================================================================*/
void copyCorrsLine(int iTau, value_t ** to, value_t ** from, int * avg, exp_info info)
{
    int iIndexDetector,
        iIndexAverage;
    value_t sum,
            count;

    for(iIndexAverage = 0; iIndexAverage < MAX_AVG; iIndexAverage++)
    {
        sum   = 0.0;
        count = 0.0;

        for(iIndexDetector = 0; iIndexDetector < info.iDetectorCount; iIndexDetector++)
        {
            if(avg[iIndexAverage * info.iDetectorCount + iIndexDetector])
            {
                sum += from[iIndexDetector][iTau];
                count++;
            }
        }

        if(count > 0.0)
            sum = sum/count;

        to[iIndexAverage][iTau] = sum;
    }
}

/*======================================================================
   FUNCTION: getTausAndCorrelations
   ROLE:     Read the rest of the file, where the first column contains
		     the delay times (taus) and the others contain the g2 values
		     (correlations) for each delay time in each detector
   PARAM:    result - struct which contains the array of taus and the
					  matrix of correlations
             averages -
             avg - array that states which detector is being considered
                   in each average
             info - experiment's auxiliary informations
   RETURN:   void
======================================================================*/
void getTausAndCorrelations(char ** cCurrentChar, dcs_file * result,
                            dcs_file * averages, int * avg, exp_info info)
{
	int iIndexTaus = 0; /* Used to index the rows */

	/* Read until reaching null char, the end of the file */
	while(** cCurrentChar)
	{
		/* Read the delay time (tau) of the current line */
		getDoubleValue(cCurrentChar, &(result->dTaus[iIndexTaus]), 1);
		averages->dTaus[iIndexTaus] = result->dTaus[iIndexTaus];

		/* printf("| %.8f |", result->dTaus[iIndexTaus]); */

		/* Read the rest of the line in the array of correlations (g2) */
		getRestOfLine(cCurrentChar, NULL, result->dCorrs, iIndexTaus, info);
        copyCorrsLine(iIndexTaus, averages->dCorrs, result->dCorrs, avg, info);

		iIndexTaus++;
		(* cCurrentChar)++;
	} /* while current char isn't null */

	/* Update the exact number of taus */
	result->iTausCount   = iIndexTaus;
	averages->iTausCount = iIndexTaus;
} /* getTausAndCorrelations */

/*======================================================================
   FUNCTION: readDataFile
   ROLE:     Read one input file with the results of the experiment
   PARAM:    cFileName - name of the file to be read
             averages - Where the averages specified by the user will
                        be stored
             avg - array that states which detector is being considered
                   in each average
   RETURN:   result - struct with all the file's information, which can
					  be verified from the function 'printFileReport'
======================================================================*/
dcs_file readDataFile(char * sFileName, dcs_file * averages, int * avg, exp_info info)
{
	/* Content of the file after has been brought to memory */
	char * sFileContent;
	/* Pointer to current position (character) in cFile */
	char * cCurrentChar;
	/* Struct where the file's informations will be uploaded */
	dcs_file result;

	/* Read the entire file into a string */
	if((result.success = uploadFileToMemory(sFileName, &sFileContent)) == 1)
	{
		/* 1. Reading the DATE of the experiment ==================== */

		/* Point to beginning of array, after the '%' */
		cCurrentChar = sFileContent + 1;

		/* Read date from the file and skip three lines (52 chars) */
		getStringValue(&cCurrentChar, result.sDate, 1);
		getStringValue(&cCurrentChar, result.sTime, 1);
		strcpy(averages->sDate, result.sDate);
		strcpy(averages->sTime, result.sTime);

		skipLine(&cCurrentChar, 1); /* Skip 1 lines - Line 2 of the data file */

		/* 2. Reading the NUMBER OF DETECTORS used ================== */

		/* Assume that there are 8 detectors */
		/* TODO: read the number of detector in the second line or get from LabVIEW */
		result.iDetectorCount = info.iDetectorCount;
		averages->iDetectorCount = MAX_AVG;

		/* Initialize the attributs of the struct result */
		initFileAttributs(&result, sFileContent, info.iDetectorCount);
        initFileAttributs(averages, sFileContent, info.iDetectorCount);

		/* 3. Reading the DURATION of the experiment ================ */

		/* Read from the file the duration of the experiment */
		getDoubleValue(&cCurrentChar, &result.fDuration, 1);
        averages->fDuration = result.fDuration;

		/* 4. Reading the TOTAL INTENSITIES detected ================ */

		/* Read the rest of the line in the array of total intensities */
		getRestOfLine(&cCurrentChar, result.dTotalIntensities, NULL, -1, info);

		/* 5. Reading TAU and CORRELATIONS (g2) values ============== */

		// cCurrentChar += 174;
		skipLine(&cCurrentChar, LINES_UNUSED_HEADER); /* Skip 3 lines */

		/* Read the rest of the file, with the delay times and g2 values */
		getTausAndCorrelations(&cCurrentChar, &result, averages, avg, info);

		free(sFileContent);
		strcpy(result.sFileName, sFileName);
		strcpy(averages->sFileName, sFileName);
	} /* If the file was successfully read */

	return result;
} /* readDataFile */

/*======================================================================
   FUNCTION: getFileName
   ROLE:     From the file number, this function returns its name
   PARAM:	 iFileNumber - Number of the file
   RETURN:   sFileName - String with the file name in the correct format
======================================================================*/
void getFileName(int iFileNumber, char * sFileName)
{
	char sAux[MAX_FILE_NAME];

	/* Put the prefix into file's name */
	strcpy(sFileName, PREFIX_FILE_NAME);

	/* Add the file number to its name */
	sprintf(sAux, "%d", iFileNumber);
	strcat(sFileName, sAux);

	/* Add the file extension to its name */
	strcat(sFileName, ".dat");
}

/*======================================================================
   FUNCTION: readAllDataFile
   ROLE:     Basically, call the function readDataFiles for all files
   	   	     written during the experiment, until a maximum number
   	   	     predefined
   PARAM:
   RETURN:   result[] - array of struct with all files' informations
======================================================================*/
experiment readAllDataFiles(dcs_file * averages, int * avg, exp_info info)
{
	/* Array where all files' informations will be uploaded */
	experiment result;
	/* Control the end of the process */
	int bThereIsFile = 1,
	/* Control the resizing of the array */
		iReallocsCount = 0;
	/* Construct file's name */
	char sFileName[MAX_FILE_NAME];

	/* Control the index of the array */
	result.iFileCount = 0;

	/* Memory allocation to the result array */
	result.files = (dcs_file *)malloc(sizeof(dcs_file) * FILE_COUNT);

	/* Repeat until reading all the files */
	while(bThereIsFile)
	{
		/* Get the name of the file from its number */
		getFileName(result.iFileCount, sFileName);
		/* Get all the informations of the file */
		result.files[result.iFileCount] = readDataFile(sFileName, averages, avg, info);

		/* If the file could be read */
		if(result.files[result.iFileCount].success)
		{
			result.iFileCount++;

			/* If the array need to be resized */
			if(result.iFileCount == FILE_COUNT * iReallocsCount + FILE_COUNT)
			{
				/* Add FILE_COUNT positions to the array */
				result.files = realloc(result.files, sizeof(result.files) + FILE_COUNT);
				iReallocsCount++;
			}
		}
		else
			bThereIsFile = 0;
	}

	/* If no file was read */
	if(result.iFileCount == 0)
	{
		free(result.files);
		result.files = NULL;
	}

	return result;
} /* readAllDataFile */

/*======================================================================
   FUNCTION: readNextFile
   ROLE:     Read the datas from a file. If the file isn't already done,
             wait for that.
   PARAM:    bThereIsFile - If the time of waiting for the file writing
             is over, it returns that there is no more file to be read
             iFileName - File's path
             averages - Where the averages specified by the user will
                        be stored
             avg - array that states which detector is being considered
                   in each average
             info - auxiliary informations for reading
   RETURN:   1 - Reading completed succesfully
             0 - There is no more files to be read
======================================================================*/
dcs_file readNextFile(int * bThereIsFile, char * sFileName,
                      dcs_file * averages, int * avg, exp_info info)
{
    dcs_file result;
	//char sFileName[MAX_FILE_NAME]; /* Construct file's name */

    time_t start,
           stop;

	/* Get the name of the file from its number */
    //getFileName(iIndexFile, sFileName);
    /* Save the file's ID */
    //result.iFileID = iIndexFile;

    /* During the reading, if the elapsed time is greater then WAIT_TIME,
     * the algorithm considers the process as done */

    time(&start);

    do
    {
        /* Get all the informations of the file */
        result = readDataFile(sFileName, averages, avg, info);

        if(result.success != 1) /* If it failed */
        {
            // TODO: sleep for a while, avoiding processing

            time(&stop); /* Check the time */

            if((stop - start) > WAIT_TIME) /* If the time is over */
                * bThereIsFile = 0;
        }
    }
    while(result.success != 1 && * bThereIsFile);

	return result;
} /* readNextFile */
