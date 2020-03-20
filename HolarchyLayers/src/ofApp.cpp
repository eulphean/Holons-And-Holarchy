#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	// A4 resolution. 
	layerResolution = glm::vec2(3508, 2480);
	// 2^n, where n = {1 ... n }
	// Total Layers created will n + 1;
	numGenerations = 12;
	// Assign rows and columns.
	gridSize = glm::vec2(std::pow(2, numGenerations), std::pow(2, numGenerations));
	
	// Can change this with a GUI probably.
	mutationProbability = 0.1;
	crossoverProbability = 0.3;
	
	// Initial layers.
	initSystem();
	// Recursive function to create fbo layers. 
	createLayers();
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	layers[layerIdx].draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == ' ') {
		layerIdx = (layerIdx + 1) % layers.size();
	}
	
	if (key == 'r') {
		// Clear all the layers.
		layers.clear();
		holons.clear();
		
		// Initialize the system again.
		gridSize = glm::vec2(std::pow(2, numGenerations), std::pow(2, numGenerations));
		initSystem();
		createLayers();
	}
	
	if (key == 's') {
		layers[layerIdx].saveToImage(layerIdx);
		cout << "Successfully saved layer - " << layerIdx << endl; 
	}
}

void ofApp::initSystem() {
	//	// Dark matter and only dark matter.
	//	Layer darkLayer(gridSize);
	//	darkLayer.create(holons);
	//	layers.push_back(darkLayer);
	
	// Size for the first holon layer.
	glm::vec2 size = glm::vec2(layerResolution.x/gridSize.x, layerResolution.y/gridSize.y);
	// Create the set of holons.
	for (int row = 0; row < gridSize.y; row++) {
		std::vector<Holon> cols;
		for (int col = 0; col < gridSize.x; col++) {
			glm::vec2 pos = glm::vec2(size.x*col, size.y*row);
			Holon h (pos, size);
			cols.push_back(h);
		}
		holons.push_back(cols);
	}
	
	// Create the first layer.
	Layer layer(layerResolution, gridSize);
	layer.create(holons);
	layers.push_back(layer);
	layer.saveToImage(layers.size());
}

void ofApp::createLayers() {
	if (gridSize.x == 1 && gridSize.y == 1) {
		cout << "End Recursive Function" << endl;
		return;
	}
	
	// Update the holon data structure.
	combineHolons(true);
	
	cout << "Combining... Grid across Width  " << gridSize.x << endl;
	gridSize.x = gridSize.x/2; // Layers get reduced to half.
	Layer layerWidth(layerResolution, gridSize);
	layerWidth.create(holons);
	layers.push_back(layerWidth);
	layerWidth.saveToImage(layers.size());
	
	combineHolons(false);
	
	cout << "Combining... Grid across Height  " << gridSize.y << endl;
	gridSize.y = gridSize.y/2; // Layers get reduced to half.
	Layer layerHeight(layerResolution, gridSize);
	layerHeight.create(holons);
	layers.push_back(layerHeight);
	layerHeight.saveToImage(layers.size());
	
	createLayers();
}

void ofApp::combineHolons(bool acrossWidth) {
	// Combination of Holons will follow the natural style of reproduction,
	// where two parents combine to create an offspring.
	int offsetW = 1; int offsetH = 1;
	
	if (acrossWidth) {
		offsetW = 2;
	} else {
		offsetH = 2;
	}
	
	std::vector<std::vector<Holon>> newHolons;
	for (int row = 0; row < gridSize.y; row=row+offsetH) {
		std::vector<Holon> newRow;
		for (int col = 0; col < gridSize.x; col=col+offsetW) {
			// We need to use sourceHolon's position and size to calculate (position, size) for our next Holon.
			auto sourceHolon = holons[row][col];
			auto oldSize = sourceHolon.getSize(); auto oldPos = sourceHolon.getPosition();
			
			glm::vec2 newSize;
			
			// New Holon dimension.
			if (acrossWidth) {
				newSize = glm::vec2(oldSize.x*2, oldSize.y);
			} else {
				newSize = glm::vec2(oldSize.x, oldSize.y*2);
			}
			
			glm::vec2 newPos = glm::vec2(oldPos.x, oldPos.y); // We use the position holanA as the position for our new holan.

			auto holanA = getRandomHolon();
			auto holanB = getRandomHolon();
			auto newHolon = crossover(holanA, holanB, newPos, newSize);
			mutate(newHolon); // Passed by reference

			newRow.push_back(newHolon);
		}
		newHolons.push_back(newRow);
	}
	
	// Not sure if this will work, but worth trying.
	holons = newHolons;
}


Holon ofApp::getRandomHolon() {
	int randRow = ofRandom(gridSize.y);
	int randCol = ofRandom(gridSize.x);
	
	while (holons[randRow][randCol].hasReproduced) {
		randRow = ofRandom(gridSize.y);
		randCol = ofRandom(gridSize.x);
	}
	
	// Make sure it has reproduced, so we don't pick it up again.
	auto holon = holons[randRow][randCol];
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
