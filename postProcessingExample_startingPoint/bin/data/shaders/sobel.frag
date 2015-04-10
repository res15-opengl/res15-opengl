// Filename: sobel.frag
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

// inputs
in vec2 vTexCoord;

// outputs
out vec4 out_color;

void main( void )
{
  // sample all 8 neighbor colors
  vec3 texelN = texture( uTexture, vTexCoord + vec2( 0.0, uTexelSize.y ) ).rgb;
  vec3 texelE = texture( uTexture, vTexCoord + vec2( uTexelSize.x, 0.0 ) ).rgb;
  vec3 texelS = texture( uTexture, vTexCoord + vec2( 0.0, -uTexelSize.y ) ).rgb;
  vec3 texelW = texture( uTexture, vTexCoord + vec2( -uTexelSize.x, 0.0 ) ).rgb;

  vec3 texelNE = texture( uTexture, vTexCoord + vec2( uTexelSize.x, uTexelSize.y ) ).rgb;
  vec3 texelSE = texture( uTexture, vTexCoord + vec2( uTexelSize.x, -uTexelSize.y ) ).rgb;
  vec3 texelSW = texture( uTexture, vTexCoord + vec2( -uTexelSize.x, -uTexelSize.y ) ).rgb;
  vec3 texelNW = texture( uTexture, vTexCoord + vec2( -uTexelSize.x, uTexelSize.y ) ).rgb;

  // Get luminance of each color sample ( R * 0.30 + G * 0.59 + B * 0.11 )
  // The benefit of this approach is that those values compensate for how
  // the human eye interprets brightness
  
  // This is an alternative to ( R + G + B ) / 4.0 which does not provide
  // this compensation.

  const vec3 RGB_TO_LUMINANCE = vec3( 0.30, 0.59, 0.11 );

  float lum[ 9 ];
  lum[ 0 ] = dot( texelNW, RGB_TO_LUMINANCE );
  lum[ 1 ] = dot( texelN,  RGB_TO_LUMINANCE );
  lum[ 2 ] = dot( texelNE, RGB_TO_LUMINANCE );
  lum[ 3 ] = dot( texelW,  RGB_TO_LUMINANCE );
  lum[ 4 ] = 0.0;  // ignore, unused - it's the center of the kernel. Included for array index clarity from below  3 x 3 kernel
  lum[ 5 ] = dot( texelE,  RGB_TO_LUMINANCE );
  lum[ 6 ] = dot( texelSW, RGB_TO_LUMINANCE );
  lum[ 7 ] = dot( texelS,  RGB_TO_LUMINANCE );
  lum[ 8 ] = dot( texelSE, RGB_TO_LUMINANCE );


  /*
      Horizontal sobel kernel:
           -1   0  +1
      X =  -2   0  +2
           -1   0  +1

      Vertical sobel kernel:
           -1  -2  -1
      Y =   0   0   0
           +1  +2  +1

  */

  float sobelX = -1.0 * lum[0] - 2.0 * lum[3] - 1.0 * lum[6]
                + 1.0 * lum[2] + 2.0 * lum[5] + 1.0 * lum[8]; 

  float sobelY = -1.0 * lum[0] - 2.0 * lum[1] - 1.0 * lum[2]
                + 1.0 * lum[6] + 2.0 * lum[7] + 1.0 * lum[8]; 


  float edge = length( vec2( sobelX, sobelY ) );

  // bring out lights and darks
  float value = pow( edge, 2.0 );
  value = clamp( value, 0.0, 1.0 );

  value = 1.0 - value; // uncomment to reverse to black on white
  out_color = vec4( value, value, value, 1.0 );

  /*
  // Can also use sobel filter to calculate fake normals based on heightfield image!
  // See fakeMetaballsExample
  float normalStrength = 2.0;
  vec3 normal = vec3( sobelX, sobelY, 1.0 / normalStrength );
  normal = normalize( normal ) * 0.5 + 0.5;

  out_color = vec4( normal, 1.0 );
  */

}

