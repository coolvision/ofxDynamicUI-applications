/*
 * AppPatch.cpp
 *
 *  Created on: Nov 9, 2013
 *      Author: sk
 */

#include "Patch.h"
#include "Composition.h"

AppPatch::AppPatch(string name_in, string path, int x, int y, int width,
        int height) {

    this->name = name_in + ofToString(Composition::name_i);
    this->path = path;
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;

    // add a socket for communication with an app
    // use executable name as an address
    client = zmq_socket(Composition::context, ZMQ_REQ);
    address.str = "ipc:///tmp/" + name;
    cout << "open ping socket" << endl;
    zmq_connect(client, address.str.c_str());

    status = NOT_RUNNING;

    ports_queried = false;
}

AppPatch::~AppPatch() {
}

bool AppPatch::open(string path) {
}

bool AppPatch::sendToApp() {
}

bool AppPatch::receiveFromApp() {
}

bool AppPatch::requestFromApp() {
}

void AppPatch::draw() {

    ofFill();
    ofSetColor(color);
    ofRect(x, y, width, height);

    ofNoFill();
    if (status == UNRESPONSIVE) {
        ofSetColor(ofColor::red);
        ofRect(x, y, width, height);
    }


    ofSetColor(text_color);
    font.draw(name + " - " + address.str, 16, (int) (x + 5), (int) (y - 3));


    for (int i = 0; i < ports.size(); i++) {

        ofRectangle r = font.getBBox(ports[i]->name, 16, (int) (x + 10), (int) (y + 20 + 20 * i));

        ofSetColor(ofColor::black);
        ofNoFill();
        ofRect(r);

        font.draw(ports[i]->name, 16, (int) (x + 10), (int) (y + 20 + 20 * i));
    }



}

