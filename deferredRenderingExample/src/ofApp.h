#pragma once

#include "ofMain.h"

#include "ofxAssimpModelLoader.h"

class ofApp : public ofBaseApp
{
public:
    
    struct Light : public ofNode
    {
        ofVec3f color;
        float   radius;
        
        Light( ofVec3f _position, ofVec3f _color, float _radius )
        : color( _color )
        , radius( _radius )
        {
            setGlobalPosition( _position );
        }
    };
    
    enum AppMode
    {
        MODE_FULL_RENDER = 0,
        MODE_GBUFFER_DEBUG = 1,
        MODE_LIGHT_ACCUM_DEBUG = 2
    };
    
    static const int TEX_POSITION          = 0;
    static const int TEX_NORMALS           = 1;
    static const int TEX_ALBEDO            = 2;
    static const int TEX_LIGHTING_ACCUM    = 3;
    
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    void createFullScreenQuad();
    
    void drawScene();
    
    void drawDebugTextures();
    void drawPointLights();
    
    void directionalLightPass();
    void pointLightPass();
    
    ofMatrix3x3 mat4ToMat3( const ofMatrix4x4& mat4 );
    
public:
    AppMode      m_appMode;
    
    ofShader     m_gBufferShader;
    ofShader     m_directionalPassShader;
    ofShader     m_pointLightPassShader;
    ofShader     m_compositeShader;
    
    ofFbo        m_gBufferFbo;
    
    ofVboMesh    m_fsQuadVbo;
    ofVboMesh    m_sphereMesh;
    ofVboMesh    m_statueMesh;
    
    ofTexture    m_statueTexture;
    ofTexture    m_whiteTexture;
    
    ofEasyCam    m_camera;
    
    float        m_lightRadius;
    
    std::vector<Light>  m_pointLights;
    std::vector<Light>  m_directionalLights;
    
    int          m_lightIndexLimit;
    
};
