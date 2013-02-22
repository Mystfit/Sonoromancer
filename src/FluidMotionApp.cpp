#include "FluidMotionApp.h"


//--------------------------------------------------------------
void FluidMotionApp::setup(){
    
    //ofEnableAlphaBlending();
	ofSetWindowShape(512, 512);
    ofSetLogLevel(OF_LOG_NOTICE);
    ofEnableArbTex();
    
    // Initial Allocation
    //
    fluid.allocate(LOWREZ_FLUID_SIZE, LOWREZ_FLUID_SIZE, 1, 1.0f);
    fluidPlayback.allocate(HIREZ_FLUID_SIZE, HIREZ_FLUID_SIZE, 0.5f);
    inputImage.allocate(LOWREZ_FLUID_SIZE, LOWREZ_FLUID_SIZE);
    inputImageGrey.allocate(LOWREZ_FLUID_SIZE, LOWREZ_FLUID_SIZE);
    tempPixels.allocate(LOWREZ_FLUID_SIZE,LOWREZ_FLUID_SIZE,3);
    
    texBlender.init(CAMERA_WIDTH, CAMERA_HEIGHT, LOWREZ_FLUID_SIZE, LOWREZ_FLUID_SIZE);
    kinectSavedOpflowBlender.init(CAMERA_WIDTH, CAMERA_HEIGHT, LOWREZ_FLUID_SIZE, LOWREZ_FLUID_SIZE);
    
    //Buffer for saving images
    velocityBufferPixels.allocate(HIREZ_FLUID_SIZE, HIREZ_FLUID_SIZE, 3);
    smokeBufferPixels.allocate(HIREZ_FLUID_SIZE, HIREZ_FLUID_SIZE, 3);
    playbackPixels.allocate(LOWREZ_FLUID_SIZE, LOWREZ_FLUID_SIZE, 3);
    playbackFrame = 0;
        
    threshold = 105;
    dyeColour.set(1.0f,1.0f,1.0f);
    dyeRadius = 10.f;
    dyeDensity = 0.1f;
    dyeVelocityMult = 1.0f;
    
    //Init gui
    initGui();
    
    //Kinect properties
    depthActivationEnd = 0.1f;
    depthActivationStart = 0.06f;
    
    // Seting the gravity set up & injecting the background image
    fluid.setDissipation(0.97);
    fluid.setVelocityDissipation(0.97);
    fluid.setDyeColour(ofVec3f(0.2f, 0.2f, 1.0f));
    
    fluidPlayback.setDissipation(0.97);
    fluidPlayback.setVelocityDissipation(0.97);
    fluidPlayback.setDyeColour(ofVec3f(0.2f, 0.2f, 1.0f));
    
    bDrawKinect = true;
    bDrawFluid = true;
    bDrawBlobs = false;
    bCalculateBlobs = true;
}

void FluidMotionApp::increaseFarDepth()
{
    depthActivationStart += 0.01;
    CLAMP(depthActivationStart, 0.0f, 1.0f);
}

void FluidMotionApp::decreaseFarDepth()
{
    depthActivationStart -= 0.01;
    CLAMP(depthActivationStart, 0.0f, 1.0f);
}

void FluidMotionApp::increaseNearDepth()
{
    depthActivationEnd += 0.01;
    CLAMP(depthActivationEnd, 0.0f, 1.0f);
}

void FluidMotionApp::decreaseNearDepth()
{
    depthActivationEnd -= 0.01;
    CLAMP(depthActivationEnd, 0.0f, 1.0f);
}


void FluidMotionApp::increaseThreshold()
{
    threshold += 1;
    CLAMP(threshold, 0, 60);
}

void FluidMotionApp::decreaseThreshold()
{
    threshold -= 1;
    CLAMP(threshold, 0, 60);

}





//  Gui
//--------------------------------------------------------------
void FluidMotionApp::exit()
{
	gui->saveSettings("GUI/guiSettings.xml");
    delete gui;
}

void FluidMotionApp::initGui(){
    float xInit = OFX_UI_GLOBAL_WIDGET_SPACING;
    float length = 255-xInit;
    
    vector<string> texList;
	texList.push_back("Velocity");
	texList.push_back("Pressure");
	texList.push_back("Temperature");
    
    gui = new ofxUICanvas(0,0,1024,1024);
    gui->addWidgetDown(new ofxUILabel("SONOROMANCER OPTIONS", OFX_UI_FONT_LARGE));
    gui->addWidgetDown(new ofxUIToggle(32, 32, true, "FULLSCREEN"));
    gui->addWidgetDown(new ofxUILabel("FPS", OFX_UI_FONT_MEDIUM));
    gui->addWidgetDown(new ofxUIFPS(OFX_UI_FONT_MEDIUM));
    gui->addSpacer(length-xInit, 2);
    gui->addRadio("DISPLAY TEXTURE", texList, OFX_UI_ORIENTATION_VERTICAL, 16, 16);

    ofAddListener(gui->newGUIEvent, this, &FluidMotionApp::guiEvent);
    gui->loadSettings("GUI/guiSettings.xml");
}

