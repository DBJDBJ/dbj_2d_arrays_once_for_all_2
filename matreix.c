#include <ubut/ubench.h>
#include <stdlib.h>

#define MATRIX_HANDLE void *

// just a struct declaration
// usage: typedef MATRIX_STRUCT(char) char_matrix ;
#define MATRIX_STRUCT(T_) \
    struct                \
    {                     \
        unsigned rows;    \
        unsigned cols;    \
        T_ data[];        \
    }

// usage:
// char_matrix_type *proto = new_matrix(3, 4);
// MATRIX_ALIAS(matrix, char, proto->cols);
// now matrix is a pointer to the array
// inside the struct from above
#define MATRIX_ALIAS(N_, T_, C_) \
    typedef T_(*N_)[C_]

// usage:
// MATRIX_CAST(mx,matrix,proto);
// mx[1][1] = 'x' ;
#define MATRIX_CAST(N_, A_, M_) \
    A_ N_ = (A_) & (M_->data)

/* --------------------------------------------------- */
typedef MATRIX_STRUCT(char) char_matrix_type;

static char_matrix_type *new_matrix(const unsigned R_, const unsigned C_)
{
    char_matrix_type *retval = calloc(1,
                                      sizeof(char_matrix_type) + sizeof(char[R_ * C_]));
    if (retval)
    {
        retval->rows = R_;
        retval->cols = C_;
    }
    return retval;
}
/* --------------------------------------------------- */
int investigate_matreix(const int argc, char **argv)
{
    char_matrix_type *proto = new_matrix(3, 4);

    MATRIX_ALIAS(matrix, char, proto->cols);

    // MATRIX_CAST(mx,matrix,proto);
    // or by hand
    matrix mx = &(proto->data);

    mx[1][1] = 'x';

    return EXIT_SUCCESS;
}

/* ---------------------------------------------------
https://www.youtube.com/watch?v=WDIkqP4JbkE 02:07
*/

typedef enum
{
    row_major,
    column_major
} traversal_order;

/*
sum up chars into an int rezult, just for fun and profit
*/
static int sum_matrix(
    const unsigned rows,
    const unsigned cols,
    char m[static rows][cols], traversal_order order)
{
    int sum = 0;

    if (order == row_major)
    {
        for (unsigned r = 0; r < rows; ++r)
            for (unsigned c = 0; c < cols; ++c)
                sum += m[r][c];
    }
    else
    { // column_major
        for (unsigned c = 0; c < cols; ++c)
            for (unsigned r = 0; r < rows; ++r)
                sum += m[r][c];
    }

    return sum;
}

enum
{
    BENCH_MATRIX_ROWS = 0xFFFF,
    BENCH_MATRIX_COLS = 0xFF,
};

char_matrix_type *the_matrix(void)
{
    static char_matrix_type *proto = 0;
    if (!proto)
    {
        proto = new_matrix(BENCH_MATRIX_ROWS, BENCH_MATRIX_COLS);

        if (!proto)
        {
            perror("Matrix Allocation Failed!");
            exit(EXIT_FAILURE);
        }
    }
    return proto; // can be NULL if ENOMEM!
}

__attribute__((constructor)) static void announce_the_matrix(void)
{
    // this will make it once
    char_matrix_type *proto = the_matrix();

    UBUT_WARN("\nMatrix struct is allocated:\n\n"
              "struct                     \n"
              "{                          \n"
              "   unsigned rows; //%5d    \n"
              "   unsigned cols; //%5d    \n"
              "   char data[%5d];         \n"
              "}\n",
              BENCH_MATRIX_ROWS, BENCH_MATRIX_COLS, BENCH_MATRIX_ROWS * BENCH_MATRIX_COLS);
}

__attribute__((destructor)) static void release_the_matrix(void)
{
    char_matrix_type *proto = the_matrix();
    free(proto);
}

UBENCH(traversals, rows_major)
{
    char_matrix_type *proto = the_matrix();
    sum_matrix(proto->rows, proto->cols, &proto->data, row_major);
}

UBENCH(traversals, columns_major)
{
    char_matrix_type *proto = the_matrix();
    sum_matrix(proto->rows, proto->cols, &proto->data, column_major);
}
