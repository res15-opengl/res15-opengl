#pragma once

#include "ofMain.h"
#include "Billboard.h"
#include <vector>

// Simple light class that extends ofNode
// We extend ofNode so that we can take ad
class Light : public ofNode
{
public:
    // Define our different light types
  enum LightType
  {
    LIGHT_TYPE_POINT = 0,
    LIGHT_TYPE_DIRECTIONAL = 1
  };

    ofFloatColor color;   // our light's color
    float        radius;  // our light's radius (0.0 if it's a directional light )
    float        power;   // our light's power in lumens
    LightType    type;    // the type of this light (point or directional)
  
    // Since we extend ofNode, we can implement a function called customDraw()
  void customDraw()
  {
    if ( type == LIGHT_TYPE_POINT )
    {
        // get the modelView matrix by getting this ofNode's global transform matrix (model matrix)
        // and multiplying with by the current view matrix (whatever the active camera has set)
        ofMatrix4x4 modelViewMatrix = this->getGlobalTransformMatrix() * ofGetCurrentViewMatrix();
        
        // reset the rotation in this modelViewMatrix through our getBillboardMatrix() function (Billboard.h)
        ofMatrix4x4 billboardMatrix = getBillboardMatrix( modelViewMatrix );
     
        // draw a sphere for a point light
        ofPushMatrix();
        
            ofSetColor( color );
            ofDrawSphere( this->getGlobalPosition(), 10.0f );//radius );
            ofNoFill();
        
            // set the active modelview matrix to the billboard matrix
            ofLoadIdentityMatrix();
            ofMultMatrix( billboardMatrix );
            ofDrawCircle( 0.0f, 0.0f, 0.0f, radius );
            ofFill();
        
       ofPopMatrix();
    }
    else if ( type == LIGHT_TYPE_DIRECTIONAL )
    {
        // draw an arrow representing our directional light's direction
        float arrowTailLen = 50.0f;
        ofVec3f lightDir = -1.0 * this->getGlobalPosition().getNormalized();
        
        ofSetColor( color );

        ofPushMatrix();
            ofTranslate( this->getGlobalPosition() );
            ofDrawArrow( ofVec3f( 0.0f, 0.0f, 0.0 ), lightDir * arrowTailLen, 10.0f );

            ofDrawBitmapString( ofToString( this->getGlobalPosition() ), 0, 40 );
        ofPopMatrix();
    }
  }
};

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


    ofEasyCam          m_camera;

    std::vector<Light> m_lights;

    bool               m_bDrawLights;
};
