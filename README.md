# canvas-drawer

Implements a simple drawing api

TODO: Add a representative image for your project here

## How to build

*Windows*

Open git bash to the directory containing this repository.

```
canvas-drawer $ mkdir build
canvas-drawer $ cd build
canvas-drawer/build $ cmake -G "Visual Studio 16 2019" ..
canvas-drawer/build $ start Draw-2D.sln
```

Your solution file should contain two projects: `pixmap_art` and `pixmap_test`.
To run from the git bash command shell, 

```
canvas-drawer/build $ ../bin/Debug/draw_test
canvas-drawer/build $ ../bin/Debug/draw_art
```

*macOS*

Open terminal to the directory containing this repository.

```
canvas-drawer $ mkdir build
canvas-drawer $ cd build
canvas-drawer/build $ cmake ..
canvas-drawer/build $ make
```

To run each program from build, you would type

```
canvas-drawer/build $ ../bin/draw_test
canvas-drawer/build $ ../bin/draw_art
```

## Supported primitives

- line
- square
- circle

## Results

Implemented unfilled circle using parametric equations. Used the base algorithm to generalize to any parametric equations using std::function. Also made gradients for all primitives.

![box](https://github.com/msunde137/canvas-drawer/blob/master/res/box_curve4.3.png)
![circle](https://github.com/msunde137/canvas-drawer/blob/master/res/extra_circle.png)
![gradcircle](https://github.com/msunde137/canvas-drawer/blob/master/res/gradient_circle.png)
![gradbox](https://github.com/msunde137/canvas-drawer/blob/master/res/gradient_box.png)
![gradlines](https://github.com/msunde137/canvas-drawer/blob/master/res/gradient_lines.png)
