
//--------------------------------------------------------------
//
//  Kevin M. Smith
//
//  Octree Test - startup scene
//
//
//  Student Name: Alan Park, Walton Ma
//  Date date: May 18, 2022


#include "ofApp.h"
#include "Util.h"


//--------------------------------------------------------------
// setup scene, lighting, state and load geometry
//
void ofApp::setup() {
    bWireframe = false;
    bDisplayPoints = false;
    bAltKeyDown = false;
    bCtrlKeyDown = false;
    bLanderLoaded = false;
    bTerrainSelected = true;
    ofSetVerticalSync(true);
    
    background.load("images/background.png");

    theCam = &easyCam;
    easyCam.setDistance(100);
    easyCam.setNearClip(0.1);
    easyCam.setFov(65.5); // approx equivalent to 28mm in 35mm format
    easyCam.setPosition(0, 300, 200);
    easyCam.lookAt(glm::vec3(0, 0, 0));
    easyCam.disableMouseInput();
    
    //trackCam.setPosition(glm::vec3(0, 75, 50));
    trackCam.setPosition(glm::vec3(0, 51, 50)); // Set at lower position so its easier to see how close rocket is to ground
    trackCam.setNearClip(0.1);
    trackCam.setFov(65.5);
    
    landerCam1.setNearClip(0.1);
    landerCam1.setFov(65.5);
    
    ofEnableSmoothing();
    ofEnableDepthTest();

    // setup rudimentary & actual lighting
    initLightingAndMaterials();

    float ambLightVal = 0.6;
    ambientLight1.setup();
    ambientLight1.enable();
    ambientLight1.setAreaLight(100, 100);
    ambientLight1.setAmbientColor(ofFloatColor(ambLightVal * 0.1, ambLightVal * 0.1, ambLightVal * 0.1));
    ambientLight1.setDiffuseColor(ofFloatColor(ambLightVal, ambLightVal, ambLightVal));
    ambientLight1.setSpecularColor(ofFloatColor(ambLightVal, ambLightVal, ambLightVal));
    ambientLight1.setPosition(0, 135, 0);

    float ambLightVa2 = 0.3;
    ambientLight2.setup();
    ambientLight2.enable();
    ambientLight2.setAreaLight(100, 100);
    ambientLight2.setAmbientColor(ofFloatColor(ambLightVa2 * 0.1, ambLightVa2 * 0.1, ambLightVa2 * 0.1));
    ambientLight2.setDiffuseColor(ofFloatColor(ambLightVa2, ambLightVa2, ambLightVa2));
    ambientLight2.setSpecularColor(ofFloatColor(ambLightVa2, ambLightVa2, ambLightVa2));
    ambientLight2.setPosition(0, 54, 0);

    float landerLightVal = 1;
    landerLight.setup();
    landerLight.enable();
    landerLight.setSpotlight();
    landerLight.setScale(.05);
    landerLight.setSpotlightCutOff(15);
    landerLight.setAttenuation(2, .01, .01);
    landerLight.setAmbientColor(ofFloatColor(landerLightVal * 0.5, landerLightVal * 0.5, landerLightVal * 0.5));
    landerLight.setDiffuseColor(ofFloatColor(landerLightVal, landerLightVal, landerLightVal));
    landerLight.setSpecularColor(ofFloatColor(landerLightVal, landerLightVal, landerLightVal));
    landerLight.rotate(-90, ofVec3f(1, 0, 0));
    landerLight.setPosition(lander.getPosition().x, lander.getPosition().y + 6, lander.getPosition().z);

    float landLightVal = 1;
    landingLight1.setup();
    landingLight1.enable();
    landingLight1.setSpotlight();
    landingLight1.setScale(.05);
    landingLight1.setSpotlightCutOff(45);
    landingLight1.setAttenuation(1, .01, .01);
    landingLight1.setAmbientColor(ofFloatColor(0, landLightVal * 0.1, 0));
    landingLight1.setDiffuseColor(ofFloatColor(0, landLightVal, 0));
    landingLight1.setSpecularColor(ofFloatColor(0, landLightVal, 0));
    landingLight1.rotate(-90, ofVec3f(1, 0, 0));
    //landingLight1.setPosition(-45, 52, -40);
    landingLight1.setPosition(-45, 58, -40);

    landingLight2.setup();
    landingLight2.enable();
    landingLight2.setSpotlight();
    landingLight2.setScale(.05);
    landingLight2.setSpotlightCutOff(45);
    landingLight2.setAttenuation(1, .01, .01);
    landingLight2.setAmbientColor(ofFloatColor(0, landLightVal * 0.1, 0));
    landingLight2.setDiffuseColor(ofFloatColor(0, landLightVal, 0));
    landingLight2.setSpecularColor(ofFloatColor(0, landLightVal, 0));
    landingLight2.rotate(-90, ofVec3f(1, 0, 0));
    //landingLight2.setPosition(23, 52, 63);
    landingLight2.setPosition(23, 58, 63);

    landingLight3.setup();
    landingLight3.enable();
    landingLight3.setSpotlight();
    landingLight3.setScale(.05);
    landingLight3.setSpotlightCutOff(45);
    landingLight3.setAttenuation(1, .01, .01);
    landingLight3.setAmbientColor(ofFloatColor(0, landLightVal * 0.1, 0));
    landingLight3.setDiffuseColor(ofFloatColor(0, landLightVal, 0));
    landingLight3.setSpecularColor(ofFloatColor(0, landLightVal, 0));
    landingLight3.rotate(-90, ofVec3f(1, 0, 0));
    //landingLight3.setPosition(83, 48, -76);
    landingLight3.setPosition(83, 54, -76);


    //  Setup terrain and lander models
    mars.loadModel("geo/PlanetTerrainMesaFinal.obj"); // custom terrain
    mars.setScaleNormalization(false);

    //lander.loadModel("geo/lander.obj"); // Placeholder lander model
    lander.loadModel("geo/red_rocket.obj"); // Custom lander model
    lander.setScaleNormalization(false);
    lander.setPosition(0, 75, 0);
    bLanderLoaded = true;

    // create sliders for testing
    //
    gui.setup();
    gui.add(numLevels.setup("Number of Octree Levels", 1, 1, 10));
    
    bHide = false;

    //  Create Octree for testing.
    //

    mouseClickID = 0;

    uint64_t initialCreateTreeTime = ofGetElapsedTimeMillis();

    octree.create(mars.getMesh(0), 20);

    uint64_t finalCreateTreeTime = ofGetElapsedTimeMillis();

    cout << "Tree Create Time: " << finalCreateTreeTime - initialCreateTreeTime << " ms" << endl;
    cout << "Number of Verts: " << mars.getMesh(0).getNumVertices() << endl;

    testBox = Box(Vector3(3, 3, 0), Vector3(5, 5, 2));
    
    // Player particle for movement
    gravityForce = new GravityForce(ofVec3f(0, -2.5, 0));
    playerParticleEmitter.sys->addForce(gravityForce);
    turbForce = new TurbulenceForce(ofVec3f(-7, 0, -7), ofVec3f(7, 0, 7));
    playerParticleEmitter.sys->addForce(turbForce);
    playerParticleEmitter.setVelocity(ofVec3f(0, 0, 0));
    playerParticleEmitter.setOneShot(true);
    playerParticleEmitter.setEmitterType(DirectionalEmitter);
    playerParticleEmitter.setPosition(ofVec3f(0, 100, 0));
    playerParticleEmitter.setLifespan(-1);
    //playerParticleEmitter.start();
    
    fuel = 100.0;
    altitude = lander.getPosition().y;
    
    // Rocket emitter
    turbForceRocket = new TurbulenceForce(ofVec3f(-1, 0, -1), ofVec3f(1, 0, 1));
    rocketEmitter.sys->addForce(turbForceRocket);
    rocketEmitter.setEmitterType(RadialEmitter);
    rocketEmitter.useOriginalRadial = false;
    rocketEmitter.setVelocity(ofVec3f(0, -100, 0));
    rocketEmitter.setGroupSize(200);
    rocketEmitter.setParticleRadius(.3);
    rocketEmitter.setRate(1);
    rocketEmitter.setLifespan(0.3);
    rocketEmitter.setOneShot(true);
    
    // Explosion Emitter
    radialForce = new ImpulseRadialForce(2000.0, 1);
    explosionEmitter.sys->addForce(radialForce);
    explosionEmitter.setEmitterType(RadialEmitter);
    explosionEmitter.useOriginalRadial = true;
    explosionEmitter.setOneShot(true);
    explosionEmitter.setVelocity(ofVec3f(0, 0, 0));
    explosionEmitter.setGroupSize(1000);
    explosionEmitter.setRate(1);
    explosionEmitter.setLifespan(0.5);
    explosionEmitter.setParticleRadius(0.2);
    
    
    trackCam.lookAt(lander.getPosition());
    landerCam1.setPosition(glm::vec3(lander.getPosition().x, lander.getPosition().y + 1.5, lander.getPosition().z + 3.5));
    landerCam1.lookAt(glm::vec3(lander.getPosition().x, -lander.getPosition().y, lander.getPosition().z));
}


