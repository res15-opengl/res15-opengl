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

	isShaderDirty = true; // initialise dirty shader

	// step 1: load our height map image

	ofDisableArbTex(); 	///< we need normalised image coordinates
	ofLoadImage(mTex1, "elevation2.png");
	ofEnableArbTex();

	// step 2: we will want to draw our geometry as instances
	// therefore, we will use a vbo

	mMshBox = ofBoxPrimitive(1, 1, 1).getMesh();
	mMshOptimisedBox = getOptimisedBox();

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

	// A word about these if(false){} sections
	// 
	// Each section contains one iteration of developing this example
	// This is so that you can see step-by step how the example was
	// written, with these sections as points where you compile the 
	// example and see what's happening.
	// 
	// They exist in the shaders, too. 
	//
	// Only one of them should be set to "true" at the same time.
	// 


	ofBackground(0.65); // pro app gray =)

	ofSetColor(ofColor::white);

	if (false) {
		mCam1.begin();

		mMshBox.draw();

		mCam1.end();
		// why can't we see anything?
		// because our box is too small.
	}

	if (false) {
		mCam1.setDistance(10); // let's pull the camera closer.
		mCam1.begin();

		// draw lots of boxes.
		mMshBox.drawInstanced(OF_MESH_FILL, NUM_INSTANCES);

		mCam1.end();

		// Whooo- i get only 5 frames per second!!!

		// Why is our frame rate so bad?
		// OVERDRAW: a tiny number of pixels is drawn over and over again.
		
		// It's just the ROP's working -> that's the blending units.
		// The graphics card cannot work properly if you destroy its parallelism.
	}

	if (false) {
		mCam1.setDistance(10); // let's pull the camera closer.

		mCam1.begin();

		// alpha blending is enabled by default,
		// let's see if disabling it will help us a little.
		ofDisableBlendMode();

		// also, let's get rid of the back faces.
		glEnable(GL_CULL_FACE); // wohooo! 200% performance boost.

		// draw lots of boxes.
		mMshBox.drawInstanced(OF_MESH_FILL, NUM_INSTANCES);
		glDisable(GL_CULL_FACE);

		ofEnableAlphaBlending();

		mCam1.end();
	}

	if (false) {

		// now, let's use a vertex shader to pull the boxes apart.

		if (isShaderDirty){
			// dirty shader pattern:
			shared_ptr<ofShader> tmpShader = shared_ptr<ofShader>(new ofShader);

			if (tmpShader->load("shaders/instanced-1.vert", "shaders/instanced.frag")){
				mShd1 = tmpShader;
				ofLogNotice() << ofGetTimestampString() << " reloaded Shader.";
			} else {
				ofLogError() << "Shader did not load successfully.";
			}

			isShaderDirty = false;
		}

		// mCam1.setDistance(10); // no need to pull the camera closer, there'll be many boxes to see, soon.

		mCam1.begin();

		// alpha blending is enabled by default,
		// let's see if disabling it will help us a little.
		ofDisableBlendMode();

		// also, let's get rid of the back faces.
		glEnable(GL_CULL_FACE); // wohooo! 200% performance boost.

		if (mShd1) {
			mShd1->begin();
			// draw lots of boxes
			mMshBox.drawInstanced(OF_MESH_FILL, NUM_INSTANCES);
			mShd1->end();
			// now switch over to the shaders.
		}
		glDisable(GL_CULL_FACE);
		ofEnableAlphaBlending();

		mCam1.end();
	}

	if (false) {

		// Let's add some animation

		// We're still using the same shader. (but this is for the last iteration block of shader instanced-1.vert)
		// but now, let's introduce a time-stepper so we can animate...
		// and also a texture to manipulate our instances.

		if (isShaderDirty){
			// dirty shader pattern:
			shared_ptr<ofShader> tmpShader = shared_ptr<ofShader>(new ofShader);

			if (tmpShader->load("shaders/instanced-1.vert", "shaders/instanced.frag")){
				mShd1 = tmpShader;
				ofLogNotice() << ofGetTimestampString() << " reloaded Shader.";
			} else {
				ofLogError() << "Shader did not load successfully.";
			}

			isShaderDirty = false;
		}

		// mCam1.setDistance(10); // let's pull the camera closer.

		mCam1.begin();

		// alpha blending is enabled by default,
		// let's see if disabling it will help us a little.
		ofDisableBlendMode();

		// also, let's get rid of the back faces.
		glEnable(GL_CULL_FACE); // wohooo! 200% performance boost.

		// and enable depth testing.

		ofEnableDepthTest();

		if (mShd1) {

			mShd1->begin();
			mShd1->setUniform1f("uElapsedTimef", ofGetElapsedTimef());
			mShd1->setUniformTexture("tex_unit_0", mTex1, 0); // first texture unit has index 0, name is not that important!
			// draw lots of boxes
			mMshBox.drawInstanced(OF_MESH_FILL, NUM_INSTANCES);
			mShd1->end();
			// now switch over to the shaders.

		}
		ofDisableDepthTest();

		glDisable(GL_CULL_FACE);
		ofEnableAlphaBlending();

		mCam1.end();
	}

	if (true) {

		// let's do something slightly more funky with this.

		// let's optimise this:

		// we're going to use a flat shaded box, with only 8 vertices per box.
		// That's going to reduce the amount of geometry (and vertex calculations) massively.

		// We're going to use a shader which scales the boxes,
		// so we can get something like a skyline.

		if (isShaderDirty){
			// dirty shader pattern:
			shared_ptr<ofShader> tmpShader = shared_ptr<ofShader>(new ofShader);

			if (tmpShader->load("shaders/instanced-2.vert", "shaders/instanced-2.frag")){
				mShd1 = tmpShader;
				ofLogNotice() << ofGetTimestampString() << " reloaded Shader.";
			} else {
				ofLogError() << "Shader did not load successfully.";
			}

			isShaderDirty = false;
		}

		// mCam1.setDistance(10); // let's pull the camera closer.

		mCam1.begin();

		// alpha blending is enabled by default,
		// let's see if disabling it will help us a little.
		ofDisableBlendMode();

		// also, let's get rid of the back faces.
		glEnable(GL_CULL_FACE); // wohooo! 200% performance boost.
		glFrontFace(GL_CCW);
		// tell GLSL to use the first vertex for flat shading the whole triangle, instead
		// of the last one, as would be the default.

		glProvokingVertex(GL_FIRST_VERTEX_CONVENTION);
		// and enable depth testing.

		ofEnableDepthTest();

		if (mShd1) {

			mShd1->begin();
			mShd1->setUniform1f("uElapsedTimef", ofGetElapsedTimef());
			mShd1->setUniformTexture("tex_unit_0", mTex1, 0); // first texture unit has index 0, name is not that important!
			// draw lots of boxes
			mMshOptimisedBox.drawInstanced(OF_MESH_FILL, NUM_INSTANCES);

			mShd1->end();
			// now switch over to the shaders.

		}
		ofDisableDepthTest();

		glDisable(GL_CULL_FACE);
		ofEnableAlphaBlending();

		mCam1.end();
	}

	// draw our frame rate
	ofSetColor(ofColor::white);
	ofDrawBitmapString(ofToString(ofGetFrameRate(),2,4,' '), ofGetWidth() - 4 * 8 - 50, 16 + 20);

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

	switch (key) {
	case ' ':
		isShaderDirty = true;
		break;
	default:
		break;
	}

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
