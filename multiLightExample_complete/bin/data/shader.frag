#version 410

#define MAX_LIGHTS 8

// Our Light structure - matches what we've defined in OF
struct Light
{
  vec4   position;
  vec3   color;
  float  radius;
  float  power;
};

// UNIFORMS
uniform Light uLights[ MAX_LIGHTS ]; // create an array sized to the max number of lights
uniform int   uNumLights;
uniform float uMaterialShininess;

// INPUTS (from vertex shader stage)
in vec3 outPositionVS;
in vec3 outNormalVS;
in vec4 outColor; 

// OUTPUTS
out vec4 fragColor; // all fragment shaders need an output - color

// CONSTANTS
const vec3 kGamma = vec3( 1.0 / 2.2 );

// -----------------------------------------------------
// ATTENUATION / FALLOFF
// -----------------------------------------------------

// Epic Course Notes (page 12) - http://blog.selfshadow.com/publications/s2013-shading-course/karis/s2013_pbs_epic_notes_v2.pdf
float getFallOff( vec3 position, vec3 lightPosition, float lightRadius )
{
  float dist = length( position - lightPosition );
  float d = pow( clamp( 1.0 - pow( dist / lightRadius, 4 ), 0.0, 1.0 ), 2 );
  return d / ( dist * dist + 1.0 );
}

// -----------------------------------------------------
// LIGHTING FUNCTIONS
// -----------------------------------------------------

// Receives a normal, surface position, light, and material color + shininess as an input
// Returns the shaded color
vec3 CalcDirectionalLight( vec3 normal, vec3 surfacePosition, Light light, vec3 materialColor, float materialShininess )
{
  vec3 lightDir = normalize( light.position.xyz );

  // diffuse
  float NdotL = max( dot( normal, lightDir ), 0.0 );

  //specular
  float specularCoefficient = 0.0;

  if ( NdotL > 0.0 )
  {
    vec3 surfaceToEye = normalize( -surfacePosition );
    specularCoefficient = pow( max( 0.0, dot( surfaceToEye, reflect( -lightDir, normal ) ) ), materialShininess );
  }

  vec3 diffuse = materialColor * light.color * NdotL;
  vec3 specular = specularCoefficient * vec3( 1.0, 1.0, 1.0 );

  return diffuse + specular;
}

// Receives a normal, surface position, light, and material color + shininess as an input
// Returns the shaded color
vec3 CalcPointLight( vec3 normal, vec3 surfacePosition, Light light, vec3 materialColor, float materialShininess )
{
  vec3 lightDir = normalize( light.position.xyz - surfacePosition );

  // diffuse
  float NdotL = max( dot( normal, lightDir ), 0.0 );

  //specular
  float specularCoefficient = 0.0;

  if ( NdotL > 0.0 )
  {
    vec3 surfaceToEye = normalize( -surfacePosition );
    specularCoefficient = pow( max( 0.0, dot( surfaceToEye, reflect( -lightDir, normal ) ) ), materialShininess );
  }

  vec3 specular = specularCoefficient * vec3( 1.0, 1.0, 1.0 );
  vec3 diffuse = materialColor * light.color * NdotL;

  float fallOff = getFallOff( surfacePosition, light.position.xyz, light.radius );

  return light.power * fallOff * ( diffuse + specular );
}

void main()
{
  // important to normalize our input normal attributes
  vec3 normalVS = normalize( outNormalVS );

  // color that we will use to add up all our our light contribution
  vec3 ambient = vec3( 0.01f, 0.01f, 0.01f );

  vec3 lightContribution = vec3( 0.0f, 0.0f, 0.0f );

  // begin our light contribution with ambient lighting
  lightContribution += ambient.rgb;
  
  // loop through all of our lights!
  for ( int i = 0; i < uNumLights; ++i )
  {
    // get current light
    Light light = uLights[ i ];

    // check if we are calculating a directional light or a point light
    if ( light.radius == 0.0f ) // directional light
    {
      lightContribution += CalcDirectionalLight( normalVS, outPositionVS, light, outColor.rgb, uMaterialShininess );
    }
    else
    {
      lightContribution += CalcPointLight( normalVS, outPositionVS, light, outColor.rgb, uMaterialShininess );
    }
  }

  // gamma correction of 2.2 (see constant at top of file)
  lightContribution.rgb = pow( lightContribution.rgb, kGamma );

  fragColor = vec4( lightContribution, outColor.a ); 
} 

