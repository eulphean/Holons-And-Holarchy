#include "StripHolarchy.h"

StripHolarchy::StripHolarchy() {
	holarchyDepth = 0;
}

// Creates the first row of Holons that determines how the rest of the tree will flow and get combined.
void StripHolarchy::init() {
	// Set new depth to tackle.
	holarchyDepth = params->getInt("Holarchy Depth");
	glm::vec2 bufferSize = params->getVec2f("Buffer Size");
	auto orderedStart = params->getBool("Ordered Start");
	auto monochrome = params->getBool("Monochrome");
	int numLeavesInCurrentLayer = std::pow(2, holarchyDepth);
	
	// 0-n is actually n+1 layers, so we add 1 to create an extra space for another layer (in y)
	auto sizeX = bufferSize.x/numLeavesInCurrentLayer;
	auto sizeY = bufferSize.y/(holarchyDepth+1);
	glm::vec2 holonSize = glm::vec2(sizeX, sizeY);
	
	// Initial batch of Holons.
	// [Important] Because it determines how the flow will look like.
	for (int i = 0; i < numLeavesInCurrentLayer; i++) {
		glm::vec2 holonPos = glm::vec2(holonSize.x * i, 0);
		Holon h (holonPos, holonSize, monochrome);
		// Update genes for an ordered start.
		if (orderedStart) {
			if (i%2==0) {
				h.updateGenes(glm::vec3(0.0, 0.0, 0.0));
			} else {
				h.updateGenes(glm::vec3(1.0, 1.0, 1.0));
			}
		}
		holons.push_back(h);
	}
	
	// Initiate the holarchy.
	Holarchy::initStrip(bufferSize, holons);
}

void StripHolarchy::update() {
	// Lowest hierarchy it can serve is 0, which will be when it's 1
	if (holarchyDepth >= 1) {
		holarchyDepth = holarchyDepth - 1;
		mateHolons();
		Holarchy::updateStrip(holons);
	}
}

void StripHolarchy::draw() {
	Holarchy::draw(); 
}

void StripHolarchy::clean() {
	holons.clear();
	Holarchy::clearFbo();
}

void StripHolarchy::mateHolons() {
	// Use the leaves of previous layer to mix and create leaves for the current layer.
	int numLeavesInPreviousLayer = std::pow(2, holarchyDepth+1);
	int numLeavesInCurrentLayer = std::pow(2, holarchyDepth);

	auto sourceHolon = holons[0];
	auto oldSize = sourceHolon.getSize();
	auto oldPosition = sourceHolon.getPosition();
	
	glm::vec2 bufferSize = params->getVec2f("Buffer Size");
	
	// Size in the y direction doesn't change. Only x size changes.
	glm::vec2 newSize = glm::vec2(bufferSize.x/(numLeavesInCurrentLayer), oldSize.y);
	
	std::vector<Holon> newHolons;
	for (int i = 0, j=0; i < numLeavesInPreviousLayer, j < numLeavesInCurrentLayer; j++, i+=2) {
		// Choose 2 consecute holons from previous layer.
		auto holonA = holons[i];
		auto holonB = holons[i+1];
		glm::vec2 newPos = glm::vec2(newSize.x*j, oldPosition.y + oldSize.y);
		auto newHolon = crossover(holonA, holonB, newPos, newSize, j);
		
		newHolons.push_back(newHolon);
	}
	
	// Set old holons to new holons.
	holons.clear(); // Free up old memory.
	holons=newHolons;
}


Holon StripHolarchy::crossover(Holon holonA, Holon holonB,
												glm::vec2 newPos, glm::vec2 newSize, int j) {
	float newGenes[3];
	auto holonAGenes = holonA.getGenes();
	auto holonBGenes = holonB.getGenes();
	
	// Extract GUI parameters.
	auto alternateMixValue = params->getBool("Alternate Mix Value");
	auto mixValue = params->getFloat("Mix Value");
	
	// When there is a monochrome image, we create this speial mixValue exchange based on each leave's position
	// Else, it'll give a flat color. This maxValue alternates and we get a good pattern.
	float mixVal = (alternateMixValue) ? ((j%2==0) ? mixValue.get() : 1-mixValue.get()) : mixValue.get();
	
	// A very linear mix of two genes into each other.
	for (int i = 0; i < 3; i++) {
		newGenes[i] = (holonAGenes[i]*mixVal + holonBGenes[i]*(1-mixVal));
	}
	
	// Create new holon and set parents.
	// These newGenes are used by children formed by using this Holon
	// in the holarchy.
	Holon newHolon(newPos, newSize, params->getBool("Monochrome"));
	auto geneA = glm::vec3(holonAGenes[0], holonAGenes[1], holonAGenes[2]);
	auto geneB = glm::vec3(holonBGenes[0], holonBGenes[1], holonBGenes[2]);
	newHolon.parents.push_back(geneA);
	newHolon.parents.push_back(geneB);
	newHolon.updateGenes(glm::vec3(newGenes[0], newGenes[1], newGenes[2]));
	newHolon.mixValue = mixVal;
	
	return newHolon;
}


