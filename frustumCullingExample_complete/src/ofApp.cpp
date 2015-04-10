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

	// initialise variables to default values

	activeCam = 1;
	mCountInFrustum = 0;

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

		vector<ofVec4f> frustumPlanes = calculateFrustumPlanes(mCam[0].getProjectionMatrix());

		// we could erase the 3rd element, that is the far plane,
		// therefore we are testing against an infinitely far frustum,
		// that is, an open frustum in view direction.

		// frustumPlanes.erase(frustumPlanes.begin() + 2); 

		ofMatrix4x4 mvm = mCam[0].getModelViewMatrix();

		for (auto & s : spheres){
			// --- note that we have to transform the sphere centres to the eye space of our frustum culling camera manually, 
			ofVec3f sInEyeSpace = s.getGlobalPosition() * mvm;
			s.inFrustum = getInFrustum(frustumPlanes, sInEyeSpace, s.radius);
		}

		// -----------

		// let's count the number of spheres within the frustum of camera 0:

		mCountInFrustum = 0;

		for(auto & s : spheres){
			s.draw();
			if (s.inFrustum) ++ mCountInFrustum;
		}

		if (activeCam == 1) {
			// let's draw the frustum of camera 0

			// but first, draw a stand-in for the actual camera.
			// this will draw a cube and an axis representation where the other camera sits.
			mCam[0].draw();

			// get the camera's near and far plane distance values.
			float nearC = mCam[0].getNearClip();
			float farC  = mCam[0].getFarClip();

			// Now comes the tricky bit: we want to draw the frustum where camera 0 sits.
			// Therefore, we move to camera 0 eye space, And draw the frustum fron there.

			ofPushMatrix(); // we are currently in world space.
			ofMultMatrix(mCam[0].getModelViewMatrix().getInverse());
			// we are now in camera2 view space!
			// where the origin is at camera2's global position.

			// draw the frustum in yellow, wireframe
			ofSetColor(ofColor::yellow);

			// we want to draw the frustum of camera 0. to do this, we grab the matrix that transforms
			// from view space into clip space (i.e. the projection matrix)
			// then we take our unit clip cube (i.e. the cube that delimits clip space)
			// (this cube is defined to be +-1 into  each x, y , z)
			// and transform it back into view space. We transform it back into
			// viewspace by applying the inverse transform viewspace -> clipspace
			// which is the inverse of applying the projection matrix, which is applying
			// the inverse projection matrix.

			// so first, grab camera 0's inverse projection matrix:
			ofMatrix4x4 projectionMatrixInverse = mCam[0].getProjectionMatrix().getInverse();

			// the edges of our unit cube in clip space:

			ofVec3f clipCube[8] = {
				ofVec3f(-1,-1,-1), ofVec3f(-1, 1,-1), ofVec3f( 1,-1,-1), ofVec3f( 1, 1,-1),
				ofVec3f(-1,-1, 1), ofVec3f(-1, 1, 1), ofVec3f( 1,-1, 1), ofVec3f( 1, 1, 1),
			};
			// since the clip cube is expressed in clip (=projection) space, we want this 
			// transformed back into our current space, view space, i.e. apply the inverse 
			// projection matrix to it.

			for (int i =0; i<8; i++){
				clipCube[i] = clipCube[i] * projectionMatrixInverse;
			} 

			// now draw our clip cube side edge rays - note that since the coordinates are
			// now in world space, we can draw them without applying any additional trans-
			// formations.
			for (int i=0;i<4;i++){
				ofLine(clipCube[i],clipCube[i+4]);
			}

			//// draw the clip cube cap
			ofRect(clipCube[0],clipCube[3].x-clipCube[0].x,clipCube[3].y-clipCube[0].y);
			ofRect(clipCube[4],clipCube[7].x-clipCube[4].x,clipCube[7].y-clipCube[4].y);

			ofPopMatrix(); // and back to world space!
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
