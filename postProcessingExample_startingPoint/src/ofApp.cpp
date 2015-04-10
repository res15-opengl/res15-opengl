#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    // Use GL_TEXTURE_2D Textures (normalized texture coordinates 0..1)
    ofDisableArbTex();
  
    m_vignetteShader.load( "shaders/passthrough.vert", "shaders/vignette.frag" );
    m_filmGrainShader.load( "shaders/passthrough.vert", "shaders/filmGrain.frag" );
    m_blurShader.load( "shaders/passthrough.vert", "shaders/boxBlur.frag" );
    m_sobelShader.load( "shaders/passthrough.vert", "shaders/sobel.frag" );

    bool bUseWebcam = false;
    
    if ( true == bUseWebcam )
    {
        m_webcam.initGrabber( 640, 480 );
        m_texture = m_webcam.getTexture();
    }
    else
    {
        ofImage image( "images/image.jpg" );
        m_texture = image.getTexture();
        m_texture.setTextureMinMagFilter( GL_LINEAR, GL_LINEAR );
        m_texture.setTextureWrap( GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE ); // this is important to keep clean edges on the blur
    }
    
    createFullScreenQuad();

    m_blurRadius = 1.0f;
}

void ofApp::createFullScreenQuad()
{
    // -1.0 to +1.0 is the full viewport (screen) if you use these as vertices in your vertex shader
    // (without multiplying by model, view, and projection matrices)

    ofVec3f vertices[4] =
    {
        ofVec3f(  1.0f,  1.0f, 0.0f ),
        ofVec3f( -1.0f,  1.0f, 0.0f ),
        ofVec3f(  1.0f, -1.0f, 0.0f ),
        ofVec3f( -1.0f, -1.0f, 0.0f )
    };
    
    ofIndexType indices[6] =
    {
        0, 1, 2,
        2, 1, 3
    };
    
    // Texture coordinates vary from 0.0 to 1.0 when they are in normalized format
    // ofDisableArbTex() was called earlier set that we're using normalized texture coordinates
    ofVec2f texCoords[4] =
    {
        ofVec2f( 1.0f, 0.0f ),
        ofVec2f( 0.0f, 0.0f ),
        ofVec2f( 1.0f, 1.0f ),
        ofVec2f( 0.0f, 1.0f )
    };
    
    m_fsQuadVbo.addVertices( vertices, 4 );
    m_fsQuadVbo.addTexCoords( texCoords, 4 );
    m_fsQuadVbo.addIndices( indices, 6 );
}

//--------------------------------------------------------------
void ofApp::update()
{
    m_webcam.update();
}

//--------------------------------------------------------------
void ofApp::draw()
{
    ofClear( 0, 0, 0, 255 );

    ofDisableDepthTest();

    glEnable( GL_CULL_FACE );
    glCullFace( GL_BACK );
    
    // size of 1 pixel in a texture if the texture coordinate range from 0..1
    ofVec2f inverseResolution( 1.0f / (float)ofGetViewportWidth(), 1.0f / (float)ofGetViewportHeight() );

    if ( true )
    {
        m_vignetteShader.begin();

            m_fsQuadVbo.draw();

        m_vignetteShader.end();
    }
    
    if ( false )
    {
        m_blurShader.begin();
        m_blurShader.setUniformTexture( "uTexture", m_texture, 0 );
 
            m_fsQuadVbo.draw();

        m_blurShader.end();
    }
    
    if ( false )
    {
        m_filmGrainShader.begin();
        m_filmGrainShader.setUniformTexture( "uTexture", m_texture, 0 );
        m_filmGrainShader.setUniform2f( "uTexelSize", inverseResolution );
        
            m_fsQuadVbo.draw();

        m_filmGrainShader.end();
    }
    
    if ( false )
    {
        m_sobelShader.begin();
        m_sobelShader.setUniformTexture( "uTexture", m_texture, 0 );
        m_sobelShader.setUniform2f( "uTexelSize", inverseResolution );

            m_fsQuadVbo.draw();

        m_sobelShader.end();
    }
  }

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y )
{
    m_blurRadius = (float)x / (float)ofGetWindowWidth() * 4.0f;
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
