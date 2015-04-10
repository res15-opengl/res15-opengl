#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
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
  
    // add them to our list of lights
    m_lights.push_back( light1 );
    m_lights.push_back( light2 );
  
    m_bDrawLights = true;
}

//--------------------------------------------------------------
void ofApp::update()
{
    float time = ofGetElapsedTimef();
  
    m_lights[ 0 ].setPosition( 300.0f, 300.0f, -300.0f );
      
    // aniamted the lights back and forth using sine
    m_lights[ 1 ].setPosition( sinf( time ) * 300.0f, 0.0f, 200.0f );
}

//--------------------------------------------------------------
void ofApp::draw()
{
    ofClear( 128.0f, 128.0f, 128.0f, 255.0f ); // clear background to grey, alpha to 0

    m_camera.begin();
    
        ofDrawAxis( 100.0f );
 
        if ( true == m_bDrawLights )
        {
            // call the customDraw() function of all our lights to draw them
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
