Flat profile:

Each sample counts as 0.01 seconds.
  %   cumulative   self              self     total           
 time   seconds   seconds    calls  ms/call  ms/call  name    
100.09      0.06     0.06        2    30.03    30.03  mandelbrot
  0.00      0.06     0.00  2880000     0.00     0.00  my_pixel_put
  0.00      0.06     0.00       71     0.00     0.00  arena_alloc
  0.00      0.06     0.00        2     0.00     0.00  colors
  0.00      0.06     0.00        2     0.00    15.01  key_hook
  0.00      0.06     0.00        1     0.00     0.00  arena_destroy
  0.00      0.06     0.00        1     0.00     0.00  arena_init
  0.00      0.06     0.00        1     0.00     0.00  calc_log
  0.00      0.06     0.00        1     0.00     0.00  freeall
  0.00      0.06     0.00        1     0.00     0.00  getlist
  0.00      0.06     0.00        1     0.00    30.03  main_mandelbrot

 %         the percentage of the total running time of the
time       program used by this function.

cumulative a running sum of the number of seconds accounted
 seconds   for by this function and those listed above it.

 self      the number of seconds accounted for by this
seconds    function alone.  This is the major sort for this
           listing.

calls      the number of times this function was invoked, if
           this function is profiled, else blank.

 self      the average number of milliseconds spent in this
ms/call    function per call, if this function is profiled,
	   else blank.

 total     the average number of milliseconds spent in this
ms/call    function and its descendents per call, if this
	   function is profiled, else blank.

name       the name of the function.  This is the minor sort
           for this listing. The index shows the location of
	   the function in the gprof listing. If the index is
	   in parenthesis it shows where it would appear in
	   the gprof listing if it were to be printed.

Copyright (C) 2012-2020 Free Software Foundation, Inc.

Copying and distribution of this file, with or without modification,
are permitted in any medium without royalty provided the copyright
notice and this notice are preserved.

		     Call graph (explanation follows)


granularity: each sample hit covers 2 byte(s) for 16.65% of 0.06 seconds

index % time    self  children    called     name
                0.03    0.00       1/2           main_mandelbrot [3]
                0.03    0.00       1/2           key_hook [2]
[1]    100.0    0.06    0.00       2         mandelbrot [1]
                0.00    0.00 2880000/2880000     my_pixel_put [6]
-----------------------------------------------
                0.00    0.03       2/2           mlx_loop [5]
[2]     50.0    0.00    0.03       2         key_hook [2]
                0.03    0.00       1/2           mandelbrot [1]
                0.00    0.00       1/1           freeall [12]
                0.00    0.00       1/2           colors [8]
-----------------------------------------------
                0.00    0.03       1/1           main [4]
[3]     50.0    0.00    0.03       1         main_mandelbrot [3]
                0.03    0.00       1/2           mandelbrot [1]
                0.00    0.00       1/1           getlist [13]
                0.00    0.00       1/2           colors [8]
                0.00    0.00       1/1           calc_log [11]
-----------------------------------------------
                                                 <spontaneous>
[4]     50.0    0.00    0.03                 main [4]
                0.00    0.03       1/1           main_mandelbrot [3]
                0.00    0.00       1/1           arena_init [10]
-----------------------------------------------
                                                 <spontaneous>
[5]     50.0    0.00    0.03                 mlx_loop [5]
                0.00    0.03       2/2           key_hook [2]
-----------------------------------------------
                0.00    0.00 2880000/2880000     mandelbrot [1]
[6]      0.0    0.00    0.00 2880000         my_pixel_put [6]
-----------------------------------------------
                0.00    0.00       1/71          calc_log [11]
                0.00    0.00      28/71          getlist [13]
                0.00    0.00      42/71          colors [8]
[7]      0.0    0.00    0.00      71         arena_alloc [7]
-----------------------------------------------
                0.00    0.00       1/2           main_mandelbrot [3]
                0.00    0.00       1/2           key_hook [2]
