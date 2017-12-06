# CalcGCN
This is a program which calculate the game chromatic number of graphs.

Please be careful that it is very dirty code and is not optimized.

# how to use
This code needs 3 arguments.
  -adjacency_matrix_file
    this is as follow
     0 1 1 0
     1 0 1 1
     0 0 0 1
     0 1 1 0
  -number_of_vertices
  -number_of_colors
  
following is ordering of arguments
$ ./a.out ${adjacency_matrix_file} ${number_of_vertices} ${number_of_colors}
