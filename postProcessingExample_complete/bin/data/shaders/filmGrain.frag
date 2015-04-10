// Filename: filmGrain.frag
// 
// Copyright © James Acres
// http://www.jamesacres.com
// http://github.com/jacres
// @jimmyacres
// 
// Created: Fri Apr  3 11:32:46 2015 (-0400)
// Last-Updated: Sat Apr  4 17:07:27 2015 (-0400)

#version 410

uniform sampler2D uTexture;
uniform vec2      uTexelSize;
uniform float     uTime;

// inputs
in vec2 vTexCoord;

// outputs
out vec4 out_color;

// Canonical random glsl function.
// It's commonly used for quick random values in GLSL, hower it relies on
// sin() being implemented in a certain way. It may or may not work on
// certain GPUs - namely mobile.
// See here for a more compatible implementation:
//   http://byteblacksmith.com/improvements-to-the-canonical-one-liner-glsl-rand-for-opengl-es-2-0/

float rand( vec2 co )
{
  return fract( sin( dot( co.xy, vec2( 12.9898, 78.233 ) ) ) * 43758.5453 );
}

void main( void )
{
  vec4 color = texture( uTexture, vTexCoord );

  // get a random value based on our texture coordinate + time offset to animate it
  float grainValue = rand( vTexCoord + vec2( uTime, uTime ) );

  // mix texture with noise (10% mix - can raise or lower this number to make
  // the noise weaker/stronger)
  color =  mix( color, vec4( grainValue ), 0.1 );
  
  out_color = vec4( color.rgb, 1.0 );
}

