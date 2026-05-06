/* wrong-test-2.c
   Errors covered:
   - Missing semicolon in global declaration
   - Invalid condition in while (empty)
   - Missing closing brace in function body
   - Invalid return expression
*/

#include <stdio.h>

#define LIMIT 50

int counter       /* missing semicolon */

int decrement(int n) {
    n = n - 1;
    return n;
}

int helper(int a, int b) {
    int diff;
    diff = a - b;

    while () {        /* empty condition */
        diff = diff - 1;
    }

    return diff + ;   /* incomplete expression */

                      /* missing closing brace */

int main() {
    int x;
    x = 10;

    while (x > 0) {
        x = decrement(x);
    }

    return x;
}
