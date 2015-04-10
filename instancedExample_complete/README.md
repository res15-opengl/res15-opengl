# Instancing Example

## Motivation

This example demonstrates how to offload shedloads of calculations to the vertex shader, whilst keeping the GPU happy.

Instanced drawing is one technique to optimise your draw calls. If you have to draw the same object multiple times, and you can express the difference between objects either through uniforms, attributes or texture data, consider using instancing.

# Main technique

This example goes through the necessary steps of setting up instanced drawing. Most of the magic happens in the vertex shader. Although the example focuses on geometry, it opens up lots of possiblities for you to tinker with the look & feel, since the fragment shader stage is empty apart from default methods.

## Sub-topics 

	* Discuss overdraw, and how bad it is for performance
	* Discuss matrix transforms, and how to write your own translation matrix.
	* Discuss parametric animation using envelope functions, and a time uniform.
	* Discuss correct pixel sampling - and the half-pixel offset
	* Discuss backface culling and glFrontFace
	* Discuss z-buffering - and early z-test
	* Demonstrate visual debugging

# Common patterns
	
	* "Dirty Shader"
	* oF shader includes
	* if(false) blocks

# Note

This project has been set up so that you can trace back the steps of how it was written. Within `ofApp.cpp`, you'll find blocks of code introduced with  `if (false)` or `if (true)` conditionals. Each block is a step in creating the final project. 

Within a scope, only one of these blocks should be active. The active block should be the one with `if(true)` at the top. Make sure the other blocks are inactive by keeping them `if(false)`.

The same principle of scopes which have been switched on or off using if-directives is used in the vertex shader.

In both cases, the scopes which have been "switched off" have no effect on execution speed whatsoever, since they will get optimised out by the compiler.

# Assets

This project contains a 512x512 pixels heightmap "data/elevation.png". This heightmap comes from [NASA data][nasa], and is from an actual place on earth, somewhere on the C1 tile: 

[nasa]: http://visibleearth.nasa.gov/view.php?id=73934
