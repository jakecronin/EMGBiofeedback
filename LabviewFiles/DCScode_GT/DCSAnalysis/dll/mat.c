/*
 * mat.c
 *
 *  Created on: Jul 15, 2010
 *      Author: Rodrigo Barbosa
 */

#include <stdio.h>
#include "mat.h"

/*======================================================================
   FUNCTION: initSimplexVertexes
   ROLE:     If not all are variables of the starting point are defined,
    		 set their definitions
   PARAM:	 x - starting point
   RETURN:	 1 - success
   	   	   	 0 - fail
======================================================================*/
void initSimplexVertexes(simplex_t * simplex, value_t * startPt, value_t * var)
{
	/* Auxiliary variables */
	int iIndexVertex;
							/* P1     P2     P3     P4     P5 */
	value_t fDbValues[]   = {var[0],  1.0e-5, 1.0e-6, 1.0e-7, 1.0e-9};
	value_t fBetaValues[] = {var[1],  0.1,   0.8,   0.7,   0.3};
	value_t fMuAValues[]  = {var[2],  0.9,   0.5,   0.7,   0.2};
	value_t fMuSValues[]  = {var[3],  1.0,  30.0,   5.0,  15.0};

	for(iIndexVertex = 0; iIndexVertex <= simplex->n; iIndexVertex++)
	{
		simplex->coordsVertex[iIndexVertex][INDEX_DB] = fDbValues[iIndexVertex];

		if(startPt[INDEX_BETA] == 1) /* If Beta is a variable */
			simplex->coordsVertex[iIndexVertex][INDEX_BETA] = fBetaValues[iIndexVertex];
		else /* If it's a constant */
			simplex->coordsVertex[iIndexVertex][INDEX_BETA] = var[1];

		if(startPt[INDEX_MU_A] == 1) /* If muA is a variable */
			simplex->coordsVertex[iIndexVertex][INDEX_MU_A] = fMuAValues[iIndexVertex];
		else
			simplex->coordsVertex[iIndexVertex][INDEX_MU_A] = var[2];

		if(startPt[INDEX_MU_S] == 1) /* If muS is a variable */
			simplex->coordsVertex[iIndexVertex][INDEX_MU_S] = fMuSValues[iIndexVertex];
		else
			simplex->coordsVertex[iIndexVertex][INDEX_MU_S] = var[3];
	}
}

/*======================================================================
   FUNCTION: setPositionVariables
   ROLE:     Set values to the variables related to the position r,
   	   	     before calculating r1 and r2
   PARAM:	 r - struct of position
   RETURN:	 void
======================================================================*/
void setPositionVariables(position * r)
{
	/* TODO: Consider when the user furnish a value to muS, that is
	 * 		 different from the constant MU_S */
	r->lt = 1.0 / MU_S; /* scattering length */
	r->Ls = (2.0/3.0) * ((1.0 + (value_t)Reff)/(1.0 - ((value_t)Reff))) * r->lt;
	r->z  = 0;
}

/*======================================================================
   FUNCTION: absf
   ROLE:     Return the absolute value of the number
   PARAM:	 number - number whose absolute value is asked
   RETURN:	 absolute value
======================================================================*/
value_t absf(value_t number)
{
	if(number >= 0)
		return number;

	return -number;
}

/*======================================================================
   FUNCTION: g2SemiInfinite
   ROLE:	 Computes an estimation for g2, from all the variables and
   	   	     constants that were set
   PARAM:	 x - point where the function will be evaluated
   	   	   	 r - position variables used in the function as coefficients
   	   	   	 dTaus - time delays
   	   	   	 g2measured - values of g2 obtained from the experiment
   	   	   	 iNumberFitPts - number of taus (and g2 values) to be
   	   	   	 	 	 	 	 considered
   RETURN:	 g2est - value for the estimation function at the point x
======================================================================*/
value_t g2SemiInfinite(value_t * x, position * r,
				       value_t * dTaus, value_t * g2measured,
				       int iFirstFitPt, int iLastFitPt)
{
	int		  iNumberFitPts = iLastFitPt - iFirstFitPt + 1;
	value_t * g2estimated = (value_t *)malloc(sizeof(value_t) * iNumberFitPts);
	value_t * g1 = (value_t *)malloc(sizeof(value_t) * iNumberFitPts);
	value_t * G1 = (value_t *)malloc(sizeof(value_t) * iNumberFitPts);
	value_t * K  = (value_t *)malloc(sizeof(value_t) * iNumberFitPts);
	value_t	  Ko,
			  sum = 0.0;
	int 	  index;

	/* Positions variables */
	r->r1 = sqrt(r->rho*r->rho + ((r->z - r->lt) * (r->z - r->lt)));
	r->r2 = sqrt(r->rho*r->rho + (r->z + r->lt + 2.0*r->Ls) * (r->z + r->lt + 2.0*r->Ls));

	Ko = sqrt(3 * x[INDEX_MU_A] * x[INDEX_MU_S]);

	// TODO: Delete debugging
	//printf("r1: %f, r2: %f, Ko: %f\n", r->r1, r->r2, Ko);

	for(index = iFirstFitPt; index < iLastFitPt; index++)
	{
		K[index - iFirstFitPt]  = sqrt(3.0 * x[INDEX_MU_A] * x[INDEX_MU_S] + 6.0 * k0 * k0 * x[INDEX_MU_S] * x[INDEX_MU_S] * x[INDEX_DB] * dTaus[index]);

		G1[index - iFirstFitPt] = 3.0 * x[INDEX_MU_S]/(4.0*PI) * (exp(-(r->r1) * K[index - iFirstFitPt])/r->r1 - exp(-(r->r2) * K[index - iFirstFitPt])/r->r2);

		g1[index - iFirstFitPt] = G1[index - iFirstFitPt]/(3.0 * x[INDEX_MU_S]/(4.0*PI) * (exp(-(r->r1) * Ko)/r->r1 - exp(-(r->r2)*Ko)/r->r2));

		g2estimated[index - iFirstFitPt] = 1.0 + x[INDEX_BETA] * absf(g1[index - iFirstFitPt] * g1[index - iFirstFitPt]);

		sum += pow(g2estimated[index - iFirstFitPt] - g2measured[index], 2.0) / 2.0;

		// TODO: Delete debugging
		/*printf("%3d) K:%2.8f, G1:%.8f, g1:%.8f, g2e:%.8f, g2m:%.8f - s:%.8f\n",
			   index,
			   K[index - iFirstFitPt],
			   G1[index - iFirstFitPt],
			   g1[index - iFirstFitPt],
			   g2estimated[index - iFirstFitPt],
			   g2measured[index],
			   sum);*/
	}

	free(g2estimated);
	free(g1);
	free(G1);
	free(K);

	/* F = sum ((g2estimated - g2measured).^2 / 2 ) */
	return sum;
}

