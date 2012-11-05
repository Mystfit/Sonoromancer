#Sonoromancer Readme v1

####Things to do in order to run this project.

1. Grab Openframeworks build 0071 (haven't tried building on 0072, will get around to it soon).


2. Clone the repository into openframeworks/apps/myApps(or devApps) with: <pre>git clone http://github.com/Mystfit/Sonoromancer.git</pre> Grab the ofxOpenNI and ofxMidi submodules by running <pre>git submodule init</pre> and <pre>git submodule update</pre>


3. You'll need to make a small change in ofImage.cpp under libs/openframeworks/grpahics. On line 874 replace <pre>type = GL_LUMINANCE;</pre> with <pre>type = GL_LUMINANCE32F_ARB;</pre>


4. At the moment, the program will directly connect to the midi devices "passthrough_in" and "passthrough_out" until I fix this in the instrument XML definitions. I recommend using MIDI Patchbay (http://notahat.com/midi_patchbay) to create virtual midi in/out ports with these names and use them to route connections to your MIDI device of choice. I'm sending mine to Ableton Live for example.


5. Install libusb for the Kinect drivers. I recommend using Homebrew (http://mxcl.github.com/homebrew/) with the command <pre>brew install libusb --universal</pre>


6. Unless I've forgotten anything, you should be able to build the Sonoromancer Xcode project now. Make sure your Kinect is plugged in before launching of course.


Special thanks goes to Patricio Gonzalez Vivo (http://www.patriciogonzalezvivo.com/) for his brilliant ofxFX addon and GPU fluid code that made this project possible. Check it out at https://github.com/patriciogonzalezvivo/ofxFX

Props also go to the team behind ofxMidi (https://github.com/chrisoshea/ofxMidi).