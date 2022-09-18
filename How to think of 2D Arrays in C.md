
# How to think of 2D Arrays in C

2D array is array of arrays. What does that mean? You have a declaration in your code:
```cpp
int arr[2][3];
```
Compiler does not know or care about rows and columns. If compiler could talk, it would explain the above declaration as:
```
2 times array of 3 integers, for which I have reserved, on the stack, 2 times 3, times size of a integer, contiguous memory block.
```
Thus we would know the compiler has allocated, on the stack
```
 2 x 3 x sizeof(int) = 24 bytes
```
That is 6 x 4, aka 24 bytes. As 4 is size of int in bytes. Memory block of 24 bytes.
```
Memory block of 24 bytes to store 6 integers
+-----+-----+-----+-----+-----+-----+-----+-----+-----+
|  0  |  1  |  2  |  3  |  .  |  .  |  .  |  22 |  23 |
+-----+-----+-----+-----+-----+-----+-----+-----+-----+
```
That is a memory allocated for int[2][3] aka int[6].

Fortunately we are not coding in assembler and we can stay on the higher level of abstraction than hardware is. We just need to know compiler has allocated an array of 6 integers as a result of that declaration.
```
Array of 6 integers
+-----+-----+-----+-----+-----+-----+
|  0  |  1  |  2  |  3  |  4  |  5  |
+-----+-----+-----+-----+-----+-----+
```
Compiler is mapping that array to a memory block above. The best way to level all that in your head is to code the above 2D array initialization in one line:
```cpp
//              two times array of 3 int's, aka 6 int's
//              this takes 24 contiguous bytes in memory
int arr[2][3] = { {1,2,3}, {4,5,6} } ;
```
But we need to make a slight detour ti try and explain why you should not think in "rows" and "columns".
## Slight detour into N dimensional space
Humans usually feel it "natural" to think of the above as "two rows of three int's", and thus to print the above as:
```
 1 2 3
 4 5 6
```
Hence the terms "rows" and "columns". For 2D arrays this might be fine but for 3 or more "dimension" this is definitely not "fine".
```cpp
// initialize this ?
int arr[2][2][2] ;
```
That is 2 x 2 x 2 = 8 integers. Just start from the right. In human language above is : Array of two int's, repeated twice and then all of that repeated twice. In some form of pseudo code that is:
```cpp
//       read from right
//       twice        twice      array of two int's
          2 *      [   2 *       int[2]             ]
```
Now we can transform that into C declaration and initialization. It might be easier if you write this from "inside". Starting from `{1,2}` and so on.
```cpp
int arr3D[2][2][2] =  
    { 
        { // once 
            {1,2}, // array of two int's
            {3,4}  // second  array of two int's
        } ,   
        { // twice 
            {5,6}, // array of two int's
            {7,8}  // second  array of two int's
        }    
    } ;   
```
Some humans find it (much) easier to say: "two levels of int[2][2] matrices". But what about this completely legal C:
```cpp
// what is your analogy for this structure?
int arr[2][2][2][2] ; 
```
Don't worry we will stop here. The point was to try and explain why is it not a "good thing" just to think in term of "rows" and "columns". Try to think of matrices as compiler "thinks". Number of 1D arrays.

### Using the matrix values

How do we use 2D array in C. And how does that work.

```cpp
//              two times array of 3 int's
int arr[2][3] = { {1,2,3}, {4,5,6} } ;
```
Again your friend compiler allows you to use the matrix values in a "natural way", by addressing the cells with their indexes.
```cpp
   printf("\n%d", arr[1][1] )
```
Compiler maps above to the required value from the internal 1D array representation.

Above print's the second value aka  "element 1" from the second (of two) int[3] arrays. Yes, those humans again. They find it much easier to think of the above as the spreadsheet aka table

|  arr[2][3] | 0  | 1  | 2 | columns
|------------|----|----|---|--------
|  row 0     | 1  | 2  | 3 |
|  row 1     | 4  | 5  | 6 |

Indexing goes from 0 as in C of course. Thus `arr[1][1]` is 5.

Please do remember. `int[2][3]` is mapped to the memory block of 24 bytes.
```
Memory block of 24 bytes keeps 6 int's from int[2][3]
+-----+-----+-----+-----+-----+-----+-----+-----+-----+
|  0  |  2  |  3  |  4  |  .  |  .  |  .  |  22 |  23 |
+-----+-----+-----+-----+-----+-----+-----+-----+-----+
```
Compiler does the "rewiring" and you do not have to worry (just a little), about a direct memory addressing. 

But. How do we allocate large 2D arrays? Stack allocation will fail rather quickly. You can try. Thus we need to use the heap. 

