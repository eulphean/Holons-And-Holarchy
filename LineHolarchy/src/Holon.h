#pragma once

#include "ofMain.h"

class Holon {
	public:
		Holon(glm::vec2 position, glm::vec2 s, bool monochrome);
		void draw();
	
		glm::vec2 &getSize();
		glm::vec2 &getPosition();
		float *getGenes(); 
		void updateGenes(glm::vec3 newGenes);
	
		std::vector<glm::vec3> parents;
		float mixValue; 
	private:
		void assignGenes();
		ofColor createColorFromGenes(); 
		glm::vec2 pos;
		glm::vec2 size;
		float genes[3]; // Hue, Saturation, Brightness.
		bool monochrome; 
};