//--------------------------------------------------------------
// incrementally update scene (animation)
//
void ofApp::update() {
    playerParticleEmitter.setPosition(ofVec3f(lander.getPosition().x, lander.getPosition().y, lander.getPosition().z));

    if (bGameStart)
    {
        if (!playEmitterStarted)
        {
            playerParticleEmitter.start();
            playEmitterStarted = true;
        }

        if (altitude <= 0) {
            if (!completedLandingSequence) {
                float finalYVelocity = playerParticleEmitter.sys->particles.at(0).velocity.y;
                playerParticleEmitter.stop();

                if (finalYVelocity < -5) {
                    explosionEmitter.sys->reset();
                    explosionEmitter.start(); // debugging for now, will move for collision detection
                    landingType = 0; // crash
                }
                else if (finalYVelocity < -2.5) {
                    landingType = 1; // hard landing
                }
                else {
                    landingType = 2; // good landing
                }
                completedLandingSequence = true;
            }
        }
        else {
            playerParticleEmitter.update();
        }

        rocketEmitter.update();
        explosionEmitter.update();
        ofVec3f landerParticlePosition = playerParticleEmitter.sys->particles.at(0).position;
        rocketEmitter.setPosition(landerParticlePosition);
        explosionEmitter.setPosition(landerParticlePosition);
        lander.setPosition(landerParticlePosition.x, landerParticlePosition.y, landerParticlePosition.z);
        trackCam.lookAt(lander.getPosition());
        landerCam1.setPosition(glm::vec3(lander.getPosition().x, lander.getPosition().y + 1.5, lander.getPosition().z + 3.5));
        landerCam1.lookAt(glm::vec3(lander.getPosition().x, -lander.getPosition().y, lander.getPosition().z));

        if (camOnLander) {
            easyCam.setPosition(lander.getPosition().x, lander.getPosition().y + 50, lander.getPosition().z + 50);
            easyCam.lookAt(lander.getPosition());
        }

        // TODO: check altitude with raycast

        ofVec3f pt;
        raySelectWithOctreeLander(pt);
        altitude = lander.getPosition().y - 5.4 - pt.y; // 5.4 for model height

        if (fuel > 0) {

            if (moveLeft || moveRight || moveForward || moveBackward || thrust || rotateLeft || rotateRight) {
                fuel -= 0.05;
            }

            if (moveLeft) {
                playerParticleEmitter.sys->particles.at(0).forces.x -= 7;
            }
            if (moveRight) {
                playerParticleEmitter.sys->particles.at(0).forces.x += 7;
            }
            if (moveForward) {
                playerParticleEmitter.sys->particles.at(0).forces.z -= 7;
            }
            if (moveBackward) {
                playerParticleEmitter.sys->particles.at(0).forces.z += 7;
            }
            if (thrust) {
                playerParticleEmitter.sys->particles.at(0).forces.y += 10;
                rocketEmitter.sys->reset();
                rocketEmitter.start();
            }
            else {
                rocketEmitter.stop();
            }
            // TODO: rotate model
            if (rotateLeft) {

            }
            if (rotateRight) {

            }

        }
    }
    
    // Adjust lander light so it sticks with lander
    landerLight.setPosition(lander.getPosition().x, lander.getPosition().y + 6, lander.getPosition().z);
}

