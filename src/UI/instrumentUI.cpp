//
//  instrumentGUI.cpp
//   Sonoromancer
//
//  Created by Byron Mallett on 1/03/13.
//
//

#include "InstrumentUI.h"

InstrumentUI::InstrumentUI(float x, float y, float w, float h) : ofxUICanvas(x, y, w, h)
{
    this->setDrawBack(true);
    this->setColorBack(ofColor(20,20,20));
}

void InstrumentUI::initWidgets(FluidInstrument * instrument){
    m_instrument = instrument;
    
    this->addWidgetDown(new ofxUILabel("INSTRUMENT", OFX_UI_FONT_LARGE));
    this->addSpacer(this->rect->width, 2);
    this->addLabel("NAME");
    this->addTextInput("NAME INPUT", instrument->name, this->rect->width)->setAutoClear(false);
    this->addLabel("DEVICE");
    this->addTextInput("DEVICE INPUT", instrument->device, this->rect->width)->setAutoClear(false);
    this->addLabel("PROGRAM");
    this->addTextInput("PROGRAM INPUT", ofToString(instrument->program), this->rect->width)->setAutoClear(false);
    
    ofxUILabelToggle * noteMapping = addLabelToggle("NOTE MAPPINGS", false);
    noteMapping->setLabelText("NOTE MAPPINGS: NOTE");
    if(instrument->noteMapping == INSTRUMENT_PLAYS_NOTES) noteMapping->setValue(true);
    else if(instrument->noteMapping == INSTRUMENT_PLAYS_CC) noteMapping->setValue(false);
    noteMapping->triggerSelf();
    
    ofxUILabelToggle * timbre = addLabelToggle("TIMBRE", false);
    timbre->setLabelText("TIMBRE: POLY");
    if(instrument->timbreType == INSTRUMENT_TYPE_POLYPHONIC) timbre->setValue(true);
    else if(instrument->timbreType == INSTRUMENT_TYPE_MONOPHONIC) timbre->setValue(false);
    timbre->triggerSelf();
    
    this->addSlider("DYE RED", 0.0, 1.0, instrument->dyeColour.x, this->rect->width, 10);
    this->addSlider("DYE GREEN", 0.0, 1.0, instrument->dyeColour.y, this->rect->width, 10);
    this->addSlider("DYE BLUE", 0.0, 1.0, instrument->dyeColour.z, this->rect->width, 10);
    
    vector<string> channelList;
    channelList.push_back("1");
    channelList.push_back("2");
    channelList.push_back("3");
    channelList.push_back("4");
    channelList.push_back("5");
    channelList.push_back("6");
    channelList.push_back("7");
    channelList.push_back("8");
    channelList.push_back("9");
    channelList.push_back("10");
    channelList.push_back("11");
    channelList.push_back("12");
    channelList.push_back("13");
    channelList.push_back("14");
    channelList.push_back("15");
    this->addDropDownList("CHAN", channelList, this->rect->width)->setAllowMultiple(false);

    ofAddListener(this->newGUIEvent,this,&InstrumentUI::guiEvent);
}


void InstrumentUI::guiEvent(ofxUIEventArgs &e){
    if(e.widget->getName() == "CHAN")
    {
        ofxUIDropDownList *dropDown = (ofxUIDropDownList *) e.widget;

        if(dropDown->getSelected().size() > 0){
            dropDown->setLabelText("CHAN: " + dropDown->getSelected()[0]->getName());
            
            ofLog(OF_LOG_NOTICE, dropDown->getSelected()[0]->getName());
        }
    }
    
    else if (e.widget->getName() == "NOTE MAPPINGS"){
        ofxUILabelToggle *button = (ofxUILabelToggle *) e.widget;
        if(button->getValue()){
            button->setLabelText("NOTE MAPPINGS: NOTE");
        } else {
            button->setLabelText("NOTE MAPPINGS: CC");
        }
    }
    
    else if (e.widget->getName() == "TIMBRE"){
        ofxUILabelToggle *button = (ofxUILabelToggle *) e.widget;
        if(button->getValue()){
            button->setLabelText("TIMBRE: POLY");
        } else {
            button->setLabelText("TIMBRE: MONO");
        }
    }
}
