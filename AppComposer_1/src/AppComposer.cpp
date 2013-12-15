#include "AppComposer.h"

#include "ofAppGLFWWindow.h"

void AppComposer::setup() {

    ofSetFrameRate(60);
    ofSetVerticalSync(false);

    cmp.setup();
}

void AppComposer::draw() {

    cmp.draw();
}

void AppComposer::update() {

    cmp.update();
}

void AppComposer::exit() {

    cmp.exit();
}

void AppComposer::keyPressed(int key) {
}
void AppComposer::keyReleased(int key) {
}
void AppComposer::mouseMoved(int x, int y) {
}
void AppComposer::mouseDragged(int x, int y, int button) {
}
void AppComposer::mousePressed(int x, int y, int button) {
}
void AppComposer::mouseReleased(int x, int y, int button) {
}
void AppComposer::windowResized(int w, int h) {
}
void AppComposer::gotMessage(ofMessage msg) {
}
void AppComposer::dragEvent(ofDragInfo dragInfo) {
}
