# Nightmare
Physics engine for physics-based animation, used with Couldron to do fancier rendering. Later on, this will have a server to pre-bake animations.

## Compiling
We use CMake, all the dependencies are self contained so you shouldn't need anything besides OpenGL installed.
```bash
$ mkdir build && cd build
$ cmake ..
$ make -jX
$ ./build/nightmare
```

This has pre-configured CLion and VScode builds as well, just open the project in either and click the run button.
