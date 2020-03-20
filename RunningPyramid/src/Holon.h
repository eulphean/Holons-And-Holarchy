#pragma once

#include "ofMain.h"

class Holon {
	public:
		Holon(glm::vec2 position, int height);
		void draw();
		glm::vec2 &getSize();
		glm::vec2 &getPosition();
	
		float genes[4]; // Hue, Saturation, Brightness.
		bool hasReproduced;
	
		int numGenes = 4;
		
	private:
		void assignGenes();
		ofColor createColorFromGenes();
		float getWidthFromGenes();
		glm::vec2 pos;
		glm::vec2 size;
};
