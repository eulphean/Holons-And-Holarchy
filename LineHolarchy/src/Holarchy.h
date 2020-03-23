#pragma once

#include "ofMain.h"
#include "Holon.h"

class Holarchy {
	public:
		Holarchy();
	
		void init(glm::vec2 layerSize, std::vector<Holon> &holons, bool monochrome);
		void update(std::vector<Holon> &holons, bool gradient, float mixValue, bool monochrome);
		void draw();
		void saveToImage(int numIdx);
		void clearFbo();
	
	private:
		ofFbo fbo;
		ofFbo saveFbo;
		ofShader shader;
};
