// vertex shader
#version 410

//     _____    ___
//    /    /   /  /     triPlanar.vert
//   /  __/ * /  /__    (c) ponies & light, 2015. All rights reserved.
//  /__/     /_____/    poniesandlight.co.uk
//
//  Created by tim on 26/03/2015.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.


#pragma include <of_default_uniforms.glsl>
#pragma include <of_default_vertex_in_attributes.glsl>
#pragma include <noise.glsl>

uniform mat4 viewMatrix;

out VertexAttrib {
	vec4 position;
	flat vec3 normal;
	float height;
} vertex;

out mat4 perInstanceModelViewMatrix;

const float TWO_PI = 6.28318530717959;
const float PI = 3.141592653589793;

uniform float uElapsedTimef; 	// time since start in float seconds.
uniform sampler2D tex_unit_0; 		// 2d texture


void main()
{

	// find out the texture dimensions in pixels, at LOD 0
	ivec2 texSize = textureSize(tex_unit_0, 0 );

	// For instanced rendering, we will get `gl_InstanceID`, a unique id for each instance.
	// We use this, to manipulate each instance individually.

	mat4 perInstanceModelMatrix;

	vec4 translation;
	translation.x = - 128 + gl_InstanceID % 256; 	// translate x
	translation.y = 0; 							// translate y
	translation.z = - 128 + gl_InstanceID / 256; 	// translate z
	translation.w =	1;						// needs to remain 1.


	vec2 sampleCoords =  vec2(0.5) + (vec2(translation.x,translation.z) / 128.0) * 0.5;
	vec2 halfPixelOffset = vec2(0.5,0.5) * vec2( 1.0/texSize.x, 1.0/texSize.y);

	// sample height from heightmap texture
	float heightSample = texture(tex_unit_0, sampleCoords + halfPixelOffset).r;

	translation.y = heightSample;

	// offset x and z translation slightly using noise, to get rid of moire effects,
	// and make the design look more organic.
	// snoise is a method included from "noise.glsl".

	translation.x += snoise(vec2(translation.x,translation.z)) * 0.5;
	translation.z += snoise(vec2(translation.z,translation.x)) * 0.5;

	translation.y = heightSample * 128 * 0.5; // set the max height for our box, based on the height sample.
	translation.w = 1.0;

	float offset = uElapsedTimef; // will perform a full revolution every (3.14 * 2) seconds =)

	// we can move the position of our "radar station". if we animate that, we get a moving radar station.

	vec2 radarstationPos = vec2(sin(mod(offset / 20.0,TWO_PI)), cos(mod(offset / 13.0,TWO_PI))) * 1;
	radarstationPos.x = snoise(radarstationPos);
	radarstationPos.y = snoise(radarstationPos);

	radarstationPos *= 6;
	radarstationPos += vec2(sin(mod(offset / 40.0,TWO_PI)), cos(mod(offset / 70.0,TWO_PI))) * 64;

	float t = 1.0 - mod( offset + TWO_PI + atan(translation.z - radarstationPos.y, translation.x - radarstationPos.x), TWO_PI) / TWO_PI;

	translation.y *= t; // modulate the height by our time-stepper

	// store the height as a vertex attribute.
	vertex.height = translation.y;

	// nice! now, let's move everything apart a little.

	translation.xyz *= 3;

	perInstanceModelMatrix[0] = vec4(1,0,0,0);
	perInstanceModelMatrix[1] = vec4(0,1,0,0);
	perInstanceModelMatrix[2] = vec4(0,0,1,0);
	perInstanceModelMatrix[3] = translation;

	mat4 scaleMatrix;

	scaleMatrix[0] = vec4(1,0,0,0);
	scaleMatrix[1] = vec4(0,translation.y*2,0,0); // we use translation value here
	scaleMatrix[2] = vec4(0,0,1,0);
	scaleMatrix[3] = vec4(0,0,0,1);

	// ---------------------------

	// We move the box, before we even apply all the other matrices.
	// This works, because the next line really says:
	// vertex.position = viewMatrix * modelMatrix * perInstanceModelMatrix * position
	mat4 translatedModelView = modelViewMatrix * perInstanceModelMatrix;
	
	perInstanceModelViewMatrix = modelViewMatrix * perInstanceModelMatrix * scaleMatrix;

	vertex.position = perInstanceModelViewMatrix * position;


	// ---------------------------	
	// # Let's calculate per-vertex normals:

	// Note that we disregard the scale operation, since in our case it must not 
	// contribute to the normal matrix. 
	vertex.normal =  mat3(translatedModelView) * normal;

	gl_Position = projectionMatrix * vertex.position;
}