void FluidMotionApp::guiEvent(ofxUIEventArgs &e)
{
    ofLog(OF_LOG_NOTICE, "--> Inside gui event. " + e.widget->getName());

	if(e.widget->getName() == "BACKGROUND VALUE")
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        ofBackground(slider->getScaledValue());
        
    } else if(e.widget->getName() == "FULLSCREEN")
    {
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        ofSetFullscreen(toggle->getValue());
        
    } else if(e.widget->getName() == "Velocity"){
        if( ((ofxUIToggle *)e.widget)->getValue() == true) fluid.drawVelocity();
    
    } else if(e.widget->getName() == "Pressure"){
        if( ((ofxUIToggle *)e.widget)->getValue() == true) fluid.drawPressure();

    } else if(e.widget->getName() == "Temperature"){
        if( ((ofxUIToggle *)e.widget)->getValue() == true) fluid.drawTemperature();
    }

}



//  Update
//--------------------------------------------------------------
void FluidMotionApp::update(){
    ofPushStyle();
    
    if(fluidKinect.isDeviceConnected){
        
        if(!isPlayingBackFrames)
        {
            //if(fluidPlayer.isPlaying) isRecordingFrames = true;
            
            //Update kinect cameras
            fluidKinect.update();
            
            //Mask kinect camera
            texBlender.updateKinectMasker(fluidKinect.getCameraTexture(), fluidKinect.getMaskTexture(), CAMERA_WIDTH, CAMERA_HEIGHT);
            
            //Update optical flow with masked kinect image
            fluidKinect.updateOpticalFlow(texBlender.kinectBuffer.dst->getTextureReference());
            
            //Blend optical flow velocites and depth camera
            texBlender.updateBlender( fluidKinect.opFlow.velTexX.getTextureReference(), fluidKinect.opFlow.velTexY.getTextureReference(), fluidKinect.getDepthTexture(), fluidKinect.getMaskTexture(), depthActivationStart, depthActivationEnd);
            
            if(isRecordingFrames){
                kinectSavedOpflowBlender.updateBlender( fluidKinect.opFlow.velTexX.getTextureReference(), fluidKinect.opFlow.velTexY.getTextureReference(), fluidKinect.getDepthTexture(), fluidKinect.getMaskTexture(), depthActivationStart, depthActivationEnd)->readToPixels(playbackPixels);
                //texBlender.blendBuffer.dst->readToPixels(playbackPixels);
                fluidInputTextures.push_back( playbackPixels );
                fluidRecordedColour.push_back(fluidPlayer.getActiveInstrument().dyeColour);
            }
            
            //Send camera velocities to fluid simulation
            fluid.setExternalVelocity(  texBlender.blendBuffer.dst->getTextureReference() );
            fluid.setDyeColour(fluidPlayer.getActiveInstrument().dyeColour);
            fluid.update();    
        
            //Do blob detection on fluid simulation output
            if(bCalculateBlobs){
                fluid.getPingPong().src->readToPixels(tempPixels);
                inputImage.setFromPixels(tempPixels.getPixels(),  fluid.getPingPong().src->getWidth(), fluid.getPingPong().src->getHeight());
                inputImage.flagImageChanged();
                inputImageGrey = inputImage;
                inputImageGrey.blurHeavily();
                //inputImageGrey.blurGaussian(7);
                inputImageGrey.threshold(threshold);
                inputImageGrey.flagImageChanged();
                blobFinder.findBlobs(inputImageGrey, false);
                
                //Generate notes from fluid blob output
                blobFinder.matchExistingBlobs();
                
                if(blobFinder.blobParams.size() > 0)
                    fluidPlayer.blobsToNotes(blobFinder.blobParams);
                else
                    fluidPlayer.clearAllNotes();
            }
        }
        
        //Replay fluid from saved optical flow source
        else
        {
            if(playbackFrame >= fluidInputTextures.size()){
                isPlayingBackFrames = false;
                isRecordingFrames = false;
                fluidInputTextures.clear();
            } else {
                //Send camera velocities to fluid simulation
                ofImage velocity = ofImage(fluidInputTextures[playbackFrame]);
                velocity.resize(HIREZ_FLUID_SIZE, HIREZ_FLUID_SIZE);
                fluidPlayback.setExternalVelocity( velocity.getTextureReference() );
                fluidPlayback.setDyeColour(fluidRecordedColour[playbackFrame]);
                fluidPlayback.update();
               
                fluidPlayback.getVelocityBuffer().src->readToPixels(velocityBufferPixels);
                fluidPlayback.getPingPong().src->readToPixels(smokeBufferPixels);
                    
                ofSaveImage(velocityBufferPixels, "frames/velocity_" + ofGetTimestampString() + "_" + ofToString(playbackFrame) + ".exr" );
                ofSaveImage(smokeBufferPixels, "frames/smoke_" + ofGetTimestampString() + "_" + ofToString(playbackFrame) + ".exr" );

                playbackFrame++;
            }
        }
    }
    
    ofPopStyle();    
}

