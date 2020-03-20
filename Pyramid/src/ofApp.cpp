#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	layerSize = glm::vec2(ofGetWidth(), ofGetHeight());
	
	ofEnableAntiAliasing();
	ofSetCircleResolution(100);
	ofEnableSmoothing();
	
	// Number of divisions the tree will be subdivided in.
	holarchyDepth = 20;
	
	// Can change this with a GUI probably.
	mutationProbability = 0.1;
	crossoverProbability = 0.1;
	
	// Initial set of holarns with current depth.
	initHolons();

	// Recursive function to create several depths.
	// createDepths();
	
	// Prepare fbo.
	// createTriangle();
}

//--------------------------------------------------------------
void ofApp::update(){
	// Lowest hierarchy it can serve is 0.
	if (holarchyDepth >= 1) {
		holarchyDepth = holarchyDepth - 1;
		cout << "Creating elements for Depth: " << holarchyDepth << endl;
		combineHolons();
		pyramid.update(holons);
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	pyramid.draw();
	
	
	// pyramid.draw();
	//triangleFbo.draw(0, 0);
}

void ofApp::createTriangle() {
	triangleFbo.allocate(layerSize.x, layerSize.y, GL_RGBA);
	triangleFbo.begin();
		ofBackground(0);
		// Initialize triangle points.
		glm::vec2 left = glm::vec2(0, 0);
		glm::vec2 right = glm::vec2(triangleFbo.getWidth(), 0);
		glm::vec2 top = glm::vec2(triangleFbo.getWidth()/2, triangleFbo.getHeight());
		ofSetColor(255);
		ofDrawTriangle(left, right, top);
	triangleFbo.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == 'r') {
		initHolons();
		createDepths();
	}
	
	if (key == 's') {
		pyramid.saveToImage(ofGetUnixTime(), triangleFbo.getTexture());
	}
}

void ofApp::initHolons() {
	cout << "Creating elements for depth: " << holarchyDepth << endl;
	int numLeaves = std::pow(2, holarchyDepth);
	
	// Create the first row.
	glm::vec2 size = glm::vec2(layerSize.x/numLeaves, layerSize.y/(holarchyDepth+1));
	// Create the holons first
	for (int i = 0; i < numLeaves; i++) {
		glm::vec2 pos = glm::vec2(size.x * i, 0);
		Holon h (pos, size);
		holons.push_back(h);
	}
	
	// Then create the pyramid.
	pyramid.init(layerSize, holons);
}

void ofApp::createDepths() {
	// Create the next holarchy depth. 
	holarchyDepth = holarchyDepth - 1;
	
	if (holarchyDepth < 0) {
		cout << "End Recursive Function" << endl;
		return;
	} else {
		cout << "Creating elements for Depth: " << holarchyDepth << endl;
		// Combine holons to create a new set of holons.
		combineHolons();
		pyramid.update(holons);
		createDepths();
	}
}

void ofApp::combineHolons() {
	// Go through each holon and mate it with another holon.
	// So two holons mate with each other.
	int numLeaves = std::pow(2, holarchyDepth);

	auto sourceHolon = holons[0];
	auto oldSize = sourceHolon.getSize();
	auto oldPosition = sourceHolon.getPosition();
	
	// Size in the y direction doesn't change. Only x size changes.
	glm::vec2 newSize = glm::vec2(layerSize.x/numLeaves, oldSize.y);
	
	std::vector<Holon> newHolons;
	for (int i = 0; i < numLeaves; i++) {
		auto holonA = getRandomHolon();
		auto holonB = getRandomHolon();
		
		glm::vec2 newPos = glm::vec2(newSize.x*i, oldPosition.y + oldSize.y);
		auto newHolon = crossover(holonA, holonB, newPos, newSize);
		mutate(newHolon);
	
		newHolons.push_back(newHolon);
	}
	
	// Set old holons to new holons.
	holons = newHolons;
}

Holon ofApp::getRandomHolon() {
	int randIdx = ofRandom(holons.size());

	while (holons[randIdx].hasReproduced) {
		randIdx = ofRandom(holons.size());
	}

	// Make sure it has reproduced, so we don't pick it up again.
	auto holon = holons[randIdx];
	holon.hasReproduced = true;
	return holon;
}


Holon ofApp::crossover(Holon holonA, Holon holonB,
												glm::vec2 newPos, glm::vec2 newSize) {
	float newGenes[3];
	for (int i = 0; i < 3; i++) {
		if (ofRandom(1) < crossoverProbability) {
			newGenes[i] = holonA.genes[i];
		} else {
			newGenes[i] = holonB.genes[i];
		}
	}

	Holon newHolon(newPos, newSize);
	std::copy(newHolon.genes, newHolon.genes+3, newGenes);

	return newHolon;
}

void ofApp::mutate(Holon &holon) {
	for (int i = 0; i < 3; i++) {
		if (ofRandom(1) < mutationProbability) {
			holon.genes[i] = ofRandom(1);
		}
	}
}

//	if (gridSize.x == 1 && gridSize.y == 1) {
//		cout << "End Recursive Function" << endl;
//		return;
//	}
//
//	// Update the holon data structure.
//	combineHolons(true);
//
//	cout << "Combining... Grid across Width  " << gridSize.x << endl;
//	gridSize.x = gridSize.x/2; // Layers get reduced to half.
//	Layer layerWidth(layerResolution, gridSize);
//	layerWidth.create(holons);
//	layers.push_back(layerWidth);
//	layerWidth.saveToImage(layers.size());
//
//	combineHolons(false);
//
//	cout << "Combining... Grid across Height  " << gridSize.y << endl;
//	gridSize.y = gridSize.y/2; // Layers get reduced to half.
//	Layer layerHeight(layerResolution, gridSize);
//	layerHeight.create(holons);
//	layers.push_back(layerHeight);
//	layerHeight.saveToImage(layers.size());

	//createLayers();
//}
//
//void ofApp::combineHolons(bool acrossWidth) {
////	// Combination of Holons will follow the natural style of reproduction,
////	// where two parents combine to create an offspring.
////	int offsetW = 1; int offsetH = 1;
////
////	if (acrossWidth) {
////		offsetW = 2;
////	} else {
////		offsetH = 2;
////	}
////
////	std::vector<std::vector<Holon>> newHolons;
////	for (int row = 0; row < gridSize.y; row=row+offsetH) {
////		std::vector<Holon> newRow;
////		for (int col = 0; col < gridSize.x; col=col+offsetW) {
////			// We need to use sourceHolon's position and size to calculate (position, size) for our next Holon.
////			auto sourceHolon = holons[row][col];
////			auto oldSize = sourceHolon.getSize(); auto oldPos = sourceHolon.getPosition();
////
////			glm::vec2 newSize;
////
////			// New Holon dimension.
////			if (acrossWidth) {
////				newSize = glm::vec2(oldSize.x*2, oldSize.y);
////			} else {
////				newSize = glm::vec2(oldSize.x, oldSize.y*2);
////			}
////
////			glm::vec2 newPos = glm::vec2(oldPos.x, oldPos.y); // We use the position holanA as the position for our new holan.
////
////			auto holanA = getRandomHolon();
////			auto holanB = getRandomHolon();
////			auto newHolon = crossover(holanA, holanB, newPos, newSize);
////			mutate(newHolon); // Passed by reference
////
////			newRow.push_back(newHolon);
////		}
////		newHolons.push_back(newRow);
////	}
////
////	// Not sure if this will work, but worth trying.
////	holons = newHolons;
//}
//
//