/*======================================================================
FUNCTION: evaluateFuctionAtX
ROLE:     Function to be minimized, that is the total sum of all
	   	  square of the difference between the g2 estimated (g2est)
	   	  and the g2 measured in the experiment, for each delay
	   	  (tau), i.e. f = sum((g2(tau) - g2'(tau))^2)
PARAM:    x - value for the variables of the function.
	   	      x[0] = Db, blood flow
	   	      x[1] = beta
	   	      x[2] = muA (not for now)
	   	      x[3] = muS (not for now)
	   	  paramCount - number of used positions in the 'x' array
RETURN:   value of the function evaluated in the point 'x'
======================================================================*/
value_t evaluateFunctionAtX(value_t * x, position * r, int approximation,
						    value_t * dTaus, value_t * g2measured,
						    int iFirstFitPt, int iLastFitPt)
{
	switch(approximation)
	{
		case SI: return g2SemiInfinite(x, r, dTaus, g2measured,
									   iFirstFitPt, iLastFitPt);
		case SL: printf("SlabSol Approximation. Not implemented yet.\n");
				 break; /* TODO: not for now */

		case FC: printf("FinCylSol Approximation. Not implemented yet.\n");
				 break; /* TODO: not for now */
	}

	return -1;
}

/*======================================================================
   FUNCTION: initMinimizationStructs
   ROLE:   	 Memory allocation to the structures used in the
   	   	     minimization process
   PARAM:	 simplex - struct with the data structures to which memory
   	   	   	   	   	   will be allocated
   	   	   	 n - dimension of the simplex, i.e. of the function
   RETURN:	 void
======================================================================*/
void initMinimizationStructs(simplex_t * simplex, int n)
{
	int index;

	/* Memory to the vertex coordinates. Each row represents one vertex */
	simplex->coordsVertex = (value_t **)malloc((n + 1) * sizeof(value_t *));

	/* TODO: Verify if this statement can be done in the declaration,
	 * 		 by doing value_t * coordVertex[MAX_VAR] */
	/* Memory to the vertex coordinates. Each column represents one variable */
	for(index = 0; index <= n; index++)
		simplex->coordsVertex[index] = (value_t *)malloc(MAX_VAR * sizeof(value_t));

	/* Memory to the array with the function evaluation in each vertex */
	simplex->fValueVertex = (value_t *) malloc ((n + 1) * sizeof(value_t));

	/* Memory to the Center, Reflection, Expansion and Contraction vertex
	 * Each row represents one vertex */
	simplex->coordsCREC = (value_t **)malloc(MAX_CREC * sizeof(value_t *));

	/* TODO: Verify if this statement can be done in the declaration,
	 * 		 by doing value_t * coordVertex[MAX_VAR] */
	/* Memory to the vertex coordinates. Each column represents one variable */
	for(index = 0; index < MAX_CREC; index++)
		simplex->coordsCREC[index] = (value_t *)malloc(MAX_VAR * sizeof(value_t));

	/* Save the simplex dimension */
	simplex->n = n;
} /* initMinimizationStructs */

/*======================================================================
   FUNCTION: createInitialSimplex
   ROLE:     Define the initial points and evaluate the function at
   	   	     these points
   PARAM:	 simplex - struct with the data structures that will be their
   	   	   	   	   	   values set
   	   	   	 The other parameters were passed from 'minimize' just for
   	   	   	 became possible the function evaluation here
   RETURN:	 void
======================================================================*/
void evaluateInitialSimplex(simplex_t * simplex, dcs_file * file,
						  int iIndexDetector, int iFirstFitPt, int iLastFitPt,
						  int iApproximation, position * r)
{
	/* Auxiliary variables */
		int iIndexVertex;

	/* find the initial function values */
	for (iIndexVertex = 0; iIndexVertex <= simplex->n; iIndexVertex++)
	{
		simplex->fValueVertex[iIndexVertex] =
				evaluateFunctionAtX(simplex->coordsVertex[iIndexVertex],
									r, iApproximation, file->dTaus,
									file->dCorrs[iIndexDetector],
									iFirstFitPt, iLastFitPt);
	}

	/* Debugging: Print out the initial values */
	/*printf("Initial Values\n");
	for(iIndexVertex = 0; iIndexVertex <= simplex->n; iIndexVertex++)
		printf("%.9f, %f, %f, %f - %f\n", simplex->coordsVertex[iIndexVertex][INDEX_DB],
										simplex->coordsVertex[iIndexVertex][INDEX_BETA],
										simplex->coordsVertex[iIndexVertex][INDEX_MU_A],
										simplex->coordsVertex[iIndexVertex][INDEX_MU_S],
										simplex->fValueVertex[iIndexVertex]);*/
} /* createInitialSimplex */