[8]      0.0    0.00    0.00       2         colors [8]
                0.00    0.00      42/71          arena_alloc [7]
-----------------------------------------------
                0.00    0.00       1/1           freeall [12]
[9]      0.0    0.00    0.00       1         arena_destroy [9]
-----------------------------------------------
                0.00    0.00       1/1           main [4]
[10]     0.0    0.00    0.00       1         arena_init [10]
-----------------------------------------------
                0.00    0.00       1/1           main_mandelbrot [3]
[11]     0.0    0.00    0.00       1         calc_log [11]
                0.00    0.00       1/71          arena_alloc [7]
-----------------------------------------------
                0.00    0.00       1/1           key_hook [2]
[12]     0.0    0.00    0.00       1         freeall [12]
                0.00    0.00       1/1           arena_destroy [9]
-----------------------------------------------
                0.00    0.00       1/1           main_mandelbrot [3]
[13]     0.0    0.00    0.00       1         getlist [13]
                0.00    0.00      28/71          arena_alloc [7]
-----------------------------------------------

 This table describes the call tree of the program, and was sorted by
 the total amount of time spent in each function and its children.

 Each entry in this table consists of several lines.  The line with the
 index number at the left hand margin lists the current function.
 The lines above it list the functions that called this function,
 and the lines below it list the functions this one called.
 This line lists:
     index	A unique number given to each element of the table.
		Index numbers are sorted numerically.
		The index number is printed next to every function name so
		it is easier to look up where the function is in the table.

     % time	This is the percentage of the `total' time that was spent
		in this function and its children.  Note that due to
		different viewpoints, functions excluded by options, etc,
		these numbers will NOT add up to 100%.

     self	This is the total amount of time spent in this function.

     children	This is the total amount of time propagated into this
		function by its children.

     called	This is the number of times the function was called.
		If the function called itself recursively, the number
		only includes non-recursive calls, and is followed by
		a `+' and the number of recursive calls.

     name	The name of the current function.  The index number is
		printed after it.  If the function is a member of a
		cycle, the cycle number is printed between the
		function's name and the index number.


 For the function's parents, the fields have the following meanings:

     self	This is the amount of time that was propagated directly
		from the function into this parent.

     children	This is the amount of time that was propagated from
		the function's children into this parent.

     called	This is the number of times this parent called the
		function `/' the total number of times the function
		was called.  Recursive calls to the function are not
		included in the number after the `/'.

     name	This is the name of the parent.  The parent's index
		number is printed after it.  If the parent is a
		member of a cycle, the cycle number is printed between
		the name and the index number.

 If the parents of the function cannot be determined, the word
 `<spontaneous>' is printed in the `name' field, and all the other
 fields are blank.

 For the function's children, the fields have the following meanings:

     self	This is the amount of time that was propagated directly
		from the child into the function.

     children	This is the amount of time that was propagated from the
		child's children to the function.

     called	This is the number of times the function called
		this child `/' the total number of times the child
		was called.  Recursive calls by the child are not
		listed in the number after the `/'.

     name	This is the name of the child.  The child's index
		number is printed after it.  If the child is a
		member of a cycle, the cycle number is printed
		between the name and the index number.

 If there are any cycles (circles) in the call graph, there is an
 entry for the cycle-as-a-whole.  This entry shows who called the
 cycle (as parents) and the members of the cycle (as children.)
 The `+' recursive calls entry shows the number of function calls that
 were internal to the cycle, and the calls entry for each member shows,
 for that member, how many times it was called from other members of
 the cycle.

Copyright (C) 2012-2020 Free Software Foundation, Inc.

Copying and distribution of this file, with or without modification,
are permitted in any medium without royalty provided the copyright
notice and this notice are preserved.

Index by function name

   [7] arena_alloc             [8] colors                  [3] main_mandelbrot
   [9] arena_destroy          [12] freeall                 [1] mandelbrot
  [10] arena_init             [13] getlist                 [6] my_pixel_put
  [11] calc_log                [2] key_hook
