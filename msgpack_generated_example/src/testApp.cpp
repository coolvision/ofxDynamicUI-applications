#include "testApp.h"
#include "ofxMsgPacker.h"
#include "ofxMsgUnPacker.h"
#include "MsgPackMyClass.h"

bool bRecord = false;
bool bPlay = false;

ofxMsgPacker<MsgPackMyClass > serializer;
ofxMsgUnPacker<MsgPackMyClass > deserializer;

//--------------------------------------------------------------
void testApp::setup(){
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    serializer.setup();
}

//--------------------------------------------------------------
void testApp::update(){
    if ( bRecord )
    {
        serializer.add(MsgPackMyClass(mouseX, mouseY));
    }
    else if ( bPlay )
    {
        deserializer.updateBuffer();
    }
    
}

//--------------------------------------------------------------
void testApp::draw(){
    if ( !bRecord )
    {
        ofDrawBitmapString("press 'r' to record mouse position", ofPoint( 10, 20));
        ofDrawBitmapString("press 'o' to play saved file", ofPoint( 10, 60));
        ofSetColor(0, 255, 0);
        ofCircle(ofGetWidth() - 50, 50, 20);
        ofSetHexColor(0xffffff);
    }
    else
    {
        ofDrawBitmapString("press 'r' to end record", ofPoint( 10, 20));
        ofDrawBitmapString("press 'o' to play saved file", ofPoint( 10, 60));
        ofSetColor(255, 0, 0);
        ofCircle(ofGetWidth() - 50, 50, 20);
        ofSetHexColor(0xffffff);
    }
    if ( bPlay )
    {
        {
            ofSetHexColor(0x0);
            MsgPackMyClass myClass;
            deserializer.get(myClass);
            ofCircle(myClass.mousePosX, myClass.mousePosY, 5);
            ofSetHexColor(0xffffff);
        }
    }
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    switch (key) {
        case 'r':
            if ( !bRecord )
            {
                serializer.clear();
                bPlay = false;
            }
            bRecord = !bRecord;
            break;
        case 'o':
            if ( !bRecord )
            {
                bPlay = !bPlay;
                deserializer.setupBuffer(serializer);
            }
            break;
        default:
            break;
    }
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}