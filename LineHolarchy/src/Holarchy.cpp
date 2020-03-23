#include "Holarchy.h"

Holarchy::Holarchy() {
	shader.load("", "mix.frag");
}

void Holarchy::init(glm::vec2 layerSize, std::vector<Holon> &holons, bool monochrome) {
	if (holons.size() > 0) {
		fbo.allocate(layerSize.x, layerSize.y, GL_RGBA);
		fbo.begin();
			ofBackground(ofColor::white);
			for (int i = 0; i < holons.size(); i++) {
				holons[i].draw(monochrome);
			}
		fbo.end();
	}
}

void Holarchy::draw() {
	// Always fit everything on the screen.
	fbo.draw(0, 0, ofGetWidth(), ofGetHeight());
}

void Holarchy::update(std::vector<Holon> &holons, bool gradient, float mixValue, bool monochrome) {
	if (holons.size() > 0) {
		fbo.begin();
			for (int i = 0; i < holons.size(); i++) {
				auto parent1 = holons[i].parents[0];
				auto parent2 = holons[i].parents[1];
				auto holonSize = holons[i].getSize();
				
				// Every Holon is drawn as an fbo so we can do gradients. If gradient is
				// true, then the colors are mixed to a gradient. Else, regular mix happens.
				// Color created by mixing the colors in crossover is used for this Holon's child
				// to be used. 
				ofFbo f;
				f.allocate(holonSize.x, holonSize.y, GL_RGBA);
				f.begin();
					shader.begin();
						shader.setUniform2f("u_resolutionFbo", holonSize.x, holonSize.y);
						shader.setUniform3f("u_colorA", parent1);
						shader.setUniform3f("u_colorB", parent2);
						shader.setUniform1f("u_gradient", gradient);
						shader.setUniform1f("u_mixValue", holons[i].mixValue);
						shader.setUniform1f("u_monochrome", monochrome);
						ofDrawRectangle(0, 0, holonSize.x, holonSize.y);
					shader.end();
				f.end(); 
				
				ofPushMatrix();
					ofTranslate(holons[i].getPosition());
					f.draw(0, 0, holonSize.x, holonSize.y);
				ofPopMatrix();
			}
		fbo.end();
	}
}

void Holarchy::saveToImage(int numIdx) {
	ofPixels pix;
	fbo.readToPixels(pix);
	ofSaveImage(pix, "image-" + ofToString(numIdx) + ".jpg", OF_IMAGE_QUALITY_BEST);
}

void Holarchy::clearFbo() {
	fbo.clear();
}
