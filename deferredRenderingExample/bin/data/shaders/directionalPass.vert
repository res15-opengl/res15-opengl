// Filename: directionalPass.vert
// 
// Copyright © James Acres
// http://www.jamesacres.com
// http://github.com/jacres
// @jimmyacres
// 
// Created: Sun Mar  8 11:59:48 2015 (-0400)
// Last-Updated: Thu Apr  2 15:46:22 2015 (-0400)

#version 410 

// OF Attributes
in vec4		position;
in vec2         texcoord;

// outputs
out vec2        vTexCoord;

void main( void )
{
  // outputs
  vTexCoord = texcoord;

  // pass the position straight through as it's already -1.0f to 1.0f
  gl_Position = position; 
}
