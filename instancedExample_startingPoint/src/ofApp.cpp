#include "ofApp.h"

//     _____    ___
//    /    /   /  /     instancedExample
//   /  __/ * /  /__    (c) ponies & light, 2015. All rights reserved.
//  /__/     /_____/    poniesandlight.co.uk
//
//  Created by @tgfrerer on 10/03/2014.
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


// Here's the general idea:
//
// 1.    Draw a field of instanced boxes, 256 in x, and 256 in y direction
// 2.    Push boxes up in y direction based on a height value sampled from the texture
// 2.a   Scale boxes in y direction to create "skyscrapers" based on sampled value
// 3.    Modulate height map texture sampling to create a "radar scanline" effect
// 4.    Add some colour effects
// 
// BONUS POINTS: use the camera live feed as your height map texture.

const int TILE_LENGTH = 256; ///< how many boxes in each x and z direction
const int NUM_INSTANCES = TILE_LENGTH * TILE_LENGTH; ///< total number of boxes

//--------------------------------------------------------------
void ofApp::setup(){

	// step 1: load our height map image

	ofDisableArbTex(); 	///< we need normalised image coordinates
	ofLoadImage(mTex1, "elevation2.png");
	ofEnableArbTex();

	// step 2: we will want to draw our geometry as instances
	// therefore, we will use a vbo

	mMshBox = ofBoxPrimitive(1, 1, 1).getMesh();

	// we will also need a camera, so we can move around in the scene
	mCam1.setupPerspective(false, 60, 0.1, 5000);
	mCam1.setDistance(1000); // set default camera distance to 1000
	mCam1.boom(400); // move camera up a little
	mCam1.lookAt(ofVec3f(0)); // look at centre of the world

}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

	ofBackground(0.65); // pro app gray =)

	ofSetColor(ofColor::white);

	mCam1.begin();
	{
		mMshBox.draw();
	}
	mCam1.end();

	// draw our frame rate
	ofSetColor(ofColor::white);
	ofDrawBitmapString(ofToString(ofGetFrameRate(),2,4,' '), ofGetWidth() - 4 * 8 - 50, 16 + 20);

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
