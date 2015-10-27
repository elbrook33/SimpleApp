#include <stdio.h>

int main() {
    double d = 0.0;
    int    i = 0;
    while( i < 2048*2048 ) {
        if( (int)d != i ) {
            printf( "Mismatch %f != %i\n", d, i );
        }
        d += 1.0;
        i += 1;
    }
    printf( "Final: %f = %i\n", d, i );
    return 0;
}
