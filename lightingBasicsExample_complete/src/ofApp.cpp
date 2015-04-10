#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    ofDisableArbTex();
    
    m_shader.load( "shader.vert", "directionalLight.frag" );
    m_shader.load( "shader.vert", "pointLight.frag" );
    
    m_image.load( "concrete.jpg" );
    
    createRandomBoxes();
    
    m_boxVbo = ofBoxPrimitive( 1.0f, 1.0f, 1.0f ).getMesh();
    
    m_boxVbo.enableColors();
    m_boxVbo.enableTextures();
    
    // set the color of all vertices to white
    for ( int i = 0; i < m_boxVbo.getNumVertices(); ++i )
    {
        m_boxVbo.addColor( ofColor( 255, 255, 255 ) );
    }
    
    m_camera.setDistance( 350.0f );
}

void ofApp::createRandomBoxes()
{
    // create randomly rotated boxes
    for ( int i = 0;  i < 1; ++i )
    {
        float x = 0;
        float y = 0;
        float z = 0;
        float size = 200.0f;
        float angle = ofRandom( 0.0f, 90.0f );
        
        ofVec3f axis = ofVec3f( ofRandom( 0.0f, 1.0f ), ofRandom( 0.0f, 1.0f ), ofRandom( 0.0f, 1.0f ) );
        axis.normalize();
        
        m_boxes.push_back( Box( ofVec3f(x, y, z), angle, axis, size ) );
    }
}

//--------------------------------------------------------------
void ofApp::update()
{
}

//--------------------------------------------------------------
void ofApp::draw()
{
  ofEnableDepthTest();
    
  ofClear( 200, 200, 200, 255 ); // clear background to grey, alpha to 0

  m_camera.begin();
      m_shader.begin();
    
          drawScene();
    
      m_shader.end();
  m_camera.end();
}

void ofApp::drawScene()
{
    m_image.bind();
    
    m_shader.setUniformTexture( "uTexture", m_image.getTexture(), 0 );
    
    // draw our randomly rotate boxes
    for ( std::vector<Box>::iterator it = m_boxes.begin(); it != m_boxes.end(); ++it )
    {
        ofPushMatrix();
            ofRotate( it->angle, it->axis.x, it->axis.y, it->axis.z );
            ofScale( it->size, it->size, it->size );
        
            m_shader.setUniformMatrix4f( "normalMatrix", ofGetCurrentNormalMatrix(), 1 );
        
            m_boxVbo.draw();
        ofPopMatrix();
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
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
