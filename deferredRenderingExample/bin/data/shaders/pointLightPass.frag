// Filename: pointLightPass.frag
// 
// Copyright © James Acres
// http://www.jamesacres.com
// http://github.com/jacres
// @jimmyacres
// 
// Created: Sun Mar  8 13:07:59 2015 (-0400)
// Last-Updated: Thu Apr  2 16:14:35 2015 (-0400)

#version 410

#pragma include "commonLighting.glsl"

// Uniforms 
uniform sampler2D u_positionTex;
uniform sampler2D u_normalsTex;

uniform vec2      u_inverseResolution;

uniform vec3      u_lightColor;
uniform vec3      u_lightPositionVS; // light position in view space
uniform float     u_lightRadius;

// outputs
layout(location = 0) out vec4 out_lighting;

void main( void )
{
  // We have to calculate the texture coordinate to look up into the GBuffer textures.
  // We do this by looking at the screen x/y of this fragment and changing it from a value
  // of 0 .. width, 0 .. height to 0 .. 1, 0 .. 1 to use as UV coordinates
  vec2 texCoord = gl_FragCoord.xy * u_inverseResolution.xy;
  texCoord.y = 1.0 - texCoord.y;

  // sample our position and normal from the GBuffer textures
  vec3 positionVS = texture( u_positionTex, texCoord ).rgb;
  vec3 normal = texture( u_normalsTex, texCoord ).rgb;
  normal = normalize( normal );

  vec3 lighting = CalcPointLight( positionVS, normal, u_lightPositionVS, u_lightRadius, u_lightColor );

  //  out_lighting = vec4( 0.1, 0.0, 0.0, 1.0 );
  out_lighting = vec4( lighting, 1.0 );
}

