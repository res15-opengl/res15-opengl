#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
  m_shader.load( "shader.vert", "shader.frag" );

  m_sphereVbo = ofSpherePrimitive( 150.0f, 32, OF_PRIMITIVE_TRIANGLES ).getMesh();
  m_sphereVbo.enableColors();
  m_sphereVbo.enableIndices();
  m_sphereVbo.setColorForIndices( 0, m_sphereVbo.getNumIndices(), ofColor( 255, 255, 255, 255 ) );

  // Define some lights
  Light light1;
  light1.type = Light::LIGHT_TYPE_DIRECTIONAL;
  light1.color = ofFloatColor( 1.0f, 0.6f, 0.0f );
  light1.radius = 0.0f; 
  light1.power = 0.5f; 

  Light light2;
  light2.type = Light::LIGHT_TYPE_POINT;
  light2.color = ofFloatColor( 0.0f, 0.0f, 1.0f );
  light2.radius = 100.0f;
  light2.power = 2000.0f;

  Light light3;
  light3.type = Light::LIGHT_TYPE_POINT;
  light3.color = ofFloatColor( 0.0f, 1.0f, 0.0f );
  light3.radius = 200.0f;
  light3.power = 2000.0f;
  
  Light light4;
  light4.type = Light::LIGHT_TYPE_POINT;
  light4.color = ofFloatColor( 1.0f, 0.0f, 0.0f );
  light4.radius = 300.0f;
  light4.power = 2000.0f;

  m_lights.push_back( light1 );
  m_lights.push_back( light2 );
  m_lights.push_back( light3 );
  m_lights.push_back( light4 );

  m_bDrawLights = true;
}

//--------------------------------------------------------------
void ofApp::update()
{
    // animate our lights
    float time = ofGetElapsedTimef();
  
    m_lights[ 0 ].setPosition( 300.0f, 300.0f, -300.0f );
    m_lights[ 1 ].setPosition( sinf( time ) * 300.0f, 0.0f, 200.0f );
    m_lights[ 2 ].setPosition( sinf( time ) * -300.0f, 0.0f, 200.0f );
    m_lights[ 3 ].setPosition( 0.0f, sinf( time ) * 300.0f, 200.0f );
}

//--------------------------------------------------------------
void ofApp::draw()
{
  ofEnableDepthTest();
  
  ofClear( 128.0f, 128.0f, 128.0f, 255.0f ); // clear background to grey, alpha to 0

  m_camera.begin();
 
  m_shader.begin();

    // set the number of lights
    m_shader.setUniform1i( "uNumLights", m_lights.size() );

    // set all of our light properties
    // note: if you wanted, you could do this in a loop by constructing strings with the index
    
    // There are a number of ways of passing data to a shader. Uniforms are the easiest way to implement.
    // Another option is using a buffer object such as a uniform buffer object (UBO) or another buffer object such
    // as texture buffer object, shader storage object.
    
    m_shader.setUniform4f( "uLights[0].position", m_lights[ 0 ].getGlobalPosition() * ofGetCurrentViewMatrix().getRotate() );
    m_shader.setUniform3f( "uLights[0].color",    m_lights[ 0 ].color.r, m_lights[ 0 ].color.g, m_lights[ 0 ].color.b );
    m_shader.setUniform1f( "uLights[0].radius",   m_lights[ 0 ].radius );
    m_shader.setUniform1f( "uLights[0].power",    m_lights[ 0 ].power ); 

    m_shader.setUniform4f( "uLights[1].position", m_lights[ 1 ].getGlobalPosition() * ofGetCurrentViewMatrix() );
    m_shader.setUniform3f( "uLights[1].color",    m_lights[ 1 ].color.r, m_lights[ 1 ].color.g, m_lights[ 1 ].color.b );
    m_shader.setUniform1f( "uLights[1].radius",   m_lights[ 1 ].radius );
    m_shader.setUniform1f( "uLights[1].power",    m_lights[ 1 ].power );

    m_shader.setUniform4f( "uLights[2].position", m_lights[ 2 ].getGlobalPosition() * ofGetCurrentViewMatrix() );
    m_shader.setUniform3f( "uLights[2].color",    m_lights[ 2 ].color.r, m_lights[ 2 ].color.g, m_lights[ 2 ].color.b );
    m_shader.setUniform1f( "uLights[2].radius",   m_lights[ 2 ].radius );
    m_shader.setUniform1f( "uLights[2].power",    m_lights[ 2 ].power );

    m_shader.setUniform4f( "uLights[3].position", m_lights[ 3 ].getGlobalPosition() * ofGetCurrentViewMatrix() );
    m_shader.setUniform3f( "uLights[3].color",    m_lights[ 3 ].color.r, m_lights[ 3 ].color.g, m_lights[ 3 ].color.b );
    m_shader.setUniform1f( "uLights[3].radius",   m_lights[ 3 ].radius );
    m_shader.setUniform1f( "uLights[3].power",    m_lights[ 3 ].power );

    // an identity matrix represents position (0, 0, 0), rotation of (0, 0, 0), and scale of (1, 1, 1):
    // 1 0 0 0
    // 0 1 0 0
    // 0 0 0 1
    
    // We'll use this for our sphere so that it's drawn at 0,0,0 with no rotation and 100% scale
    ofMatrix4x4 identity;
    identity.newIdentityMatrix();
    
    m_shader.setUniformMatrix4f( "modelMatrix", identity, 1 );
    m_shader.setUniformMatrix4f( "viewMatrix", ofGetCurrentViewMatrix(), 1 );
    m_shader.setUniformMatrix4f( "normalMatrix", ofGetCurrentNormalMatrix(), 1 );

    // the shininess of our material - this affects the specular highlight
    m_shader.setUniform1f( "uMaterialShininess", 20.0f );

    m_sphereVbo.draw();
  
  m_shader.end();


  if ( true == m_bDrawLights )
  {
    for ( Light& light : m_lights )
    {
      light.customDraw();
    }
  }

  m_camera.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
  if ( key == ' ' ) // SPACE
  {
    m_bDrawLights = !m_bDrawLights; 
  }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
