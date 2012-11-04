//
//  FluidNote.cpp
//  FluidMotion
//
//  Created by Byron Mallett on 8/10/12.
//
//

#include "FluidNote.h"

FluidNote::FluidNote()
{
    FluidNote(-1, "default", -1);
}

FluidNote::FluidNote(int noteId, string instrumentName, int type){
    m_type = type;
    m_noteId = noteId;
    m_noteStatus = ON;
    m_value = 0;
    m_preferredValue = 0;
    m_source = 0;
    m_instrument = instrumentName;
};

void FluidNote::setValue(int value){
    m_value = value;
    bNoteChanged = true;
}

FluidNoteGroup::FluidNoteGroup()
{
}

FluidNoteGroup::~FluidNoteGroup()
{
}

FluidNoteGroup::FluidNoteGroup(vector<FluidNote> noteArray)
{
}

void FluidNoteGroup::addNote(FluidNote note)
{
    note.setDirty();
    m_notes.push_back(note);
}