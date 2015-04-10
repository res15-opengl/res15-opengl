// Filename: fullscreenQuad.frag
// 
// Copyright © James Acres
// http://www.jamesacres.com
// http://github.com/jacres
// @jimmyacres
// 
// Created: Fri Apr  3 11:32:46 2015 (-0400)
// Last-Updated: Sat Apr  4 17:44:04 2015 (-0400)

#version 330

// inputs
in vec2 vTexCoord;

// outputs
out vec4 out_color;

void main( void )
{
  out_color = vec4( vTexCoord.st, 0.0, 1.0 );
}