/*======================================================================
   FUNCTION: pointsSort (unused)
   ROLE:     Sort points (vertex) in ascending order
   PARAM:	 simplex - struct where are the variables that control the
   	   	   	   	   	   vertex
   RETURN:	 void
======================================================================*/
void pointsSort(simplex_t * simplex)
{
	int iIndexVertex,
		iIndexFast,
		iAuxIndex;
	value_t auxVertex[MAX_VAR],
		    auxValue;

	/* Sorting loops */
	for(iIndexVertex = 0; iIndexVertex <= simplex->n; iIndexVertex++)
	{
		for(iIndexFast = iIndexVertex + 1; iIndexFast <= simplex->n; iIndexFast++)
		{
			/* If it isn't in ascending order, change vertex */
			if(simplex->fValueVertex[iIndexVertex] > simplex->fValueVertex[iIndexFast])
			{
				/* Aux = IndexVertex */
				for(iAuxIndex = 0; iAuxIndex < MAX_VAR; iAuxIndex++)
					auxVertex[iAuxIndex] = simplex->coordsVertex[iIndexVertex][iAuxIndex];
				auxValue = simplex->fValueVertex[iIndexVertex];

				/* IndexVertex = IndexFast */
				for(iAuxIndex = 0; iAuxIndex < MAX_VAR; iAuxIndex++)
					simplex->coordsVertex[iIndexVertex][iAuxIndex] = simplex->coordsVertex[iIndexFast][iAuxIndex];
				simplex->fValueVertex[iIndexVertex] = simplex->fValueVertex[iIndexFast];

				/* IndexFast = Aux */
				for(iAuxIndex = 0; iAuxIndex < MAX_VAR; iAuxIndex++)
					simplex->coordsVertex[iIndexFast][iAuxIndex] = auxVertex[iAuxIndex];
				simplex->fValueVertex[iIndexFast] = auxValue;

			}
		}
	}
/*
	printf("Sorted: \n");
	for(iIndexVertex = 0; iIndexVertex <= simplex->n; iIndexVertex++)
		printf("%.12f, %f, %f, %f - %f\n", simplex->coordsVertex[iIndexVertex][INDEX_DB],
										  simplex->coordsVertex[iIndexVertex][INDEX_BETA],
										  simplex->coordsVertex[iIndexVertex][INDEX_MU_A],
										  simplex->coordsVertex[iIndexVertex][INDEX_MU_S],
										  simplex->fValueVertex[iIndexVertex]);*/
} /* pointsSort */

/*======================================================================
   FUNCTION: getSmallestMiddleHighest
   ROLE:	 Define the vertex with the smallest, intermediate and
   	   	     highest function evaluation
   PARAM:	 simplex - struct where are the variables that control the
   	   	   	   	   	   vertex
   RETURN:	 void
======================================================================*/
void getSmallestMiddleHighest(simplex_t * simplex)
{
	int iIndexVertex,
		largestID  = 0,
		middleID,
		smallestID = 0;

	for(iIndexVertex = 1; iIndexVertex <= simplex->n; iIndexVertex++)
		if(simplex->fValueVertex[iIndexVertex] > simplex->fValueVertex[largestID])
			largestID = iIndexVertex;

	simplex->largestID = largestID;

	for(iIndexVertex = 1; iIndexVertex <= simplex->n; iIndexVertex++)
		if(simplex->fValueVertex[iIndexVertex] < simplex->fValueVertex[smallestID])
			smallestID = iIndexVertex;

	simplex->smallestID = smallestID;

	middleID = smallestID;

	for(iIndexVertex = 0; iIndexVertex <= simplex->n; iIndexVertex++)
			if(simplex->fValueVertex[iIndexVertex] > simplex->fValueVertex[smallestID] &&
			   simplex->fValueVertex[iIndexVertex] < simplex->fValueVertex[largestID])
				middleID = iIndexVertex;

	simplex->middleID = middleID;

	/* State that for now neither one expansion nor one contraction
	 * has been done */
	simplex->fValueCREC[INDEX_EXPANS]  = -1;
	simplex->fValueCREC[INDEX_CONTRAC] = -1;
/*
	printf("** Sorted: \n");
	printf("%.12f, %f, %f, %f - %f\n", simplex->coordsVertex[smallestID][INDEX_DB],
	 								   simplex->coordsVertex[smallestID][INDEX_BETA],
									   simplex->coordsVertex[smallestID][INDEX_MU_A],
									   simplex->coordsVertex[smallestID][INDEX_MU_S],
									   simplex->fValueVertex[smallestID]);
	printf("%.12f, %f, %f, %f - %f\n", simplex->coordsVertex[middleID][INDEX_DB],
		 							   simplex->coordsVertex[middleID][INDEX_BETA],
									   simplex->coordsVertex[middleID][INDEX_MU_A],
									   simplex->coordsVertex[middleID][INDEX_MU_S],
									   simplex->fValueVertex[middleID]);
	printf("%.12f, %f, %f, %f - %f\n", simplex->coordsVertex[largestID][INDEX_DB],
			 						   simplex->coordsVertex[largestID][INDEX_BETA],
									   simplex->coordsVertex[largestID][INDEX_MU_A],
									   simplex->coordsVertex[largestID][INDEX_MU_S],
									   simplex->fValueVertex[largestID]);*/
}

