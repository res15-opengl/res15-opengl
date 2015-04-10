#version 400

//     _____    ___
//    /    /   /  /     triPlanar.frag
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

const float TWO_PI = 6.28318530717959;
const float PI = 3.141592653589793;

uniform mat4 modelViewMatrix;
uniform mat4 viewMatrix;

// Note: you need TEXTURE_2D textures, wrap_mode_repeat
// will not work with TEXTURE_RECTANGLE textures.
// https://www.opengl.org/registry/specs/ARB/texture_rectangle.txt

uniform sampler2D tex_unit_0;
uniform sampler2D tex_unit_1;
uniform sampler2D tex_unit_2;


in VertexAttrib {
	vec4 position;
	vec3 normal;
	vec2 texcoord;
} vertex;

out vec4 fragColor;

struct Vertex {
	vec4 position;
	vec3 normal;
	vec2 texcoord;
};

in TriplanarAttrib {
	vec3 worldPos;
	vec3 worldNormal;
} texInfo;


// ----------------------------------------------------------------------

void main(){

	/// acquire our vertex data from out uniform input
	Vertex v = Vertex(vertex.position, vertex.normal, vertex.texcoord);

	vec3 normal = normalize(vertex.normal);

	// set albedo to constant white.
	fragColor = vec4(1);
		
	vec3 texSample = vec3(0);

	vec3 blendWeight;
	
	// use the world normal to find out in which 
	// direction the face normal points

	blendWeight = abs(texInfo.worldNormal);
	blendWeight = normalize(blendWeight);

	// visualise blendWeights
	texSample += vec3(1,0,0) * blendWeight.x;
	texSample += vec3(0,1,0) * blendWeight.y;
	texSample += vec3(0,0,1) * blendWeight.z;

	// now, instead of colours, use textures for each direction

	// ...


	
	fragColor.xyz *= texSample;

	// fragColor.xyz = texSample;
	// fragColor = vec4(vertex.texcoord.x, vertex.texcoord.y, 0 , 1.0);
	// fragColor = vec4(normal * 0.5 + vec3(0.5,0.5,0.5),1.0);
	// fragColor = vec4(normalize(texInfo.worldNormal) * 0.5 + vec3(0.5,0.5,0.5),1.0);
	// fragColor = vec4(texInfo.worldPos * 0.5 + vec3(0.5,0.5,0.5),1.0);
	// if (vertex.position.x > 0) fragColor.r = 1.0;
}

