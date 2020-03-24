#pragma once

#include "ofMain.h"
#include "Holon.h"
#include "Holarchy.h"

class FrameHolarchy : public Holarchy {
	public:
		FrameHolarchy();
		void init();
		void update();
		void draw();
		void mateHolons(bool acrossWidth);
		void setNextIdx(); 
		Holon crossover(Holon holonA, Holon holonB, glm::vec2 newPos, glm::vec2 newSize, int j);
		void clean(); 
	
	private:
		glm::vec2 gridSize;
		std::vector<vector<Holon>> holons;
};
