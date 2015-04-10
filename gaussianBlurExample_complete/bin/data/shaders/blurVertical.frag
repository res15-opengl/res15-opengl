// Filename: gaussianBlurVertical.frag
// 
// Copyright © James Acres
// http://www.jamesacres.com
// http://github.com/jacres
// @jimmyacres
// 
// Created: Fri Apr  3 11:24:40 2015 (-0400)
// Last-Updated: Sat Apr  4 17:28:33 2015 (-0400)

#version 410

//#define NUM_TAPS_5
#define NUM_TAPS_9

uniform sampler2D uTexture;
uniform vec2      uTexelSize;
uniform float     uBlurRadius;

// inputs
in vec2 vTexCoord;

// outputs
out vec4 out_color;

// weights from: http://dev.theomader.com/gaussian-kernel-calculator/

// sigma: 2, kernel size: 5
#ifdef NUM_TAPS_5
const float weight[ 3 ] = float[]( 0.250301, 0.221461, 0.153388 );
const int loopCount = 3;
#endif

// sigma: 2, kernel size: 9
#ifdef NUM_TAPS_9
const float weight[ 5 ] = float[]( 0.20236, 0.179044, 0.124009, 0.067234, 0.028532 );
const int loopCount = 5;
#endif

void main( void )
{
  vec4 color = texture( uTexture, vTexCoord ) * weight[ 0 ];

  vec2 texelStep = uTexelSize * vec2( 0.0, 1.0 ) * uBlurRadius;

  for ( int i = 1; i < loopCount; ++i )
  {
    vec2 texelOffset = i * texelStep;
   
    color += texture( uTexture, vTexCoord + texelOffset ) * weight[ i ];
    color += texture( uTexture, vTexCoord - texelOffset ) * weight[ i ];
  }

  out_color = vec4( color.rgb, 1.0 );
}

