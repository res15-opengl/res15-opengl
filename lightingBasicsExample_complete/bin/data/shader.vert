#version 410

// Uniforms - automatically supplied by OF (except normal matrix)
uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 normalMatrix;

// Inputs (Attributes) - automatically supplied by OF when drawing a Vbo or VboMesh
in vec4 position;
in vec4 color;
in vec4 normal;
in vec2 texcoord;

// Outputs - what we are passing to the fragment shader
out vec3 outPositionVS;
out vec4 outColor; // vertex shader is outputting color to next stage (fragment shader)
out vec3 outNormal;
out vec2 outTexCoord;

void main()
{
  // view space position is vertex position multiplied by modelViewMatrix
  vec4 positionVS = modelViewMatrix * position;

  // outputs to our fragment shader
  outPositionVS = positionVS.xyz;
  outColor = color;

  // send our normal to the fragment shader. multiply it by the normalMatrix so that they normals are
  // transformed by our model rotation and our camera rotation
  outNormal = normalize( ( normalMatrix * normal ).xyz );  

  outTexCoord = texcoord;
  
  // multiplying the view space position by projectionMatrix gives us the proper position after projection
  gl_Position = projectionMatrix * positionVS;
}

