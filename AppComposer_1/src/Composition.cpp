/*
 * CompositionManager.cpp
 *
 *  Created on: Nov 5, 2013
 *      Author: sk
 */

#include "Composition.h"

#include "ofMain.h"

#include "zmq.h"
#include "msgpack.hpp"

void *context;
void *requester;

Composition::Composition() {

}

Composition::~Composition() {

}

void Composition::exit() {

    cout << "Composition::exit" << endl;

    zmq_close(requester);
    zmq_ctx_destroy(context);
}

void Composition::setup() {

    cout << "Composition::setup" << endl;

    // title
    name = "Test";

    // ui layout settings
    button_h = 20;
    button_w = 175;
    margin_top = 20;
    margin_left = 20;
    margin_bottom = 10;

    // toolbox buttons
    int start_y = margin_top;
    int inc_y = button_h + margin_bottom;

    ButtonPatch *add_app = new ButtonPatch("Add application", margin_left,
            start_y, button_w, button_h);

    ButtonPatch *add_button = new ButtonPatch("Add button", margin_left,
            start_y + inc_y, button_w, button_h);

    patches_toolbox.push_back(add_app);
    patches_toolbox.push_back(add_button);

    patches_toolbox.push_back(
            new ButtonPatch("App1", margin_left, start_y + inc_y * 2, button_w,
                    button_h));
    patches_toolbox.push_back(
            new ButtonPatch("App2", margin_left, start_y + inc_y * 3, button_w,
                    button_h));
    patches_toolbox.push_back(
            new ButtonPatch("App3", margin_left, start_y + inc_y * 4, button_w,
                    button_h));
    patches_toolbox.push_back(
            new ButtonPatch("App4", margin_left, start_y + inc_y * 5, button_w,
                    button_h));

    for (int i = 0; i < patches_toolbox.size(); i++) {
        patches_toolbox[i]->setDraggable(false);
    }

    ofAddListener(add_app->click_event, this, &Composition::addApp);
    ofAddListener(add_button->click_event, this, &Composition::addButton);

    ofDisableAntiAliasing();
    ofDisableSmoothing();

    Patch::font.setup("UbuntuMono-R.ttf");

    context = zmq_ctx_new();
    requester = zmq_socket(context, ZMQ_REQ);
    zmq_connect(requester, "tcp://localhost:5555");
}

void Composition::update() {

    // client update
    zmq_msg_t msg;
    zmq_msg_init(&msg);

    if (zmq_msg_recv(&msg, requester, ZMQ_DONTWAIT) != -1) {

        // received a request
        cout << "received a message!" << endl;
        char *data = (char *) zmq_msg_data(&msg);
        for (int i = 0; i < zmq_msg_size(&msg); i++) {
            cout << data[i];
        }
        cout << endl;
    }


    for (int i = 0; i < patches.size(); i++) {
        patches[i]->update();
    }
}

void Composition::addButton(string &s) {

    static int msg_n = 0;

    msg_n++;

    // send a message, with a serialized protocol
    // send message number


    // send a message
    zmq_msg_t msg;

    // send a message pack
    vector<string> target;
    target.push_back("Hello" + ofToString(msg_n) + '\0');

    msgpack::sbuffer sbuf;  // simple buffer
    msgpack::pack(&sbuf, target);

    printf("Sending %s\n", sbuf.data());

    zmq_msg_init_size(&msg, sbuf.size());
    memcpy(zmq_msg_data(&msg), sbuf.data(), sbuf.size());

    zmq_msg_send(&msg, requester, ZMQ_NOBLOCK);
    zmq_msg_close(&msg);

    // add a draggable button to the canvas
    patches.push_back(
                      new ButtonPatch("button", ofGetWindowWidth() / 2 + ofRandom(0, 100),
                                      ofGetWindowHeight() / 2 + ofRandom(0, 100), 175, 20));
}

void Composition::addApp(string &s) {

    // toolbox buttons
    int start_y = margin_top;
    int inc_y = button_h + margin_bottom;

    // get the application path from a dialog
    // filename will be the button's label
    ofFileDialogResult res = ofSystemLoadDialog("please select application",
            true);

    // add a new application button
    cout << "got " << res.getPath() << endl;

    // this button with trigger adding of the application patch
    patches_toolbox.push_back(
            new ButtonPatch(res.getName(), margin_left,
                    start_y + inc_y * patches_toolbox.size(), button_w,
                    button_h));
    patches_toolbox.back()->setDraggable(false);

    app_paths.insert(pair<string, string>(res.getName(), res.getPath()));

    ButtonPatch *b = patches_toolbox.back();
    ofAddListener(b->click_event, this, &Composition::openApp);
}

void Composition::openApp(string &s) {

    string path = app_paths[s] + "/Contents/MacOS/" + s.substr(0, s.size() - 4);

    string command = path + " &";
    int r = system(command.c_str());

    cout << "command: " << command << endl;
    cout << "r: " << r << endl;

    patches.push_back(
            new AppPatch(s, path, ofGetWindowWidth() / 2 + ofRandom(0, 100),
                    ofGetWindowHeight() / 2 + ofRandom(0, 100), 200, 100));
}

void Composition::draw() {

    // background
    ofBackground(50, 50, 50);

    Patch::font.draw("FPS: " + ofToString(ofGetFrameRate()), 16,
            ofGetWindowWidth() - 100, 20);
    Patch::font.draw("Zoom: 100%", 16, ofGetWindowWidth() - 100, 40);

    // toolbox
    for (int i = 0; i < patches_toolbox.size(); i++) {
        patches_toolbox[i]->draw();
    }

    // content
    for (int i = 0; i < patches.size(); i++) {
        patches[i]->draw();
    }
}

bool Composition::patchExists(int uid) {

    return false;
}

Patch *Composition::getPatch(int uid) {

    return NULL;
}

