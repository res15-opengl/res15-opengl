#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    m_appMode = MODE_FULL_RENDER;
    
    ofDisableArbTex();
    //    ofEnableNormalizedTexCoords(); // Use GL_TEXTURE_2D Textures (normalized texture coordinates 0..1)
    
    // FBO with multiple render targets
    // We'll have 4 targets:
    //   0: view space position
    //   1: view space normals
    //   2: albedo texture (just our surfae color/texture, unlit)
    //   3: lighting accumulation - where we add up all our point and directional lights
    
    ofFbo::Settings fboSettings;
    fboSettings.width = ofGetWindowWidth();
    fboSettings.height = ofGetWindowHeight();
    
    // let's use 32 bit float values - we need more precision than 8 bit
    // (which is only 0-255) for things like normals and position
    // it also helps with lighting accumulation as we can achieve HDR lighting
    // which is then normalized down to 0-255 before final render
    fboSettings.internalformat = GL_RGBA32F;
    
    fboSettings.numSamples = 0;
    fboSettings.useDepth = true;
    fboSettings.useStencil = false;
    fboSettings.depthStencilAsTexture = false;
    fboSettings.depthStencilInternalFormat = GL_DEPTH32F_STENCIL8;
    fboSettings.textureTarget = GL_TEXTURE_2D;
    fboSettings.minFilter = GL_LINEAR;
    fboSettings.maxFilter = GL_LINEAR;
    fboSettings.wrapModeHorizontal = GL_CLAMP_TO_BORDER;
    fboSettings.wrapModeVertical = GL_CLAMP_TO_BORDER;
    fboSettings.numColorbuffers = 4;
    
    m_gBufferFbo.allocate( fboSettings );
    
    createFullScreenQuad();
    
    // Point light sphere
    ofIcoSpherePrimitive sphere;
    sphere.set( 2.0f, 2 ); // set radius 2.0f and resolution of 16
    sphere.enableColors();
    sphere.enableNormals();
    sphere.enableTextures();
    
    m_sphereMesh = sphere.getMesh();
    m_sphereMesh.setColorForIndices( 0, m_sphereMesh.getNumIndices(), ofFloatColor( 1.0f, 1.0f, 1.0f, 1.0f ) );
    
    
    // Statue model
    ofxAssimpModelLoader loader;
    loader.loadModel( "models/statue.obj" );
    m_statueMesh = loader.getMesh( 0 );
    m_statueMesh.setColorForIndices( 0, m_statueMesh.getNumIndices(), ofFloatColor( 1.0f, 1.0f, 1.0f, 1.0f ) );
    m_statueTexture = loader.getTextureForMesh( 0 );
    
    // Pure white texture for texturing point light spheres
    ofImage whiteImage( "textures/white.jpg" );
    m_whiteTexture = whiteImage.getTexture();
    
    // Camera
    m_camera.setupPerspective();
    
    m_camera.setNearClip( 0.1f );
    m_camera.setFarClip( 500.0f );
    m_camera.setDistance( 120.0f );
    m_camera.lookAt( ofVec3f( 0.0f, 0.0f, 0.0f ) );
	m_camera.setVFlip(true);
	
    
    // Shaders
    m_gBufferShader.load( "shaders/gbuffer.vert", "shaders/gbuffer.frag" );
    m_directionalPassShader.load( "shaders/directionalPass.vert", "shaders/directionalPass.frag" );
    m_pointLightPassShader.load( "shaders/pointLightPass.vert", "shaders/pointLightPass.frag" );
    m_compositeShader.load( "shaders/deferredComposite.vert", "shaders/deferredComposite.frag" );
    
    
    // create point and directional lights
    m_lightRadius = 30.0f;
    
    float lightSpacing = 40.0f;
    
    for ( int i = 0; i < 100; ++i )
    {
        ofVec3f lightPosition( ofRandom( -lightSpacing, lightSpacing ), ofRandom( -lightSpacing, lightSpacing ), ofRandom( -lightSpacing, lightSpacing ) );
        ofVec3f lightColor( ofRandom( 1.0f ), ofRandom( 1.0f ), ofRandom( 1.0f ) );
        
        Light light( lightPosition, lightColor, m_lightRadius );
        m_pointLights.push_back( light );
    }
    
    
    Light directionalLight( ofVec3f( 1.0f, 1.0f, 0.0f ), ofVec3f( 0.1f, 0.1f, 0.1f ), 0.0f );
    m_directionalLights.push_back( directionalLight );
    
    m_lightIndexLimit = 10;
}

