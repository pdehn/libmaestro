# libmaestro

This is a library for controlling Pololu Maestro servo controllers. It attempts to wrap the usage in a more convenient form for C++ projects.

## Build

```sh
$ cd build
$ cmake ..
$ make
$ make install
```

## Basic Use

```c++

#include "maestro.h"

int main(int arc, char* argv[]) {
    // Instantiation will automatically open the first Maestro device found.
    // Support for multiple devices is not yet supported.
    // If no valid device is found, a NoMaestroFound error will be thrown.
    // If a valid device is found but fails to open, a MaestroInitializationError will be thrown.
    Maestro maestro;

    // set maximum speed
    maestro.setSpeed(0 /* servo */, 100 /* speed in 0.25µs/10ms */);

    // set maximum acceleration
    maestro.setAcceleration(0 /* servo */, 255 /* accel in 0.25µs/10ms/80ms */);

    // set position
    maestro.setTarget(0 /* servo */, 4000 /* position in 0.25µs */);

    return 0;
}
```

## License

Significant portions of the code were derived from Pololu's USB SDK; their license is included in [LICENSE-Pololu.txt](https://github.com/pdehn/libmaestro/blob/master/LICENSE-Pololu.txt).

The same license conditions apply to the rest of this code; see [LICENSE.txt](https://github.com/pdehn/libmaestro/blob/master/LICENSE.txt).
