#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include  "ofxAssimpModelLoader.h"
#include "Octree.h"
#include <glm/gtx/intersect.hpp>
#include "Particle.h"
#include "ParticleEmitter.h"
#include "ray.h"
//#include <fstream>



class ofApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent2(ofDragInfo dragInfo);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    void drawAxis(ofVec3f);
    void initLightingAndMaterials();
    void savePicture();
    void toggleWireframeMode();
    void togglePointsDisplay();
    void toggleSelectTerrain();
    void setCameraTarget();
    bool mouseIntersectPlane(ofVec3f planePoint, ofVec3f planeNorm, ofVec3f &point);
    bool raySelectWithOctree(ofVec3f &pointRet);
    bool raySelectWithOctreeLander(ofVec3f &pointRet);
    glm::vec3 getMousePointOnPlane(glm::vec3 p , glm::vec3 n);

//    ofEasyCam cam;
    
    // OpenFramework cameras
    ofEasyCam easyCam; // General overview
    ofCamera trackCam; // Tracking camera, constantly on lander
    ofCamera landerCam1; // Camera on-board lander
    ofCamera *theCam;

    ofImage background;

    // OpenFramework lights
    ofLight ambientLight1, ambientLight2;   // Ambient lighting, ensure entire scene isn't pitch black
    ofLight landerLight;    // Light to illuminate lander.
    ofLight landingLight1, landingLight2, landingLight3;    // Lights to mark landing spots
    
    ofxAssimpModelLoader mars, lander;
    ofLight light;
    Box boundingBox, landerBounds;
    Box testBox;
    vector<Box> colBoxList;
    bool bLanderSelected = false;
    Octree octree;
    TreeNode selectedNode;
    glm::vec3 mouseDownPos, mouseLastPos;
    bool bInDrag = false;


    ofxIntSlider numLevels;
    ofxPanel gui;

    bool bAltKeyDown;
    bool bCtrlKeyDown;
    bool bWireframe;
    bool bDisplayPoints;
    bool bPointSelected;
    bool bHide;
    bool pointSelected = false;
    bool bDisplayLeafNodes = false;
    bool bDisplayOctree = false;
    bool bDisplayBBoxes = false;
    bool bGameStart = false;
    
    bool bLanderLoaded;
    bool bTerrainSelected;

    ofVec3f selectedPoint;
    ofVec3f intersectPoint;

    vector<Box> bboxList;

    const float selectionRange = 4.0;
    
    int mouseClickID;
    
    // physics based movement using Particle Emitter
    GravityForce *gravityForce;
    TurbulenceForce *turbForce;
    ParticleEmitter playerParticleEmitter;
    
    TurbulenceForce *turbForceRocket;
    ParticleEmitter rocketEmitter;
    
    ImpulseRadialForce *radialForce;
    ParticleEmitter explosionEmitter;
    
    bool moveForward;
    bool moveBackward;
    bool moveLeft;
    bool moveRight;
    bool thrust;
    bool rotateLeft;
    bool rotateRight;
    
    float altitude;
    float fuel;
    
    bool showAltitude = true;
    bool camOnLander = true;
    bool completedLandingSequence = false;
    int landingType = -1;
};