//--------------------------------------------------------------
void ofApp::update()
{
    // rotate all of the point lights
    float speed = 0.0f;
    
    for ( auto& light : m_pointLights )
    {
        light.rotateAround( -ofGetElapsedTimef() / 10000.0f + speed, ofVec3f( 0.0f, 1.0f, 0.0f ), ofVec3f( 0.0f, 0.0f, 0.0f ) );
        speed += 0.005f;
    }
}

void ofApp::createFullScreenQuad()
{
    // -1.0 to +1.0 is the full viewport (screen) if you use these as vertices in your vertex shader (without multiplying by model, view, and projection matrices)
    ofVec3f vertices[4] =
    {
        ofVec3f( -1.0f, -1.0f, 0.0f ),
        ofVec3f( -1.0f,  1.0f, 0.0f ),
        ofVec3f(  1.0f,  1.0f, 0.0f ),
        ofVec3f(  1.0f, -1.0f, 0.0f ),
    };
    
    ofIndexType indices[6] =
    {
        0, 1, 2,
        0, 2, 3
    };
    
    ofVec2f texCoords[4] =
    {
        ofVec2f( 0.0f, 1.0f ),
        ofVec2f( 0.0f, 0.0f ),
        ofVec2f( 1.0f, 0.0f ),
        ofVec2f( 1.0f, 1.0f ),
    };
    
    m_fsQuadVbo.addVertices( vertices, 4 );
    m_fsQuadVbo.addTexCoords( texCoords, 4 );
    m_fsQuadVbo.addIndices( indices, 6 );
}

void ofApp::drawPointLights()
{
    // draw spheres at the position of each point light
    ofPushMatrix();
    
    ofMatrix4x4 viewMatrix = ofGetCurrentViewMatrix();
    
    for ( auto& light : m_pointLights )
    {
        ofMatrix4x4 transform = light.getGlobalTransformMatrix();
        
        ofSetMatrixMode( OF_MATRIX_MODELVIEW );
        ofLoadMatrix( transform * viewMatrix );
        
        m_sphereMesh.draw();
    }
    
    ofPopMatrix();
}

void ofApp::drawDebugTextures()
{
    int halfWidth = ofGetWindowWidth() / 2;
    int halfHeight = ofGetWindowHeight() / 2;
    
    m_gBufferFbo.getTexture( TEX_POSITION ).draw( 0, 0, halfWidth, halfHeight );
    m_gBufferFbo.getTexture( TEX_NORMALS ).draw( halfWidth, 0, halfWidth, halfHeight );
    m_gBufferFbo.getTexture( TEX_ALBEDO ).draw( 0, halfHeight, halfWidth, halfHeight);
    m_gBufferFbo.getTexture( TEX_LIGHTING_ACCUM ).draw( halfWidth, halfHeight, halfWidth, halfHeight );
    
    ofDrawBitmapString( "View Space Position", 50, 50 );
    ofDrawBitmapString( "View Space Normals", halfWidth + 50, 50 );
    ofDrawBitmapString( "Albedo", 50, halfHeight + 50 );
    ofDrawBitmapString( "Lighting Accumulation", halfWidth + 50, halfHeight + 50 );
}

ofMatrix3x3 ofApp::mat4ToMat3( const ofMatrix4x4& mat4 )
{
    return ofMatrix3x3( mat4._mat[0][0], mat4._mat[0][1], mat4._mat[0][2],
                        mat4._mat[1][0], mat4._mat[1][1], mat4._mat[1][2],
                        mat4._mat[2][0], mat4._mat[2][1], mat4._mat[2][2] );
}

// This function draws a full screen quad for each directional light,
// Directional lights have no defined shape / influence area, just a direction.
// Due to this, directional lights always light every fragment on the screen
void ofApp::directionalLightPass()
{
    m_directionalPassShader.begin();
    
    // bind GBuffer Textures
    m_directionalPassShader.setUniformTexture( "u_positionTex", m_gBufferFbo.getTexture( TEX_POSITION ), 0 );
    m_directionalPassShader.setUniformTexture( "u_normalsTex", m_gBufferFbo.getTexture( TEX_NORMALS ), 0 );
    
    // set camera near and far clip (this is needed for
    for ( auto& light : m_directionalLights )
    {
        // directional light 1
        m_directionalPassShader.setUniform3f( "u_lightDirectionVS", light.getGlobalPosition() );
        m_directionalPassShader.setUniform3f( "u_lightColor", light.color );
        
        // draw a full screen rectangle - it's a good idea to use viewport width and height vs. window width and height
        m_fsQuadVbo.draw();
    }
    
    m_directionalPassShader.end();
}


