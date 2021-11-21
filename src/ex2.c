#include "ex2.h"
#include <stdio.h>

int isNeg (magnitude a) {
    int msb = 1 << 31;
    return (a & msb) ? 1 : 0;
}

int isAbove(magnitude a, magnitude b) {
    int msb = 1 << 31;
    return ((a & ~msb) > (b & ~msb)) ? 1 : 0;
}

magnitude flipSign(magnitude a) {
    int msb = 1 << 31;
    if (isNeg(a))
        return a - msb;
    else
        return a + msb;
}

magnitude add(magnitude a, magnitude b) {
   if (!isNeg(a) && !isNeg(b)) {
       return a + b;
   }
   if (isNeg(a) && isNeg(b)) {
       return a + b;
   }
   if (isNeg(a)) {
       if (isAbove(a, b)) {
           return flipSign(flipSign(a) - b);
       } else {
           return b - flipSign(a);
       }
   } else {
       if (isAbove(a, b)) {
           return a - flipSign(b);
       } else {
           return flipSign(flipSign(b) - a);
       }
   }
}
magnitude sub(magnitude a, magnitude b) {
    if (!isNeg(a) && !isNeg(b)) {
        if (isAbove(a, b)) {
            return a - flipSign(b);
        } else {
            return flipSign(flipSign(b) - a);
        }    }
    if (isNeg(a) && isNeg(b)) {
        if (isAbove(a, b)) {
            return flipSign(flipSign(a) - b);
        } else {
            return b - flipSign(a);
        }    }
    if (isNeg(a)) {
        return a + flipSign(b);
    } else {
        return flipSign(a) + flipSign(b);
    }
}
magnitude multi(magnitude a, magnitude b) {
    if (!isNeg(a) && !isNeg(b)) {
        return a * b;
    }
    if (isNeg(a) && isNeg(b)) {
        return a * b;
    }
    if (isNeg(a)) {
        return flipSign(flipSign(a) * b);
    } else {
        return flipSign(flipSign(b) * a);
    }
}

int equal(magnitude a, magnitude b) {
    return (a == b) ? 1 : 0;
}
int greater(magnitude a, magnitude b) {

}

void print_bits(int num)
{
    for(int i=31 ; i>=0 ; i--) {
        if(num & 1<< i)
            printf("1");
        else
            printf("0");
        if (i == 8 || i == 16 || i == 24)
            printf(" ");
    }
printf("\n");
}

