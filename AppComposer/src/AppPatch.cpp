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

    ofPushStyle();

    ofSetColor(text_color);
    font.draw(name + " - " + address.str, 16, (int) (x + 5), (int) (y - 3));

    float max_w_in = -1.0f;
    float max_w_out = -1.0f;
    float max_y = -1.0f;

    ofSetColor(ofColor::black);
    ofNoFill();
    for (int i = 0; i < ports_in.size(); i++) {

        ofRectangle r = font.getBBox(ports_in[i]->name, 16, (int) (x + 10),
                (int) (y + 20 + 20 * i));

        ports_in[i]->r = r;

        if (r.width > max_w_in) {
            max_w_in = r.width;
        }
        if (r.getMaxY() > max_y) {
            max_y = r.getMaxY();
        }
    }

    for (int i = 0; i < ports_out.size(); i++) {

        ofRectangle r = font.getBBox(ports_out[i]->name, 16, (int) (x + 10 + width - 100),
                (int) (y + 20 + 20 * i));

        ports_out[i]->r = r;
        
        if (r.width > max_w_out) {
            max_w_out = r.width;
        }
        if (r.getMaxY() > max_y) {
            max_y = r.getMaxY();
        }
    }

    if (max_y > 0.0f) {
        height = max_y - y + 10;
    }

    ofFill();

    if (status == UNRESPONSIVE) {
        ofSetColor(ofColor::gray);
    } else {
        ofSetColor(color);
    }
    ofRect(x, y, width, height);


    for (int i = 0; i < ports_in.size(); i++) {
        ofSetColor(ofColor::black);
        ofNoFill();
        Patch::font.draw(ports_in[i]->name, 16, (int) (x + 10),
                         (int) (y + 20 + 20 * i));
        //ofRect(ports_in[i]->r);

        ofRectangle socket = ports_in[i]->r;
        socket.x = x;
        socket.width = 5;
        ofFill();
        ofSetColor(ofColor::orange);
        ofRect(socket);
    }

    for (int i = 0; i < ports_out.size(); i++) {
        ofSetColor(ofColor::black);
        ofNoFill();
        font.draw(ports_out[i]->name, 16, (int) (x + width - ports_out[i]->r.width - 10),
                  (int) (y + 20 + 20 * i));
        //ofRect(ports_out[i]->r);

        ofRectangle socket = ports_out[i]->r;
        socket.x = x + width - 5;
        socket.width = 5;
        ofFill();
        ofSetColor(ofColor::orange);
        ofRect(socket);
    }

    ofPopStyle();
}

