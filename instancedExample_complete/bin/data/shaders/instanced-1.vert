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

out VertexAttrib {
	vec4 position;
	vec4 color;
	vec3 normal;
	vec2 texcoord;
} vertex;

const float TWO_PI = 6.28318530717959;
const float PI = 3.141592653589793;

uniform float uElapsedTimef; 	// time since start in float seconds.
uniform sampler2D tex_unit_0; 		// 2d texture


void main()
{

	vertex.color = vec4(1); // we have to set our own color here.
	vertex.texcoord = texcoord;

	// find out the texture dimensions in pixels, at LOD 0
	ivec2 texSize = textureSize(tex_unit_0, 0 );

	// For instanced rendering, we will get `gl_InstanceID`, a unique id for each instance.
	// We use this, to manipulate each instance individually.
	

	mat4 perInstanceModelMatrix;

	if (false) {
		// we need a translation matrix. 
		// An OpenGL/GLSL matrix is column major!
		// the translation portion of a matrix is therefore in the last column.
		vec4 translation;
		translation.x = - 128 + gl_InstanceID % 256; 	// translate x
		translation.y = 0; 							// translate y
		translation.z = - 128 + gl_InstanceID / 256; 	// translate z
		translation.w =	1;							// needs to remain 1.
		perInstanceModelMatrix[0] = vec4(1,0,0,0);
		perInstanceModelMatrix[1] = vec4(0,1,0,0);
		perInstanceModelMatrix[2] = vec4(0,0,1,0);
		perInstanceModelMatrix[3] = translation;
	}

	if (false) {

		// now let's get funky.
		// make y dependent on phi

		vec4 translation;
		translation.x = - 128 + gl_InstanceID % 256; 	// translate x
		translation.y = 0; 							// translate y
		translation.z = - 128 + gl_InstanceID / 256; 	// translate z
		translation.w =	1;							// needs to remain 1.

		// now, how do we get the angle?
		// the angle is atan(z / x)
		// but really, that would be killing kittens.

		// let's try something:

		translation.y = atan(translation.z/128.0,translation.x/128.0);

		// let's contain it to elements that sit on a circle, 
		// where the distance to the midpoint is less than 128.0

		translation.y *= (1.0 - step( 128.0, length(vec2(translation.z,translation.x))));

		// now scale the angle 
		translation.y *= 50.0;


		// now scale it linearly by distance to the centre.

		// translation.y *= length(vec2(translation.z,translation.x)) / 128;

		// now make sure it only goes in positive direction.

		// translation.y = abs(translation.y);

		// translation.xyz *= 3;

		// we know how to transform our translation coordinate to an unit circle.
		// that is normalising it, by dividing by float 128.

		// we could just as well do all calculations with cos phi, because 
		// cos phi already tells us anything we want to know about where we 
		// sit on the circle.

		// cos phi is nothing but the z coordinate of our translation, divided by 128


		perInstanceModelMatrix[0] = vec4(1,0,0,0);
		perInstanceModelMatrix[1] = vec4(0,1,0,0);
		perInstanceModelMatrix[2] = vec4(0,0,1,0);
		perInstanceModelMatrix[3] = translation;
	}

	if (false) {

		// let's take a closer look.
		// and use colors to debug things.

		vec4 translation;
		translation.x = - 128 + gl_InstanceID % 256; 	// translate x
		translation.y = 0; 							// translate y
		translation.z = - 128 + gl_InstanceID / 256; 	// translate z
		translation.w =	1;						// needs to remain 1.

		if (false) {
			float t = mod(PI + atan(translation.z, translation.x), TWO_PI) / TWO_PI;
			vertex.color = vec4(vec3(t),1);
		}

		if (true) {
			float t = mod(TWO_PI + atan(translation.z, translation.x), TWO_PI) / TWO_PI;
			vertex.color = vec4(vec3(t),1);
			// let's contain it to elements that sit on a circle, 
			// where the distance to the midpoint is less than 128.0

			vertex.color *= 1.0 - step( 128.0, length(vec2(translation.z,translation.x)));

		}

		perInstanceModelMatrix[0] = vec4(1,0,0,0);
		perInstanceModelMatrix[1] = vec4(0,1,0,0);
		perInstanceModelMatrix[2] = vec4(0,0,1,0);
		perInstanceModelMatrix[3] = translation;
	}

	// now you need to progress to / activate section # Animation in ofApp.cpp
	// we're going to use a time stepper
	// we're also going to use a texture to manipulate our instances.

	if (false) {

		// let's animate this.

		vec4 translation;
		translation.x = - 128 + gl_InstanceID % 256; 	// translate x
		translation.y = 0; 							// translate y
		translation.z = - 128 + gl_InstanceID / 256; 	// translate z
		translation.w =	1;						// needs to remain 1.

		float offset = uElapsedTimef; // will perform a full revolution every (3.14 * 2) seconds =)
		float t = mod( offset + TWO_PI + atan(translation.z, translation.x), TWO_PI) / TWO_PI;

		// let's flip this, so that the value in the wake of our scanline 1, not 0.

		t = 1.0 - mod( offset + TWO_PI + atan(translation.z, translation.x), TWO_PI) / TWO_PI;

		vertex.color = vec4(vec3(t),1);
		
		// let's contain it to elements that sit on a circle, 
		// where the distance to the midpoint is less than 128.0
		vertex.color *= 1.0 - step( 128.0, length(vec2(translation.z,translation.x)));


		perInstanceModelMatrix[0] = vec4(1,0,0,0);
		perInstanceModelMatrix[1] = vec4(0,1,0,0);
		perInstanceModelMatrix[2] = vec4(0,0,1,0);
		perInstanceModelMatrix[3] = translation;
	}

	if (false) {

		vec4 translation;
		translation.x = - 128 + gl_InstanceID % 256; 	// translate x
		translation.y = 0; 							// translate y
		translation.z = - 128 + gl_InstanceID / 256; 	// translate z
		translation.w =	1;						// needs to remain 1.

		// float offset = uElapsedTimef; // will perform a full revolution every (3.14 * 2) seconds =)
		// float t = mod( offset + TWO_PI + atan(translation.z, translation.x), TWO_PI) / TWO_PI;
		// vertex.color = vec4(vec3(t),1);

		// let's take a step back, and sample the colors for each of our boxes using 
		// a texture. First, get the correct sample coordinates.

		vec2 sampleCoords = vec2(vec2(translation.x,translation.z) / 128.0);

		// let's just visualise these to see if they are correct!
		vertex.color.b = 0;
		vertex.color.rg = sampleCoords;
		
		// hmmm. that looks squished. 
		// it's because the texture coordinates run from -1 to +1, but should go from 0 to 1.

		// first divide by 2

		sampleCoords *= vec2(0.5);

		// then add 0.5

		sampleCoords += vec2(0.5);

		// sampleCoords now mapped to 0..1 !
		// let's have a look
		vertex.color.rg = sampleCoords;

		// ok, now sample from the texture.

		// now, to hit the pixel fair and square, we need to offset the sampling point by half a pixel
		// because we have normalised texture coordinates, we need to find out how much the offset 
		// woud be in normalised pixel coordinates. We get there, by dividing half a pixel in x through 
		// width in pixels in x, and half a pixel in y divided by height in pixels in y.

		vec2 sampleOffset = vec2(0.5,0.5) * vec2(1.0/texSize.x,1.0/texSize.y);

		vertex.color.rgb = texture(tex_unit_0, sampleCoords + sampleOffset).rrr;

		// let's contain it to elements that sit on a circle, 
		// where the distance to the midpoint is less than 128.0
		// vertex.color *= 1.0 - step( 128.0, length(vec2(translation.z,translation.x)));

		perInstanceModelMatrix[0] = vec4(1,0,0,0);
		perInstanceModelMatrix[1] = vec4(0,1,0,0);
		perInstanceModelMatrix[2] = vec4(0,0,1,0);
		perInstanceModelMatrix[3] = translation;
	}
	
	if (true) {

		// last iteration block:
		
		// now, let's move our boxes around, based on the sample color from the texture.

		vec4 translation;
		translation.x = - 128 + gl_InstanceID % 256; 	// translate x
		translation.y = 0; 							// translate y
		translation.z = - 128 + gl_InstanceID / 256; 	// translate z
		translation.w =	1;						// needs to remain 1.


		vec2 sampleCoords = vec2(vec2(translation.x,translation.z) / 128.0);

		// let's just visualise these to see if they are correct!
		vertex.color.b = 0;
		vertex.color.rg = sampleCoords;
		
		// hmmm. that looks squished. 
		// it's because the texture coordinates run from -1 to +1, but should go from 0 to 1.

		// first divide by 2

		sampleCoords *= vec2(0.5);

		// then add 0.5

		sampleCoords += vec2(0.5);

		// sampleCoords now mapped to 0..1 !
		// let's have a look
		vertex.color.rg = sampleCoords;
		vertex.color.a = 1.0;

		// ok, now sample from the texture.

		// now, to hit the pixel fair and square, we need to offset the sampling point by half a pixel
		// because we have normalised texture coordinates, we need to find out how much the offset 
		// woud be in normalised pixel coordinates. We get there, by dividing half a pixel in x through 
		// width in pixels in x, and half a pixel in y divided by height in pixels in y.


		vec2 sampleOffset = vec2(0.5,0.5) * vec2(1.0/texSize.x,1.0/texSize.y);

		float heightSample = texture(tex_unit_0, sampleCoords + sampleOffset).r;

		translation.y = heightSample;

		// not very spectacular. let's scale that a bit.

		translation.y = heightSample * 100;

		// how about we use the code from before, to scale our height value based on our rotating scanline?

		float offset = uElapsedTimef; // will perform a full revolution every (3.14 * 2) seconds =)
		float t = 1.0 - mod( offset + TWO_PI + atan(translation.z, translation.x), TWO_PI) / TWO_PI;

		translation.y *= t;

		// nice! now, let's push everything apart a little.

		translation.xyz *= 2.0;

		// do we like the colors? how about using t to control the brightness of the boxes?

		vertex.color.rgb = vec3(1) * t;

		// Bonus points: still don't like the colors? go wild =) !

		// --------------------------

		// let's contain it to elements that sit on a circle, 
		// where the distance to the midpoint is less than 128.0
		vertex.color *= 1.0 - step( 128.0 * 2, length(vec2(translation.z,translation.x)));

		perInstanceModelMatrix[0] = vec4(1,0,0,0);
		perInstanceModelMatrix[1] = vec4(0,1,0,0);
		perInstanceModelMatrix[2] = vec4(0,0,1,0);
		perInstanceModelMatrix[3] = translation;

		// Bonus Points: translation alone too boring?

		// how about squeezing in a rotation matrix?

	}
	// We move the box, before we even apply all the other matrices.
	// This works, because the next line really says:
	// vertex.position = viewMatrix * modelMatrix * perInstanceModelMatrix * position
	vertex.position = modelViewMatrix * perInstanceModelMatrix * position;
	
	// just in case you need the normals - but have a look at instanced-2.vert for an optimised version:
	// vertex.normal = (transpose(inverse(modelViewMatrix * perInstanceModelMatrix)) * vec4(normal,0.0)).xyz;

	gl_Position = projectionMatrix * vertex.position;
}
