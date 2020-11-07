# Final Project: Python Library for reading and writing off files

## What and why?

#### python c interface
- read off file
- https://scipy-lectures.org/advanced/interfacing_with_c/interfacing_with_c.html
- return tuple of np.ndarray: vertices and triangles
- option: vertices as 32 or 64 bit array
- options: python c api, ctypes, swig, cython
- too difficult?

#### off file volume

- read off file
- use psb data
- handle strange format (missing newline)
- check if consistently oriented and watertight
- compute volume (if possible) and surface

#### convert polyons to triangles

- off files
- simple algorithm: [0, 1, 2, 3, ..., n-1] -> 
  [0, 1, 2], [0, 2, 3], [0, 3, 4], ... [0, n-2, n-1]

## What is an off file?

http://www.holmes3d.net/graphics/offfiles/offdesc.txt

https://en.wikipedia.org/wiki/OFF_(file_format)


## How to make python extensions in C

## How to make numpy extensions in C

## How to make unit tests

## How to construct a package

## How to make that package available to others