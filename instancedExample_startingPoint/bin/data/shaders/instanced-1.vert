// vertex shader
#version 400

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

uniform mat4 viewMatrix;
uniform float uElapsedTimef; 		// time since start in float seconds.
uniform sampler2D tex_unit_0; 		// 2d texture

out VertexAttrib {
	vec4 position;
	vec4 color;
	vec3 normal;
	vec2 texcoord;
} vertex;

const float TWO_PI = 6.28318530717959;
const float PI = 3.141592653589793;

void main()
{

	vertex.color = vec4(1); // we have to set our own color here.
	vertex.texcoord = texcoord;

	// find out the texture dimensions in pixels, at LOD 0
	ivec2 texSize = textureSize(tex_unit_0, 0 );

	// For instanced rendering, we will get `gl_InstanceID`, a unique id for each instance.
	
	// We use this, to manipulate each instance individually.
	
	mat4 perInstanceModelMatrix;

	// First, we need a per-instance translation matrix.

	// An OpenGL/GLSL matrix is (usually) column major.
	// When you fetch a vec4 from a GLSL matrix using 
	// the array operator [], you get the columns.
	//
	// The translation portion of a matrix is in the last column.

	vec4 translation;
	translation.x = - 128 + gl_InstanceID % 256; 	// translate x
	translation.y = 0; 								// translate y
	translation.z = - 128 + gl_InstanceID / 256; 	// translate z
	translation.w =	1;								// needs to remain 1.


	perInstanceModelMatrix[0] = vec4(1,0,0,0);
	perInstanceModelMatrix[1] = vec4(0,1,0,0);
	perInstanceModelMatrix[2] = vec4(0,0,1,0);
	perInstanceModelMatrix[3] = translation;

	// We move the box, before we even apply all the other matrices.
	// This works, because the next line really says:
	// vertex.position = viewMatrix * modelMatrix * perInstanceModelMatrix * position
	// Read the above line from right to left.
	vertex.position = modelViewMatrix * perInstanceModelMatrix * position;
	
	// just in case you need the normals - but have a look at instanced-2.vert for an optimised version:
	// vertex.normal = (transpose(inverse(modelViewMatrix * perInstanceModelMatrix)) * vec4(normal,0.0)).xyz;

	gl_Position = projectionMatrix * vertex.position;
}
