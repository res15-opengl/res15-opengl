#include "ofApp.h"

// HERE'S THE GENERAL IDEA:
//
// 1. Draw a mesh using a vbo, 
// 2. Pass the camera view matrix separately to the shader
// 3. Pass three tileable textures to the shader, one for each coordinate plane.
// 3. On the vertex shader, add a world position and normal attribute to each vertex
// 4. On the fragment shader, use world attributes to calculate a blend of 
//    the three textures, based on world position & normal
//
// BONUS POINTS: Create some non-photorealistic effects

//--------------------------------------------------------------

void ofApp::setup(){

    isShaderDirty = true;
    
	ofDisableArbTex();
    
    // if you enable mipmaps before loading pixels into
    // your texture, mipmaps will be automatically
    // generated and used.
    
    mTex[0].enableMipmap();
    mTex[1].enableMipmap();
    mTex[2].enableMipmap();
    
    ofLoadImage(mTex[0], "assets/rock.jpg");
    ofLoadImage(mTex[1], "assets/dirt.jpg");
    ofLoadImage(mTex[2], "assets/grass.jpg");

	mTex[0].setTextureWrap(GL_REPEAT, GL_REPEAT);
	mTex[1].setTextureWrap(GL_REPEAT, GL_REPEAT);
	mTex[2].setTextureWrap(GL_REPEAT, GL_REPEAT);

	ofEnableArbTex();
    
    mCam1.setupPerspective(false, 60, 0.1, 5000);
    mCam1.setDistance(10);
    mCam1.boom(4);

	mMsh1.load("assets/geometry.ply");
	// mMsh1.load("assets/landscape.ply"); //< this will take a while to load!

}

//--------------------------------------------------------------

void ofApp::update(){
}

//--------------------------------------------------------------

void ofApp::draw(){

	ofBackground(0.3);
	

	// draw a funky background, but make sure not to touch the z-buffer.
	if (TRUE) {
		glDepthMask(GL_FALSE); ///< disable depth buffer 
		ofPushMatrix();
		ofTranslate( ofGetWidth() * 0.5 * (1.0 - 1.3 ) , ofGetHeight() * 0.5 * (1.0 - 1.0));
		ofScale(1.3, 1.0);
		ofBackgroundGradient( ofColor::fromHex(0xbbbbbb), ofColor::fromHex(0x000000));
		ofPopMatrix();
		glDepthMask(GL_TRUE);  ///< re-enable depht buffer
	}


	if (isShaderDirty) {
		// this will destruct any previously bound ofShaders
        shared_ptr<ofShader> tmpShader = std::shared_ptr<ofShader>(new ofShader());
        
        if (tmpShader->load("shaders/triPlanar.vert", "shaders/triPlanar.frag")){
            ofLogNotice() << "Shader loaded successfully";
            mShd1 = tmpShader;
        } else {
            ofLogError() << "Error while loading shader";
        }
		
		isShaderDirty = false;
	}

    // Contrary to its name, the camera's modelviewmatrix really is
    // just the view matrix. So let's fetch it, and store it so we can
    // pass it to the shader.
    
    ofMatrix4x4 viewMatrix = mCam1.getModelViewMatrix();
    
    mCam1.begin();


    if (mShd1){
        mShd1->begin();
        
        ofEnableDepthTest();
        glFrontFace(GL_CCW); 
//      glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        
        mShd1->setUniformTexture("tex_unit_0", mTex[0], 0);
        mShd1->setUniformTexture("tex_unit_1", mTex[1], 1);
        mShd1->setUniformTexture("tex_unit_2", mTex[2], 2);
        mShd1->setUniformMatrix4f("viewMatrix", viewMatrix);

		// Now draw the scene.
        ofPushMatrix();
        int iterations = 1; ///< try more!
        for (int i=0; i < iterations; i++){
            ofPushMatrix();
            ofTranslate(0.2 * ofVec3f(i % iterations - iterations * 0.5 , 0, i/iterations - iterations * 0.5) );
            ofRotate( (i % iterations) * 10, 1, 2, 3);

            // important: as soon as you change your modelView matrix (by using any of the ofTranslate / ofRotate methods)
            // you have to re-calculate the normal matrix and let the shader know!
            ofMatrix4x4 normalMatrix = ofGetCurrentNormalMatrix();
            mShd1->setUniformMatrix4f("normalMatrix", normalMatrix.getPtr());

			// mMsh1.draw();
            ofDrawBox(0, 0, 0, 2, 2, 2);
			// ofDrawSphere(0, 0, 1);
            ofPopMatrix();
        }
        ofPopMatrix();
        
//      glDisable(GL_CULL_FACE);
        ofDisableDepthTest();
        mShd1->end();
    }
    
	mCam1.end();

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

	if (key == ' ' ) {
		isShaderDirty = true;
	}
	else if (key == 'f'){
		ofToggleFullscreen();
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