//--------------------------------------------------------------
void ofApp::draw() {
    ofBackground(ofColor::black);

    glDepthMask(false);
    ofSetColor(ofColor::white);
    background.draw(0, 0, ofGetWindowWidth(), ofGetWindowHeight()); // Unsure if should use width & height of window or actual image
    glDepthMask(true);

    theCam->begin();

    ofPushMatrix();
    
    if (bWireframe) {                    // wireframe mode  (include axis)
        ofDisableLighting();
        ofSetColor(ofColor::slateGray);
        mars.drawWireframe();
        if (bLanderLoaded) {
            lander.drawWireframe();
            if (!bTerrainSelected) drawAxis(lander.getPosition());
        }
        if (bTerrainSelected) drawAxis(ofVec3f(0, 0, 0));
    }
    else {
        ofEnableLighting();              // shaded mode
        mars.drawFaces();
        ofMesh mesh;
        if (bLanderLoaded) {
            lander.drawFaces();
            if (!bTerrainSelected) drawAxis(lander.getPosition());
            if (bDisplayBBoxes) {
                ofNoFill();
                ofSetColor(ofColor::white);
                for (int i = 0; i < lander.getNumMeshes(); i++) {
                    ofPushMatrix();
                    ofMultMatrix(lander.getModelMatrix());
                    ofRotate(-90, 1, 0, 0);
                    Octree::drawBox(bboxList[i]);
                    ofPopMatrix();
                }
            }

            if (bLanderSelected) {

                ofVec3f min = lander.getSceneMin() + lander.getPosition();
                ofVec3f max = lander.getSceneMax() + lander.getPosition();

                Box bounds = Box(Vector3(min.x, min.y, min.z), Vector3(max.x, max.y, max.z));
                ofNoFill();
                ofSetColor(ofColor::white);
                Octree::drawBox(bounds);

                // draw colliding boxes
                //
                ofSetColor(ofColor::lightBlue);
                for (int i = 0; i < colBoxList.size(); i++) {
                    Octree::drawBox(colBoxList[i]);
                }
            }
        }
    }
    if (bTerrainSelected) drawAxis(ofVec3f(0, 0, 0));



    if (bDisplayPoints) {                // display points as an option
        glPointSize(3);
        ofSetColor(ofColor::green);
        mars.drawVertices();
    }

    // highlight selected point (draw sphere around selected point)
    //
    if (bPointSelected) {
        ofSetColor(ofColor::blue);
        ofDrawSphere(selectedPoint, .1);
    }


    // recursively draw octree
    //
    ofDisableLighting();
    int level = 0;
    //	ofNoFill();

    if (bDisplayLeafNodes) {
        octree.drawLeafNodes(octree.root);
        cout << "num leaf: " << octree.numLeaf << endl;
    }
    else if (bDisplayOctree) {
        ofNoFill();
        ofSetColor(ofColor::white);
        octree.draw(numLevels, 0);
    }

    // if point selected, draw a sphere
    //
//    if (pointSelected) {
//        ofVec3f p = octree.mesh.getVertex(selectedNode.points[0]);
//        ofVec3f d = p - easyCam.getPosition();
//        ofSetColor(ofColor::lightGreen);
//        ofDrawSphere(p, .02 * d.length());
//    }

    if (bGameStart)
    {
        rocketEmitter.draw();
        explosionEmitter.draw();
    }
    
    ofPopMatrix();
    theCam->end();
    
    glDepthMask(false);
    if (!bHide) gui.draw();

    if (!bGameStart)
    {
        ofSetColor(ofColor::green);
        ofDrawBitmapString("Press 'ENTER' to start simulation", (ofGetWindowWidth() / 2) - 130, 25);
    }
    if (bGameStart)
    {
        ofSetColor(ofColor::white);
        string fuelText = "Fuel: " + std::to_string(fuel);
        if (fuel > 0) {
            ofDrawBitmapString(fuelText, ofGetWindowWidth() - 200, 25);
        }
        else {
            ofDrawBitmapString("Fuel: EMPTY", ofGetWindowWidth() - 200, 25);
        }
        if (showAltitude) {
            if (completedLandingSequence) {
                if (landingType == 0) {
                    ofDrawBitmapString("Altitude: CRASHED", ofGetWindowWidth() - 200, 50);
                }
                else if (landingType == 1) {
                    ofDrawBitmapString("Altitude: HARD LANDING", ofGetWindowWidth() - 200, 50);
                }
                else if (landingType == 2) {
                    ofDrawBitmapString("Altitude: GOOD LAND!", ofGetWindowWidth() - 200, 50);
                }
            }
            else {
                string altitudeText = "Altitude: " + std::to_string(altitude);
                ofDrawBitmapString(altitudeText, ofGetWindowWidth() - 200, 50);
            }
        }
    }

    glDepthMask(true);
}