/*======================================================================
   FUNCTION: pointsSort
   ROLE:     Calculate the center of mass of all vertex
   PARAM:	 simplex - struct where are the variables that control the
   	   	   	   	   	   vertex
   	   	   	 The other parameters were passed from 'minimize' just for
   	   	   	 became possible the function evaluation here
   RETURN:	 void
======================================================================*/
void getCenterOfMass(simplex_t * simplex, dcs_file * file,
		  	  	  	 int iIndexDetector, int iFirstFitPt, int iLastFitPt,
		  	  	  	 int iApproximation, position * r)
{
	int iIndexVertex,
		iIndexVar;

	for(iIndexVar = 0; iIndexVar < simplex->n; iIndexVar++)
	{
		simplex->coordsCREC[INDEX_CENTER][iIndexVar] = 0.0;

		for(iIndexVertex = 0; iIndexVertex <= simplex->n; iIndexVertex++)
			/* The largest vertex isn't considered */
			if(iIndexVertex != simplex->largestID)
				simplex->coordsCREC[INDEX_CENTER][iIndexVar] +=
				 simplex->coordsVertex[iIndexVertex][iIndexVar];

		simplex->coordsCREC[INDEX_CENTER][iIndexVar] =
		 simplex->coordsCREC[INDEX_CENTER][iIndexVar]/simplex->n;

		if(simplex->coordsCREC[INDEX_CENTER][iIndexVar] < 0)
			simplex->coordsCREC[INDEX_CENTER][iIndexVar] = 0.0;
	}

	/* The variables that are not being considered are only copied */
	for(; iIndexVar < MAX_VAR; iIndexVar++)
		simplex->coordsCREC[INDEX_CENTER][iIndexVar] =
			simplex->coordsVertex[simplex->smallestID][iIndexVar];

	/* Function evaluation at the center */
	simplex->fValueCREC[INDEX_CENTER] =
		evaluateFunctionAtX(simplex->coordsCREC[INDEX_CENTER],
							r, iApproximation, file->dTaus,
							file->dCorrs[iIndexDetector],
							iFirstFitPt, iLastFitPt);
/*
	printf("C: %.12f, %f, %f, %f - %f\n", simplex->coordsCREC[INDEX_CENTER][INDEX_DB],
									     simplex->coordsCREC[INDEX_CENTER][INDEX_BETA],
									     simplex->coordsCREC[INDEX_CENTER][INDEX_MU_A],
									     simplex->coordsCREC[INDEX_CENTER][INDEX_MU_S],
									     simplex->fValueCREC[INDEX_CENTER]);*/
} /* getCenterOfMass */

/*======================================================================
   FUNCTION: getReflectionPoint
   ROLE:	 Compute the reflection point of the largest vertex in
   	   	     relation to the center
   PARAM:	 simplex - struct where are the variables that control the
   	   	   	   	   	   vertex
   	   	   	 The other parameters were passed from 'minimize' just for
   	   	   	 became possible the function evaluation here
   RETURN:	 void
======================================================================*/
void getReflectionPoint(simplex_t * simplex, dcs_file * file,
	  	  	 	 	 	int iIndexDetector, int iFirstFitPt, int iLastFitPt,
	  	  	 	 	 	int iApproximation, position * r)
{
	int iIndexVar;

	/* Reflection: Pr = Pc + f*(Pc - Ph), where:
	 * Pr: reflection point, Pc: center, Ph: highest vertex, f: factor */
	for(iIndexVar = 0; iIndexVar < simplex->n; iIndexVar++)
	{
		simplex->coordsCREC[INDEX_REFLEC][iIndexVar] =
			simplex->coordsCREC[INDEX_CENTER][iIndexVar] + REFLEC_COEF *
			(simplex->coordsCREC[INDEX_CENTER][iIndexVar] -
			 simplex->coordsVertex[simplex->largestID][iIndexVar]);

		if(simplex->coordsCREC[INDEX_REFLEC][iIndexVar] < 0)
			simplex->coordsCREC[INDEX_REFLEC][iIndexVar] = 0.0;
	}

	/* The variables that are not being considered are only copied */
	for(; iIndexVar < MAX_VAR; iIndexVar++)
		simplex->coordsCREC[INDEX_REFLEC][iIndexVar] =
			simplex->coordsCREC[INDEX_CENTER][iIndexVar];

	/* Function evaluation at the reflection point */
	simplex->fValueCREC[INDEX_REFLEC] =
		evaluateFunctionAtX(simplex->coordsCREC[INDEX_REFLEC],
							r, iApproximation, file->dTaus,
							file->dCorrs[iIndexDetector],
							iFirstFitPt, iLastFitPt);
/*
	printf("R: %.12f, %f, %f, %f - %f\n", simplex->coordsCREC[INDEX_REFLEC][INDEX_DB],
									      simplex->coordsCREC[INDEX_REFLEC][INDEX_BETA],
									      simplex->coordsCREC[INDEX_REFLEC][INDEX_MU_A],
									      simplex->coordsCREC[INDEX_REFLEC][INDEX_MU_S],
									      simplex->fValueCREC[INDEX_REFLEC]);*/
} /* getReflectionPoint */

/*======================================================================
   FUNCTION: getExpansionPoint
   ROLE:	 Compute the expansion point, that is the reflection of the
    		 center in relation to reflection point
   PARAM:	 simplex - struct where are the variables that control the
   	   	   	   	   	   vertex
   	   	   	 The other parameters were passed from 'minimize' just for
   	   	   	 became possible the function evaluation here
   RETURN:	 void
======================================================================*/
void getExpansionPoint(simplex_t * simplex, dcs_file * file,
					   int iIndexDetector, int iFirstFitPt, int iLastFitPt,
					   int iApproximation, position * r)
{
	int iIndexVar;

	/* Expansion: Pe = Pc + f*(Pr - Pc), where:
	 * Pe: expansion point, Pr: reflection point, Pc: center, f: factor */
	for(iIndexVar = 0; iIndexVar < simplex->n; iIndexVar++)
	{
		simplex->coordsCREC[INDEX_EXPANS][iIndexVar] =
		 simplex->coordsCREC[INDEX_CENTER][iIndexVar] + EXPANS_COEF *
			(simplex->coordsCREC[INDEX_REFLEC][iIndexVar] -
			 simplex->coordsCREC[INDEX_CENTER][iIndexVar]);

		if(simplex->coordsCREC[INDEX_EXPANS][iIndexVar] < 0)
			simplex->coordsCREC[INDEX_EXPANS][iIndexVar] = 0.0;
	}

	/* The variables that are not being considered are only copied */
	for(; iIndexVar < MAX_VAR; iIndexVar++)
		simplex->coordsCREC[INDEX_EXPANS][iIndexVar] =
		 simplex->coordsCREC[INDEX_CENTER][iIndexVar];

	/* Function evaluation at the reflection point */
	simplex->fValueCREC[INDEX_EXPANS] =
		evaluateFunctionAtX(simplex->coordsCREC[INDEX_EXPANS],
							r, iApproximation, file->dTaus,
							file->dCorrs[iIndexDetector],
							iFirstFitPt, iLastFitPt);

	/*printf("E: %.12f, %f, %f, %f - %f\n", simplex->coordsCREC[INDEX_EXPANS][INDEX_DB],
									      simplex->coordsCREC[INDEX_EXPANS][INDEX_BETA],
									      simplex->coordsCREC[INDEX_EXPANS][INDEX_MU_A],
									      simplex->coordsCREC[INDEX_EXPANS][INDEX_MU_S],
									      simplex->fValueCREC[INDEX_EXPANS]);*/
} /* getExpansionPoint */

