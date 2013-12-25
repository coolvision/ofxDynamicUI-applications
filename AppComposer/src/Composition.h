/*
 * Composition.h
 *
 *  Created on: Nov 5, 2013
 *      Author: sk
 */

#pragma once

#include "ofMain.h"
#include "ofxMSAInteractiveObject.h"
#include "Patch.h"

#include "ofxFontStash.h"

using namespace std;

class Composition {
public:

    Composition();
    ~Composition();

    static void *context;
    //void *requester; // mostly for testing

    void *name_server; // for getting
    static int name_i;

    string name; // composition name

    // store and display available patches list
    vector<ButtonPatch *> patches_toolbox;

    // dynamically created patches
    static int patch_id;
    static vector<Patch *> patches;

    map<string, string> app_paths;

    int button_h;
    int button_w;
    int margin_top;
    int margin_left;
    int margin_bottom;

    void setup();
    void draw();
    void update();
    void exit();

    bool patchExists(int uid);
    Patch *getPatch(int uid);

    void addApp(string &s);
    void openApp(string &s);
    void addButton(string &s);
    void addBlankApp(string &s);
    void addSlider(string &s);


};
