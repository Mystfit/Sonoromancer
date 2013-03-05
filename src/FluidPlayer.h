//
//  FluidPlayer.h
//  FluidMotion
//
//  Created by Byron Mallett on 9/10/12.
//
//

#ifndef __FluidMotion__FluidPlayer__
#define __FluidMotion__FluidPlayer__

#define DEMISEMIQUAVER 3
#define SEMIQUAVER 6
#define QUAVER 12
#define CROTCHET 24

#define MIDI_ALL_NOTES_OFF 123

#include <iostream>
#include "ofxMidi.h"
#include "ofUtils.h"
#include "ofxCvBlobFinder.h"
#include "FluidInstrument.h"
#include "ofxCvComplexBlob.h"
#include "XmlCore.h"

class FluidPlayer : public ofxMidiListener {
public:
    FluidPlayer();
    
    //Updaters
    void musicTick();
    void update();
    void updateNotes();
    void sendNotes();
    void sendCC();
    void newMidiMessage(ofxMidiMessage& eventArgs);
    
    
    // Instrument controls
    void setMidiDevice(string device, int target);
    
    enum midiDirection{ MIDI_IN = 0, MIDI_OUT };

    void loadInstruments();
    void saveInstruments();
    
    FluidInstrument readInstrumentXML(ofxXmlSettings xmlInstrument);
    void writeInstrumentXML(FluidInstrument instrument);
    
    void setInstrument(FluidInstrument * instrument);
    FluidInstrument * getInstrumentByName(string name);
    int getInstrumentIndex(FluidInstrument * instrument);
    
    FluidInstrument * getActiveInstrument(){return m_activeInstrument;};
    void nextInstrument();
    void prevInstrument();
    
    
    //Scale controls
    void loadScales();
    ScaleDef getScaleByName(string name);
    void setScale(ScaleDef scale){m_activeScale = scale;};
    
    void setRootNote(int note){ m_rootNote = note; };
    int getRootNote(){return m_rootNote; };

    
    //Performance controls
    void startPerformance();
    void stopPerformance();
        
    float getBpm(){ return bpm; };
    bool isBeat, isBar, isPlaying;
    
    bool isBeatDirty(){return beatDirty; };               //Check if beat is dirty
    void setBeatClean(){beatDirty = false; };      //Mark beat as fresh (phat beats yo)
    void clearAllNotes(){bClearAllNotes = true;};
    
    ofxMidiIn getMidiIn(){return midiIn;};
    ofxMidiOut getMidiOut(){return midiOut;};

    //Blob references
    int getBlobIndexFromNoteId(int blobId);
    void blobsToNotes(vector<BlobParam> blobParameters);
    
private:
    FluidInstrument * m_activeInstrument;
    vector<FluidInstrument * > instrumentList;
    vector<ScaleDef> scaleList;
    int currentInstrumentIndex;
    
    int m_rootNote;
    ScaleDef m_activeScale;
    
    //Blobs
    vector<BlobParam> _blobParams;
    
        
    //MIDI
    ofxMidiIn midiIn;
    ofxMidiOut midiOut;
    bool bIsPlaying;
    bool bClearAllNotes;
    
    //Clock parameters
    int clockPastTime, clockTick, clockPeriod, beatCount, firstBeatMs, clockPeriodValue, BITPERMIN;
    float bpm;
   
    ofxMidiMessage lastMessage;
    
    //Beat parameters
    int baseNoteMessageLength, lowerTimesig, upperTimesig;
    bool beatDirty;
    
};

#endif /* defined(__FluidMotion__FluidPlayer__) */