/*======================================================================
   FUNCTION: getContractionPoint
   ROLE:	 Compute the contraction point, which depends on the type
   	   	     of contraction. It can be either between the center and the
   	   	     reflection point (outside) or between the center and the
   	   	     largest vertex (inside)
   PARAM:	 simplex - struct where are the variables that control the
   	   	   	   	   	   vertex
   	   	   	 contractionType - defines the type of the contraction
   	   	   	 The other parameters were passed from 'minimize' just for
   	   	   	 became possible the function evaluation here
   RETURN:	 void
======================================================================*/
void getContractionPoint(simplex_t * simplex, int contractionType, dcs_file * file,
						 int iIndexDetector, int iFirstFitPt, int iLastFitPt,
						 int iApproximation, position * r)
{
	int iIndexVar;

	if(contractionType == OUTSIDE_CONTRAC)
	{
		/* Contraction: Px = Pc + f*(Pr - Pc), where:
		 * Px: contraction point, Pr: reflection point, Pc: center, f: factor */
		for(iIndexVar = 0; iIndexVar < simplex->n; iIndexVar++)
		{
			simplex->coordsCREC[INDEX_CONTRAC][iIndexVar] =
			 simplex->coordsCREC[INDEX_CENTER][iIndexVar] + CONTRAC_COEF *
				(simplex->coordsCREC[INDEX_REFLEC][iIndexVar] -
				 simplex->coordsCREC[INDEX_CENTER][iIndexVar]);

			if(simplex->coordsCREC[INDEX_CONTRAC][iIndexVar] < 0)
				simplex->coordsCREC[INDEX_CONTRAC][iIndexVar] = 0.0;
		}
	}
	else
	{
		/* Contraction: Px = Pc + f*(Pc - Pl), where:
		 * Px: contraction point, Pl: largest vertex, Pc: center, f: factor */
		for(iIndexVar = 0; iIndexVar < simplex->n; iIndexVar++)
		{
			simplex->coordsCREC[INDEX_CONTRAC][iIndexVar] =
			 simplex->coordsCREC[INDEX_CENTER][iIndexVar] + CONTRAC_COEF *
				(simplex->coordsCREC[INDEX_CENTER][iIndexVar] -
				 simplex->coordsVertex[simplex->largestID][iIndexVar]);

			if(simplex->coordsCREC[INDEX_CONTRAC][iIndexVar] < 0)
				simplex->coordsCREC[INDEX_CONTRAC][iIndexVar] = 0.0;
		}
	}

	/* The variables that are not being considered are only copied */
	for(; iIndexVar < MAX_VAR; iIndexVar++)
		simplex->coordsCREC[INDEX_CONTRAC][iIndexVar] =
		 simplex->coordsCREC[INDEX_CENTER][iIndexVar];

	/* Function evaluation at the reflection point */
	simplex->fValueCREC[INDEX_CONTRAC] =
		evaluateFunctionAtX(simplex->coordsCREC[INDEX_CONTRAC],
							r, iApproximation, file->dTaus,
							file->dCorrs[iIndexDetector],
							iFirstFitPt, iLastFitPt);

	/*printf("X: %.12f, %f, %f, %f - %f\n", simplex->coordsCREC[INDEX_CONTRAC][INDEX_DB],
									      simplex->coordsCREC[INDEX_CONTRAC][INDEX_BETA],
									      simplex->coordsCREC[INDEX_CONTRAC][INDEX_MU_A],
									      simplex->coordsCREC[INDEX_CONTRAC][INDEX_MU_S],
									      simplex->fValueCREC[INDEX_CONTRAC]);*/
} /* getContractionPoint */

/*======================================================================
   FUNCTION: reduceAllDimensions
   ROLE:	 New simplex around the smallest vertex, reducing largest and middle
   PARAM:	 simplex - struct where are the variables that control the
   	   	   	   	   	   vertex
   	   	   	 The other parameters were passed from 'minimize' just for
   	   	   	 became possible the function evaluation here
   RETURN:	 void
======================================================================*/
void reduceAllDimensions(simplex_t * simplex, dcs_file * file,
					     int iIndexDetector, int iFirstFitPt, int iLastFitPt,
					     int iApproximation, position * r)
{
	int iIndexVertex,
		iIndexVar;

	for(iIndexVertex = 0; iIndexVertex <= simplex->n; iIndexVertex++)
		/* The smallest vertex keeps equal */
		if(iIndexVertex != simplex->smallestID)
		{
			for(iIndexVar = 0; iIndexVar < simplex->n; iIndexVar++)
				simplex->coordsVertex[iIndexVertex][iIndexVar] =
				 simplex->coordsVertex[simplex->smallestID][iIndexVar] +
			    (simplex->coordsVertex[iIndexVertex][iIndexVar] -
			 	 simplex->coordsVertex[simplex->smallestID][iIndexVar])/2.0;
		}

	/* New function evaluations */
	simplex->fValueVertex[simplex->middleID] =
		evaluateFunctionAtX(simplex->coordsVertex[simplex->middleID],
							r, iApproximation, file->dTaus,
							file->dCorrs[iIndexDetector],
							iFirstFitPt, iLastFitPt);

	simplex->fValueVertex[simplex->largestID] =
			evaluateFunctionAtX(simplex->coordsVertex[simplex->largestID],
								r, iApproximation, file->dTaus,
								file->dCorrs[iIndexDetector],
								iFirstFitPt, iLastFitPt);
} /* reduceAllDimensions */

