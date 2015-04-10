#version 410

// UNIFORMS
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
  vec3 normal = normalize( outNormal );

  // Our view space position will be used to calculate the direction to
  // the camera. This is used for to calculate specular highlights.
  // Specular highlights move as they camera moves. That is why the direction
  // to the camera is required to calculate them.
  
  vec3 positionVS = outPositionVS;

  // Our ambient and diffuse lighting terms
  vec3 ambient = vec3( 0.15, 0.15, 0.2 );
  vec3 diffuse = vec3( 0.0, 0.0, 0.0 );

  // Light and surface properties

  // our material's color and texture multiplied together
  vec3 surfaceColor = outColor.rgb * texture( uTexture, outTexCoord.st ).rgb; 


  // point lights have a position and radius, but no direction

  // World space position
  vec3 lightPositionWS = vec3( 0.0, 0.0, 220.0 );
  vec3 lightColor = vec3( 1.0, 1.0, 1.0 );
  float lightRadius = 500.0; // radius of point light in units
  float lightPower = 5000.0; // power of light in lumens


  // Convert world space to view space
  vec3 lightPositionVS = ( viewMatrix * vec4( lightPositionWS, 1.0 ) ).xyz;
  

  // We need to calculate light direction - subtract surface position from light position
  vec3 lightDir = normalize( lightPositionVS - positionVS );


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
  float NdotL = dot( normal, lightDir );

  // as dot product ranges from -1 to +1, make sure it is never less than 0.0
  // This way if light is facing away from surface, it will be lit by 0.0 of the percent of the light
  // If light is facing head on, it will be lit by 1.0 percent of the light
  NdotL = max( NdotL, 0.0 );   

  // lightColor * surfaceColor is fully lit surface color 
  // lambert term (NdotL) which ranges in 0 .. 1.0
  diffuse = lightColor * surfaceColor * NdotL;

  // ATTENUATION
  // Calculate a light falloff based on the distance of the light from the surface and the radius of the point light
  // Graph of some different functions:
  // https://www.desmos.com/calculator/f3mwnikyio

  float distToLight = length( lightPositionVS - positionVS ); // distance from surface position to light
  
  // Epic Course Notes (page 12) - http://blog.selfshadow.com/publications/s2013-shading-course/karis/s2013_pbs_epic_notes_v2.pdf
  // Unreal Engine 4 attenuation - uses lumens units for light power
  float denom = pow( clamp( 1.0 - pow( distToLight / lightRadius, 4 ), 0.0, 1.0 ), 2 );
  float attenuation = denom / ( distToLight * distToLight + 1.0 );
  diffuse = diffuse * attenuation * lightPower; 

  /*
  // Alternate attenuation method
  // Similar curve to the UE4 method above (see the graphs in desmos.com link)
  // Does not use light power
  // These are all just approximations - it's up to you what you want to use
  // depending on the look you want to achieve

  float attenuation = clamp( 1.0 - ( distToLight * distToLight ) / ( lightRadius * lightRadius ), 0.0, 1.0 );
  attenuation = attenuation * attenuation;
  // multiply diffuse color by attenuation
  diffuse = diffuse * attenuation;
  */

  
  // Add out ambient and difuse terms together for final lighting contribution
  fragColor = vec4( ambient + diffuse, 1.0 ); 

  // Debug color, view space position and normal
  // fragColor = vec4( outColor.rgb, 1.0 ); 
  // fragColor = vec4( outTexCoord.st, 0.0, 1.0 ); 
  // fragColor = vec4( positionVS, 1.0 );
  // fragColor = vec4( normal, 1.0 );
} 

