# infinifractal

https://infinifractal.hilarylau.io

A Mandlebrot set fractal viewer in C++

https://github.com/user-attachments/assets/288905e8-af30-48d5-953c-795411c5eebf

#### Implementation

The graphics is handled by SDL2. On every rerender, we lock the pixel buffer, update all the pixels with the new colors, and then unlock it.

For the calculation of the iterations under the Mandlebrot scheme, we set the divergence limit to be 2.0 and count the iterations. The maximum
iterations we allow is 500 for performance issues. However, integer iterations cause banding in the colors. Therefore, we use a estimation formula 
as described in [this article](https://rubenvannieuwpoort.nl/posts/smooth-iteration-count-for-the-mandelbrot-set) that allows for fractional iteration 
estimates, since the number of iterations under the scheme is actually continuous.

Previously, there was a bottleneck in the program due to the rounding performed while converting from HSL to RGB, which caused color banding. To prevent that, we created
a function that cycles through colors directly in RGB using a sine function and appropriate phase shifts for each color.

We also implemented WebAssembly support through `index.html`, with proper thread usage. Since browser resources are much more limited, we only allow 100
maximum iterations of the Mandlebrot scheme, which may decrease the quality of the resultant image.

#### Runtime

The original implementation is single threaded, naively rendering one pixel at a time. The draw time of a single frame is around `5000ms`.

The current version parallelizes the process using a threadpool implemented in `Threadpool.cpp`. Each horizontal line of pixels is handled by a separate thread.
With threading, the draw time of a frame is around `925ms`, indicating a 5.4x speedup over the single threaded version.

#### Limitations & Future Improvements

- Hardware acceleration like CUDA could potentially allow for performance improvements
- We might want to avoid manipulating pixels directly for performance
- We could consider using a Automatic Dwell Limit algorithm to improve framerate.

#### Installation

##### GUI

Depends on SDL2. Run `brew install sdl2`.

To install, run `make` and then the application can be started by running `./infinifractal`.

##### WASM

If you want to run this on a browser using WebAssembly, run `make wasm` and then `./corsserver.py` to serve the files with the correct CORS headers. Then
access `localhost:8000` to see the fractal.
