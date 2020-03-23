#pragma once

#include "ofMain.h"

class Holon {
	public:
		Holon(glm::vec2 position, glm::vec2 s);
		void draw();

		glm::vec2 &getSize();
		glm::vec2 &getPosition();
	
		float genes[3]; // Hue, Saturation, Brightness. 
		bool hasReproduced;
	
	private:
		void assignGenes();
		ofColor createColorFromGenes(); 
		glm::vec2 pos;
		glm::vec2 size;
};