//
// Draw an XYZ axis in RGB at world (0,0,0) for reference.
//
void ofApp::drawAxis(ofVec3f location) {

    ofPushMatrix();
    ofTranslate(location);

    ofSetLineWidth(1.0);

    // X Axis
    ofSetColor(ofColor(255, 0, 0));
    ofDrawLine(ofPoint(0, 0, 0), ofPoint(1, 0, 0));


    // Y Axis
    ofSetColor(ofColor(0, 255, 0));
    ofDrawLine(ofPoint(0, 0, 0), ofPoint(0, 1, 0));

    // Z Axis
    ofSetColor(ofColor(0, 0, 255));
    ofDrawLine(ofPoint(0, 0, 0), ofPoint(0, 0, 1));

    ofPopMatrix();
}


void ofApp::keyPressed(int key) {
    switch (key) {
        case '1':
            /*if (pointSelected) {
                ofVec3f p = octree.mesh.getVertex(selectedNode.points[0]);
                easyCam.lookAt(p);
            }*/

            if (bPointSelected)
            {
                easyCam.lookAt(selectedPoint);
            }

            camOnLander = false;
            break;
        case '2':
            if (!camOnLander) camOnLander = true;
            else camOnLander = false;
            easyCam.lookAt(lander.getPosition());
            break;
        case 'B':
        case 'b':
            bDisplayBBoxes = !bDisplayBBoxes;
            break;
        case 'C':
        case 'c':
            if (easyCam.getMouseInputEnabled()) easyCam.disableMouseInput();
            else easyCam.enableMouseInput();
            break;
        case 'F':
        case 'f':
            ofToggleFullscreen();
            break;
        case 'H':
        case 'h':
            if (!showAltitude) showAltitude = true;
            else showAltitude = false;
            break;
        case 'L':
        case 'l':
            bDisplayLeafNodes = !bDisplayLeafNodes;
            break;
        case 'O':
        case 'o':
            bDisplayOctree = !bDisplayOctree;
            break;
        case 'r':
            easyCam.reset();
            break;
        case 's':
            savePicture();
            break;
        case 't':
            setCameraTarget();
            break;
        case 'u':
            break;
        case 'v':
            togglePointsDisplay();
            break;
        case 'V':
            break;
        case 'w':
            toggleWireframeMode();
            break;
        case OF_KEY_ALT:
            easyCam.enableMouseInput();
            bAltKeyDown = true;
            break;
        case OF_KEY_CONTROL:
            bCtrlKeyDown = true;
            break;
        case OF_KEY_SHIFT:
            break;
        case OF_KEY_DEL:
            break;
                
        case OF_KEY_RETURN:
            bGameStart = true;
            break;

        case OF_KEY_LEFT:
            moveLeft = true;
            break;
        case OF_KEY_RIGHT:
            moveRight = true;
            break;
        case OF_KEY_UP:
            moveForward = true;
            break;
        case OF_KEY_DOWN:
            moveBackward = true;
            break;
        case ' ':
            thrust = true;
            break;
        case 'z':
            rotateLeft = true;
            break;
        case 'x':
            rotateRight = true;
            break;

        case OF_KEY_F1:
            theCam = &easyCam;
            break;
        case OF_KEY_F2:
            theCam = &trackCam;
            break;
        case OF_KEY_F3:
            theCam = &landerCam1;
            break;
        default:
            break;
    }
}

