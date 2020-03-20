#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	layerSize = glm::vec2(ofGetWidth(), ofGetHeight());
	
	ofEnableAntiAliasing();
	ofSetCircleResolution(100);
	ofEnableSmoothing();
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	
}
//
//void ofApp::initHolons() {
//	cout << "Creating elements for depth: " << holarchyDepth << endl;
//	int startingPopulation = 3000;
//
//	// Create the first row.
//	float xPos = 0;
//	// Create the holons first
//	for (int i = 0; i < startingPopulation; i++) {
//		glm::vec2 pos = glm::vec2(xPos, runningYPos);
//		Holon h (pos, depthHeight);
//		holons.push_back(h);
//		xPos = xPos + h.getSize().x;
//		if (xPos > ofGetWidth()) {
//			cout << "Max population is: " << i << endl;
//			break;
//		}
//	}
//
//	// Then create the pyramid.
//	pyramid.init(layerSize, holons);
//}
//
//void ofApp::createDepths() {
//	// Create the next holarchy depth.
//	holarchyDepth = holarchyDepth - 1;
//
//	if (holarchyDepth < 0) {
//		cout << "End Recursive Function" << endl;
//		return;
//	} else {
//		cout << "Creating elements for Depth: " << holarchyDepth << endl;
//		// Combine holons to create a new set of holons.
//		combineHolons();
//		pyramid.update(holons);
//		createDepths();
//	}
//}
//
//void ofApp::combineHolons() {
//	// Go through each holon and mate it with another holon.
//	// So two holons mate with each other.
//	int numLeaves = std::pow(2, holarchyDepth);
//
//	auto sourceHolon = holons[0];
//	auto oldSize = sourceHolon.getSize();
//	auto oldPosition = sourceHolon.getPosition();
//
//	// Size in the y direction doesn't change. Only x size changes.
//	glm::vec2 newSize = glm::vec2(layerSize.x/numLeaves, oldSize.y);
//
//	std::vector<Holon> newHolons;
//	for (int i = 0; i < numLeaves; i++) {
//		auto holonA = getRandomHolon();
//		auto holonB = getRandomHolon();
//
//		glm::vec2 newPos = glm::vec2(newSize.x*i, oldPosition.y + oldSize.y);
//		auto newHolon = crossover(holonA, holonB, newPos, newSize);
//		mutate(newHolon);
//
//		newHolons.push_back(newHolon);
//	}
//
//	// Set old holons to new holons.
//	holons = newHolons;
//}
//
//Holon ofApp::getRandomHolon() {
//	int randIdx = ofRandom(holons.size());
//
//	while (holons[randIdx].hasReproduced) {
//		randIdx = ofRandom(holons.size());
//	}
//
//	// Make sure it has reproduced, so we don't pick it up again.
//	auto holon = holons[randIdx];
//	holon.hasReproduced = true;
//	return holon;
//}
//
//
//Holon ofApp::crossover(Holon holonA, Holon holonB,
//												glm::vec2 newPos, glm::vec2 newSize) {
//	float newGenes[3];
//	for (int i = 0; i < 3; i++) {
//		if (ofRandom(1) < crossoverProbability) {
//			newGenes[i] = holonA.genes[i];
//		} else {
//			newGenes[i] = holonB.genes[i];
//		}
//	}
//
//	Holon newHolon(newPos, depthHeight);
//	std::copy(newHolon.genes, newHolon.genes+3, newGenes);
//
//	return newHolon;
//}
//
//void ofApp::mutate(Holon &holon) {
//	for (int i = 0; i < 3; i++) {
//		if (ofRandom(1) < mutationProbability) {
//			holon.genes[i] = ofRandom(1);
//		}
//	}
//}
