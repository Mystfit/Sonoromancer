//
//  XmlLoader.h
//   Sonoromancer
//
//  Created by Byron Mallett on 2/03/13.
//
//

#ifndef ___Sonoromancer__XmlLoader__
#define ___Sonoromancer__XmlLoader__

#include <iostream>
#include "FluidInstrument.h"
#include "FluidSim.h"
#include "ofxXmlSettings.h"

#endif /* defined(___Sonoromancer__XmlLoader__) */

class XmlCore {
public:    
    static FluidInstrument  readInstrumentXML(ofxXmlSettings xmlInstrument);
    static FluidSettings    readFluidSettings(ofxXmlSettings xmlFluid);
    
    static void writeInstrumentXML(FluidInstrument instrument);
    static void writeFluidSettings(FluidSettings settings);
private:
};