/*======================================================================
   FUNCTION: copyVertex
   ROLE:	 Copy the coordinates from the source to the target
   PARAM:	 simplex - Struct where as the variables involved
   			 iIndexTarget - Index of the vertex
   			 	 	 	    (in the array simplex->coordVertex) that
   			 	 	 	    will have his coordinates set
   			 iIndexSource - Index of the vertex
   			 	 	 	    (in the array simplex->coordCREC) that will
   			 	 	 	    provide his values for the copy
   RETURN:	 void
======================================================================*/
void copyVertex(simplex_t * simplex, int iIndexTarget, int iIndexSource)
{
	int iIndexVar;

	for(iIndexVar = 0; iIndexVar < simplex->n; iIndexVar++)
		simplex->coordsVertex[iIndexTarget][iIndexVar] =
		 simplex->coordsCREC[iIndexSource][iIndexVar];

	simplex->fValueVertex[iIndexTarget] = simplex->fValueCREC[iIndexSource];
}

/*======================================================================
   FUNCTION: noneIsBetterThanLargest
   ROLE:	 After has been create the reflection and the expansion or
   	   	     contraction points, this function evaluates if at least one
   	   	     of them is best then the largest in the simplex. If so,
   	   	     it's replaced
   PARAM:	 simplex - struct where are the vertex
   RETURN:	 1 - If neither the reflection nor the contraction or the
   	   	   	     expansion points are better than the largest
   	   	   	 0 - If at least one of them if better
======================================================================*/
int noneIsBetterThanLargest(simplex_t * simplex, int * action)
{
	int bNotReplaced = 1;

	/* If there was an expansion */
	if(simplex->fValueCREC[INDEX_EXPANS] > 0)
	{
		/* If the expansion vertex is better */
		if(simplex->fValueCREC[INDEX_EXPANS] < simplex->fValueCREC[INDEX_REFLEC] &&
		   simplex->fValueCREC[INDEX_EXPANS] < simplex->fValueVertex[simplex->largestID])
		{
			copyVertex(simplex, simplex->largestID, INDEX_EXPANS);
			bNotReplaced = 0;
			* action = 'E';
		}
	}
	else
		/* If there was an contraction */
		if(simplex->fValueCREC[INDEX_CONTRAC] > 0)
		{
			/* If the contraction vertex is better */
			if(simplex->fValueCREC[INDEX_CONTRAC] < simplex->fValueCREC[INDEX_REFLEC] &&
			   simplex->fValueCREC[INDEX_CONTRAC] < simplex->fValueVertex[simplex->largestID])
			{
				copyVertex(simplex, simplex->largestID, INDEX_CONTRAC);
				bNotReplaced = 0;
				* action = 'C';
			}
		}

	/* If neither the expansion nor the contraction point is better than
	 * the largest, verify if the reflection point is */
	if(bNotReplaced)
		if(simplex->fValueCREC[INDEX_REFLEC] < simplex->fValueVertex[simplex->largestID])
		{
			copyVertex(simplex, simplex->largestID, INDEX_REFLEC);
			bNotReplaced = 0;
			* action = 'R';
		}

	return bNotReplaced;
} /* noneIsBetterThanLargest */


/*======================================================================
   FUNCTION: testConvergence
   ROLE:	 Test if the algorithm has converged, by computing the
   	   	     standard deviation considering the three vertex of the
   	   	     simplex
   PARAM:	 simplex - struct where are the vertex
   RETURN:
======================================================================*/
int testConvergence(simplex_t simplex)
{
	int iIndexVertex;
	value_t sum   = 0.0,
			sigma = 0.0,
			average;

	/* Compute average */
	for(iIndexVertex = 0; iIndexVertex <= simplex.n; iIndexVertex++)
		sum += simplex.fValueVertex[iIndexVertex];

	average = sum/(simplex.n + 1);

	for(iIndexVertex = 0; iIndexVertex <= simplex.n; iIndexVertex++)
		sigma += pow((simplex.fValueVertex[iIndexVertex] - average), 2.0)/(simplex.n);

	sigma = sqrt(sigma);

	if (sigma < EPSILON)
		return 1;

	return 0;
}

