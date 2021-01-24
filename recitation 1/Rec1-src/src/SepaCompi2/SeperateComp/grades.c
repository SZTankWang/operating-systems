/*
 *          File: grades.c
 *        Author: Robert I. Pitts <rip@cs.bu.edu>
 * Last Modified: March 4, 2000
 *         Topic: Modules, Separate Compilation, Make Files
 * ----------------------------------------------------------------
 *
 * OVERVIEW:
 * =========
 * This is the "implementation" for functions of the grade
 * module.  There are functions to print and adjust grades.
 *
 */

#include <stdio.h>
#include "grades.h"


/************************ Function Definitions *********************/


void PrintGrades(float grades[], int howmany)
{
  int i;  /* loop variable */

  for (i = 0; i < howmany; i++) {
    printf(" %.2f", grades[i]);
  }

  printf("\n");  /* end of line */
}


void AdjustGrades(float grades[],
                  float adjustments[],
                  int howmany)
{
  float newGrade;
  int i;  /* loop variable */

  for (i = 0; i < howmany; i++) {
    /* This two-step assignment can be done in one step. */
    newGrade = AdjustedGrade(grades[i], adjustments[i]);
    grades[i] = newGrade;
  }
}


float AdjustedGrade(float grade, float adjustment)
{
  grade += adjustment;
  return grade;
}

