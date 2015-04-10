#version 410

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


// Everything, unless otherwise noted, always happens in: 
//       ______              _____                     
//      / ____/_  _____     / ___/____  ____ _________ 
//     / __/ / / / / _ \    \__ \/ __ \/ __ `/ ___/ _ \
//    / /___/ /_/ /  __/   ___/ / /_/ / /_/ / /__/  __/
//   /_____/\__, /\___/   /____/ .___/\__,_/\___/\___/ 
//         /____/             /_/                      


const float TWO_PI = 6.28318530717959;
const float PI = 3.141592653589793;

in mat4 perInstanceModelViewMatrix;

in VertexAttrib {
	vec4 position;
	flat vec3 normal;
	float height;
} vertex;

out vec4 fragColor;

// ----------------------------------------------------------------------
 
void addHemisphereLighting(in vec3 N, in vec3 colourSky, in vec3 colourGround, inout vec3 ambientLight_){
 
	// colourSky = vec3(1, 0.537254, 0.211764 );
	// colourGround = vec3( 0.305882, 0.1254901, 0.39215)

	vec3 hemisphereLighting       = vec3(0);
	float hemisphereLightingScale = 0.25;
 
	vec3 lightDirection = normalize((perInstanceModelViewMatrix * vec4(0,1,0,0)).xyz); // straight above, transformed from global space to eye space
 
	float hemisphereFactor = dot(lightDirection, N); // = cos of angle between normal & global light in eye space.
 
	hemisphereLighting = mix(colourGround, colourSky, hemisphereFactor);
 
	ambientLight_ += hemisphereLighting * hemisphereLightingScale;
 
}

// ----------------------------------------------------------------------

vec3 getHeatmapRGBColourForNormalisedValue(const in float value_){
	float value = 1.0-value_;
	vec3 colour = 	(0.5+0.5*smoothstep(0.0, 0.1, value)) *
	vec3(smoothstep(0.5, 0.3, value),
		 value < 0.3 ? smoothstep(0.0, 0.3, value) : smoothstep(1.0, 0.6, value),
		 smoothstep(0.4, 0.6, value));
	return colour;
}

// ----------------------------------------------------------------------

void addRimLighting(in vec3 N, in vec3 viewVector, inout vec3 rimLight_){
	// we are interested in the angle between the surface normal and the viewVector. 
	// since we are in eye space, our viewing vector for traditional rim lighting 
	// would be towards negative z (0,0,-1). But other vectors give nice effects, too!
	
	rimLight_ += vec3(clamp(dot(N, viewVector), 0.0001, 1.0)); 
}

// ----------------------------------------------------------------------

void main(){

	
	fragColor = vec4(0,0,0,1);

	vec3 ambientLight = vec3(0);
	vec3 rimLight = vec3(0);

	vec3 boxColor = getHeatmapRGBColourForNormalisedValue(vertex.height / 16.0);
	vec3 N = normalize(vertex.normal);

	// addHemisphereLighting(N, vec3(0.2, 0.537254, 1.0 ), vec3( 0.305882, 0.1254901, 0.39215), ambientLight);

	addRimLighting(N, vec3(0,0,1), rimLight);

	fragColor.rgb += boxColor;
	fragColor.rgb += rimLight * 0.2;


	float fogFactor = (gl_FragCoord.z / gl_FragCoord.w) / 500.0;

	fogFactor = max(smoothstep(0.0, 0.8, fogFactor),0);

	vec3 normalColor = vertex.normal * 0.5 + vec3(0.5,0.5,0.5);
	
	//fragColor.rgb = normalColor;
	//fragColor.rgb = vertex.color.rgb;
	// fragColor.rgb = mix(fragColor.rgb, vec3(0.0), fogFactor);
	
}

