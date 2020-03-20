#include "Holon.h"

Holon::Holon(glm::vec2 position, glm::vec2 s) {
	pos.x = position.x; pos.y = position.y;
	size.x = s.x; size.y = s.y;
	hasReproduced = false; 
	assignGenes();
}

void Holon::draw() {
	ofPushMatrix();
		ofTranslate(pos);
		ofPushStyle();
			ofColor c = createColorFromGenes();
			ofSetColor(c);
			ofDrawRectangle(0, 0, size.x, size.y);
		ofPopStyle();
	ofPopMatrix();
}

glm::vec2 &Holon::getSize() {
	return size;
}

glm::vec2 &Holon::getPosition() {
	return pos;
}

void Holon::assignGenes() {
	for (int i = 0; i < 3; i++) {
		genes[i] = ofRandom(1);
	}
}

ofColor Holon::createColorFromGenes() {
	int hue = ofMap(genes[0], 0, 1, 0, 255, true);
	int saturation = ofMap(genes[1], 0, 1, 0, 255, true);
	int brightness = ofMap(genes[2], 0, 1, 0, 255, true);
	return ofColor::fromHsb(hue, saturation, brightness);
}