void ofApp::toggleWireframeMode() {
    bWireframe = !bWireframe;
}

void ofApp::toggleSelectTerrain() {
    bTerrainSelected = !bTerrainSelected;
}

void ofApp::togglePointsDisplay() {
    bDisplayPoints = !bDisplayPoints;
}

void ofApp::keyReleased(int key) {
    switch (key) {
        case OF_KEY_ALT:
            easyCam.disableMouseInput();
            bAltKeyDown = false;
            break;
        case OF_KEY_CONTROL:
            bCtrlKeyDown = false;
            break;
        case OF_KEY_SHIFT:
            break;
        case OF_KEY_LEFT:
            moveLeft = false;
            break;
        case OF_KEY_RIGHT:
            moveRight = false;
            break;
        case OF_KEY_UP:
            moveForward = false;
            break;
        case OF_KEY_DOWN:
            moveBackward = false;
            break;
        case ' ':
            thrust = false;
            break;
        case 'z':
            rotateLeft = false;
            break;
        case 'x':
            rotateRight = false;
            break;
        default:
            break;

    }
}



//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}


//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

    // if moving camera, don't allow mouse interaction
    if (easyCam.getMouseInputEnabled()) return;

    // if rover is loaded, test for selection
    if (bLanderLoaded) {
        glm::vec3 origin = easyCam.getPosition();
        glm::vec3 mouseWorld = easyCam.screenToWorld(glm::vec3(mouseX, mouseY, 0));
        glm::vec3 mouseDir = glm::normalize(mouseWorld - origin);

        ofVec3f min = lander.getSceneMin() + lander.getPosition();
        ofVec3f max = lander.getSceneMax() + lander.getPosition();

        Box bounds = Box(Vector3(min.x, min.y, min.z), Vector3(max.x, max.y, max.z));
        bool hit = bounds.intersect(Ray(Vector3(origin.x, origin.y, origin.z), Vector3(mouseDir.x, mouseDir.y, mouseDir.z)), 0, 10000);
        if (hit) {
            bLanderSelected = true;
            mouseDownPos = getMousePointOnPlane(lander.getPosition(), easyCam.getZAxis());
            mouseLastPos = mouseDownPos;
            bInDrag = true;
        }
        else {
            bLanderSelected = false;
        }
    }
    else {
        mouseClickID++;
        ofVec3f p;
        uint64_t initialRaySelectTime = ofGetElapsedTimeMillis();
        raySelectWithOctree(p);
        uint64_t finalRaySelectTime = ofGetElapsedTimeMillis();
        cout << "Mouse Click " << mouseClickID << ", Ray Selection Time: " << finalRaySelectTime - initialRaySelectTime << " ms" << endl;
    }
}

