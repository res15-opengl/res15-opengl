// Filename: directionalPass.frag
// 
// Copyright © James Acres
// http://www.jamesacres.com
// http://github.com/jacres
// @jimmyacres
// 
// Created: Sun Mar  8 12:02:07 2015 (-0400)
// Last-Updated: Thu Apr  2 15:58:29 2015 (-0400)

#version 410

#pragma include "commonLighting.glsl"

// Uniforms 
uniform sampler2D u_positionTex;
uniform sampler2D u_normalsTex;

uniform vec2      u_inverseResolution;

uniform vec3      u_lightColor;
uniform vec3      u_lightDirectionVS; // light direction in view space

// inputs
in vec2           vTexCoord;

// outputs
layout(location = 0) out vec4 out_lighting;

void main( void )
{
  // sample our position and normal from the GBuffer textures
  vec3 positionVS = texture( u_positionTex, vTexCoord ).rgb;
  vec3 normal = texture( u_normalsTex, vTexCoord ).rgb;
  normal = normalize( normal );

  // normalize our light direction (surface to light, so negate)
  vec3 lightDir = normalize( u_lightDirectionVS );

  vec3 lighting = CalcDirectionalLight( positionVS, normal, lightDir, u_lightColor );

  out_lighting = vec4( lighting, 1.0 );
}

