
#ifndef UNTITLED2_EX2_H
#define UNTITLED2_EX2_H

typedef int magnitude;


magnitude add(magnitude a, magnitude b); // a + b
magnitude sub(magnitude a, magnitude b); // a - b
magnitude multi(magnitude a, magnitude b); // a * b

// true = 1, false = 0
int equal(magnitude a, magnitude b); // a = b
int greater(magnitude a, magnitude b); // a > b

void print_bits (int ot);
int getSign(magnitude a);

int sameSign(magnitude a, magnitude b);
magnitude setSignAs(magnitude a, magnitude b);
magnitude getMagnitude(magnitude a);
int isAbove(magnitude a, magnitude b);

#endif //UNTITLED2_EX2_H
