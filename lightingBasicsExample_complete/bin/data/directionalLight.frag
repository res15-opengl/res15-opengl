#version 410

uniform mat4 viewMatrix;
uniform sampler2D uTexture;

// INPUTS (from vertex shader stage)
in vec4 outColor;
in vec3 outPositionVS;
in vec3 outNormal;
in vec2 outTexCoord;

// OUTPUTS
out vec4 fragColor; // all fragment shaders need an output - color

void main()
{
  // Our surface normal will be used to calculate the angle between the
  // light direction and the surface.
  // This angle will be our diffuse or lambert term
  vec3 normal = outNormal;

  // Our view space position will be used to calculate the direction to
  // the camera. This is used for to calculate specular highlights.
  // Specular highlights move as they camera moves. That is why the direction
  // to the camera is required to calculate them.
  
  vec3 positionVS = outPositionVS;

  // Our ambient and diffuse lighting terms
  vec3 ambient = vec3( 0.2, 0.2, 0.2 );
  vec3 diffuse = vec3( 0.0, 0.0, 0.0 );

  // Light and surface properties
  vec3 surfaceColor = vec3( 1.0, 1.0, 1.0 ) * texture( uTexture, outTexCoord ).rgb;
  vec3 lightColor = vec3( 1.0, 1.0, 1.0 );

  // Light direction in World Space
  vec3 lightDirWS = normalize( vec3( 1.0, 0.0, 0.0 ) );

  // We're doing lighting in View Space, need to multiply the light direction
  // by the rotation part of the View Matrix so that it doesn't rotate with the camera
  vec3 lightDirVS = mat3( viewMatrix )* lightDirWS;

  // Diffuse - calculate lambert term
  // by finding the dot product of the surface normal and the light direction

  // Note: dot product of two NORMALIZED vectors (unit vectors) is mathematically equivalent
  // to the cosine of the angle between the two vectors:
  // a . b = ||a|| ||b|| * cos( theta )
  //       = 1.0 * 1.0 * cos( theta )
  //       = cos( theta )    
  //
  // This is why the dot product is used for lighting calculations.
  // We'll get a value of -1.0 to +1.0 (cosine of the angle between light direction and surface normal)
  // If it's a negative number, the light is behind the surface and unlit
  float NdotL = dot( normal, lightDirVS );

  // as dot product ranges from -1 to +1, make sure it is never less than 0.0
  // This way if light is facing away from surface, it will be lit by 0.0 of the percent of the light
  // If light is facing head on, it will be lit by 1.0 percent of the light
  NdotL = max( NdotL, 0.0 );   

  // lightColor * surfaceColor is fully lit surface color 
  // lambert term (NdotL) which ranges in 0 .. 1.0
  diffuse = lightColor * surfaceColor * NdotL;
  

  // Add our terms together
  fragColor = vec4( ambient + diffuse, 1.0 ); 


  // Debug color, view space position and normal
  // fragColor = vec4( outColor.rgb, 1.0 ); 
  // fragColor = vec4( outTexCoord.st, 0.0, 1.0 ); 
  // fragColor = vec4( positionVS, 1.0 );
  // fragColor = vec4( normal, 1.0 );
} 

