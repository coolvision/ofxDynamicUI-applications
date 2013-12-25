/*
 * AppPatch.cpp
 *
 *  Created on: Nov 9, 2013
 *      Author: sk
 */

#include "Patch.h"
#include "Composition.h"
#include "ofMain.h"

#ifdef TARGET_OSX
    // ofSystemUtils.cpp is configured to build as
    // objective-c++ so as able to use Cocoa dialog panels
    // This is done with this compiler flag
    //      -x objective-c++
    // http://www.yakyak.org/viewtopic.php?p=1475838&sid=1e9dcb5c9fd652a6695ac00c5e957822#p1475838
    #include <Cocoa/Cocoa.h>

    void restoreAppWindowFocus() {

        NSWindow * appWindow = (NSWindow *)ofGetCocoaWindow();
        if(appWindow) {
            [appWindow makeKeyAndOrderFront:0];
        }
    }
#else
    void restoreAppWindowFocus() {
    }
#endif

AppPatch::AppPatch(bool opened, string name_in, string path, int x, int y,
        int width, int height) {

    type = PATCH;
    ports_queried = false;

    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    this->opened = opened;

    if (opened) {

        automatic_address = true;

        this->name = name_in + ofToString(Composition::name_i);
        this->path = path;

        client = zmq_socket(Composition::context, ZMQ_REQ);
        address.str = "ipc:///tmp/" + name;
        zmq_connect(client, address.str.c_str());

        status = NOT_RUNNING;

    } else {

        automatic_address = false;

        this->name = name_in;
        this->path = path;
        address.str = path;
    }

    ofAddListener(click_event, this, &AppPatch::click);
}

void AppPatch::click(string &s) {

    ofPoint mouse(ofGetMouseX(), ofGetMouseY());

    if (automatic_address) {
        return;
    }

    if (title_box.inside(mouse)) {

        name = ofSystemTextBoxDialog("title", name);

        restoreAppWindowFocus();
    }

    if (address_box.inside(mouse)) {

        string new_address = ofSystemTextBoxDialog("address", address.str);

        restoreAppWindowFocus();

        if (new_address != address.str) {

            address.str = new_address;

            if (client != NULL) {
                zmq_close(client);
            }

            client = zmq_socket(Composition::context, ZMQ_REQ);
            zmq_connect(client, address.str.c_str());
            status = NOT_RUNNING;
            opened = true;
        }

    }
}

bool AppPatch::hitTest(int tx, int ty) {

    return ((tx > x) && (tx < x + width) && (ty > y - title_box.height + 5) &&
            (ty < y + height + address_box.height - 5));

    return true;
}

AppPatch::~AppPatch() {
}
