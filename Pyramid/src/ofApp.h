#pragma once

#include "ofMain.h"
#include "Holon.h"

class Pyramid {
	public:
		Pyramid() {
		
		}
	
		void init(glm::vec2 layerSize, std::vector<Holon> &holons) {
			if (holons.size() > 0) {
				fbo.allocate(layerSize.x, layerSize.y, GL_RGBA);
				saveFbo.allocate(layerSize.x, layerSize.y, GL_RGBA);
				fbo.begin();
					ofBackground(ofColor::red);
					for (int i = 0; i < holons.size(); i++) {
						holons[i].draw();
					}
				fbo.end();
			}
		}
	
		void update(std::vector<Holon> &holons) {
			if (holons.size() > 0) {
				fbo.begin();
					for (int i = 0; i < holons.size(); i++) {
						holons[i].draw();
					}
				fbo.end();
			}
		}
	
		void draw() {
			fbo.draw(0, 0, ofGetWidth(), ofGetHeight());
		}
	
		void saveToImage(int numIdx, ofTexture &maskTex) {
			ofPixels pix;
			auto tex = fbo.getTexture();
			tex.setAlphaMask(maskTex);
			saveFbo.begin();
				tex.draw(0, 0);
			saveFbo.end();
			saveFbo.readToPixels(pix); 
			ofSaveImage(pix, "image-" + ofToString(numIdx) + ".jpg", OF_IMAGE_QUALITY_BEST);
		}
	
		void setMask(ofTexture &maskTex) {
			fbo.getTexture().setAlphaMask(maskTex);
		}
	
	private:
		ofFbo fbo;
		ofFbo saveFbo;
}; 

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void initHolons();
		void combineHolons();
	
		void createDepths();
		Holon getRandomHolon();
		Holon crossover(Holon a, Holon b, glm::vec2 position, glm::vec2 size);
		void mutate(Holon &a);
		void createTriangle(); 

		void keyPressed(int key);
	
	private:
		std::vector<Holon> holons;
	
		float mutationProbability;
		float crossoverProbability;
	
		// Depth of the recursion tree. 
		int holarchyDepth;
		glm::vec2 layerSize;
		Pyramid pyramid;
	
		ofFbo triangleFbo;
};