## Allocated memory has no effective type

That is very important concept. Memory block above has no type info attached to it. It is just 24 bytes or RAM.  Upon memory allocation program dictates what type will be mapped to the allocated memory. In C you do not need to. But here is why you want to do that.

```cpp
// just allocate a block of 24 bytes and give it no type
// block has no effective type
// malloc returns void pointer
void * block = malloc( 24 ) ;

// here is how you "put" int 64 to that block
// starting from the "left" end of the block
*((int*)(block + 0)) = 64 ;

// and here is the same operation if you have 
// given the effective type to the block allocated
int * int_block = malloc( 6 * sizeof(int) ) ;
int_block[0] = 64 ;
```
In C we can code the above in many different ways in regards what program needs to do. Things are becoming interesting.

The following section is taken completely [from Stack Overflow](https://stackoverflow.com/a/32050859/10870835). Caution, it is sharp and short. Just what you need :) The question to this answer was: "How to allocate the 2D array"?

# [Allocating 2D Arrays](https://stackoverflow.com/a/32050859/10870835) 

In C never use a pointer-to-pointer to allocate multi-dimensional arrays. It is wide-spread but bad and incorrect practice. Doing so will not give you a true 2D array, and it will lead to slower code because of heap fragmentation. It also makes the code harder to write, read and maintain, which in turn increases the potential for memory leaks.

Instead, allocate the 2D array correctly in adjacent memory cells, like this:
```cpp
int x;
int y;

// store some values in x and y here

int(*matrix)[y] = malloc (sizeof(int[x][y]));

if(matrix == NULL)
{
  // error handling here
}

matrix[i][j] ; // do something with the matrix

free(matrix);
```
If you insist on keeping this code in a function, then it would be:
```cpp
// NOTE: I have changed this function slightly
void* allocMatrix (int nRow, int nCol, size_t typesize )
{
  // return malloc (sizeof(int[nRow][nCol]));
  // calloc is better, it zeroes the memory allocated
  return calloc ( nRow * nCol, typesize);
}

// NOTE: I have added the third argument
// NOTE: you need to understand fully this line
int(*matrix)[y] = allocMatrix(x, y, sizeof(int) );
```

## Explanation of the code and array pointers.

In the line (commented out):
```cpp
 int(*matrix)[y] = malloc (sizeof(int[x][y])); 
 ```
 the `sizeof(int[x][y])` is pretty straight-forward, it is just the size of a 2D array of integers with dimensions x*y. It is using the concept of variable-length arrays from the C99 standard, which allows array dimensions to be specified in runtime.

(Ie have replaced that with the `calloc()` call, as explained in the code.)

An array pointer is a somewhat special type in C, it is able to point to whole arrays, rather than just at the first item of the array, as a normal pointer will do. The array pointer, unlike a regular pointer, knows the size of the array.

An array pointer is written as `type(*name)[size]`, so for example an array pointer to an array of 5 int's would be written as `int(*arr_ptr)[5] = &the_array;`.

When accessing the contents pointed at, an array pointer behaves just as any pointer, you access the contents of it with *. So `*arr_ptr` gives the array pointed at, and `(*arr_ptr)[0]` gives the first item of that array.

For multi-dimensional arrays, the same rules apply. Given the array int `arr[x][y]`, an array pointer to this type will be `int(*arr_ptr)[x][y] = &arr;`. Accessing the contents `*arr_ptr` will give you a two-dimensional array, which is equivalent to an array of arrays. `(*arr_ptr)[0]` will therefore give the first array in the array of arrays. The usual rule for any array name when used in an expression, is that it "decays" into a pointer to the first element. Same applies here, so `(*arr_ptr)[0]` will therefore also be the same as a pointer to the first element in the first array. And `(*arr_ptr)[0][0]` will give the first element of the first array.

Now this syntax `(*arr_ptr)[0][0]` looks a bit hard to read; to get the first item of a 2D array, we are used at writing just `arr[0][0]`. 

### So when declaring the array pointer, there is a handy trick.

Instead of declaring the complete and correct array pointer: `int(*matrix)[x][y]`, an array pointer to a 2D array of dimensions `x*y`, we instead declare it as `int(*matrix)[y]`, which is an array pointer to a 1D array with dimension y. It will point at the first item in the 2D array, which is a 1D array of size y. We know that the 2D array contains x such items.

And because of this trick, we'll now be able to use the array pointer with the same syntax as when accessing a 2D array, namely `matrix[i][j]`, rather than the hard-to-read `(*matrix)[i][j]`.

In case you would like to read another (much longer) answer on the same subject from the same author please [proceed here](https://stackoverflow.com/a/42094467/10870835).

# An Explanation of the above

Phew! That was a short and hard kick directly from the deep basement levels of C. And that is all correct. That is how you too, will code 2D arrays in C. Here is why and how.

## Array Pointer

That is a key C programming concept here. Lets re-use our int[2][3] from above.
```cpp
// array declaration and initialization
// this is two times int[3]
int arr[2][3] =  { {1,2,3}, {4,5,6} } ;

// array pointer declaration
int (*arp)[2][3];

// pointing to the whole int[2][3]
arp = &arr ;
```
Visually:
```
 +--- int arr[2][3] ---------------------+
 |                                       |
 | |     int[3]      |      int[3]     | |
 | +-----+-----+-----+-----+-----+-----+ |
 | |  0  |  1  |  2  |  3  |  4  |  5  | |
 | +-----+-----+-----+-----+-----+-----+ |
 +---------------------------------------+
     ^
     |
     + -- int (*arp)[2][3]
```
Understand that diagram and you are done here. Almost. Just the next "trick"to be explained. How do we index the above block allocated in the "natural" way?  Aka `matrix[x][y]` ?  With that `arp` above,as declared, we need to do `(*arp)[x][y]` to reach the value x,y in that 2D array.

Let's build up to the solution. Let's use another pointer to the same block from above.
```cpp
// the pointer to the first 1D array int[3]
// and nothing else but the first int[3]
// not good, we can not reach the second int[3]
int(*matrix)[3] = arp[0] ;

// the pointer to the first 1D array int[3]
// of the whole of int[2][3]
// the casting is necessary 
// this is what we want, after this we can say: matrix[1][1]
int(*matrix)[3] = (int(*)[3])arp ;

// but why not using this as "effective" type
// of the block allocated
// no casting is necessary
int(*matrix)[3] = malloc( sizeof(int[2][3]) ) ;
```
To paraphrase the author above: `matrix` will point at the first item in the 2D array, which is a 1D int array of size 3. And. We know that the 2D array contains 2 such items, 0 and 1. 

Namely : `matrix[0]` and `matrix[1]`. `matrix` is a real pointer, not an array decayed into pointer.

That non-trivial situation is best understood when visualised. (note: `0 .. 5` are indices not values)

```
         + -- int (*arp)[2][3]
         V
 +------------- int arr[2][3] -----------+
 |                                       |
 | |     int[3]      |      int[3]     | |
 | +-----+-----+-----+-----+-----+-----+ |
 | |  0  |  1  |  2  |  3  |  4  |  5  | |
 | +-----+-----+-----+-----+-----+-----+ |
 | |   matrix[0]     |     matrix[1]   | |
 +---------------------------------------+
        ^
        + int(*matrix)[3]
```
Let us repeat the key line once more:
```cpp
int(*matrix)[3] = malloc( sizeof(int[2][3]) ) ;
```
One more trick. In C the casting of the malloc result is not required. Thus we can do the above without compiler warnings. We have given the "effective type" to the allocated block.

Since matrix is the real pointer pointing to the above structure we can use the indexing operators on it.

```cpp
// we have initialized with
// { {1,2,3}, {4,5,6} } 
// matrix[0] is the first 1D array: int[3]
// thus
    matrix[0][1] ; // equals 2
```

## Excursion for the reader

By far the best course of action is to employ your good IDE and debugger and follow slowly this little program:

```cpp
// (c) 2021 by dbj at dbj dot org
// https://dbj.org/license_dbj
// https://godbolt.org/z/rzzaPMPah
#include <assert.h>
#include <stdlib.h>
#include <string.h>

int main(void) 
{
// 2D array declaration and initialization, on stack
int arr[2][3] =  { {1,2,3}, {4,5,6} } ;

typedef unsigned char uchar ;
// pointer to the first byte of the underlying memory block
uchar *  block = (uchar*)&arr ;
// pointer to the third int
uchar * int_no_3_ptr = block + (2 * sizeof(int)) ;
// the value is 3
int int_no_3 = (int)*int_no_3_ptr;

// regular pointer to the whole of int[2][3]
int (*arp)[2][3] = malloc( sizeof(int[2][3]) ) ;

// pointing to the whole of the: int arr[2][3]
// has to be casted
int(*matrix)[3] = (int(*)[3])arp ;
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
// as mem block to mem block
// source is arr[2][3] 
memcpy( arp, &arr, sizeof(int[2][3]) ) ;

assert( matrix[0][1] == 2 );

free( matrix ) ;    
    return 42;
}
```

I think we are getting somewhere. Well done,