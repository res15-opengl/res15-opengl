#include "ofApp.h"


// HERE'S THE GENERAL IDEA:
//
// 1.	Load the LUT texture data
// 2.	Push the LUT textue data the GPU as a 3D Texture
// 3.	Bind your 3d texture to your post processing shader
// 4.	Bind your final scene texture to your post processing shader
// 5.	On the shader, use the 3D LUT texture to color grade your final scene

//--------------------------------------------------------------

void ofApp::setup(){

	// disable default (GL_TEXTURE_RECTANGLE_ARB) Texture mode and use
	// normalised texture coordinates (GL_TEXTURE_2D) instead.
	ofDisableArbTex();

	// set up defaults for texture ID, shader dirty flag
	mLutTexID = 0;
	isShaderDirty = true;

	// define a static mesh as a canvas to draw our
	// color graded texture using a shader later.
	{
		ofVec3f vertices[4] = {
			ofVec3f(0,0,0),
			ofVec3f(0,1,0),
			ofVec3f(1,1,0),
			ofVec3f(1,0,0),
		};
		ofIndexType indices[6] = {
			0,1,2,
			0,2,3
		};
		ofVec2f texCoords[4] = {
			ofVec2f(0,0),
			ofVec2f(0,1),
			ofVec2f(1,1),
			ofVec2f(1,0),
		};
		mVbo1.addVertices(vertices, 4);
		mVbo1.addTexCoords(texCoords, 4);
		mVbo1.addIndices(indices, 6);
	}

	// load default (identity) color cube (lookup table)
	loadLut("hald_4_identity.png");

	// open video grabber
	// mVid.setup(640,480, true);

	// load image we want to color grade
	ofLoadImage(mTex1, "images/brighton.jpg");

	string currentStatus = "";
	long whenStatusAppeared = 0;

}

//--------------------------------------------------------------

void ofApp::loadLut(const string& filename_){

	// TODO ...

	currentStatus = "loaded lut: " + filename_;
	whenStatusAppeared = ofGetElapsedTimeMillis();
}
//--------------------------------------------------------------

void ofApp::update(){

	// update video grabber texture if there is a new frame waiting.
	// mVid.update();

}

//--------------------------------------------------------------

void ofApp::draw(){

	// (re) load shader
	if (isShaderDirty){
		mShd1 = shared_ptr<ofShader>(new ofShader());
		mShd1->load("shaders/lut.vert","shaders/lut.frag");
		isShaderDirty = false;
	}

	ofBackground(ofColor::black);

	// Display the colour graded image texture by drawing the canvas geometry with
	// LUT texture and the image texture bound manually.
	ofPushMatrix();
	mShd1->begin();
	{
		// Bind textures:

		// 1. bind the render texture
		// mShd1->setUniformTexture("tex_unit_0", mVid.getTexture(), 0); // use this if you want to use a video =)
		mShd1->setUniformTexture("tex_unit_0", mTex1, 0);

		// 2. bind the LUT 3D texture - we don't need any 3d texture coordinates for this texture,
		// we just need to pass it as texture unit 1, and bind it to the GL_TEXTURE_3D texture
		// target.
		{
			// TODO ...
		}

		float texProportions = mTex1.getHeight() / mTex1.getWidth();
		// make sure our canvas respects the aspect ratio of the texture we want to colour grade.
		ofScale(ofGetWidth(), ofGetWidth() * texProportions);

		// this is where the magic happens
		mVbo1.draw();
	}
	mShd1->end();
	ofPopMatrix();

	ofSetColor(ofColor::white);
	ofDrawBitmapStringHighlight("Drag & Drop lut png to change lookup table.", 10, 20);

	// draw our status message if it is not older than 2 seconds
	if (ofGetElapsedTimeMillis() - whenStatusAppeared < 2000) {
		ofDrawBitmapStringHighlight(currentStatus, 10, 20 + 20);
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

	switch (key) {
	case ' ':
		isShaderDirty ^= true;
		break;
	default:
		break;
	}

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

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



	for (auto &f : dragInfo.files){
		ofLogNotice() << f;

		if (ofFilePath::getFileExt(f) == "png"){
			// we have a png file here, so let's load it.
			loadLut(f);
			break; /// we don't want to load more than the first file.
		}
	}
}