//--------------------------------------------------------------
void FluidMotionApp::draw(){
    ofPushStyle();
    ofBackground(0);
    
    if(fluidPlayer.isBeatDirty()){
        ofFloatColor(1.0f, 0.0f, 0.0f);
        ofCircle(10.0f, 20.0f, 10.0f);
        fluidPlayer.setBeatClean();
    }
    
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    
    if(bDrawFluid)
        (!isPlayingBackFrames) ? fluid.draw((ofGetScreenWidth() - ofGetScreenHeight()) * 0.5f,0,ofGetScreenHeight(),ofGetScreenHeight()) : fluidPlayback.draw((ofGetScreenWidth() - ofGetScreenHeight()) * 0.5f,0,ofGetScreenHeight(),ofGetScreenHeight());;
    if(bDrawBlobs) blobFinder.draw((ofGetScreenWidth() - ofGetScreenHeight()) * 0.5f,0,ofGetScreenHeight(),ofGetScreenHeight());
    
    ofSetHexColor(0xFFFFFF);
    
    string instrumentStr = "Instrument: " + fluidPlayer.getActiveInstrument().name + ". ";
    
    ofDrawBitmapString(instrumentStr, 10.0f, 10.0f);
    
    if(!fluidPlayer.isPlaying)
        ofDrawBitmapString(instrumentStr + "Waiting for play message from Live",10.0f,10.0f);
    else if(isRecordingFrames)
        ofDrawBitmapString(instrumentStr + "Recording performance",10.0f,10.0f);
    else if(isPlayingBackFrames)
        ofDrawBitmapString(instrumentStr + "Saving hiRez simulation...", 10.0f,10.0f);
    
    ofDisableBlendMode();
    ofPopStyle();
}

//--------------------------------------------------------------
void FluidMotionApp::keyPressed(int key){
    if( key == 'p')
        bPaint = !bPaint;
    if( key == 'h')
        gui->toggleVisible();
    if( key == 'o')
        bDrawFluid = !bDrawFluid;
    if( key == 'i')
         bDrawKinect = !bDrawKinect;
    if( key == 'u')
        bDrawBlobs = ! bDrawBlobs;
    if( key == 'b')
        bCalculateBlobs = ! bCalculateBlobs;
    if( key == 'y')
        fluidKinect.opFlow.bDrawLines != fluidKinect.opFlow.bDrawLines;
    if( key == 'v')
        fluid.bDrawVelocity != fluid.bDrawVelocity;
    if( key == 'c')
        fluid.bDrawPressure != fluid.bDrawPressure;
    if( key =='r'){
        if(!isRecordingFrames) {
            isRecordingFrames = true;
        } else if(isRecordingFrames && !isPlayingBackFrames){
            isRecordingFrames = false;
            isPlayingBackFrames = true;
            fluidPlayer.stopPerformance();
        }
    }
    if( key == OF_KEY_LEFT)
        fluidPlayer.nextInstrument();
    if( key == OF_KEY_RIGHT)
        fluidPlayer.prevInstrument();
    if( key == OF_KEY_UP)
        increaseNearDepth();
    if( key == OF_KEY_DOWN)
        decreaseNearDepth();
    if( key == OF_KEY_PAGE_UP)
        increaseNearDepth();
    if( key == OF_KEY_PAGE_DOWN)
        decreaseNearDepth();
    if( key == OF_KEY_HOME)
        decreaseThreshold();
    if( key == OF_KEY_END)
        increaseThreshold();
    
    //Forward keypress to kinect
    //fluidKinect.keyPressed(key);
}


void FluidMotionApp::recordHirezSim()
{
    isPlayingBackFrames = true;
    playbackFrame = 0;
}


//--------------------------------------------------------------
void FluidMotionApp::keyReleased(int key){

}

//--------------------------------------------------------------
void FluidMotionApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void FluidMotionApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void FluidMotionApp::mousePressed(int x, int y, int button){
    bMouseHeld = true;
    
    if(button == 0)
        fluidPlayer.nextInstrument();
    else if(button == 2)
        fluidPlayer.prevInstrument();
}

//--------------------------------------------------------------
void FluidMotionApp::mouseReleased(int x, int y, int button){
    
    bMouseHeld = false;


}

//--------------------------------------------------------------
void FluidMotionApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void FluidMotionApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void FluidMotionApp::dragEvent(ofDragInfo dragInfo){ 

}

