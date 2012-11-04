#Sonoromancer Readme v1

####Things to do in order to run this project.

1. Grab Openframeworks build 0071 (haven't tried building on 0072 building, will get around to it soon)

2. You'll need to make a small change in ofImage.cpp under libs/openframeworks/grpahics. On line 874 replace "type = GL_SOMETHINGSOMETHINGCANTREMEMBER;" with "type = GL_LUMINANCE32F_ARB;"

3. At the moment, the program will directly connect to the midi devices "passthrough_in" and ",passthrough_out" until I fix this in the instrument XML definitions. I recommend using MIDI Patchbay (http://notahat.com/midi_patchbay) to create virtual midi in/out ports with these names and use them to route connections to your MIDI device of choice. I'm sending mine to Ableton Live for example.

4. Unless I've forgotten anything, you should be able to build the Sonoromancer Xcode project now. Make sure your Kinect is plugged in before launching of course.


Special thanks to Patricio Gonzalez Vivo (http://www.patriciogonzalezvivo.com/) for his brilliant ofxFX addon and GPU fluid code that made this project possible. Check it out at https://github.com/patriciogonzalezvivo/ofxFX