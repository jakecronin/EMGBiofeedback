#ifndef DEF_H
#define DEF_H

#define TRUE  1
#define FALSE 0

/* TODO: Get from LabVIEW */
//#define PREFIX_FILE_NAME "../data/NML29_112509_1_flow_"         /* Clean data (Linux) */
//#define PREFIX_FILE_NAME "../data/regular/STH04_121609_3_flow_" /* Regular data (Linux) */
//#define PREFIX_FILE_NAME "data\\clean\\NML29_112509_1_flow_"    /* Clean data (Windows) */
//#define PREFIX_FILE_NAME "data\\regular\\STH04_121609_3_flow_"    /* Regular data (Windows) */

//#define PREFIX_FILE_NAME "C:\\Data\\4_20100906_2\\tst4_flow_"

//#define PREFIX_FILE_NAME "C:\\Data\\regular\\STH04_121609_3_flow_"    /* Regular data (Windows) */
#define PREFIX_FILE_NAME "C:\\Data\\clean\\NML29_112509_1_flow_"    /* Clean data (Windows) - 8 detectors */

#define FILE_COUNT     122 /* Number to make memory expansion in the array where all files are uploaded */ /* TODO: Fix the realloc problem */
#define MAX_FIELD_LEN   25 /* Maximum string length for all values */
#define MAX_FILE_NAME   50 /* Maximum string length for a file name */
#define WAIT_TIME        6 /* Maximum time of wait for reading the file writen from LabVIEW */

#define CR 13 /* Decimal code of Carriage Return char */
#define LF 10 /* Decimal code of Line Feed char */
#define TB  9 /* Decimal code of Tab char */

#define LINES_UNUSED_HEADER 3 /* Header unused in the data file from line 4 to 6 */

#define DETEC_COUNT 8      /* TODO: Delete! */
#define MAX_DETEC   8      /* TODO: Get from LabVIEW */
#define MAX_AVG     4

#define MAX_ITERATIONS 100 /* Maximum number of iterations allowed in the minimization process */
#define SCALE 			 1 /* Used to define the initial points of the simplex */

/* Constants of g2 */
#define c      299792458	/* Light speed TODO: Confirm this value */
#define n0     1.4			/* Index of refraction */
#define lambda 0.0000785	/* Wave length */
#define PI     3.141592653589793

/* Effective reflection coefficient to account for the index
 * mismatch between tissue and air */
#define Reff   -1.440/((n0)*(n0)) + 0.710/(n0) + 0.668 + 0.0636 * (n0)
#define k0     2.0*PI * n0/lambda
#define v0     c/n0	/* Speed in the medium (tissue): c/n0 */

#define MAX_VAR 4 /* Maximum number of variables in the function: [Db, beta, muA, muS] */

/* Constants for the starting point, used in case they aren't specified */
#define DB	 0.00000001 /* 1e-8*/
#define BETA 0.5
#define MU_A 0.05
#define MU_S 10.0

#define INDEX_DB   0
#define INDEX_BETA 1
#define INDEX_MU_A 2
#define INDEX_MU_S 3

#define VAR_NAMES {"Db", "Beta", "Mua", "Mus\'"}

#define MAX_CREC 4 /* Number of additional vertex in the optimization process */

/* Used to access the array coordCREC of the simplex_t struct */
#define INDEX_CENTER  0
#define INDEX_REFLEC  1
#define INDEX_EXPANS  2
#define INDEX_CONTRAC 3

#define REFLEC_COEF  1.0  /* reflection coefficient  */
#define CONTRAC_COEF 0.5  /* contraction coefficient */
#define EXPANS_COEF  2.0  /* expansion coefficient   */

#define OUTSIDE_CONTRAC 0
#define INSIDE_CONTRAC  1

#define EPSILON 1.0e-9 /* Convergence criteria */

/* Type of approximation */
#define SI 0 /* Semi-Infinite Approximation */
#define SL 1
#define FC 2

/* Used for the file writing, after the fitting process */
#define MAX_VAR_HEADER 35
#define DETEC_LABEL "Detector "
#define DETEC_LABEL_LEN 10
#define MARGIN "          |"
#define BORDER "==========|"

