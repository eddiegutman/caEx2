#include "ex2.h"
#include <stdio.h>

int msb = 1 << 31;

int getSign(magnitude a) {
    return (a & msb) ? 1 : 0;
}

magnitude setSignAs(magnitude a, magnitude b) {
    return getSign(b) ? (a | msb) : (a & ~msb);
}

int sameSign(magnitude a, magnitude b) {
    return !(getSign(a) ^ getSign(b));
}

magnitude getMagnitude(magnitude a) {
    return (a & ~msb);
}

int isAbove(magnitude a, magnitude b) {
    return (getMagnitude(a) > getMagnitude(b)) ? 1 : 0;
}

magnitude add(magnitude a, magnitude b) {
    magnitude c;
    if (sameSign(a, b)) {
        c = getMagnitude(a) + getMagnitude(b);
        return setSignAs(c, a);
    } else {
        if (getMagnitude(a) == getMagnitude(b))
            return 0;
        if (isAbove(a, b)) {
            c = getMagnitude(a) - getMagnitude(b);
            return setSignAs(c, a);
        }
        else {
            c = getMagnitude(b) - getMagnitude(a);
            return setSignAs(c, b);
        }
    }
}
magnitude sub(magnitude a, magnitude b) {
    magnitude c;
    if (sameSign(a, b)) {
        if (getMagnitude(a) == getMagnitude(b))
            return 0;
        if (isAbove(a, b)) {
            c = getMagnitude(a) - getMagnitude(b);
            return setSignAs(c, a);
        }
        else {
            c = getMagnitude(b) - getMagnitude(a);
            return setSignAs(c, b);
        }
    } else {
        c = getMagnitude(a) + getMagnitude(b);
        return setSignAs(c , a);
    }
}
magnitude multi(magnitude a, magnitude b) {
    magnitude c;
    if (getMagnitude(a) == 0 || getMagnitude(b) == 0)
        return 0;
    if (sameSign(a, b)) {
        c = getMagnitude(a) * getMagnitude(b);
        return setSignAs(c, 0);
    } else {
        c = getMagnitude(a) * getMagnitude(b);
        return setSignAs(c , msb);
    }
}

int equal(magnitude a, magnitude b) {
    if (getMagnitude(a) == 0 && getMagnitude(b) == 0)
        return 1;
    return (a == b) ? 1 : 0;
}
int greater(magnitude a, magnitude b) {
    if (sameSign(a, b)) {
        if (getSign(a))
            return isAbove(a, b) ? 0 : 1;
        else
            return isAbove(a, b) ? 1 : 0;
    } else {
        return getSign(a) ? 0 : 1;
    }
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

