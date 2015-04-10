// Filename: pointLightPass.vert
// 
// Copyright © James Acres
// http://www.jamesacres.com
// http://github.com/jacres
// @jimmyacres
// 
// Created: Sun Mar  8 13:17:00 2015 (-0400)
// Last-Updated: Thu Apr  2 15:29:09 2015 (-0400)

#version 410

// OF Uniforms
uniform mat4    projectionMatrix;
uniform mat4	modelViewMatrix;

// OF Attributes
in vec4		position;

void main( void )
{
  gl_Position = projectionMatrix * modelViewMatrix * position; 
}
