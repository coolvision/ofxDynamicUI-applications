/*
 * CompositionManager.cpp
 *
 *  Created on: Nov 5, 2013
 *      Author: sk
 */

#include "Composition.h"

#include "ofMain.h"

#include <zmq.h>
#include <msgpack.hpp>

void *Composition::context = NULL;
int Composition::name_i = 0;

Composition::Composition() {

    context = zmq_ctx_new();
}

Composition::~Composition() {

}

void Composition::exit() {

    cout << "Composition::exit" << endl;

    zmq_close(requester);
    //zmq_ctx_destroy(context);
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

    // make a fixed app
    string app_name = "ReadCSV.app";
    string app_path =
            "/Users/sk/of_v0.8.0_osx_release/apps/myApps/ReadCSV/bin/ReadCSV.app";

    patches_toolbox.push_back(
            new ButtonPatch(app_name, margin_left,
                    start_y + inc_y * patches_toolbox.size(), button_w,
                    button_h));
    patches_toolbox.back()->setDraggable(false);

    app_paths.insert(pair<string, string>(app_name, app_path));

    ButtonPatch *b = patches_toolbox.back();
    ofAddListener(b->click_event, this, &Composition::openApp);

    // common socket, mostly just for testing
    requester = zmq_socket(context, ZMQ_REQ);
    zmq_connect(requester, "ipc:///tmp/0");

    name_server = zmq_socket(context, ZMQ_REP);
    zmq_bind(name_server, "ipc:///tmp/name");
}

void Composition::update() {

    // mostly just dbeugging and testing
    zmq_msg_t msg;
    zmq_msg_init(&msg);
    if (zmq_msg_recv(&msg, requester, ZMQ_DONTWAIT) != -1) {
        Message m(&msg);
    }
    zmq_msg_close(&msg);

    // reply to name requests
    zmq_msg_t name_msg;
    zmq_msg_init(&name_msg);
    if (zmq_msg_recv(&name_msg, name_server, ZMQ_DONTWAIT) != -1) {
        cout << "reply to name request " <<  name_i << endl;
        Message m(&name_msg);
        Message out_m(ofToString(name_i));
        out_m.send(name_server);
        name_i++;
    }
    zmq_msg_close(&name_msg);

    // send pind messages to the app patches btw
    for (int i = 0; i < patches.size(); i++) {
        patches[i]->update();
    }
}

void Composition::addButton(string &s) {

    static int msg_n = 0;

    msg_n++;

    // make a message object
    Message m("test");
    m.addValue("Hello");
    m.addValue("message");
    m.addIntValue(msg_n);
    m.send(requester);

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

