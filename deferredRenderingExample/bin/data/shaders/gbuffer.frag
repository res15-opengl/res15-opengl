#version 410

// OF Uniforms (built-in)
uniform sampler2D u_texture;

in vec4  vPositionVS;
in vec3  vNormal;
in vec3  vColor;
in vec2  vTexCoord;

layout(location = 0) out vec4 out_position;
layout(location = 1) out vec4 out_normals;
layout(location = 2) out vec4 out_albedo;

void main( void )
{
  vec3 normal = normalize( vNormal );

  vec3 textureColor = texture( u_texture, vTexCoord ).rgb;
  vec3 color = vColor * textureColor;

  out_position = vPositionVS;
  out_normals  = vec4( normal.xyz, 1.0 );
  out_albedo   = vec4( color, 1.0 );
}