/*======================================================================
   FUNCTION: minimize
   ROLE:	 Minimize the function for the specific file and detector
   METHOD:	 Nelder-Mead Simplex Method
   PARAM:	 file - File where is the detector considered
			 indexDetector - Detector whose values will supply the
			 	 	 	     minimization process
			 iFirstFitPt - First index to be considered in the 'taus' array
		  	 iLastFitPt - Last index to be considered in the 'taus' array
			 r - struct with the position variables
			 simplex - simplex structure for the minimization
   RETURN:	 void
======================================================================*/
void minimize(dcs_file * file, int iIndexDetector,
			  int iFirstFitPt, int iLastFitPt, int iApproximation,
			  position * r, simplex_t simplex)
{
	/* Struct that group all data structures used in the minimization */
	//simplex_t simplex;
	int iIteration   = 0, /* itr - Number of interation  */
		bConverged   = 0;
	//int action = '#'; /* Debugging */

	/* Define the initial points and evaluate the function at these points */
	evaluateInitialSimplex(&simplex, file, iIndexDetector, iFirstFitPt, iLastFitPt, iApproximation, r);

	/* While the maximum number of iterations allowed hasn't been
	 * reached and the algorithm hasn't converged */
	while(iIteration < MAX_ITERATIONS && !bConverged)
	{
		/* Define the indexes and, doing so, the order between the vertexes */
		getSmallestMiddleHighest(&simplex);

		// TODO: Delete this debugging
		//printIteration(simplex, iIteration); /* Debugging */

		 /* Calculate the center */
		getCenterOfMass(&simplex, file, iIndexDetector, iFirstFitPt, iLastFitPt, iApproximation, r);

		/* Calculate the reflection point */
		getReflectionPoint(&simplex, file, iIndexDetector, iFirstFitPt, iLastFitPt, iApproximation, r);

		// smallest | reflection | middle | largest
		if(simplex.fValueCREC[INDEX_REFLEC] <  simplex.fValueVertex[simplex.middleID] &&
		   simplex.fValueCREC[INDEX_REFLEC] >= simplex.fValueVertex[simplex.smallestID])
			// largest = reflection
			copyVertex(&simplex, simplex.largestID, INDEX_REFLEC);
		else
		{
			// reflection | smallest | middle | largest
			if(simplex.fValueCREC[INDEX_REFLEC] <  simplex.fValueVertex[simplex.smallestID])
			{
				// Calculate the expansion point
				getExpansionPoint(&simplex, file, iIndexDetector, iFirstFitPt,
				 	    		  iLastFitPt, iApproximation, r);

				// expansion | reflection | smallest | middle | largest
				if(simplex.fValueCREC[INDEX_EXPANS] <  simplex.fValueCREC[INDEX_REFLEC])
					copyVertex(&simplex, simplex.largestID, INDEX_EXPANS);
				else
					copyVertex(&simplex, simplex.largestID, INDEX_REFLEC);
			}
			else
				// smallest | middle | .. | reflection | ..
				if(simplex.fValueCREC[INDEX_REFLEC] >= simplex.fValueVertex[simplex.middleID])
				{
					// smallest | middle | reflection | largest
					if(simplex.fValueCREC[INDEX_REFLEC] < simplex.fValueVertex[simplex.largestID])
						// Outside contraction
						getContractionPoint(&simplex, OUTSIDE_CONTRAC, file, iIndexDetector,
										    iFirstFitPt, iLastFitPt, iApproximation, r);
					else
						// smallest | middle | largest | reflection
						// Inside contraction
						getContractionPoint(&simplex, INSIDE_CONTRAC, file, iIndexDetector,
										    iFirstFitPt, iLastFitPt, iApproximation, r);

					// smallest | middle | contraction | largest
					if(simplex.fValueCREC[INDEX_CONTRAC] <  simplex.fValueVertex[simplex.largestID])
						copyVertex(&simplex, simplex.largestID, INDEX_CONTRAC);
					else
					{
						// New simplex around the smallest vertex, reducing largest and middle
						reduceAllDimensions(&simplex, file, iIndexDetector, iFirstFitPt,
								 	 	 	iLastFitPt, iApproximation, r);
					}
				}
		} // If reflection isn't the second value (greater then the smallest and smaller then the middle)

		// TODO: Delete this debugging
		//printf("{%.8f, %.8f, %.8f | (%c)}\n", simplex.fValueCREC[INDEX_REFLEC], simplex.fValueCREC[INDEX_EXPANS], simplex.fValueCREC[INDEX_CONTRAC], action);

		/* Test if the algorithm has converged */
		bConverged = testConvergence(simplex);

		iIteration++;
	} /* while(iIteration < MAX_ITERATIONS && !bConverged) */

	file->bestFit[iIndexDetector][INDEX_DB] =
	 simplex.coordsVertex[simplex.smallestID][INDEX_DB];
	file->bestFit[iIndexDetector][INDEX_BETA] =
	 simplex.coordsVertex[simplex.smallestID][INDEX_BETA];
	file->bestFit[iIndexDetector][INDEX_MU_A] =
	 simplex.coordsVertex[simplex.smallestID][INDEX_MU_A];
	file->bestFit[iIndexDetector][INDEX_MU_S] =
	 simplex.coordsVertex[simplex.smallestID][INDEX_MU_S];
} /* minimize */

/*======================================================================
   FUNCTION: getBestXfit
   ROLE:	 Try to figure out the best values for the parameters that
   	   	     compose the structure 'x_point', that is, try to figure
   	   	     out the best values for Db, beta, etc. Calls minimize
   	   	     for all detectors from all files
   PARAM:	 allFiles - Informations about the experiment
   	   	     n - dimension of the function
   RETURN:	 void
======================================================================*/
void getBestXfit(experiment * allFiles, exp_info info, int n, value_t * var)
{
	int 	 iIndexFile,
			 iIndexDetector;
	position r;
	simplex_t simplex; /* Used in the minimization process */
	FILE *   fileBestFit;

	/* Set 'lt', 'Ls' and 'z' */
	setPositionVariables(&r);

	/* Memory allocation to the structures used in the minimization */
	initMinimizationStructs(&simplex, n);

	fprintfHeader(&fileBestFit, * allFiles, info);
	printf("File Count: %d\n", allFiles->iFileCount);

	/* For each files TODO: Erase comment in this condition! */
	for(iIndexFile = 0; iIndexFile < allFiles->iFileCount; iIndexFile++)
	{
		printf("File %d: \n", iIndexFile);
		/* Memory allocation to the array where the result will be saved */
		initBestFitArray(&(allFiles->files[iIndexFile]));

		/* For each detector */
		for(iIndexDetector = 0;
			iIndexDetector < allFiles->files[iIndexFile].iDetectorCount;
			iIndexDetector++)
		{
			/* If the detector is being considered */
			if(allFiles->iUsedDetectors[iIndexDetector])
			{
				r.rho = allFiles->dRhos[iIndexDetector];
				initSimplexVertexes(&simplex, allFiles->startPt, var);

				/* Call minimize with the parameters which had been
				 * set in the function setParameters */
				minimize(&(allFiles->files[iIndexFile]), iIndexDetector,
						   allFiles->iFirstFitPt, allFiles->iLastFitPt,
						   allFiles->approximation, &r, simplex);

				printCoordsBestFit(allFiles->files[iIndexFile], iIndexDetector);
			}
		}

		//fprintfQuickFileBestFit(&fileBestFit, allFiles->files[iIndexFile], iIndexFile, info);
		freeBestFitArray(&(allFiles->files[iIndexFile]));
	}

	fprintf(fileBestFit, "%s END\n", BORDER);
	fclose(fileBestFit);
	freeSimplex(&simplex);
} /* getBestXfit */

