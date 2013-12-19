#include "testApp.h"

#include <zmq.h>
#include <msgpack.hpp>

//--------------------------------------------------------------
void testApp::setup() {

    ofSetFrameRate(60);

    // ConnectedApp
    initConnection("testApp.app");


    ports_in.push_back(new Port("test_in1", Port::INPUT));
    ports_in.push_back(new Port("test_in2", Port::INPUT));
    ports_in.push_back(new Port("test_in3", Port::INPUT));
    ports_in.push_back(new Port("test_in4", Port::INPUT));

    ports_out.push_back(new Port("test_out1", Port::OUTPUT));
    ports_out.push_back(new Port("test2", Port::OUTPUT));
    ports_out.push_back(new Port("out3", Port::OUTPUT));
}

//--------------------------------------------------------------
void testApp::update() {

    // ConnectedApp
    processMessages();
}

//--------------------------------------------------------------
void testApp::draw() {

    ofBackground(255.0f);

    ofSetColor(0.0f);

    ofDrawBitmapString("FPS: " + ofToString(ofGetFrameRate()),
            ofGetWindowWidth() - 100, 20);

    // also draw url string
    ofDrawBitmapString("address: " + address.str, 20, 20);

    for (int i = 0; i < messages.size(); i++) {
        ofDrawBitmapString(messages[i], 20, 50 + 20 * i);
    }
}

//--------------------------------------------------------------
void testApp::keyPressed(int key) {

}

//--------------------------------------------------------------
void testApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo) {

}
