#include "ex2.h"
#include <stdio.h>

// create a 32 bit number where the msb is 1 and the others are 0.
int msb = 1 << 31;

/**
 * returns the sign of a given magnitude.
 *
 * @param a given magnitude.
 * @return 1 if negative sign and 0 if positive.
 */

int getSign(magnitude a) {
    return (a & msb) ? 1 : 0;
}

/**
 * Sets the sign of magnitude 'a' as the sign of magnitude 'b'.
 *
 * @param a given magnitude to change its sign.
 * @param b given magnitude with the desired sign.
 * @return magnitude 'a' with the changed sign.
 */

magnitude setSignAs(magnitude a, magnitude b) {
    return getSign(b) ? (a | msb) : (a & ~msb);
}

/**
 * Checks if two given magnitudes share the same sign.
 *
 * @param a first given magnitude.
 * @param b second given magnitude.
 * @return 1 if both have the same sign and 0 otherwise.
 */

int sameSign(magnitude a, magnitude b) {
    return !(getSign(a) ^ getSign(b));
}

/**
 * Return the absolute value of given magnitude.
 *
 * @param a given magnitude.
 * @return absolute value.
 */

magnitude getMagnitude(magnitude a) {
    return (a & ~msb);
}

/**
 * Checks if absolute value of magnitude 'a' is bigger than absolute value of magnitude 'b'.
 *
 * @param a first magnitude.
 * @param b second magnitude.
 * @return 1 if 'a' has bigger absolute value than 'b' and false otherwise.
 */

int isAbove(magnitude a, magnitude b) {
    return (getMagnitude(a) > getMagnitude(b)) ? 1 : 0;
}

/**
 * Addition two magnitudes.
 *
 * @param a first magnitude.
 * @param b second magnitude.
 * @return the sum of 'a' and 'b'
 */

magnitude add(magnitude a, magnitude b) {
    magnitude c;

    // checks for the same sign.
    if (sameSign(a, b)) {

        // add the magnitudes, and set the sign accordingly.
        c = getMagnitude(a) + getMagnitude(b);
        return setSignAs(c, a);
    } else { // different signs.

        // if opposite numbers.
        if (getMagnitude(a) == getMagnitude(b))
            return 0;

        // if |a| > |b|.
        if (isAbove(a, b)) {

            // calc |a| - |b|, and set the sign to 'a''s sign.
            c = getMagnitude(a) - getMagnitude(b);
            return setSignAs(c, a);
        }
        else { // |b| > |a|

            // calc |b| - |a|, and set the sign to 'b''s sign.
            c = getMagnitude(b) - getMagnitude(a);
            return setSignAs(c, b);
        }
    }
}

/**
 * subtract 'b' from 'a'
 *
 * @param a first magnitude.
 * @param b second magnitude.
 * @return a - b.
 */

magnitude sub(magnitude a, magnitude b) {
    magnitude c;

    //checks for the same sign.
    if (sameSign(a, b)) {

        // same numbers.
        if (getMagnitude(a) == getMagnitude(b))
            return 0;

        // if |a| > |b|
        if (isAbove(a, b)) {

            // calc |a| - |b|, and set the sign to 'a''s sign.
            c = getMagnitude(a) - getMagnitude(b);
            return setSignAs(c, a);
        }
        else { // |b| < |a|

            // calc |b| - |a|, and set the sign to 'b''s sign.
            c = getMagnitude(b) - getMagnitude(a);
            return setSignAs(c, b);
        }
    } else { // different signs.

        // calc |a| + |b|, and set the sign to 'a''s sign.
        c = getMagnitude(a) + getMagnitude(b);
        return setSignAs(c , a);
    }
}

/**
 * Calculate the multiplication of 'a' by 'b'.
 *
 * @param a first magnitude.
 * @param b second magnitude.
 * @return a * b.
 */
magnitude multi(magnitude a, magnitude b) {
    magnitude c;

    // if one the them is 0 or -0.
    if (getMagnitude(a) == 0 || getMagnitude(b) == 0)
        return 0;

    // check for the same sign.
    if (sameSign(a, b)) {

        // calc |a|*|b| and set the sign to positive.
        c = getMagnitude(a) * getMagnitude(b);
        return setSignAs(c, 0);
    } else { // different signs.

        // calc |a|*|b| and set the sign to negative.
        c = getMagnitude(a) * getMagnitude(b);
        return setSignAs(c , msb);
    }
}

/**
 * checks if two magnitudes are equal.
 *
 * @param a first magnitude.
 * @param b second magnitude.
 * @return 1 if equal and 0 otherwise.
 */

int equal(magnitude a, magnitude b) {

    // check if they are 0 or -0.
    if (getMagnitude(a) == 0 && getMagnitude(b) == 0)
        return 1;

    // check bit similarity.
    return (a == b) ? 1 : 0;
}

/**
 * checks if magnitude 'a' is greater than magnitude 'b'.
 *
 * @param a first magnitude.
 * @param b second magnitude.
 * @return 1 if a is greater and 0 otherwise.
 */

int greater(magnitude a, magnitude b) {

    // check for the same sign.
    if (sameSign(a, b)) {

        // if positive sign.
        if (getSign(a))
            return isAbove(a, b) ? 0 : 1;

        // if negative sign.
        else
            return isAbove(a, b) ? 1 : 0;
    } else { // different signs.
        return getSign(a) ? 0 : 1;
    }
}

/**
 * prints the given magnitude by bits.
 * @param num given magnitude.
 */

void print_bits(int num)
{
    int i = 31;
    for(; i>=0 ; i--) {
        if(num & 1<< i)
            printf("1");
        else
            printf("0");
        if (i == 8 || i == 16 || i == 24)
            printf(" ");
    }
printf("\n");
}

