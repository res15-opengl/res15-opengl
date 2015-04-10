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

uniform mat4 normalMatrix;
uniform mat4 viewMatrix;

out VertexAttrib {
	vec4 position;
	vec3 normal;
	vec2 texcoord;
} vertex;

out TriplanarAttrib {
	vec3 worldPos;
	vec3 worldNormal;
} texInfo;

void main()
{
	vertex.normal = (transpose(inverse(modelViewMatrix)) * vec4(normal,0.0)).xyz; // should return same result as next line:
	vertex.position = modelViewMatrix * position;
	
	/// we set the texCoord to be the position
	vertex.texcoord = texcoord;	
	
	// now, we want the world position!

	// vertex.position is in eye space, and we need to get back to world space. 
	// eye space = viewMatrix * modelMatrix * vertex
	// this means, we will have to undo the last transformation, and that means,
	// pre-multiply with the inverse view matrix.

	// BONUS POINTS: Is this the best way to do it?

	texInfo.worldPos 	=  (inverse(viewMatrix) * vertex.position).xyz;

	// vertex.normal is in eye space, too!
	// so we have to do the same thing as above. 

	texInfo.worldNormal =  mat3(inverse(viewMatrix)) * vertex.normal;

	// we need to re-calculate the vertex positions in this shader,
	// for we render the sphere at the outer radius 
	
	gl_Position = modelViewProjectionMatrix * position;
}
