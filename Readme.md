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
- Ninja build
### Dependencies
**!! Currently I haven't found a way to download the following dependencies automatically !!**
- zlib
- libpng
- bzip2
- brotli

*I am quite sure most linux distros come with these preinstalled, or can be easily installed as a dependency of Freetype, I've run into the problem on windows only*

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

## Using the engine
### Code organization
Alright, so you actually want to work with this, great. Here's how you do that:

The `engine/` folder contains code related to how **the engine actually works**. My naming convention is quite simple, `ptc` stands for petrichor, and then the class name it manages. If you want to add your own functionality (mostly dev tools), it's done in this folder. For now there's no documentation, so you'll sadly need to dig through the engine if you need something, but for now it's quite small. The code inside here should ***never need to access code from the `scripts/` folder***. Speaking of,

The `scripts/` folder contains the ***game-related scripts*** and logic, and is completely separate from the dev tools the engine provides. These scripts should be the ones that make the game you're developing work\
The structure for that is the following:
```
scripts/
  ├── include/      # Header files
  ├── src/          # Source files
  ├── standalone/   # Standalone source files
  ├── game_main.cpp # Acts as a bootstrap
```
You will need a way to set your objects up. Currently (since scenes are not implemented yet), `game_main.cpp` will act as a "bootstrap", or basically your scene. Here you can create your objects, order them in the hierarchy, and similar things\
Think of it as a code-based way to describe a hierarchy

For general development it's recommended to **split your code into header and source files**, since that's the convention for C++\
The **option to only write standalone source files** is still **available**, just for the sake of people who wish to work that way

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
