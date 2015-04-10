// Filename: boxBlur.frag
// 
// Copyright © James Acres
// http://www.jamesacres.com
// http://github.com/jacres
// @jimmyacres
// 
// Created: Fri Apr  3 11:32:46 2015 (-0400)
// Last-Updated: Sat Apr  4 17:07:27 2015 (-0400)

// 9 tap box blur shader for example purposes only
// It's much more efficient to do a separable 2 pass blur

#version 410

uniform sampler2D uTexture;

// inputs
in vec2 vTexCoord;

// outputs
out vec4 out_color;

void main( void )
{
  vec4 color texture( uTexture, vTexCoord );

  out_color = vec4( color.rgb, 1.0 );
}

