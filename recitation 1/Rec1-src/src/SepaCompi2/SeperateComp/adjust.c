/*
 *          File: adjust.c
 *        Author: Robert I. Pitts <rip@cs.bu.edu>
 * Last Modified: March 4, 2000
 *         Topic: Modules, Separate Compilation, Make Files
 * ----------------------------------------------------------------
 *
 * OVERVIEW:
 * =========
 * This program makes adjustments to students' grades.
 * The program prompts for the number of grades.  Then,
 * the user enters adjustments for each grade, such as
 * +10 (meaning plus 10 pts).  Next, the program prompts
 * for the number of students.  It then reads in the grades
 * for each student and prints these grades out before and
 * after the adjustment.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "grades.h"


#define MAX_GRADES  10  /* The max. number of grades per student. */


int main()
{
  float adjustments[MAX_GRADES];  /* Grade adjustments for all students. */
  float grades[MAX_GRADES];       /* Grades for current student.         */
  int numGrades;                  /* Number of adjustments.              */
  int numStudents;                /* Number of students.                 */
  int studentNum;                 /* Index of current student.           */
  int i;                          /* Looping variable.                   */

  printf("How many grades per student? ");
  scanf("%d", &numGrades);

  if (numGrades < 1 || numGrades > MAX_GRADES) {
    printf("I can only handle 1..%d grades!", MAX_GRADES);
    exit(1);  /* Terminate program, return error status. */
  }

  for (i = 0; i < numGrades; i++) {
    printf("\nEnter adjustment for HW%d> ", i+1);
    scanf("%f", &adjustments[i]);
  }

  printf("\nHow many students? ");
  scanf("%d", &numStudents);

  for (studentNum = 1; studentNum <= numStudents; studentNum++) {
    printf("\n");  /* Blank line */

    /* Get each grade for the current student. */

    printf("Student #%d\n", studentNum);

    for (i = 0; i < numGrades; i++) {
      printf("\nEnter grade for HW%d> ", i+1);
      scanf("%f", &grades[i]);
    }

    /* Print grades before and after adjustment. */

    printf("\n\nOld Student #%d grades:", studentNum);
    PrintGrades(grades, numGrades);

    AdjustGrades(grades, adjustments, numGrades);

    printf("\nNew Student #%d grades:", studentNum);
    PrintGrades(grades, numGrades);
  }

  return 0;
}

