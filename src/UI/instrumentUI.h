//
//  instrumentGUI.h
//   Sonoromancer
//
//  Created by Byron Mallett on 1/03/13.
//
//

#ifndef ___Sonoromancer__instrumentUI__
#define ___Sonoromancer__instrumentUI__

#include <iostream>
#include "ofxUI.h"
#include "FluidInstrument.h"




class InstrumentUI : public ofxUICanvas {
public:
    InstrumentUI(float x, float y, float w, float h);
        
    void initWidgets(FluidInstrument * instrument);

private:
    
    FluidInstrument * m_instrument;
    void guiEvent(ofxUIEventArgs &e);
};

#endif /* defined(___Sonoromancer__instrumentGUI__) */

