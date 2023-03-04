#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(30);
	ofSetWindowTitle("openFrameworks");

	ofBackground(0);
	ofEnableDepthTest();

	auto ico_sphere = ofIcoSpherePrimitive(300, 5);
	this->triangle_list.insert(this->triangle_list.end(), ico_sphere.getMesh().getUniqueFaces().begin(), ico_sphere.getMesh().getUniqueFaces().end());

	this->frame.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);

	this->mesh.clear();
	this->frame.clear();

	for (int i = 0; i < 2; i++) {

		auto radius = (ofGetFrameNum() * 3 + i * 180) % 320;
		auto threshold = radius < 300 ? ofMap(radius, 0, 300, 0.5, 0) : 0;

		auto noise_seed = ofRandom(1000);
		for (int k = 0; k < this->triangle_list.size(); k++) {

			glm::vec3 avg = (this->triangle_list[k].getVertex(0) + this->triangle_list[k].getVertex(1) + this->triangle_list[k].getVertex(2)) / 3;
			auto noise_value = ofNoise(noise_seed, avg.y * 0.005 + ofGetFrameNum() * 0.05, avg.x * 0.005, avg.z * 0.005);

			if (noise_value < 0.5 - threshold || noise_value > 0.5 + threshold) { continue; }

			vector<glm::vec3> vertices;

			vertices.push_back(glm::normalize(this->triangle_list[k].getVertex(0)) * radius);
			vertices.push_back(glm::normalize(this->triangle_list[k].getVertex(1)) * radius);
			vertices.push_back(glm::normalize(this->triangle_list[k].getVertex(2)) * radius);

			this->mesh.addVertices(vertices);
			this->frame.addVertices(vertices);

			for (int k = 0; k < vertices.size(); k++) {

				auto alpha = radius < 250 ? 255 : ofMap(radius, 250, 320, 255, 0);

				this->mesh.addColor(ofColor(0, alpha));
				this->frame.addColor(ofColor(0, 255, 255, alpha));
			}

			this->mesh.addTriangle(this->mesh.getNumVertices() - 1, this->mesh.getNumVertices() - 2, this->mesh.getNumVertices() - 3);

			this->frame.addIndex(this->frame.getNumVertices() - 1); this->frame.addIndex(this->frame.getNumVertices() - 2);
			this->frame.addIndex(this->frame.getNumVertices() - 2); this->frame.addIndex(this->frame.getNumVertices() - 3);
			this->frame.addIndex(this->frame.getNumVertices() - 3); this->frame.addIndex(this->frame.getNumVertices() - 1);
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateY(ofGetFrameNum());

	this->mesh.draw();
	this->frame.drawWireframe();

	this->cam.end();

	/*
	ostringstream os;
	os << setw(4) << setfill('0') << ofGetFrameNum();
	ofImage image;
	image.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
	image.saveImage("image/img_" + os.str() + ".jpg");
	if (ofGetFrameNum() >= 30 * 20 + 1) {

		std::exit(1);
	}
	*/

	// > ffmpeg -i img_%04d.jpg -r 30 out.mp4
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}