// Filename: deferredComposite.frag
// 
// Copyright © James Acres
// http://www.jamesacres.com
// http://github.com/jacres
// @jimmyacres
// 
// Created: Wed Apr  1 15:52:26 2015 (-0400)
// Last-Updated: Sat Apr  4 14:33:27 2015 (-0400)

#version 410

// Uniforms 
uniform sampler2D u_albedoTex;
uniform sampler2D u_lightingTex;

// inputs
in vec2 vTexCoord;

// outputs
out vec4 out_color;

void main( void )
{
  vec2 texCoord = vTexCoord;

  // sample our position and normal from the GBuffer textures
  vec3 lighting = texture( u_lightingTex, texCoord ).rgb;

  texCoord.y = 1.0 - texCoord.y;
  vec3 albedo = texture( u_albedoTex, texCoord ).rgb;

  out_color = vec4( albedo * lighting, 1.0 );
}

