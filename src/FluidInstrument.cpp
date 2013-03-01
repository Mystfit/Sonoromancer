//
//  FluidInstrument.cpp
//  FluidMotion
//
//  Created by Byron Mallett on 8/10/12.
//
//

#include "FluidInstrument.h"

FluidInstrument::FluidInstrument(){
    FluidInstrument("Instrument", "passthrough_out", 0, 0, INSTRUMENT_TYPE_MONOPHONIC, INSTRUMENT_PLAYS_CC);
}

FluidInstrument::FluidInstrument(string _name, string _device, int _channel, int _program, int _timbreType, int _noteMapping)
{    
    name = _name;
    device = _device;
    channel = _channel;
    program = _program;
    _timbreType = _timbreType;
    noteMapping = _noteMapping;
    setID(1);
}


void FluidInstrument::addNoteParam(InstrumentParameter param, bool dampable = true)
{
    FluidNote newNote(0, name, param.noteType);
    newNote.setStatus(ON);
    newNote.setSource(param.source);
    newNote.setDirty();
    newNote.isDampable = dampable;
    (param.noteType == INSTRUMENT_PLAYS_CC) ? newNote.setCCchan(param.channel) : newNote.setCCchan(0);
    
    noteParams.push_back(newNote);
}

void FluidInstrument::tickNoteParams()
{
    float  dampening = 0.1f;
    for(int i = 0; i < noteParams.size(); i++)
    {
        if(noteParams[i].isDampable){
            noteParams[i].setValue( ((float)noteParams[i].getValue() + ((float)(noteParams[i].getPreferredValue() - (float)noteParams[i].getValue()) * dampening )));
            ofLog(OF_LOG_NOTICE, "Note Preferred:" + ofToString(noteParams[i].getPreferredValue()) + " Note Actual:" + ofToString(noteParams[i].getValue()  ));
            
            if(noteParams[i].getValue() != noteParams[i].getPreferredValue()){
                noteParams[i].setDirty();
                noteParams[i].setStatus(ON);
            }
        } else {
            noteParams[i].setValue(noteParams[i].getPreferredValue());
            noteParams[i].setDirty();
            noteParams[i].setStatus(ON);
        }
    }
}


vector<int> FluidInstrument::createNotesFromBlobParameters(BlobParam blobParameter)
{
    vector<int> resultValues;
    for(int i = 0; i < params.size(); i++)
    {        
        //Map the note from the source
        float paramValue = blobParamValueFromSource(blobParameter, params[i].source );
        int noteValue = lerpNote(paramValue, params[i].upperNoteRange, params[i].lowerNoteRange);
        
        if(params[i].source == INSTRUMENT_SOURCE_CCNOTEON) noteValue = 127;
        if(params[i].source == INSTRUMENT_SOURCE_CCNOTEOFF) noteValue = 0;
        resultValues.push_back(noteValue);
    }
    
    return resultValues;
}

vector<InstrumentParameter> FluidInstrument::getParametersByTagType(int paramType)
{
    vector<InstrumentParameter> paramList;
    for(int i = 0; i < params.size(); i++)
    {
        if(params[i].noteType == paramType) paramList.push_back(params[i]);
    }
    
    return paramList;
}


float FluidInstrument::blobParamValueFromSource(BlobParam blobParam, int source)
{
    switch(source){
        case INSTRUMENT_SOURCE_BLOBX: return blobParam.position.x; break;
        case INSTRUMENT_SOURCE_BLOBY: return blobParam.position.y; break;
        case INSTRUMENT_SOURCE_CURVATURE: return blobParam.curvature; break;
        case INSTRUMENT_SOURCE_AREA: return blobParam.area; break;
        case INSTRUMENT_SOURCE_INTENSITY: return blobParam.intensity; break;
    }

}



InstrumentParameter FluidInstrument::getParamFromSource(int source)
{
    for(int i = 0; i < params.size(); i++)
        if(params[i].source == source )
            return params[i]; 
}

int FluidInstrument::getNoteParamIndexFromSource(int source)
{
    int index = -1;
    int i = 0;
    while(i < noteParams.size())
    {
        if(noteParams[i].getSource() == source ){
            index = i;
            break;
        }
        i++;
    }
    
    return index;
}



/*
 * Converts string parameter tags to int constants
 */
int FluidInstrument::getParamSourceFromString(string source)
{
    if(source == "blobX")
        return INSTRUMENT_SOURCE_BLOBX;
    else if(source == "blobY")
        return INSTRUMENT_SOURCE_BLOBY;
    else if(source == "curvature")
        return INSTRUMENT_SOURCE_CURVATURE;
    else if(source == "area")
        return INSTRUMENT_SOURCE_AREA;
    else if(source == "intensity")
        return INSTRUMENT_SOURCE_INTENSITY;
    else if(source == "noteOn")
        return INSTRUMENT_SOURCE_CCNOTEON;
    else if(source == "noteOff")
        return INSTRUMENT_SOURCE_CCNOTEOFF;

}


/*
 * Converts parameters constants to original strings
 */
string FluidInstrument::getParamStringFromSource(int source)
{
    ofLog(OF_LOG_NOTICE, "--> Source type: " + ofToString(source));
    if(source == INSTRUMENT_SOURCE_BLOBX)
        return "blobX";
    else if(source == INSTRUMENT_SOURCE_BLOBY)
        return "blobY";
    else if(source == INSTRUMENT_SOURCE_CURVATURE)
        return "curvature";
    else if(source == INSTRUMENT_SOURCE_AREA)
        return "area";
    else if(source == INSTRUMENT_SOURCE_INTENSITY)
        return "intensity";
    else if(source == INSTRUMENT_SOURCE_CCNOTEON)
        return "noteOn";
    else if(source == INSTRUMENT_SOURCE_CCNOTEOFF)
        return "noteOff";
}



