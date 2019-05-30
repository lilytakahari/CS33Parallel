# CS 33 Parallel Lab
~~Suffering for the sake of optimization.~~ Learning how to optimize and parallelize code using OpenMP.
### How to run the code:
1. Download the files into an empty directory.
2. `make` to compile
3. `./Test` to run

`make clean` to delete everything but the source files

### What the code does:
__In main.c:__ Initializes all elements of a 3D array to a random value from 0 to 999.

__In the `work_it` function:__ 
1. Avoiding the outer shell of the original 3D array/cube, for each element in that 3D array: multiplies the element by the value returned from `we_need_the_func()`, then divides by the value returned from `gimmie_the_func()`. Adds the resulting value to `aggregate`. _Essentially: Do some math to each element in the array, and add it all together._
2. Adds all the values of a 3x3 cubic chunk of the original 3D array to the element in the new 3D array that is the center of that 3x3 cube, coordinate-wise. Then divides that new array's value by 27. _Essentially: Construct a new 3D array that consists of the averages of every 3x3 chunk in the original._
3. Avoiding the outer shell of the new 3D array, divides each element in the new 3D array by 100. Based on that, increments the value in an array that represents a histogram. _Essentially: Construct a histogram based on the hundreds digit of the averages calculated in step 2._

### Optimization techniques used
In a subjective order of how much they contributed to speed-up, from most to least effective:

* OpenMP directives
* efficient memory access and caching
* loop blocking/tiling 
* reduction in operation strength
* loop unrolling
* accumulator variables
* code motion
