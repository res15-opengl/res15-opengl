#pragma once

#include "ofMain.h"

//     _____    ___
//    /    /   /  /     frustumCullingExample
//   /  __/ * /  /__    (c) ponies & light, 2015. All rights reserved.
//  /__/     /_____/    poniesandlight.co.uk
//
//  Created by @tgfrerer on 17/02/2015.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.


//--------------------------------------------------------------

class SphereNode : public ofNode
{
public: 
	float radius;
	bool inFrustum;

	SphereNode()
		: inFrustum(false)
		, radius(10) 
	{
	};

	void customDraw(){

		// Note that customDraw draws within the context of the current node,
		// The origin for all draw operations herein is at the origin of the 
		// current node. "Custom Draw happend in the current Node's space."


		if (inFrustum){
			ofSetColor(ofColor::white);
		} else {
			ofSetColor(ofColor::red);
		}

		ofDrawSphere(radius);
	}
};

//--------------------------------------------------------------

class ofApp : public ofBaseApp{

	vector<ofVec4f> calculateFrustumPlanes(const ofMatrix4x4& projectionMatrix_){

		float fPL[6];
		vector<ofVec4f> fP;

		// We follow this guide to get the frustum planes: 
		// http://gamedevs.org/uploads/fast-extraction-viewing-frustum-planes-from-world-view-projection-matrix.pdf
		// but we have to transpose our projection matrix, since openFrameworks has different
		// row/column majority than default OpenGL.
		ofMatrix4x4 pM = ofMatrix4x4::getTransposedOf( projectionMatrix_ );

		fP.push_back( pM.getRowAsVec4f(3) - pM.getRowAsVec4f(0) ); // right
		fP.push_back( pM.getRowAsVec4f(3) - pM.getRowAsVec4f(1) ); // top
		fP.push_back( pM.getRowAsVec4f(3) - pM.getRowAsVec4f(2) ); // far
		fP.push_back( pM.getRowAsVec4f(3) + pM.getRowAsVec4f(0) ); // left
		fP.push_back( pM.getRowAsVec4f(3) + pM.getRowAsVec4f(1) ); // bottom
		fP.push_back( pM.getRowAsVec4f(3) + pM.getRowAsVec4f(2) ); // near

		for(int i=0; i<6; i++) {
			// get the length (= magnitude of the .xyz part of the row, so that 
			// we can normalize later)
			fPL[i] = ofVec3f(fP[i].x,fP[i].y,fP[i].z).length();
		}

		for(int i=0; i < 6; i++) {
			// normalize & apply frustum plane representation
			fP[i] = fP[i] / fPL[i];
		}
		return (fP);
	};


	//--------------------------------------------------------------

	bool getInFrustum(const vector<ofVec4f>frustumPlanes, ofVec3f sphereCentreInEyeSpace_, float sphereRadius_){
		bool inFrustum = true;

		// ...

		return inFrustum;
	}

	//--------------------------------------------------------------

	int mCountInFrustum;

	ofEasyCam mCam[2];
	int activeCam; ///< this will tell us through which camera we are watching the scene.

	vector<SphereNode> spheres;

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

};
