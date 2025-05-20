# µgame
A small game-esq thing written in modern C++23.

This is a cleaned up version of a game I'm building [live](https://www.youtube.com/watch?v=TwW9UPc8ImU&list=PLmm8GpQIMryC1Lm_YgphPgzwPue5RAZdi). It has better tests and docs as well as some QoL fixes. It is however functionally behind the live one.

## Features
- 3D rendering using OpenGL
- Physics with Jolt
- HDR
- Lua scripting

## Building
This has only been built and tested on Windows using msvc.

```
cd build
cmake ..
cmake --build .


ctest .
```

## Running
You will need to build the resource pack before running.

```
cd build
./tools/resource_packer/resource_packer.exe ../assets/ ./resource
```

After that you can run the game with:

```
cd build
./src/game.exe ../assets/
```

The resource file needs to be in the same directory as the shader, until they get added to the resource pack.

## Help?
Feel free to join my [discord](https://discord.gg/9FkkMgXSUV) if you have any questions