bool ofApp::raySelectWithOctree(ofVec3f &pointRet) {
    ofVec3f mouse(mouseX, mouseY);
    ofVec3f rayPoint = easyCam.screenToWorld(mouse);
    ofVec3f rayDir = rayPoint - easyCam.getPosition();
    rayDir.normalize();
    Ray ray = Ray(Vector3(rayPoint.x, rayPoint.y, rayPoint.z),
        Vector3(rayDir.x, rayDir.y, rayDir.z));

    pointSelected = octree.intersect(ray, octree.root, selectedNode);

    if (pointSelected) {
        pointRet = octree.mesh.getVertex(selectedNode.points[0]);
    }
    return pointSelected;
}

bool ofApp::raySelectWithOctreeLander(ofVec3f &pointRet) {
    ofVec3f landerOrig = ofVec3f(lander.getPosition().x, lander.getPosition().y, lander.getPosition().z);
    ofVec3f rayDir = ofVec3f(0, -1, 0);
    rayDir.normalize();
    Ray ray = Ray(Vector3(landerOrig.x, landerOrig.y, landerOrig.z),
        Vector3(rayDir.x, rayDir.y, rayDir.z));

    pointSelected = octree.intersect(ray, octree.root, selectedNode);

    if (pointSelected) {
        pointRet = octree.mesh.getVertex(selectedNode.points[0]);
    }
    return pointSelected;
}




