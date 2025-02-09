# infinifractal

A Mandlebrot set fractal viewer

![Demo](demo.png)

#### Runtime

The current implementation is single threaded, and naively rendering one pixel at a time. The draw time is around `5000ms`.

### Installation

Depends on SDL2. Run `brew install sdl2`.

To install, run `make` and then the application can be started by running `./app`.