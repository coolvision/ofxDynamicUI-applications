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
    zmq_connect(client, address.str.c_str());

    status = NOT_RUNNING;

    ports_queried = false;

    type = PATCH;
}

AppPatch::~AppPatch() {
}