//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

    // if moving camera, don't allow mouse interaction
    //
    if (easyCam.getMouseInputEnabled()) return;

    if (bInDrag) {

        glm::vec3 landerPos = lander.getPosition();

        glm::vec3 mousePos = getMousePointOnPlane(landerPos, easyCam.getZAxis());
        glm::vec3 delta = mousePos - mouseLastPos;

        landerPos += delta;
        lander.setPosition(landerPos.x, landerPos.y, landerPos.z);
        mouseLastPos = mousePos;

        ofVec3f min = lander.getSceneMin() + lander.getPosition();
        ofVec3f max = lander.getSceneMax() + lander.getPosition();

        Box bounds = Box(Vector3(min.x, min.y, min.z), Vector3(max.x, max.y, max.z));

        colBoxList.clear();
        octree.intersect(bounds, octree.root, colBoxList);


        /*if (bounds.overlap(testBox)) {
            cout << "overlap" << endl;
        }
        else {
            cout << "OK" << endl;
        }*/


    }
    else {
        ofVec3f p;
        raySelectWithOctree(p);
    }
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
    bInDrag = false;
}



// Set the camera to use the selected point as it's new target
//
void ofApp::setCameraTarget() {

}


//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}



//--------------------------------------------------------------
// setup basic ambient lighting in GL  (for now, enable just 1 light)
//
void ofApp::initLightingAndMaterials() {

    static float ambient[] =
    { .5f, .5f, .5, 1.0f };
    static float diffuse[] =
    { 1.0f, 1.0f, 1.0f, 1.0f };

    static float position[] =
    {5.0, 5.0, 5.0, 0.0 };

    static float lmodel_ambient[] =
    { 1.0f, 1.0f, 1.0f, 1.0f };

    static float lmodel_twoside[] =
    { GL_TRUE };


    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, position);

    glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT1, GL_POSITION, position);


    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
    glLightModelfv(GL_LIGHT_MODEL_TWO_SIDE, lmodel_twoside);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
//	glEnable(GL_LIGHT1);
    glShadeModel(GL_SMOOTH);
}

void ofApp::savePicture() {
    ofImage picture;
    picture.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
    picture.save("screenshot.png");
    cout << "picture saved" << endl;
}

//--------------------------------------------------------------
//
// support drag-and-drop of model (.obj) file loading.  when
// model is dropped in viewport, place origin under cursor
//
void ofApp::dragEvent2(ofDragInfo dragInfo) {

    ofVec3f point;
    mouseIntersectPlane(ofVec3f(0, 0, 0), easyCam.getZAxis(), point);
    if (lander.loadModel(dragInfo.files[0])) {
        lander.setScaleNormalization(false);
//		lander.setScale(.1, .1, .1);
    //	lander.setPosition(point.x, point.y, point.z);
        lander.setPosition(1, 1, 0);

        bLanderLoaded = true;
        for (int i = 0; i < lander.getMeshCount(); i++) {
            bboxList.push_back(Octree::meshBounds(lander.getMesh(i)));
        }

        cout << "Mesh Count: " << lander.getMeshCount() << endl;
    }
    else cout << "Error: Can't load model" << dragInfo.files[0] << endl;
}

