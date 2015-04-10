#pragma once

#include "ofMain.h"
#include "Billboard.h"
#include <vector>

class ofApp : public ofBaseApp
{
    // simple struct to hold the position, size, and orientation of each box
    struct Box
    {
        ofVec3f  pos;
        float    size;
        float    angle;
        ofVec3f  axis;
        
        Box( ofVec3f _pos, float _angle, ofVec3f _axis, float _size )
        :pos( _pos )
        ,size( _size )
        ,angle(_angle )
        ,axis( _axis )
        {}
    };
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
    
    void createRandomBoxes();
    void drawScene();


    ofShader           m_shader;
    ofVboMesh          m_boxVbo;
    ofImage            m_image;
    
    ofEasyCam          m_camera;
    
    std::vector<Box>   m_boxes;
};
