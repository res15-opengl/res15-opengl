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
uniform vec2      uTexelSize;
uniform float     uBlurRadius;

// inputs
in vec2 vTexCoord;

// outputs
out vec4 out_color;

void main( void )
{
  vec4 color = vec4( 0.0, 0.0, 0.0, 0.0 );

  float numSamples = 1.0;
  
  // single pass is 9 x 9 = 81 texel fetches for a 9 tap blur!
  // much more efficient to do it in 2 separate passes than doing it this way
  // 2 separate passes reduces from 81 to (2 * 9) = 18 texel fetches for 9 tap
  for ( int x = -4; x <= 4; ++x )
  {
    for ( int y = -4; y <= 4; ++y )
    {
      vec2 texelOffset = uTexelSize * vec2( x, y );
      texelOffset *= uBlurRadius;

      color += texture( uTexture, vTexCoord + texelOffset );

      ++numSamples;
    }
  }

  color = color / numSamples;

  out_color = vec4( color.rgb, 1.0 );
}

