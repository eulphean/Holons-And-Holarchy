#pragma once

#include "ofMain.h"
#include "Holon.h"
#include "ofxGui.h"

class Holarchy {
	public:
		virtual void initFrame(glm::vec2 totalBufferSize, glm::vec2 gridSize, std::vector<std::vector<Holon>> &holons);
		virtual void initStrip(glm::vec2 totalBufferSize, std::vector<Holon> &holons);
		virtual void updateStrip(std::vector<Holon> &holons);
		virtual void updateFrame(std::vector<std::vector<Holon>> & holons, glm::vec2 gridSize);
		virtual void draw();
		virtual void clearFbo();
		void setParams(ofParameterGroup *params);
		void drawHolonFbo(Holon curHolon);
		void saveToImage();
	
		// Inherited Members should have full access
		// to these variables.
		int holarchyDepth;
		ofFbo fbo;
		ofShader shader;
		ofParameterGroup *params;
};
