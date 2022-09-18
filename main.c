#define UBENCH_IMPLEMENTATION
#include <ubut/ubench.h>

// (c) 2021 by dbj at dbj dot org
// https://dbj.org/license_dbj
// https://godbolt.org/z/Gh5z66jer
#include <assert.h>
#include <stdlib.h>
#include <string.h>

extern int investigate_matreix(const int argc, char **argv);

int investigate_matrix_strategies(const int argc, char **argv)
{
    investigate_matreix(argc, argv);
    // 2D array declaration and initialization, on stack
    int arr[2][3] = {{1, 2, 3}, {4, 5, 6}};

    typedef unsigned char uchar;
    // pointer to the first byte of the underlying memory block
    uchar *block = (uchar *)&arr;
    // pointer to the third int
    uchar *int_no_3_ptr = block + (2 * sizeof(int));
    // the value is 3
    int int_no_3 = (int)*int_no_3_ptr;

    // regular pointer to the whole of int[2][3]
    int(*arp)[2][3] = malloc(sizeof(int[2][3]));

    // pointing to the whole of the: int arr[2][3]
    // has to be casted
    int(*matrix)[3] = (int(*)[3])arp;
    /*
using the above we can index "naturaly"
for example we can fill one by one
matrix[0][0] = 1;
matrix[0][1] = 2;
matrix[0][2] = 3;

matrix[1][0] = 4;
matrix[1][1] = 5;
matrix[1][2] = 6;
*/
    // or we can copy the array to array
    // arr[2][3] is on stack
    memcpy(arp, &arr, sizeof(int[2][3]));

    if (errno != 0)
        perror("memcpy failed.");
    else
        assert(matrix[0][1] == 2);

    free(arp);
    return 42;
}

UBENCH_STATE; // note there is no ()!
int main(int argc, const char *const argv[])
{
    return ubench_main(argc, argv);
}