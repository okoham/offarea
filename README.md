# Final Project: Area of 3D shapes

This is a small C program that calculates the surface area of a three
dimensional shape, either a suface or a closed volume.
The geometry of the shape is given in [OFF](https://en.wikipedia.org/wiki/OFF_(file_format)) file format.

_USAGE_:

`offarea tests/cube4.off`

_OUTPUT_:

`area: 6.000000` 

There's also a little video [here](https://youtu.be/YBKPEIJwl9c). 

## What and why?

There are a lot of tools and libraries out there that perform this task
much better than this little tool, e.g. [MeshLab](https://www.meshlab.net/), 
[PyMesh](https://github.com/PyMesh/PyMesh), [VCGLib](http://vcglib.net/), 
or [Open3D](http://www.open3d.org/). 
However, I wanted to improve my C pointer skills in this final project.

OFF files are text files describing three dimensional geometry. 
The geometric shape is represented as a collection of polygons. 
A very simple example looks like this: 

```
OFF
4 2 0
0.0 0.0 0.0
1.0 0.0 0.0
1.0 1.0 0.0
0.0 1.0 0.0
3 0 1 2
3 2 3 0
```

The second line defines the number of vertices, faces, and edges.
The next four lines contain the vertex coordinates.
The last two lines define the connectivity of the polygons. 
The first number defines the number of vertices in the polygon, followed by 
the indices into the coordinate array. In the example, all polygons are 
triangles.

The file format allows for other polygon shapes than triangles. 
The simple shape above (it's a square) can also be defined with rectangle 
instead of two triangles, like shown below. Different polygon types can be 
mixed in one file.

```
OFF
4 1 0
0.0 0.0 0.0
1.0 0.0 0.0
1.0 1.0 0.0
0.0 1.0 0.0
4 0 1 2 3
```

This freedom means that we cannot define a fixed data structure for the 
indices. That's what I found interesting to implement in C.

