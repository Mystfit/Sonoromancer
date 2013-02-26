//
//  OpFlowThread.h
//   Sonoromancer
//
//  Created by Byron Mallett on 26/02/13.
//
//

#ifndef ___Sonoromancer__OpFlowThread__
#define ___Sonoromancer__OpFlowThread__

#include <iostream>
#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxOpticalFlowLK.h"
#include "FluidKinect.h"

class OpFlowThread : public ofThread {
    
public:
    
    // the thread function
    void init();
    void threadedFunction();
    
    ofTexture & getVelXTex(){ return opFlow.velTexX.getTextureReference(); };
    ofTexture & getVelYTex(){return opFlow.velTexY.getTextureReference(); };
    
    void setKinectTexture(ofTexture & inputTex);
        
    ofFloatImage velTexX, velTexY;
    
    void flagKinectUpdate() { bKinectFlagged = true; };
    
private:
    ofxCvColorImage blurImage;
    ofPixels maskPixels;
    ofxOpticalFlowLK opFlow;
    ofTexture kinectTexture;
    
    bool bKinectFlagged;
};

#endif /* defined(___Sonoromancer__OpFlowThread__) */
