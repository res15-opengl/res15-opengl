// vertex shader
#version 410 core

//     _____    ___
//    /    /   /  /     lut.vert
//   /  __/ * /  /__    (c) ponies & light, 2014. All rights reserved.
//  /__/     /_____/    poniesandlight.co.uk
//
//  Created by tim on 17/01/2014.
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


#pragma include <of_default_vertex_in_attributes.glsl>
#pragma include <of_default_uniforms.glsl>

uniform mat4 normalMatrix;

// Everything, unless otherwise noted, always happens in: 
//
//       ______              _____                     
//      / ____/_  _____     / ___/____  ____ _________ 
//     / __/ / / / / _ \    \__ \/ __ \/ __ `/ ___/ _ \
//    / /___/ /_/ /  __/   ___/ / /_/ / /_/ / /__/  __/
//   /_____/\__, /\___/   /____/ .___/\__,_/\___/\___/ 
//         /____/             /_/                      
//
// ------------------------------------------------------


out VertexAttrib {
	vec3 position;
	vec2 texcoord;
} vertex;

void main()
{
	vertex.position = (modelViewMatrix * position).xyz;
	vertex.texcoord = texcoord;

	gl_Position = modelViewProjectionMatrix * position;
}
