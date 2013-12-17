#include "ReadCSV.h"

#include <zmq.h>
#include <msgpack.hpp>

//--------------------------------------------------------------
void ReadCSV::setup() {

    ofSetFrameRate(60);

    // ConnectedApp
    initConnection("ReadCSV.app");


    ports_in.push_back(new Port("test_in1", Port::INPUT));
    ports_in.push_back(new Port("test_in2", Port::INPUT));
    ports_in.push_back(new Port("test_in3", Port::INPUT));
    ports_in.push_back(new Port("test_in4", Port::INPUT));

    ports_out.push_back(new Port("test_out1", Port::OUTPUT));
    ports_out.push_back(new Port("test2", Port::OUTPUT));
    ports_out.push_back(new Port("out3", Port::OUTPUT));
}

//--------------------------------------------------------------
void ReadCSV::update() {

    // ConnectedApp
    processMessages();
}

//--------------------------------------------------------------
void ReadCSV::draw() {

    ofBackground(50, 50, 50);

    ofDrawBitmapString("FPS: " + ofToString(ofGetFrameRate()),
            ofGetWindowWidth() - 100, 20);

    // also draw url string
    ofDrawBitmapString("address: " + address.str, 20, 20);

    for (int i = 0; i < messages.size(); i++) {
        ofDrawBitmapString(messages[i], 20, 50 + 20 * i);
    }
}

//--------------------------------------------------------------
void ReadCSV::keyPressed(int key) {

}

//--------------------------------------------------------------
void ReadCSV::keyReleased(int key) {

}

//--------------------------------------------------------------
void ReadCSV::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ReadCSV::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ReadCSV::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ReadCSV::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ReadCSV::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ReadCSV::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ReadCSV::dragEvent(ofDragInfo dragInfo) {

}
