# RubikCubeSolve
A Rubik's Cube solver

## The Solver (v5) ##
Compile with MinGW C compiler  
`$ gcc magicCubeSolver_p1table.c –o magicCubeSolver_p1table.exe`  
`$ gcc magicCubeSolver_v5.c –o magicCubeSolver_v5.exe`

Run p1table before running v5 solver.  
(1.1GB memory and disk space is required.)  
`$ magicCubeSolver_p1table.exe`  

Run the v5 solver and input the state of a cube in the following order:  
`$ magicCubeSolver_v5.exe`, and then  

1.  encode the six color with 1~6  
2.  put the cube in front of you  
3.  do the following steps three times  
  1.  input the colors of facelets of the current top face of the cube  
NOTICE: To input the colors of facelets on a face of the cube, please input in the order:  
1 2 3  
4 5 6  
7 8 9  
  2.   rotate the cube so that the right face become the top and the top face become facing to you  
(rotate the cube counter-clock-wise by 120deg according to the upper right corner near to you )
4.  rotate the cube so that the face facing to you become the bottom and the bottom face become facing to you  
(swap the bottom face and the face facing to you)
5.  do the step 3 again

## The Generator ##
You can use the generator to generate a cube in a random state and get its state description in the input format of the solver.
`$ gcc magicCubeGenerate.c –o magicCubeGenerate.exe`  
`$ magicCubeGenerate.exe`  
* To set the state of the cube:  
  type `input` or `i`, and then type the state of a cube in the format described above.  
* To randomly rotate the faces of the cube:  
  type `random ` followed by a number of steps.  
* To rotate the faces in a specific way:  
  type `(U|D|L|R|F|B)('|2)?`  
  examples:  
  *  `U`,    rotate the top face clock-wise;  
  *  `L'`,   rotate the left face clock-wise;  
  *  `F2`,   rotate the front face(facing to you) twice;  
* To reset the state of the cube to initial state:  
  type `reset`.  
* To print the state of the cube in the format of the solver:  
  type `print` or `p`.  
* To quit:  
  type `quit` or `q`.  
  
## Reference ##
Cube Explorer: http://kociemba.org/
