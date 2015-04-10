#include "ofApp.h"

// HERE'S THE GENERAL IDEA:
//
// 0. Place some spheres at random positions into the scene.
// 1. Calculate which spheres can be culled from camera 0's view
// 2. Flag the culled spheres, so they will be rendered in red instead of white.
// 3. Draw the scene.
// 4. Then: BONUS: draw the culling camera's frustum (camera 0) into the scene,
//    But only, if we're not watching the scene through that camera.

//--------------------------------------------------------------

void ofApp::setup(){

	// initialize variables to default values

	activeCam = 1;
	mCountInFrustum = 0; ///< number of elements in frustum

	ofSetSphereResolution(4);

	// populate the scene with 100 randomly distributed spheres

	for (int i =0; i < 100; i++){
		SphereNode s;
		s.setGlobalPosition(ofVec3f(ofRandom(-100,100),ofRandom(-100,100),ofRandom(-100,100)));
		s.radius = ofRandom(4,15);
		spheres.push_back(s);
	}

	mCam[0].setupPerspective(false,60, 20, 300);
	mCam[0].disableMouseInput();
	mCam[0].setDistance(300);
	mCam[0].reset();


	mCam[1].setupPerspective(false,30, 0.1, 20000);
	mCam[1].enableMouseInput();
	mCam[1].setDistance(2600);
	mCam[1].reset();

}

//--------------------------------------------------------------

void ofApp::update(){

}

//--------------------------------------------------------------

void ofApp::draw(){
	ofBackground(ofFloatColor(0.23));
	ofSetColor(ofColor::white);
	ofNoFill();
	ofEnableDepthTest();

	mCam[activeCam].begin();
	{

		ofSetColor(ofColor::white);

		// calculate the frustum Planes
		vector<ofVec4f> frustumPlanes = calculateFrustumPlanes(mCam[0].getProjectionMatrix());

		// fetch the culling camera's view matrix so that we can 
		// calculate the sphere's positions in camera 0's space.
		ofMatrix4x4 mvm = mCam[0].getModelViewMatrix();

		for (auto & s : spheres){
			// --- note that we have to transform the sphere centres to the eye 
			// space of our frustum culling camera manually
			ofVec3f sInEyeSpace = s.getGlobalPosition() * mvm;
			// inFrustum is a flag in the sphere object that controls the 
			// object's draw colour:
			// Culled objects will be drawn in red, others will be drawn in white.
			s.inFrustum = getInFrustum(frustumPlanes, sInEyeSpace, s.radius);
		}

		// -----------

		// let's draw & count the number of spheres within the frustum of camera 0:

		mCountInFrustum = 0;

		for(auto & s : spheres){
			s.draw();
			if (s.inFrustum) ++ mCountInFrustum;
		}

		if (activeCam == 1) {
			//// BONUS: let's draw the frustum of camera 0

			//// the edges of our unit cube in clip space:
			//ofVec3f clipCube[8] = {
			//	ofVec3f(-1,-1,-1), ofVec3f(-1, 1,-1), ofVec3f( 1,-1,-1), ofVec3f( 1, 1,-1),
			//	ofVec3f(-1,-1, 1), ofVec3f(-1, 1, 1), ofVec3f( 1,-1, 1), ofVec3f( 1, 1, 1),
			//};
			
			//// draw the clip cube caps
			// ofRect(clipCube[0],clipCube[3].x-clipCube[0].x,clipCube[3].y-clipCube[0].y);
			// ofRect(clipCube[4],clipCube[7].x-clipCube[4].x,clipCube[7].y-clipCube[4].y);

		}
	}

	mCam[activeCam].end();

	ofDrawBitmapStringHighlight(
		"Active Cam: "+ ofToString(activeCam,4,' ') + "\n" +
		"Visible spheres: "+ ofToString(mCountInFrustum,4,' ') + "\n" +
		"Press 'c' to switch cameras.", 200, 20);

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	switch (key)
	{
	case 'c':
		mCam[activeCam].disableMouseInput();
		activeCam = (activeCam + 1) % 2;
		mCam[activeCam].enableMouseInput();
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
