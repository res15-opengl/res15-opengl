#version 410

// OF Uniforms
uniform mat4    projectionMatrix;
uniform mat4	modelViewMatrix;
uniform mat3    normalMatrix;

in vec4		position;
in vec3		normal;
in vec4         color;
in vec2         texCoord;

// Outputs to fragment stage
out vec4  vPositionVS;
out vec3  vNormal;
out vec3  vColor;
out vec2  vTexCoord;

void main( void )
{
  // calculate view space position
  vec4 position_vs = modelViewMatrix * position;

  // outputs
  vPositionVS = position_vs;
  vNormal = normalMatrix * normal;
  vColor = color.rgb;
  vTexCoord = texCoord;

  gl_Position = projectionMatrix * position_vs;
}