typedef double value_t;

/* Define a type dcs_file to be a struct containing all the
   informations of the file with one result of the experiment */
typedef struct
{
	/* Info */
	int      iFileID;                  /* File's ID */
	char     sFileName[MAX_FILE_NAME]; /* Name of the file where the informatios were extracted */
	char     sDate[MAX_FIELD_LEN];     /* Date when the experiment was done - MM/DD/YYYY HH:MM:SS PM */
	char     sTime[MAX_FIELD_LEN];     /* Time when the experiment was done - HH:MM:SS PM */
	value_t  fDuration;			       /* Duration of the experiment, in miliseconds */
	int      iDetectorCount; 		   /* TODO: Put in the struct 'experiment'! - Number of detectors */

	/* Datas */
	value_t *  dTotalIntensities;      /* Array with the total intensities detected in each detector */
	int 	   iTausCountEstim;	       /* TODO: DELETE!!! */
	int 	   iTausCount;		       /* TODO: Put in the struct 'experiment'! - Number of time delays in the experiment */
	value_t *  dTaus;				       /* Value of each time delay */
	value_t ** dCorrs;			       /* Matrix Tau x Detector, with the values of g2 (correlation) */

	/* Answer */
	value_t ** bestFit;				   /* Best value for 'x_point' in each detector considered */
	int 	   success;			       /* TODO: DELETE!!! Indicate if the upload has been done successfully */
} dcs_file;

/* Group all informations about the experiment */
typedef struct
{
	dcs_file * files;
    //exp_info   info;          /* TODO: Use it! */
    int		   iFileCount;
    int        iDetectorCount;   /* Number of detectors (4 or 8) */
	int	  *	   iUsedDetectors;   /* Array with 0 (detector not used) or 1 (detector used) in each position */
	value_t *  dRhos;			 /* Distances between each detector and the sources */

	value_t	   startPt[MAX_VAR]; /* Starting point for the minimization process */
	int		   iFirstFitPt;		 /* First index of tau to be considered  */
	int		   iLastFitPt;       /* Last index of tau to be considered  */
	int		   approximation;	 /* Type of approximation */
} experiment;

/* Group all experiment's informations comom for all files */
typedef struct
{
	int		   iFileCount;
    int        iDetectorCount;   /* Number of detectors (4 or 8) */
    char       sFitFileName[MAX_FILE_NAME]; /* File with the result of the analysis */
	int	  *	   iUsedDetectors;   /* Array with 0 (detector not used) or 1 (detector used) in each position */
	value_t *  dRhos;			 /* Distances between each detector and the sources */

    value_t    dRhosAvg[MAX_AVG]; /* Rhos' values for averages */
    int        iUsedAvg[MAX_AVG]; /* Array with 0 (average not used) or 1 (average used) */

	value_t	   startPt[MAX_VAR]; /* Starting point for the minimization process */
	int		   iFirstFitPt;		 /* First index of tau to be considered  */
	int		   iLastFitPt;       /* Last index of tau to be considered  */
	int		   approximation;	 /* Type of approximation */
} exp_info;

/* Define a type 'position' to group all variables related to position (source and detector) */
typedef struct
{
	value_t lt,  /* scattering length*/
		    Ls,	/* distance above the turbid medium */
		    z,	/* distance perpendicular to the tissue boundary */
		    rho, /* distance between each detector and the source */
		    r1,
		    r2;
} position;

/* Group all data structures used in the minimization process */
typedef struct
{
	/* Function's dimension, that depends on how much information was
	 * set in the struct of the starting point */
	int n;

	int smallestID; /* vs - Vertex with smallest value */
	int middleID;   /* vh - Vertex with next smallest value */
	int largestID;  /* vg - Vertex with largest value */

	value_t ** coordsVertex; /* v - Coordinates of the (n + 1) vertex */
	value_t * fValueVertex;  /* f - Value of function at each vertex */

	value_t ** coordsCREC; /* Coordinates of the Center, Reflection, Expansion and Contraction vertex */
	value_t fValueCREC[MAX_CREC]; /* Value of function at Reflection, Expansion and Contraction */
} simplex_t;

#endif // DEF_H
