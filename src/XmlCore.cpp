//
//  XmlLoader.cpp
//   Sonoromancer
//
//  Created by Byron Mallett on 2/03/13.
//
//

#include "XmlCore.h"
#include "FluidSim.h"


/*
 * Parse instrument xml
 */
FluidInstrument XmlCore::readInstrumentXML(ofxXmlSettings xmlInstrument){
    xmlInstrument.pushTag("instrument");
    xmlInstrument.pushTag("properties");
    
    //Create a new instrument definition
    FluidInstrument instrument;
    //instrument.setID(i);
    instrument.name = xmlInstrument.getValue("name", "");
    instrument.device = xmlInstrument.getValue("device", "");
    instrument.channel = xmlInstrument.getValue("channel", 0);
    instrument.program = xmlInstrument.getValue("program", 0);
    
    instrument.dyeColour = ofVec3f(xmlInstrument.getValue("colourR",1.0f), xmlInstrument.getValue("colourG",1.0f), xmlInstrument.getValue("colourB",1.0f));
    instrument.usesCCNoteTriggers = ofToBool(xmlInstrument.getValue("usesCCNoteTriggers", ""));
    
    int timbreType, noteMapping;
    
    if(xmlInstrument.getValue("timbre", "") == "mono")
        instrument.timbreType = INSTRUMENT_TYPE_MONOPHONIC;
    else if(xmlInstrument.getValue("timbre", "") == "poly")
        instrument.timbreType = INSTRUMENT_TYPE_POLYPHONIC;
    
    if(xmlInstrument.getValue("noteMappings", "") == "note")
        instrument.noteMapping = INSTRUMENT_PLAYS_NOTES;
    else if(xmlInstrument.getValue("noteMappings", "") == "cc")
        instrument.noteMapping = INSTRUMENT_PLAYS_CC;
    
    xmlInstrument.popTag();
    xmlInstrument.pushTag("parameters");
    
    //Set instrument parameters and mappings -- CC
    int j;
    int numCCParams = xmlInstrument.getNumTags("cc");
    for(j = 0; j < numCCParams; j++)
    {
        xmlInstrument.pushTag("cc", j);
        InstrumentParameter param;
        param.noteType = INSTRUMENT_PLAYS_CC;
        param.channel = xmlInstrument.getValue("channel" , 0);
        param.source = instrument.getParamSourceFromString( xmlInstrument.getValue("source", "") );
        param.value = xmlInstrument.getValue("value" , 0);
        param.lowerNoteRange = xmlInstrument.getValue("min", 0);
        param.upperNoteRange = xmlInstrument.getValue("max", 0);
        instrument.addparam(param);
        instrument.addNoteParam(param, true);
        xmlInstrument.popTag();
    }
    
    
    //Set instrument parameters and mappings -- NOTE
    int numNoteParams = xmlInstrument.getNumTags("note");
    for(j = 0; j < numNoteParams; j++)
    {
        xmlInstrument.pushTag("note", j);
        InstrumentParameter param;
        param.noteType = INSTRUMENT_PLAYS_NOTES;
        param.source = instrument.getParamSourceFromString( xmlInstrument.getValue("source", "") );
        param.lowerNoteRange = xmlInstrument.getValue("min", 0);
        param.upperNoteRange = xmlInstrument.getValue("max", 0);
        instrument.addparam(param);
        instrument.addNoteParam(param, false);
        
        xmlInstrument.popTag();
    }
    
    return instrument;
}




void XmlCore::writeInstrumentXML(FluidInstrument instrument){
    
    ofxXmlSettings xmlInstrument;
    
    xmlInstrument.addTag("instrument");
    xmlInstrument.pushTag("instrument");
    
    xmlInstrument.addTag("properties");
    xmlInstrument.pushTag("properties");
    
    xmlInstrument.addValue("name", instrument.name);
    xmlInstrument.addValue("device", instrument.device);
    xmlInstrument.addValue("channel", instrument.channel);
    xmlInstrument.addValue("program", instrument.program);
    xmlInstrument.addValue("recordableChannel", instrument.recordableChannel);
    
    if(instrument.timbreType == INSTRUMENT_TYPE_MONOPHONIC) xmlInstrument.addValue("timbre", "mono");
    else if(instrument.timbreType == INSTRUMENT_TYPE_POLYPHONIC) xmlInstrument.addValue("timbre", "poly");
    
    if(instrument.noteMapping == INSTRUMENT_PLAYS_NOTES) xmlInstrument.addValue("noteMappings", "note");
    else if(instrument.noteMapping == INSTRUMENT_PLAYS_CC) xmlInstrument.addValue("noteMappings", "cc");
    
    xmlInstrument.addValue("colourR", instrument.dyeColour.x);
    xmlInstrument.addValue("colourG", instrument.dyeColour.y);
    xmlInstrument.addValue("colourB", instrument.dyeColour.z);
    
    xmlInstrument.addValue("usesCCNote", instrument.usesCCNoteTriggers);
    
    xmlInstrument.popTag();
    
    xmlInstrument.addTag("parameters");
    xmlInstrument.pushTag("parameters");
    
    for(int i = 0; i < instrument.params.size(); i++){
        InstrumentParameter param = instrument.params[i];
        
        ofLog(OF_LOG_NOTICE, "--> Saving Instrument: " + instrument.name + ", Param:" + ofToString(param.source));
        
        if(param.noteType == INSTRUMENT_PLAYS_NOTES) {
            xmlInstrument.addTag("note");
            xmlInstrument.pushTag("note");
        } else if(param.noteType == INSTRUMENT_PLAYS_CC){
            xmlInstrument.addTag("cc");
            xmlInstrument.pushTag("cc");
        }
        
        if(param.noteType == INSTRUMENT_PLAYS_CC) xmlInstrument.addValue("channel", param.channel);
        xmlInstrument.addValue("min", param.lowerNoteRange);
        xmlInstrument.addValue("max", param.upperNoteRange);
        xmlInstrument.addValue("source", instrument.getParamStringFromSource( param.source ));
        
        xmlInstrument.popTag();
    }
    
    xmlInstrument.popTag();
    xmlInstrument.saveFile("instruments/" + instrument.name + ".xml");
}




FluidSettings XmlCore::readFluidSettings(ofxXmlSettings fluidSettings){
    FluidSettings settings;
    
    return settings;
}



void XmlCore::writeFluidSettings(FluidSettings settings){
    
}
