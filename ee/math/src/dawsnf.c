/*							dawsnf.c
 *
 *	Dawson's Integral
 *
 *
 *
 * SYNOPSIS:
 *
 * float x, y, dawsnf();
 *
 * y = dawsnf( x );
 *
 *
 *
 * DESCRIPTION:
 *
 * Approximates the integral
 *
 *                             x
 *                             -
 *                      2     | |        2
 *  dawsn(x)  =  exp( -x  )   |    exp( t  ) dt
 *                          | |
 *                           -
 *                           0
 *
 * Three different rational approximations are employed, for
 * the intervals 0 to 3.25; 3.25 to 6.25; and 6.25 up.
 *
 *
 * ACCURACY:
 *
 *                      Relative error:
 * arithmetic   domain     # trials      peak         rms
 *    IEEE      0,10        50000       4.4e-7      6.3e-8
 *
 *
 */

/*							dawsn.c */


/*
Cephes Math Library Release 2.1:  January, 1989
Copyright 1984, 1987, 1989 by Stephen L. Moshier
Direct inquiries to 30 Frost Street, Cambridge, MA 02140
*/

#include "mconf.h"
/* Dawson's integral, interval 0 to 3.25 */
static float AN[10] = {
    1.13681498971755972054E-11,
    8.49262267667473811108E-10,
    1.94434204175553054283E-8,
    9.53151741254484363489E-7,
    3.07828309874913200438E-6,
    3.52513368520288738649E-4,
    -8.50149846724410912031E-4,
    4.22618223005546594270E-2,
    -9.17480371773452345351E-2,
    9.99999999999999994612E-1,
};
static float AD[11] = {
    2.40372073066762605484E-11,
    1.48864681368493396752E-9,
    5.21265281010541664570E-8,
    1.27258478273186970203E-6,
    2.32490249820789513991E-5,
    3.25524741826057911661E-4,
    3.48805814657162590916E-3,
    2.79448531198828973716E-2,
    1.58874241960120565368E-1,
    5.74918629489320327824E-1,
    1.00000000000000000539E0,
};

/* interval 3.25 to 6.25 */
static float BN[11] = {
    5.08955156417900903354E-1,
    -2.44754418142697847934E-1,
    9.41512335303534411857E-2,
    -2.18711255142039025206E-2,
    3.66207612329569181322E-3,
    -4.23209114460388756528E-4,
    3.59641304793896631888E-5,
    -2.14640351719968974225E-6,
    9.10010780076391431042E-8,
    -2.40274520828250956942E-9,
    3.59233385440928410398E-11,
};
static float BD[10] = {
    /*  1.00000000000000000000E0,*/
    -6.31839869873368190192E-1,
    2.36706788228248691528E-1,
    -5.31806367003223277662E-2,
    8.48041718586295374409E-3,
    -9.47996768486665330168E-4,
    7.81025592944552338085E-5,
    -4.55875153252442634831E-6,
    1.89100358111421846170E-7,
    -4.91324691331920606875E-9,
    7.18466403235734541950E-11,
};

/* 6.25 to infinity */
static float CN[5] = {
    -5.90592860534773254987E-1,
    6.29235242724368800674E-1,
    -1.72858975380388136411E-1,
    1.64837047825189632310E-2,
    -4.86827613020462700845E-4,
};
static float CD[5] = {
    /* 1.00000000000000000000E0,*/
    -2.69820057197544900361E0,
    1.73270799045947845857E0,
    -3.93708582281939493482E-1,
    3.44278924041233391079E-2,
    -9.73655226040941223894E-4,
};


extern float PIF, MACHEPF;
#define fabsf(x) ((x) < 0 ? -(x) : (x))
#ifdef ANSIC
float polevlf(float, float *, int);
float p1evlf(float, float *, int);
#else
float polevlf(), p1evlf();
#endif

#ifdef ANSIC
float dawsnf(float xxx)
#else
float dawsnf(xxx) double xxx;
#endif
{
    float xx, x, y;
    int sign;

    xx = xxx;
    sign = 1;
    if (xx < 0.0) {
        sign = -1;
        xx = -xx;
    }

    if (xx < 3.25) {
        x = xx * xx;
        y = xx * polevlf(x, AN, 9) / polevlf(x, AD, 10);
        return (sign * y);
    }


    x = 1.0 / (xx * xx);

    if (xx < 6.25) {
        y = 1.0 / xx + x * polevlf(x, BN, 10) / (p1evlf(x, BD, 10) * xx);
        return (sign * 0.5 * y);
    }


    if (xx > 1.0e9)
        return ((sign * 0.5) / xx);

    /* 6.25 to infinity */
    y = 1.0 / xx + x * polevlf(x, CN, 4) / (p1evlf(x, CD, 5) * xx);
    return (sign * 0.5 * y);
}
