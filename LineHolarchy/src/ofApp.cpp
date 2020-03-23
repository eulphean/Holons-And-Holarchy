#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	initGui();
	
	ofEnableAntiAliasing();
	ofSetCircleResolution(100);
	ofEnableSmoothing();
}

//--------------------------------------------------------------
void ofApp::update(){
	stripHolarchy.update();
	frameHolarchy.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
	stripHolarchy.draw();
	frameHolarchy.draw();
	
	if (!hideGui) {
		gui.draw();
	}
}

void ofApp::exit() {
  gui.saveToFile("Holarchy.xml");
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == 'c') {
		frameHolarchy.clean();
		stripHolarchy.clean();
		if (isStripHolarchy) {
			stripHolarchy.setParams(&params.generalParams);
			stripHolarchy.init();
		} else {
			frameHolarchy.setParams(&params.generalParams);
			frameHolarchy.init();
		}
	}
	
	if (key == 's') {
		 if (isStripHolarchy) {
		 	stripHolarchy.saveToImage();
		 } else {
		 	frameHolarchy.saveToImage();
		 }
	}
	
	if (key == 'h') {
		hideGui = !hideGui;
	}
}

void ofApp::initGui() {
	otherParams.setName("System Params");
	otherParams.add(isStripHolarchy.set("Is Strip Holarchy", true));
	otherParams.add(params.generalParams);
	gui.setup(otherParams);
	gui.loadFrom("Holarchy.xml");
}