// This function draws a sphere at the position of each point light.
// The sphere's radius is equal to the point light's radius.
// This sphere is known as a light volume - it's a 3D volume that represents the area lit by the point light.
// Drawing this sphere light volume allows us to restrict performing lighting calculations in the shader
// to only the area that the light will affect.
void ofApp::pointLightPass()
{
    glEnable( GL_CULL_FACE );
    ofDisableDepthTest();
    
    m_camera.begin();
    
    m_pointLightPassShader.begin();
    
        // bind GBuffer Textures
        m_pointLightPassShader.setUniformTexture( "u_positionTex", m_gBufferFbo.getTexture( TEX_POSITION ), 0 );
        m_pointLightPassShader.setUniformTexture( "u_normalsTex", m_gBufferFbo.getTexture( TEX_NORMALS ), 1 );
        
        // inverse resolution - used to calculate texture coord
        m_pointLightPassShader.setUniform2f( "u_inverseResolution", ofVec2f( 1.0f / ofGetViewportWidth(), 1.0f / ofGetViewportHeight() ) );
        
        int m_currLightIndex = 0;
        
        for ( auto& light : m_pointLights )
        {
            if ( m_currLightIndex >= m_lightIndexLimit )
            {
                continue;
            }
            
            // check if camera is inside/outside the point light's sphere to decide which faces to draw (front or back)
            // this is important as if we draw both front and back when we're outside the sphere, the surface will be light twice
            float distToCamera = ( m_camera.getGlobalPosition() - light.getGlobalPosition() ).length();
            
            // if we're inside the sphere of the point light, only draw the backfaces
            if ( distToCamera > light.radius )
            {
                glCullFace( GL_BACK );
            }
            else // if we're outside of the point light's sphere, only draw the front faces
            {
                glCullFace( GL_FRONT );
            }
            
            m_pointLightPassShader.setUniform3f( "u_lightPositionVS", light.getGlobalPosition() * m_camera.getModelViewMatrix() );
            m_pointLightPassShader.setUniform1f( "u_lightRadius", light.radius );
            m_pointLightPassShader.setUniform3f( "u_lightColor", light.color );
            
            // draw the sphere light volume
            ofDrawSphere( light.getGlobalPosition(), light.radius );
            
            ++m_currLightIndex;
        }
    
    m_pointLightPassShader.end();
    
    m_gBufferFbo.getTexture( TEX_POSITION ).unbind();
    m_gBufferFbo.getTexture( TEX_NORMALS ).unbind();
    
    m_camera.end();
}

void ofApp::drawScene()
{
    m_camera.begin();
        // get normal matrix (it's calculated as the inverse transpose of camera's view matrix * the model matrix)
        /// We must recalculate it every time the camera position or orientation changes (the view matrix is changed)
        ofMatrix3x3 normalMatrix = mat4ToMat3( ofGetCurrentMatrix( OF_MATRIX_MODELVIEW ) );
        normalMatrix.invert();
        normalMatrix.transpose();
        m_gBufferShader.setUniformMatrix3f( "normalMatrix", normalMatrix );   // normal matrix - required to rotate our normals to the same orientation of the camera and scale of the model
        
        // draw our statue model - bind its texture to unit 0
        m_gBufferShader.setUniformTexture( "u_texture", m_statueTexture, 0 );
        
        glCullFace( GL_BACK );
        
        ofPushMatrix();
            // center the statue mesh
            ofTranslate( -10.0f, -50.0f, 0.0f );
            m_statueMesh.draw();
            m_statueTexture.unbind();
        ofPopMatrix();
        
        glCullFace( GL_FRONT );
    
        // draw our point light models
        m_gBufferShader.setUniformTexture( "u_texture", m_whiteTexture, 0 );
        drawPointLights();
        
    m_camera.end();
}

