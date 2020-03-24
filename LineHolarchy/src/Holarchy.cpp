#include "Holarchy.h"

void Holarchy::initFrame(glm::vec2 totalBufferSize, glm::vec2 gridSize, std::vector<std::vector<Holon>> &holons) {
	shader.load("", "mix.frag");
	
	// Create all the FBO needed to save the content of this holarchy.
	fbo.allocate(totalBufferSize.x, totalBufferSize.y, GL_RGBA);
	
	fbo.begin();
		ofBackground(255);
		// Draw the holon.
		for (int row = 0; row < gridSize.y; row++) {
			for (int col = 0; col < gridSize.x; col++) {
				Holon h = holons[row][col];
				h.draw();
			}
		}
	fbo.end();
	
	frameFbos.push_back(fbo);
}

void Holarchy::initStrip(glm::vec2 totalBufferSize, std::vector<Holon> &holons) {
	// Load shader. 
	shader.load("", "mix.frag");
	
	// Create all the FBO needed to save the content of this holarchy. 
	fbo.allocate(totalBufferSize.x, totalBufferSize.y, GL_RGBA);
	
	// Holons should just draw themselves on the screen.
	if (holons.size() > 0) {
		fbo.begin();
			for (int i = 0; i < holons.size(); i++) {
				holons[i].draw();
			}
		fbo.end();
	}
}

void Holarchy::draw() {
	// Always fit everything on the screen.
	fbo.draw(0, 0, ofGetWidth(), ofGetHeight());
}

void Holarchy::drawFrame(int fboIdx) {
	if (frameFbos.size() > 0) {
		frameFbos[fboIdx].draw(0, 0, ofGetWidth(), ofGetHeight());
	}
}

void Holarchy::setParams(ofParameterGroup *generalParams) {
	params = generalParams;
}

void Holarchy::updateFrame(std::vector<std::vector<Holon>> &holons, glm::vec2 gridSize) {
	// Reset the fbo.
	glm::vec2 newFboSize = params->getVec2f("Buffer Size");
	ofFbo f;
	f.allocate(newFboSize.x, newFboSize.y, GL_RGBA);
	f.begin();
		for (int row = 0; row < gridSize.y; row++) {
			for (int col = 0; col < gridSize.x; col++) {
				auto curHolon = holons[row][col];
				drawHolonFbo(curHolon);
			}
		}
	f.end();
	frameFbos.push_back(f);
}

void Holarchy::updateStrip(std::vector<Holon> &holons) {
	if (holons.size() > 0) {
		fbo.begin();
			for (int i = 0; i < holons.size(); i++) {
				drawHolonFbo(holons[i]);
			}
		fbo.end();
	}
}

void Holarchy::saveToImage(bool isFrameHolarchy) {
	ofPixels pix;
	string txt;
	if (isFrameHolarchy) {
		auto f = frameFbos[fboIdx];
		f.readToPixels(pix);
		txt = ofToString(fboIdx) + "-" + ofToString(ofGetUnixTime());
	} else {
		fbo.readToPixels(pix);
		txt = ofToString(ofGetUnixTime());
	}
	
	ofSaveImage(pix, "image-" + txt + ".jpg", OF_IMAGE_QUALITY_BEST);
}

void Holarchy::clearFbo() {
	fbo.clear();
	frameFbos.clear();
}

void Holarchy::drawHolonFbo(Holon curHolon) {
	auto parent1 = curHolon.parents[0];
	auto parent2 = curHolon.parents[1];
	auto holonSize = curHolon.getSize();

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
			shader.setUniform1f("u_gradient", params->getBool("Gradient"));
			shader.setUniform1f("u_mixValue", curHolon.mixValue); // Use mix value from the Holon because this could have changed.
			shader.setUniform1f("u_monochrome", params->getBool("Monochrome"));
			ofDrawRectangle(0, 0, holonSize.x, holonSize.y);
		shader.end();
	f.end();

	ofPushMatrix();
		ofTranslate(curHolon.getPosition());
		f.draw(0, 0, holonSize.x, holonSize.y);
	ofPopMatrix();
}

