/*
 * Headerfile f"ur alle mathematischen Funktionen
 */

#ifndef _Math_
#define _Math_

#define PI  3.141592
#define PIm2 (PI*2)
#define PId2 (PI/2)
/*
 * Sign of x
 */
#define sgn(x)  ( (x<0) ? -1 : (x==0) ? 0 : 1)
/*
 * x^2
 */
#define sqr(x) ((x)*(x))
/*
 * Absolute value -- uses "ftmp" to store value of argument, which
 * keeps us from evaluating expressions more than once.
 */
extern double Rmfabs;
#define fabs(x)                 ((Rmfabs=(x)) < 0. ? -(Rmfabs) : (Rmfabs))
#define abs(x)                  ((x) < 0 ? -(x) : (x))
/*
 * Close enough for us.
 */
#define equal(a, b)             (fabs((a) - (b)) < EPSILON)
/*
 * Maximum/Minimum functions
 */
#define max(a, b)               ((a) > (b) ? (a) : (b))
#define min(a, b)               ((a) < (b) ? (a) : (b))
/*
 * Convertion routines.
 */
#define DegToRad(x) ((x)*PI/180.0)
#define RadToDeg(x) ((x)*180.0/PI)

/*
 * Determines if a 2d point intersects a 2d line.
 * Returns true if it does.
 */
extern int RmLineIntersect();

#endif _Math_