/*======================================================================
   FUNCTION: getFileBestFit
   ROLE:	 Try to figure out the best values for the parameters Db,
             Beta, Mua and Mus'. Call minimize() for all detectors.
   PARAM:	 fileResult - File to be writen with the result of the
             fitting process
             fileInfo - Information about the file that will be fitted
             startPt - Initial values for the variables of the problem
             [Db, Beta, Mua, Mus']
   	   	     n - Dimension of the function
   RETURN:	 void
======================================================================*/
void getFileBestFit(FILE ** fileResult, int iFileIndex, experiment * allFiles, int n, value_t * var)
{
    position  r;
	simplex_t simplex; /* Used in the minimization process */
	int       iIndexDetector;

    printf("File %d: \n", iFileIndex); /* Debugging */

	/* Set 'lt', 'Ls' and 'z' */
	setPositionVariables(&r);

	/* Memory allocation to the structures used in the minimization */
	initMinimizationStructs(&simplex, n);

	/* Memory allocation to the array where the result will be saved */
	initBestFitArray(&(allFiles->files[iFileIndex]));

    /* For each detector */
    for(iIndexDetector = 0;
        iIndexDetector < allFiles->files[iFileIndex].iDetectorCount;
		iIndexDetector++)
    {
        /* If the detector is being considered */
        if(allFiles->iUsedDetectors[iIndexDetector])
        {
            r.rho = allFiles->dRhos[iIndexDetector];
            initSimplexVertexes(&simplex, allFiles->startPt, var);

            /* Call minimize with the parameters which had been
             * set in the function setParameters */
            minimize(&(allFiles->files[iFileIndex]),
                     iIndexDetector, allFiles->iFirstFitPt, allFiles->iLastFitPt,
                     allFiles->approximation, &r, simplex);

            printCoordsBestFit(allFiles->files[iFileIndex], iIndexDetector);
        }
    }

    //fprintfQuickFileBestFit(fileResult, allFiles->files[iFileIndex],
    //                        allFiles->iFileCount, allFiles->startPt);
    freeBestFitArray(&(allFiles->files[iFileIndex]));

	freeSimplex(&simplex);
} /* getFileBestFit */

/*======================================================================
   FUNCTION: getBestFit
   ROLE:	 Try to figure out the best values for the parameters Db,
             Beta, Mua and Mus'. Call minimize() for all detectors.
   PARAM:	 reportFile - File to be writen with the result of the
             fitting process
             iFileIndex - File's ID
             dataFile - Information about the file that will be fitted
             experimentInfo - All relevant parameters
   RETURN:	 void
======================================================================*/
void getBestFit(FILE ** reportFile, dcs_file * dataFile, dcs_file * averages,
                exp_info experimentInfo, value_t * var)
{
    position  r;
	simplex_t simplex; /* Used in the minimization process */
	int       iIndexDetector,
              iIndexAverage,
              n = experimentInfo.startPt[INDEX_DB]   +
                  experimentInfo.startPt[INDEX_BETA] +
                  experimentInfo.startPt[INDEX_MU_A] +
                  experimentInfo.startPt[INDEX_MU_S];

    //printf("File %d: \n", iFileIndex); /* Debugging */

	/* Set 'lt', 'Ls' and 'z' */
	setPositionVariables(&r);

	/* Memory allocation to the structures used in the minimization */
	initMinimizationStructs(&simplex, n);

	/* Memory allocation to the array where the result will be saved */
	initBestFitArray(dataFile);
	initBestFitArray(averages);

    /* For each detector */
    for(iIndexDetector = 0; iIndexDetector < dataFile->iDetectorCount; iIndexDetector++)
    {
        /* If the detector is being considered */
        if(experimentInfo.iUsedDetectors[iIndexDetector])
        {
            r.rho = experimentInfo.dRhos[iIndexDetector];
            initSimplexVertexes(&simplex, experimentInfo.startPt, var);

            /* Call minimize with the parameters which had been
             * set in the function setParameters */
            minimize(dataFile, iIndexDetector, experimentInfo.iFirstFitPt,
                     experimentInfo.iLastFitPt, experimentInfo.approximation, &r, simplex);

            printCoordsBestFit(* dataFile, iIndexDetector);
        }
    }

    /* For each average */
    for(iIndexAverage = 0; iIndexAverage < MAX_AVG; iIndexAverage++)
    {
        /* If the detector is being considered */
        if(experimentInfo.iUsedAvg[iIndexAverage])
        {
            r.rho = experimentInfo.dRhosAvg[iIndexAverage];
            initSimplexVertexes(&simplex, experimentInfo.startPt, var);

            minimize(averages, iIndexAverage, experimentInfo.iFirstFitPt,
                     experimentInfo.iLastFitPt, experimentInfo.approximation, &r, simplex);

            printCoordsBestFit(* averages, iIndexAverage);
        }
    }

    fprintfQuickFileBestFitAndClose(reportFile, experimentInfo,
                                    * dataFile, * averages);

	freeSimplex(&simplex);
}
