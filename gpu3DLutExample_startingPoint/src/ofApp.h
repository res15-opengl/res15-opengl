#pragma once

#include "ofMain.h"

//     _____    ___
//    /    /   /  /     gpu3DLutExample
//   /  __/ * /  /__    (c) ponies & light, 2015. All rights reserved.
//  /__/     /_____/    poniesandlight.co.uk
//
//  Created by @tgfrerer on 23/12/2014.
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

class ofApp : public ofBaseApp {
	
	GLuint					mLutTexID; ///< persistent id of our 3d lut texture object
	//ofVideoGrabber			mVid; ///< video grabber so that we have something to color-grade
	ofTexture				mTex1; ///< texture we want to color grade
	ofVboMesh				mVbo1; ///< static mesh used as canvas to render video grabber texture
	shared_ptr<ofShader>	mShd1; ///< shader used to render lookup table
	bool					isShaderDirty; ///< flag to indicate whether we should re-load shader upon next draw call

	/// \brief	Loads LUT from 24bit png
	/// \note	Keep the png at 8 bit per channel, rgb.
	void loadLut(const string& filename_);

	// Show last loaded LUT path for 2 seconds 
	string currentStatus;
	long whenStatusAppeared;

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
