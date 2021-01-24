/*
 *          File: grades.h
 *        Author: Robert I. Pitts <rip@cs.bu.edu>
 * Last Modified: March 4, 2000
 *         Topic: Modules, Separate Compilation, Make Files
 * ----------------------------------------------------------------
 *
 * OVERVIEW:
 * =========
 * This is the "interface" for the module that provides
 * basic functions to print and adjust (floating point)
 * grades.  In the case of functions that deal with
 * multiple grades and adjustments, those values should
 * be stored in arrays.
 *
 */


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
void PrintGrades(float grades[], int howmany);

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
void AdjustGrades(float grades[],
                  float adjustments[],
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

