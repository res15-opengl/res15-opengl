#version 410

// INPUTS - (from OF) OF attributes defined in the VBO
in vec4 position;
in vec4 color;
in vec4 normal;

// UNIFORMS - OF sets these, except for normalMatrix
uniform mat4 modelMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 normalMatrix;

// OUTPUTS - (to Fragment Shader)
out vec3 outPositionVS; // position in view space
out vec3 outNormalVS;   // normal in view space
out vec4 outColor;      // surface color

void main()
{
  // view space position is vertex position multiplied by modelViewMatrix
  vec4 positionVS = modelViewMatrix * position;

  // outputs to our fragment shader
  outPositionVS = positionVS.xyz;

  // multiplying a normal by the normal matrix gives us the normal oriented in view space
  outNormalVS = normalize( ( normalMatrix * normal ).xyz );  

  // our surface material color
  outColor = color;
  
  // multiply our view space position by the projection matrix`
  gl_Position = projectionMatrix * positionVS;
}