bool ofApp::mouseIntersectPlane(ofVec3f planePoint, ofVec3f planeNorm, ofVec3f &point) {
    ofVec2f mouse(mouseX, mouseY);
    ofVec3f rayPoint = easyCam.screenToWorld(glm::vec3(mouseX, mouseY, 0));
    ofVec3f rayDir = rayPoint - easyCam.getPosition();
    rayDir.normalize();
    return (rayIntersectPlane(rayPoint, rayDir, planePoint, planeNorm, point));
}

//--------------------------------------------------------------
//
// support drag-and-drop of model (.obj) file loading.  when
// model is dropped in viewport, place origin under cursor
//
void ofApp::dragEvent(ofDragInfo dragInfo) {
    if (lander.loadModel(dragInfo.files[0])) {
        bLanderLoaded = true;
        lander.setScaleNormalization(false);
        lander.setPosition(0, 0, 0);
        cout << "number of meshes: " << lander.getNumMeshes() << endl;
        bboxList.clear();
        for (int i = 0; i < lander.getMeshCount(); i++) {
            bboxList.push_back(Octree::meshBounds(lander.getMesh(i)));
        }

        //		lander.setRotation(1, 180, 1, 0, 0);

                // We want to drag and drop a 3D object in space so that the model appears
                // under the mouse pointer where you drop it !
                //
                // Our strategy: intersect a plane parallel to the camera plane where the mouse drops the model
                // once we find the point of intersection, we can position the lander/lander
                // at that location.
                //

                // Setup our rays
                //
        glm::vec3 origin = easyCam.getPosition();
        glm::vec3 camAxis = easyCam.getZAxis();
        glm::vec3 mouseWorld = easyCam.screenToWorld(glm::vec3(mouseX, mouseY, 0));
        glm::vec3 mouseDir = glm::normalize(mouseWorld - origin);
        float distance;

        bool hit = glm::intersectRayPlane(origin, mouseDir, glm::vec3(0, 0, 0), camAxis, distance);
        if (hit) {
            // find the point of intersection on the plane using the distance
            // We use the parameteric line or vector representation of a line to compute
            //
            // p' = p + s * dir;
            //
            glm::vec3 intersectPoint = origin + distance * mouseDir;

            // Now position the lander's origin at that intersection point
            //
            glm::vec3 min = lander.getSceneMin();
            glm::vec3 max = lander.getSceneMax();
            float offset = (max.y - min.y) / 2.0;
            lander.setPosition(intersectPoint.x, intersectPoint.y - offset, intersectPoint.z);

            // set up bounding box for lander while we are at it
            //
            landerBounds = Box(Vector3(min.x, min.y, min.z), Vector3(max.x, max.y, max.z));
        }
    }


}

//  intersect the mouse ray with the plane normal to the camera
//  return intersection point.   (package code above into function)
//
glm::vec3 ofApp::getMousePointOnPlane(glm::vec3 planePt, glm::vec3 planeNorm) {
    // Setup our rays
    //
    glm::vec3 origin = easyCam.getPosition();
    glm::vec3 camAxis = easyCam.getZAxis();
    glm::vec3 mouseWorld = easyCam.screenToWorld(glm::vec3(mouseX, mouseY, 0));
    glm::vec3 mouseDir = glm::normalize(mouseWorld - origin);
    float distance;

    bool hit = glm::intersectRayPlane(origin, mouseDir, planePt, planeNorm, distance);

    if (hit) {
        // find the point of intersection on the plane using the distance
        // We use the parameteric line or vector representation of a line to compute
        //
        // p' = p + s * dir;
        //
        glm::vec3 intersectPoint = origin + distance * mouseDir;

        return intersectPoint;
    }
    else return glm::vec3(0, 0, 0);
}
