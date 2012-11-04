//
//  KinectBlender.h
//  FluidMotion
//
//  Created by Byron Mallett on 16/10/12.
//
//

#ifndef __FluidMotion__KinectBlender__
#define __FluidMotion__KinectBlender__

#include "ofMain.h"
#include "ofxFXObject.h"

class KinectBlender : public ofxFXObject{
public:
    
    KinectBlender();
    void init(int w, int h, int outWidth, int outHeight);
    ofFbo * updateBlender(ofTexture & redChan, ofTexture & greenChan, ofTexture & blueChan, ofTexture & mask, float maxDist, float minDist);
    ofFbo * updateKinectMasker(ofTexture & colourCamera, ofTexture & mask, int w, int h);
    
    ofShader blendShader;
    ofShader kinectMaskShader;
    
    ofxSwapBuffer blendBuffer;
    ofxSwapBuffer kinectBuffer;
    
    int blenderWidth, blenderHeight, recordWidth, recordHeight;
};

#endif /* defined(__FluidMotion__KinectBlender__) */
