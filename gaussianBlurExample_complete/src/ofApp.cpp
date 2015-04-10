#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    // Use GL_TEXTURE_2D Textures (normalized texture coordinates 0..1)
    ofDisableArbTex();
  
    // FBO with multiple render targets
    ofFbo::Settings fboSettings;
    fboSettings.width = ofGetWindowWidth();
    fboSettings.height = ofGetWindowHeight();
    fboSettings.internalformat = GL_RGBA8;
    fboSettings.numSamples = 0;
    fboSettings.useDepth = false;
    fboSettings.useStencil = false;
    fboSettings.textureTarget = GL_TEXTURE_2D;
    fboSettings.minFilter = GL_LINEAR;
    fboSettings.maxFilter = GL_LINEAR;
    
    // this is important to keep clean edges on the blur
    fboSettings.wrapModeHorizontal = GL_CLAMP_TO_EDGE;
    fboSettings.wrapModeVertical = GL_CLAMP_TO_EDGE;

    // create our FBO with the above settings
    m_fbo.allocate( fboSettings );
    
    // load our single pass shader
    m_singlePassBoxBlurShader.load( "shaders/passthrough.vert", "shaders/singlePassBoxBlur.frag" );

    // load our two pass blur shader
    m_blurHorizShader.load( "shaders/passthrough.vert", "shaders/blurHorizontal.frag" );
    m_blurVerticalShader.load( "shaders/passthrough.vert", "shaders/blurVertical.frag" );

    // the image we want to blur
    ofImage image( "images/image.jpg" );
    m_texture = image.getTexture();
    m_texture.setTextureMinMagFilter( GL_LINEAR, GL_LINEAR );
    m_texture.setTextureWrap( GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE ); // this is important to keep clean edges on the blur

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
        ofVec2f( 1.0f, 1.0f ),
        ofVec2f( 0.0f, 1.0f ),
        ofVec2f( 1.0f, 0.0f ),
        ofVec2f( 0.0f, 0.0f )
    };
    
    m_fsQuadVbo.addVertices( vertices, 4 );
    m_fsQuadVbo.addTexCoords( texCoords, 4 );
    m_fsQuadVbo.addIndices( indices, 6 );
}

//--------------------------------------------------------------
void ofApp::update()
{

}

//--------------------------------------------------------------
void ofApp::draw()
{
    ofClear( 0, 0, 0, 255 );

    ofDisableDepthTest();

    glEnable( GL_CULL_FACE );
    glCullFace( GL_BACK );

    bool bDoSinglePassBlur = true;

    if ( true == bDoSinglePassBlur )
    {
        // Completing the blur in a single pass instead of separate passes.
        m_singlePassBoxBlurShader.begin();
        m_singlePassBoxBlurShader.setUniform1f( "uBlurRadius", m_blurRadius );
        m_singlePassBoxBlurShader.setUniformTexture( "uTexture", m_texture, 0 );
        
        // the size of 1 pixel (texel) in a normalized (0.0 .. 1.0) texture uv space is:
        // pixel_width: 1.0 / width
        // pixel_height: 1.0 / height
        m_singlePassBoxBlurShader.setUniform2f( "uTexelSize", 1.0f / (float)ofGetViewportWidth(), 1.0f / (float)ofGetViewportHeight() );

            m_fsQuadVbo.draw();

        m_singlePassBoxBlurShader.end();
    }
    else
    {
        // Separable 2-pass blur
        
        // Horizontal blur pass
        // render the result to an FBO
        m_fbo.begin();
        {
            m_blurHorizShader.begin();
            m_blurHorizShader.setUniform1f( "uBlurRadius", m_blurRadius );
            m_blurHorizShader.setUniformTexture( "uTexture", m_texture, 0 );
            m_blurHorizShader.setUniform2f( "uTexelSize", 1.0f / (float)ofGetViewportWidth(), 1.0f / (float)ofGetViewportHeight() );

                m_fsQuadVbo.draw(); 

            m_blurHorizShader.end();
        }
        m_fbo.end();

        // Vertical blur pass - take the result of the horizontal blur pass and perform the vertical blur on it
        // This cuts down on the number of samples - a 9x9 tap blur will be 18 samples instead of 81!
        m_blurVerticalShader.begin();
            m_blurVerticalShader.setUniform1f( "uBlurRadius", m_blurRadius );

            // use result from horizontal pass as source texture
            m_blurVerticalShader.setUniformTexture( "uTexture", m_fbo.getTexture(), 0 );
            m_blurVerticalShader.setUniform2f( "uTexelSize", 1.0f / (float)ofGetViewportWidth(), 1.0f / (float)ofGetViewportHeight() );

                m_fsQuadVbo.draw();

        m_blurVerticalShader.end();
        
        // Note - you could render the 2nd pass to another fbo vs. the screen
        // This would allow you to use the blurred texture elsewhere for further post-processing for example
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
