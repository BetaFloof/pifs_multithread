#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>


/*  expm = 16^p mod ak.  This routine uses the left-to-right binary exponentiation scheme. */
static double expmm (double p, double ak)
{
    int i, j;
    double p1, pt, r;
#define ntp 25
    static double tp[ntp];
    static int tp1 = 0;

    if (ak == 1.) {
        return 0.;
    }

    /*  If this is the first call to expm, fill the power of two table tp. */

    if (tp1 == 0) {
        tp1 = 1;
        tp[0] = 1.;

        for (i = 1; i < ntp; i++) {
            tp[i] = 2. * tp[i-1]; // Multiply the previous value (tp[i-1]) by 2
        }
    }

    /*  Find the greatest power of two less than or equal to p. */

    for (i = 0; i < ntp; i++) {
        if (tp[i] > p) {
            break;
        }
    }

    pt = tp[i-1];
    p1 = p;
    r = 1.;

    /*  Perform binary exponentiation algorithm modulo ak. */

    for (j = 1; j <= i; j++) {
        if (p1 >= pt) {
            r = 16. * r;
            r = r - (int) (r / ak) * ak;
            p1 = p1 - pt;
        }

        pt = 0.5 * pt;
        if (pt >= 1.) {
            r = r * r;
            r = r - (int) (r / ak) * ak;
        }
    }

    return r;
}

/*  This routine evaluates the series  sum_k 16^(id-k)/(8*k+m) using the modular exponentiation technique. */
static double seriesm (int m, int id)
{
    int k;
    double ak, p, s, t;
#define eps 1e-17

    s = 0.;

    /*  Sum the series up to id. */

    for (k = 0; k < id; k++) {
        ak = 8 * k + m;
        p = id - k;
        t = expmm (p, ak);
        s = s + t / ak;
        s = s - (int) s;
    }

    /*  Compute a few terms where k >= id. */

    for (k = id; k <= id + 100; k++) {
        ak = 8 * k + m;
        t = pow (16., (double) (id - k)) / ak;

        if (t < eps) {
            break;
        }

        s = s + t;
        s = s - (int) s;
    }
    return s;
}

unsigned char get_byte(int id)
{
    int num;
    double r1, r2, r3, r4;
    unsigned char result;

    num = atoi(argv[1]);

    t1 = clock();

    r1 = seriesm(1, num);
    r2 = seriesm(4, num);
    r3 = seriesm(5, num);
    r4 = seriesm(6, num);

    double pid = 4. * r1 - 2. * r2 - r3 - r3;
    pid = pid - (int) pid + 1.;

    double y = fabs(pid);
    y = 16. * (y - floor (y));
    unsigned char first = y;
    y = 16. * (y - floor (y));
    unsigned char second = y;

    return (first << 4) | second;
}

int main(int argc, char *argv[]) {
    printf("%lu\n", sizeof(unsigned char));

    clock_t t1;


    t1 = clock() - t1;
    double time1_taken = ((double)t1)/CLOCKS_PER_SEC; // in seconds

    printf("%f\n", r1);
    printf("%f\n", r2);
    printf("%f\n", r3);
    printf("%f\n\n", r4);

    printf("%f\n\n", pid);

    printf("%.2x\n", first);
    printf("%.2x\n\n", second);

    printf("%.2x\n", result);
    printf("Took %f seconds to execute \n",time1_taken);

    return 0;
}