//--------------------------------------------------------------
void ofApp::draw()
{
    ofClear( 0, 0, 0, 255 ); // clear background to black, alpha to 255
    
    // States
    ofEnableAlphaBlending();
    ofEnableDepthTest();
    glEnable( GL_CULL_FACE );
    
    
    // RENDER OUR SCENE TO THE G-BUFFER (Position, Normals, Depth, Albedo)
    
    // We're going to render to the FBO (to all 4 attached texture targets)
    m_gBufferFbo.begin();
    
        // sets things up so that we can write to all render targets from our fragment shader
        m_gBufferFbo.activateAllDrawBuffers();
        
        // important to clear an FBO before drawing to it - this fills the textures with whatever clear color we have set
        ofClear( 0, 0, 0, 255 );
        
        m_gBufferShader.begin();
            drawScene();
        m_gBufferShader.end();
        
    m_gBufferFbo.end();
        
        
        
    // LIGHT ACCUMULATION PASS
    
    // We will be looping through all of the lights and accumulating each light's contribution
    // to a render target attached to our FBO (5th texture - TEX_LIGHTING_ACCUM)
    
    // We turn on additive blending so that each light pass we do is added/accumulated over the previous
    ofEnableBlendMode( OF_BLENDMODE_ADD );
    
    // Lights can be processed in any order, we don't want depth testing to remove some lights
    ofDisableDepthTest();
    
    glCullFace( GL_FRONT );
    
    // Bind our G Buffer FBO so that we can render to the texture
    m_gBufferFbo.bind();
    
        // Set our active draw buffer to the Lighting Accumulation texture
        // Since it's the ONLY active draw buffer, it's accessible from our fragment shaders at location=0
        m_gBufferFbo.setActiveDrawBuffer( TEX_LIGHTING_ACCUM );
        ofClear( 0, 0, 0, 255 );
        
        directionalLightPass();
        pointLightPass();
    
    m_gBufferFbo.unbind();
    
    
    // disable additive blending
    ofDisableBlendMode();
    glCullFace( GL_FRONT );
    
    
    if ( m_appMode == MODE_FULL_RENDER )
    {
        // RENDER THE FINAL RESULT (Light Accumulation * Albedo)
        // This pass combines our textured non-lit scene (Albedo) with our light contribution (Light Accumulation)
        
        // clear our window
        ofClear( 0, 0, 0, 255 );
        ofSetColor( 255, 255, 255 );
        
        m_compositeShader.begin();
            m_compositeShader.setUniformTexture( "u_albedoTex", m_gBufferFbo.getTexture( TEX_ALBEDO ), 0 );
            m_compositeShader.setUniformTexture( "u_lightingTex", m_gBufferFbo.getTexture( TEX_LIGHTING_ACCUM ), 1 );
            
            m_fsQuadVbo.draw();
        m_compositeShader.end();
        
        ofDrawBitmapString( "Final Render (Albedo * Light Accumulation) - LEFT/RIGHT Arrow keys to cycle through light accumulation", 50, 50 );
    }
    else if ( m_appMode == MODE_GBUFFER_DEBUG )
    {
        ofClear( 0, 0, 0, 255 );
        drawDebugTextures();
    }
    else if ( m_appMode == MODE_LIGHT_ACCUM_DEBUG )
    {
        ofClear( 0, 0, 0, 255 );
        ofSetColor( 255, 255, 255 );
        m_gBufferFbo.getTexture( TEX_LIGHTING_ACCUM ).draw( 0, 0, ofGetViewportWidth(), ofGetViewportHeight() );
        
        ofDrawBitmapString( "Light Accumulation Texture - LEFT/RIGHT Arrow keys to cycle through light accumulation", 50, 50 );
    }
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
    switch( key )
    {
        case '1':
            m_appMode = MODE_FULL_RENDER;
            break;
            
        case '2':
            m_appMode = MODE_GBUFFER_DEBUG;
            break;
            
        case '3':
            m_appMode = MODE_LIGHT_ACCUM_DEBUG;
            break;
            
        case OF_KEY_RIGHT:
            if ( m_lightIndexLimit < m_pointLights.size()-1 )
            {
                ++m_lightIndexLimit;
            }
            break;
            
        case OF_KEY_LEFT:
            if ( m_lightIndexLimit > 0 )
            {
                --m_lightIndexLimit;
            }
            break;
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
