# Petrichor Engine

**Petrichor Engine** is a general-purpose **2D/3D game engine** mainly made as a hobby project by **yours truly**, since Unity won't run on my laptop properly (2m+ wait times for script compilation, *really?*). **Of course** the only **reasonable** response to that is **making my own engine**.

The main idea behind it is **you can make the game you want** with this engine providing you a **bunch of tools**, and when you are done with the game, you can **just remove the dev tools**, and the game will run by itself! ***How cool is that?!***

***Everything in the resources folder is for testing, and will be removed at a later date, as soon as I'm no longer in need of them! I have not used anything that I have not made, or that isn't freely accessible to anyone online. In case of any problems, feel free to create an issue and I will remove anything requested!***
## Features
There are some, not much for now, will edit this as soon as I get farther :D

## Getting Started
### Prerequisites
- CMake
- C++ Compiler (GCC/Clang/MSVC)
- OpenGL
### Dependencies
The engine has some dependencies, but they should be downloaded upon the first launch in case your system doesn't have them already
- SDL3
- Freetype
- glm
- glad
- stb_image

### Building the Engine
```sh
git clone https://github.com/rlackbights/petrichor-engine.git
cd petrichor-engine
cmake -S . -B build -G "Ninja"
ninja -C build
```
### Running the Engine
```sh
./bin/petrichor_engine
```

## Roadmap
- [ ] Implement scene saving/loading (most likely using reflection, which will be a pain in the ass)
- [ ] Add UI, so things can be edited in-engine
- [ ] Add an audio engine
- [ ] Implement a physics engine for both 2D and 3D objects
- [ ] Add support for animation and skeletal rigging
- [ ] Implement networking capabilities for multiplayer support
- [ ] Add example projects

### Contributing
I'd like to develop this **alone**, since it's mainly used for **my own improvement**, and also **university**, but **bug reports, and recommendations** to which parts of the engine I should optimise/work on more **are welcome**, but ***I will implement them myself***!

### License
This project is licensed under **Mozilla Public License 2.0 (MPL-2.0)**.