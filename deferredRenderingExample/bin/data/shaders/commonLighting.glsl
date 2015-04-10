// Filename: commonLighting.glsl
// 
// Copyright © James Acres
// http://www.jamesacres.com
// http://github.com/jacres
// @jimmyacres
// 
// Created: Wed Apr  1 20:14:57 2015 (-0400)
// Last-Updated: Wed Apr  1 21:03:01 2015 (-0400)

float CalcAttenuation( float lightDistance, float lightRadius )
{
  float distSqr = lightDistance * lightDistance;
  float attenuation = max( 0.0, 1.0 - distSqr * ( 1.0 / ( lightRadius * lightRadius ) ) );
  attenuation = attenuation * attenuation;
  attenuation = clamp( attenuation, 0.0, 1.0 );

  return attenuation;
}

vec3 CalcDirectionalLight( vec3 surfacePosition, vec3 normal, vec3 lightDirection, vec3 lightColor )
{
  // calculate diffuse lighting using dot product of surface normal and light direction
  float NdotL = dot( normal, lightDirection );

  vec3 diffuseContribution = vec3( 0.0, 0.0, 0.0 );
  vec3 specularContribution = vec3( 0.0, 0.0, 0.0 );

  // if light is facing towards surface
  if ( NdotL > 0.0 )
  {
    // multiply the surface color by the light color
    diffuseContribution = lightColor * NdotL;

    float shininess = 127.0;
    float specularLevel = 1.0f;

    vec3 reflectionVec = normalize( reflect( lightDirection, normal ) );

    // This is a vector in the direction of cameraPosition - surfacePosition
    // Since we are in view space, our camera is at (0, 0, 0)
    // So we can simply negate the surface position.
    vec3 surfaceToCamera = normalize( surfacePosition );
    float cosAngle = max( 0.0, dot( -reflectionVec, surfaceToCamera ) );

    float specularCoefficient = pow( cosAngle, shininess );
    specularContribution = specularCoefficient * specularLevel * vec3( 1.0, 1.0, 1.0 );
  }

  return diffuseContribution + specularContribution;
}

vec3 CalcPointLight( vec3 surfacePosition, vec3 normal, vec3 lightCenter, float lightRadius, vec3 lightColor )
{
  // normalize our light direction (view space positions)
  vec3 surfaceToLight = lightCenter - surfacePosition;

  // units away from light
  float distToLight = length( surfaceToLight );

  // light direction is normalized direction of the surface to the light
  vec3 lightDirection = normalize( surfaceToLight );
  
   // calculate diffuse lighting using dot product of surface normal and light direction
  float NdotL = dot( normal, lightDirection );

  // our diffuse and specular lighting contribution
  vec3 diffuseContribution = vec3( 0.0, 0.0, 0.0 );
  vec3 specularContribution = vec3( 0.0, 0.0, 0.0 );

  // if light is facing towards surface
  if ( NdotL > 0.0 )
  {
    // how should the light falloff away from center towards the edges of its sphere
    float attenuation = CalcAttenuation( distToLight, lightRadius );
    
    // multiply the surface color by the light color
    diffuseContribution = lightColor * NdotL;
    diffuseContribution *= attenuation;

    float shininess = 127.0;
    float specularLevel = 1.0;

    // reflect the light direction along the normal vector
    vec3 reflectionVec = normalize( reflect( lightDirection, normal ) );

    // This is a vector in the direction of cameraPosition - surfacePosition
    // Since we are in view space, our camera is at (0, 0, 0)
    // So we can simply negate the surface position.
    vec3 surfaceToCamera = normalize( surfacePosition );

    // find the cosine of the angle between the reflection and view vector
    float cosAngle = max( 0.0, dot( reflectionVec, surfaceToCamera ) );
    float specularCoefficient = pow( cosAngle, shininess );

    specularContribution = specularCoefficient * specularLevel * vec3( 1.0, 1.0, 1.0 );
    specularContribution *= attenuation;
  }

  return diffuseContribution + specularContribution;
}

