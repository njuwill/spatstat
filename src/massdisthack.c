/*
  HACKED from R-2.0.1/src/appl/massdist.c
  by Adrian Baddeley
  Changes indicated by 'AB'
*/

/*
 *  R : A Computer Language for Statistical Data Analysis
 *  Copyright (C) 1996-2004     Robert Gentleman and Ross Ihaka and the
 *				R Development Core Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <R_ext/Arith.h>
#include <R_ext/Applic.h>

void massdisthack(double *x, int *nx, 
		  double *xmass, /* AB: new variable */
		  double *xlow, double *xhigh,
	      double *y, int *ny)
{
  double fx, xdelta, xmi, xpos;   /* AB */
    int i, ix, ixmax, ixmin;

    ixmin = 0;
    ixmax = *ny - 2;
    /* AB: line deleted */
    xdelta = (*xhigh - *xlow) / (*ny - 1);

    for(i=0; i < *ny ; i++)
	y[i] = 0;

    for(i=0; i < *nx ; i++) {
      if(R_FINITE(x[i])) {
	xpos = (x[i] - *xlow) / xdelta;
	ix = floor(xpos);
	fx = xpos - ix;
	xmi = xmass[i];   /* AB: new line  */
	if(ixmin <= ix && ix <= ixmax) {
	  y[ix] += (1 - fx) * xmi;   /* AB */
	  y[ix + 1] += fx * xmi; /* AB */
	}
	else if(ix == -1) {
	  y[0] += fx * xmi;  /* AB */
	}
	else if(ix == ixmax + 1) {
	  y[ix] += (1 - fx) * xmi;  /* AB */
	}
      }
    }

    /* AB: lines deleted */
}
