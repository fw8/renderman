/*
 * Mathematische Routinen allgemeiner Art
 */
#include "RiTypes.h"

double Rmfabs; /* temporary variable used by fabs macro */

/**********************************************************

   Determines if point px, py intersects line x1,y1,x2,y2.
   Returns true if it does.

 **********************************************************/

int
RmLineIntersect(px, py, x1, y1, x2, y2)
  double px, py, x1, y1, x2, y2;
{
  double t;

  x1 -= px; x2 -= px;           /* translate line */
  y1 -= py; y2 -= py;

  if ((y1 > 0.0) && (y2 > 0.0)) return(RI_FALSE);
  if ((y1 < 0.0) && (y2 < 0.0)) return(RI_FALSE);
  if ((x1 < 0.0) && (x2 < 0.0)) return(RI_FALSE);

  if (y1 == y2) {
    if (y1 != 0.0) return(RI_FALSE);
    if ((x1 > 0.0) || (x2 > 0.0)) return(RI_FALSE);
    return(RI_TRUE);
  }

  t = (-y1) / (y2 - y1);

  if ((x1 + t * (x2 - x1)) > 0.0) return(RI_TRUE);
  return(RI_FALSE);
}
