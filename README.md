# sfml-flecs
A basic, lightweight, 2D game engine that uses SFML for rendering and FLECS for the ECS.

I eventually got tired of rewriting the same base, so this is a little implementation of a game engine structure that runs on top of SFML. *This is not meant to be a standalone framework*, instead it is a base upon which a game can be built. Because of that, it might make more sense to fork this repository into the repository of your game as opposed to utilizing it as a submodule.

## Current State
Currently, there's a mac branch, which is the code that only works on mac compiling with clang. When I get some time I'll do a windows branch and eventually merge everything into main when it's cross platform.