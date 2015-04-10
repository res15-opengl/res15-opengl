// Filename: vignette.frag
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

// inputs
in vec2 vTexCoord;

// outputs
out vec4 out_color;

void main( void )
{
  vec4 color = texture( uTexture, vTexCoord );

  float distFromCenter = length( 2.0 * ( vTexCoord.st - vec2( 0.5, 0.5 ) ) );

  float RADIUS = 0.75;
  float SOFTNESS = 0.8;

  float vignette = smoothstep( RADIUS, RADIUS - SOFTNESS, distFromCenter );
  
  //  color = color * vignette;

  // apply the vignette with 80% opacity
  // mix the original image with the vignette multiplied one
  color = mix( color, color * vignette, 0.8 );

  out_color = vec4( color.rgb, 1.0 );
}

