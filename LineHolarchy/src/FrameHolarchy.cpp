#include "FrameHolarchy.h"

FrameHolarchy::FrameHolarchy() {
	holarchyDepth = 0;
	gridSize = glm::vec2(0, 0);
}

void FrameHolarchy::init() {
	holarchyDepth = params->getInt("Holarchy Depth");
	glm::vec2 initialBufferSize = params->getVec2f("Buffer Size");
	// This ia Frame by Frame holon representation (it's long)
	auto numFrames = holarchyDepth*2 + 1;
	glm::vec2 totalBufferSize = glm::vec2(initialBufferSize.x, initialBufferSize.y*numFrames);
	
	gridSize = glm::vec2(std::pow(2, holarchyDepth), std::pow(2, holarchyDepth));
	glm::vec2 size = glm::vec2(initialBufferSize.x/gridSize.x, initialBufferSize.y/gridSize.y);
	int j = 0;
	for (int row = 0; row < gridSize.y; row++) {
		std::vector<Holon> cols;
		for (int col = 0; col < gridSize.x; col++) {
			glm::vec2 pos = glm::vec2(size.x*col, size.y*row);
			Holon h (pos, size, params->getBool("Monochrome"));
			if (params->getBool("Ordered Start")) {
				if (j%2==0) {
					h.updateGenes(glm::vec3(0.0, 0.0, 0.0));
				} else {
					h.updateGenes(glm::vec3(1.0, 1.0, 1.0));
				}
			}
			cols.push_back(h);
			
			j++; // Keep track for ordered start.
		}
		
		j++; // Keep track for ordered start.
		holons.push_back(cols);
	}
	
	Holarchy::initFrame(totalBufferSize, gridSize, holons);
}

void FrameHolarchy::update() {
	if (gridSize.x == 0 && gridSize.y == 0) {
		// Nothing to do.
	} else if (gridSize.x == 1 && gridSize.y == 1) {
		cout << "Successfully finished creating Holarchy" << endl;
	} else {
		// Mate Holons across width.
		mateHolons(true);
		gridSize.x = gridSize.x/2;
		Holarchy::updateFrame(holons, gridSize);
		
		// Mate Holons across height.
		mateHolons(false);
		gridSize.y = gridSize.y/2;
		Holarchy::updateFrame(holons, gridSize);
	}
}

void FrameHolarchy::draw() {
	//Holarchy::draw();
}

void FrameHolarchy::mateHolons(bool acrossWidth) {
	// Combination of Holons will follow the same two parent combination that
	// we follow in strip holarchy. 
	int offsetW = acrossWidth ? 2 : 1;
	int offsetH = acrossWidth ? 1 : 2;
	
	std::vector<std::vector<Holon>> newHolons;
	int j = 0;
	for (int row = 0; row < gridSize.y; row+=offsetH) {
		std::vector<Holon> newRow;
		for (int col = 0; col < gridSize.x; col+=offsetW) {
			// Use sourceHolon's position and size to calculate (position, size) for our next Holon.
			auto sourceHolon = holons[row][col];
			auto oldSize = sourceHolon.getSize();
			auto oldPos = sourceHolon.getPosition();
			
			// Which way do we combine the holons?
			auto newSize = acrossWidth ? glm::vec2(oldSize.x*2, oldSize.y):glm::vec2(oldSize.x, oldSize.y*2);
			auto newPos = glm::vec2(oldPos.x, oldPos.y + params->getVec2f("Buffer Size")->y); // yPosition of each element + ySize of the buffer.

			auto holonA = holons[row][col];
			auto holonB = acrossWidth ? holons[row][col+1]:holons[row+1][col];
			auto newHolon = crossover(holonA, holonB, newPos, newSize, j);
			newRow.push_back(newHolon);
			j++;
		}
		newHolons.push_back(newRow);
	}
	
	// Not sure if this will work, but worth trying.
	holons = newHolons;
}

Holon FrameHolarchy::crossover(Holon holonA, Holon holonB,
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

void FrameHolarchy::clean() {
	Holarchy::clearFbo();
	holons.clear();
}
