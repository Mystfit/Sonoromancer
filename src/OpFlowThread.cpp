//
//  OpFlowThread.cpp
//   Sonoromancer
//
//  Created by Byron Mallett on 26/02/13.
//
//

#include "OpFlowThread.h"

void OpFlowThread::init(){
    blurImage.allocate(CAMERA_WIDTH, CAMERA_HEIGHT, GL_RGBA);
    maskPixels.allocate(CAMERA_WIDTH, CAMERA_HEIGHT, OF_PIXELS_RGBA);
    
    //Setup optical flow
    opFlow.setup(ofRectangle(0,0, OPFLOW_WIDTH, OPFLOW_HEIGHT ));
    opFlow.setOpticalFlowBlur(15);
    opFlow.setOpticalFlowSize(5);
    
    startThread(true, true);
}

void OpFlowThread::threadedFunction(){
        
    while(isThreadRunning()) {
        
        //Update opflow
        
        if(bKinectFlagged)
        {
            lock();
            
            blurImage.setFromPixels(maskPixels);
            //blurImage.resize(opFlow.sizeSml.width, opFlow.sizeSml.height);
            blurImage.flagImageChanged();
            opFlow.update(blurImage);
            
            bKinectFlagged = false;
            
            unlock();
        }
        
    }
}


void OpFlowThread::setKinectTexture(ofTexture & inputTex){
    flagKinectUpdate();
    kinectTexture = inputTex;
    kinectTexture.readToPixels(maskPixels);
}


