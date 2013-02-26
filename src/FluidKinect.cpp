//
//  FluidKinect.cpp
//  FluidMotion
//
//  Created by Byron Mallett on 10/10/12.
//
//

#include "FluidKinect.h"


FluidKinect::FluidKinect()
{
    
	isLive			= true;
	isTracking		= true;
	isFiltering		= false;
	isMasking		= true;
    
	nearThreshold = 500;
	farThreshold  = 1000;
    
	filterFactor = 0.1f;
    
    opticalBlur = 17;
    
    init();
}

void FluidKinect::init()
{

    
    
#if defined (TARGET_OSX) //|| defined(TARGET_LINUX) // only working on Mac/Linux at the moment (but on Linux you need to run as sudo...)
	hardware.setup();				// libusb direct control of motor, LED and accelerometers
#endif
    
    if(hardware.getDeviceConnectionStatus()){
        hardware.setLedOption(LED_GREEN); // turn off the led just for yacks (or for live installation/performances ;-)

        recordContext.setup();	// all nodes created by code -> NOT using the xml config file at all
        //recordContext.setupUsingXMLFile();
        recordDepth.setup(&recordContext);
        recordImage.setup(&recordContext);
        
        recordDepth.setDepthColoring(COLORING_GREY);

        recordUser.setup(&recordContext);
        recordUser.setSmoothing(filterFactor);				// built in openni skeleton smoothing...
        recordUser.setUseMaskPixels(isMasking);
        recordUser.setMaxNumberOfUsers(5);					// use this to set dynamic max number of users (NB: that a hard upper limit is defined by MAX_NUMBER_USERS in ofxUserGenerator)
        recordContext.toggleRegisterViewport();
        recordContext.toggleMirror();
        
        isDeviceConnected = true;
    } else {
        ofLog(OF_LOG_ERROR, "---> No Kinect detected!");    //Replace with GUI warning
    }
    
    cameraImage.allocate(CAMERA_WIDTH, CAMERA_HEIGHT);
    cameraDepthImage.allocate(CAMERA_WIDTH, CAMERA_HEIGHT);
    
    maskImage.allocate(CAMERA_WIDTH, CAMERA_HEIGHT);
    
    depthPixels.allocate(CAMERA_WIDTH, CAMERA_HEIGHT, OF_PIXELS_RGB);
    maskTexture.allocate(CAMERA_WIDTH, CAMERA_HEIGHT , GL_LUMINANCE);
    
   }

void FluidKinect::update()
{
    hardware.update();
    if (isLive && isDeviceConnected) {
        
        // update all nodes
		recordContext.update();
        recordDepth.update();
        recordImage.update();
        
        // update tracking/recording nodes
		if (isTracking){
            recordUser.update();
            hardware.setLedOption(LED_BLINK_RED_YELLOW);
        }
        
		if (isTracking && isMasking)
              maskTexture.loadData(recordUser.getUserPixels(),recordUser.getWidth(), recordUser.getHeight(), GL_LUMINANCE);
        
    }
}



void FluidKinect::draw()
{
    ofPushStyle();
    //recordDepth.draw(0,0,320,240);
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_DST_COLOR);
    //blurImage.draw(ofGetScreenWidth() - blurImage.width,0);
    //opFlow.draw(ofGetScreenWidth() - OPFLOW_WIDTH, 0, OPFLOW_WIDTH, OPFLOW_HEIGHT);
    glDisable(GL_BLEND);
    
    if (isTracking){
        recordUser.draw();
    }
    
    glPushMatrix();
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ZERO);
    allUserMasks.draw(CAMERA_WIDTH, 0, CAMERA_WIDTH, CAMERA_HEIGHT);
    glDisable(GL_BLEND);
    glPopMatrix();
    ofPopStyle();

}


void FluidKinect::keyPressed(int key){
    
	float smooth;
    
	switch (key) {
#ifdef TARGET_OSX // only working on Mac at the moment
		case 357: // up key
			hardware.setTiltAngle(hardware.tilt_angle++);
			break;
		case 359: // down key
			hardware.setTiltAngle(hardware.tilt_angle--);
			break;
#endif
		case 't':
		case 'T':
			isTracking = !isTracking;
			break;
        case 'm':
		case 'M':
			isMasking = !isMasking;
			recordUser.setUseMaskPixels(isMasking);
			break;
		case '9':
		case '(':
			smooth = recordUser.getSmoothing();
			if (smooth - 0.1f > 0.0f) {
				recordUser.setSmoothing(smooth - 0.1f);
			}
			break;
		case '0':
		case ')':
			smooth = recordUser.getSmoothing();
			if (smooth + 0.1f <= 1.0f) {
				recordUser.setSmoothing(smooth + 0.1f);
			}
			break;
		case '[':
            //case '{':
			if (filterFactor - 0.1f > 0.0f) {
				filterFactor = filterFactor - 0.1f;
            }
			break;
		case '>':
		case '.':
			farThreshold += 50;
			if (farThreshold > recordDepth.getMaxDepth()) farThreshold = recordDepth.getMaxDepth();
			break;
		case '<':
		case ',':
			farThreshold -= 50;
			if (farThreshold < 0) farThreshold = 0;
			break;
            
		case '+':
		case '=':
			nearThreshold += 50;
			if (nearThreshold > recordDepth.getMaxDepth()) nearThreshold = recordDepth.getMaxDepth();
			break;
            
		case '-':
		case '_':
			nearThreshold -= 50;
			if (nearThreshold < 0) nearThreshold = 0;
			break;
		case 'r':
			recordContext.toggleRegisterViewport();
			break;
		default:
			break;
	}
}
