#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "Holarchy.h"
#include "Holon.h"

class StripHolarchy : public Holarchy {
	public:
		StripHolarchy();
		void init();
		void update();
		void draw(); 
		void clean();
		void mateHolons();
		Holon crossover(Holon holonA, Holon holonB, glm::vec2 newPos, glm::vec2 newSize, int j);
		
	private:
		// Here because different types of holarchy require different types of holon data structure.
		std::vector<Holon> holons;
};
