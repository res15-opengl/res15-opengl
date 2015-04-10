#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp
{
public:
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

    ofVideoGrabber m_webcam;
    ofVboMesh   m_fsQuadVbo;
    
    ofShader    m_vignetteShader;
    ofShader    m_filmGrainShader;
    ofShader    m_blurShader;
    ofShader    m_sobelShader;
    
    ofTexture   m_texture;
    
    float       m_blurRadius;
};
