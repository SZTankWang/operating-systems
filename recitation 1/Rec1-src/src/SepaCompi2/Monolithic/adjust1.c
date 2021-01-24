
/*
 *          File: adjust1.c
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



#define MAX_GRADES  10  /* The max. number of grades per student. */


/************************ Function Prototypes *********************/


/*
 * Function: PrintGrades
 * Usage: PrintGrades(grades, howmany)
 * ---------------------------
 * Prints the integer grades in the array `grades'.
 * `Howmany' indicates the number of grades.
 * Grades are all printed on one line separated by
 * spaces and terminated by a newline.
 */
void PrintGrades(float grades[MAX_GRADES], int howmany);

/*
 * Function: AdjustGrades
 * Usage: AdjustGrades(grades, adjustments, howmany)
 * -------------------------------
 * Adjusts each grade in the `grades' array using
 * the corresponding change in the `adjustments' array.
 * `Howmany' indicates the number of grades.  On return
 * from the function, the `grades' array that was passed
 * will contain the new grades.
 */
void AdjustGrades(float grades[MAX_GRADES],
                  float adjustments[MAX_GRADES],
                  int howmany);

/*
 * Function: AdjustedGrade
 * Usage: newGrade = AdjustedGrade(grade, adjustment)
 * -------------------------------
 * Adjusts one grade and returns the new grade.
 * Currently, the adjustment technique is to
 * add the adjustment to the grade.
 */
float AdjustedGrade(float grade, float adjustment);


int main()
{
  int i;                          /* Looping variable.                   */
  float adjustments[MAX_GRADES];  /* Grade adjustments for all students. */
  int numGrades;                  /* Number of adjustments.              */
  int numStudents;                /* Number of students.                 */
  int studentNum;                 /* Index of current student.           */
  float grades[MAX_GRADES];       /* Grades for current student.         */

  printf("How many grades per student? ");
  scanf("%d", &numGrades);

  if (numGrades < 1 || numGrades > MAX_GRADES) {
    printf("I can only handle 1..%d grades!", MAX_GRADES);
    exit(1);  /* Terminate program, return error status. */
  }

  i = 0;

  while (i < numGrades) {
    printf("\nEnter an adjustment> ");
    scanf("%f", &adjustments[i++]);
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


/************************ Function Definitions *********************/


void PrintGrades(float grades[MAX_GRADES], int howmany)
{
  int i;  /* loop variable */

  for (i = 0; i < howmany; i++) {
    printf(" %.2f", grades[i]);
  }

  printf("\n");  /* end of line */
}


void AdjustGrades(float grades[MAX_GRADES],
                  float adjustments[MAX_GRADES],
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

