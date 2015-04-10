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
/// \brief 			Calculate cheap Blinn Phong light contribution 
///
/// \param light 	Point light position in eye space
///
vec4 blinnPhong(vec4 light, Vertex vert){ 
	const vec3 Ld = vec3(0.7,0.7,0.7);		// Colour of (diffuse) light source 1
	const float Sd = 1.0;					// Constant Diffuse scalar.

	const vec3 Ls = vec3(0.9,0.8,0.7);		// Colour of (specular) light source 1
	const float Ss = 0.7;					// Constant Specular scalar.
	const float Es = 12;					// specular exponent

	vec3 L = normalize(light.xyz - vert.position.xyz);
	vec3 N = normalize(vert.normal);
	
	float Id = Sd * max(0,dot(L,N));		// diffuse light intensity is relative to angle between light and normal
	
	vec3 V = normalize(-vert.position.xyz);	// view direction. since we are in eye space, our target point, the eye, is at the origin.
												// we thus subtract the current position from the origin to get the vector shooting from
												// the current position to the eye.
	vec3 H = normalize(L + V);					// half vector. this is calculated as the vector bisecting the angle between Light direction  and eye direction)
	
	float Is = pow(max(dot(H,N),0), Es);		// calculate Specular intensity
	
	vec4 albedo = vec4(vec3(0.1) + Ld * Id * Sd + Ls * Is * Ss, 1.0);
	return albedo;
}

// ----------------------------------------------------------------------

void main(){

	/// acquire our vertex data from out uniform input
	Vertex v = Vertex(vertex.position, vertex.normal, vertex.texcoord);

	vec3 normal = normalize(vertex.normal);

	if (false) {

		// do some cheap light calculations, based on a single point light far, far, away.

		vec4 light1 = vec4(-2000.f, 500.f, 0.f, 1.f); // light position in world space !!!

		// tansform light into eye space: beware: this cannot use the current modelview matrix,
		// because if this shader is bound to more than one object, each object might have its own 
		// modelviewmatrix.
		// the correct matrix to transform the light into eye space is the view matrix.

		light1 = viewMatrix * light1;

		// light1 is now in eye space.

		fragColor = blinnPhong(light1, v );
		vec3 ambientLight = vec3(0.3);
		fragColor.xyz += ambientLight;
	} else {

		// set light to constant white.
		fragColor = vec4(1);
	
	}
		
	vec3 texSample = vec3(0);


	vec3 blendWeight;
	vec3 plateauSize =  vec3(0.0, 0.0, 0.0); 	// how much to extend each texture "reach", 0.0 == identity
	vec3 transitionSpeed = vec3(1,1,1); 		// "sharpness" of blend curve, 1.0 == identity.
	
	blendWeight = abs(texInfo.worldNormal) - plateauSize;
	
	blendWeight = max(blendWeight, vec3(0.0)); 	// let's keep things positive =)
	blendWeight = pow(blendWeight, transitionSpeed);
	blendWeight = normalize(blendWeight);

	texSample += texture(tex_unit_0, texInfo.worldPos.xy  ).rgb * blendWeight.z;
	texSample += texture(tex_unit_1, texInfo.worldPos.yz  ).rgb * blendWeight.x;
	texSample += texture(tex_unit_2, texInfo.worldPos.xz  ).rgb * blendWeight.y;

	// // uncomment these to visualise blendWeights
	// texSample += vec3(1,0,0) * blendWeights.x;
	// texSample += vec3(0,1,0) * blendWeights.y;
	// texSample += vec3(0,0,1) * blendWeights.z;

	fragColor.xyz *= texSample;
	// fragColor.xyz = texSample;

	// fragColor = vec4(vertex.texcoord.x, vertex.texcoord.y, 0 , 1.0);
	// fragColor = vec4(normal * 0.5 + vec3(0.5,0.5,0.5),1.0);
	// fragColor = vec4(normalize(texInfo.worldNormal) * 0.5 + vec3(0.5,0.5,0.5),1.0);

	// fragColor = vec4(texInfo.worldPos * 0.5 + vec3(0.5,0.5,0.5),1.0);
	// if (vertex.position.x > 0) fragColor.r = 1.0;
	
}

