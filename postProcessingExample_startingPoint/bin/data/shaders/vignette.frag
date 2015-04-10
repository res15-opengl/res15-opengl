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

// outputs
out vec4 out_color;

void main( void )
{
  vec4 color = vec4( 1.0, 0.0, 0.0, 1.0 );

  out_color = vec4( color.rgb, 1.0 );
}



