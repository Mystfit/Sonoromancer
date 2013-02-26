//
//  FluidSimulation.h
//   Sonoromancer
//
//  Created by Byron Mallett on 2/11/12.
//
//

#ifndef ___Sonoromancer__FluidSim__
#define ___Sonoromancer__FluidSim__


/*
 *  FluidSim
 *
 *  Created by Patricio González Vivo on 9/29/11.
 *  Copyright 2011 http://PatricioGonzalezVivo.com All rights reserved.
 *
 *  Created ussing:
 *
 *    - Mark Harris article from GPU Gems 1
 *      http://http.developer.nvidia.com/GPUGems/gpugems_ch38.html
 *
 *    - Phil Rideout
 *      http://prideout.net/blog/?p=58
 *
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the author nor the names of its contributors
 *       may be used to endorse or promote products derived from this software
 *       without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 *  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 *  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 *  OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 *  OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 *  OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 *  OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *  ************************************************************************************
 *
 *  tex0 -> obstacles
 *
 */


#include "ofMain.h"
#include "ofxFXObject.h"

typedef struct  {
    ofVec3f color;
    ofVec2f pos;
    ofVec2f vel;
    float   rad;
    float   temp;
    float   den;
} pForce;


class FluidSim : public ofxFXObject, public ofThread {
public:
    FluidSim();
    
    void allocate(int _width, int _height, float _scale = 0.5, float velocityScale = 1.0f);
    
    //FluidSim&   setDensity(ofTexture & _tex){setTextureToBuffer(_tex, densityBuffer); return * this; };
    FluidSim&   setTexture(ofTexture & _tex){setTextureToBuffer(_tex, pingPong); return * this; };
    FluidSim&   setExternalVelocity(ofTexture & _tex){setTextureToBuffer(_tex, externalVelocityBuffer); return * this; };
    FluidSim&   setUserMasking(ofTexture & _tex){setTextureToBuffer(_tex, usersMaskBuffer); return * this; };
    FluidSim&   setVelocity(ofTexture & _tex){setTextureToBuffer(_tex, velocityBuffer); return * this; };
    FluidSim&   setTemperature(ofTexture & _tex){setTextureToBuffer(_tex, temperatureBuffer); return * this; };
    FluidSim&   setGravity(ofPoint _force){ gForce = _force; return * this;};
    
    FluidSim&   setDissipation(float _diss){pingPong.diss = _diss; return * this;};
    FluidSim&   setVelocityDissipation(float _diss){velocityBuffer.diss = _diss; return * this;};
    FluidSim&   setTemperatureDissipation(float _diss){temperatureBuffer.diss = _diss; return * this;};
    
    ofxSwapBuffer getVelocityBuffer(){ return velocityBuffer; };
    
    void setDyeColour(ofVec3f colour){dyeColour = colour; };
    
    void    addTemporalForce(ofPoint _pos, ofPoint _dir, ofFloatColor _col, float _rad = 1.0f, float _temp = 10.f, float _den = 1.f );
    void    addConstantForce(ofPoint _pos, ofPoint _dir, ofFloatColor _col, float _rad = 1.0f, float _temp = 10.f, float _den = 1.f );
    
    void    update();
    void    draw(int x = 0, int y = 0, float _width = -1, float _height = -1);
    void    drawVelocity();
    void    drawPressure();
    void    drawTemperature();
    void    drawInputVectors();
    
    bool bIsAddingDye;
    bool bDrawVelocity;
    bool bDrawPressure;
    bool bDrawTemperature;
    bool bDrawInputVectors;
    
private:
    void    setTextureToBuffer(ofTexture & _tex, ofxSwapBuffer & _buffer);
    
    void    advect(ofxSwapBuffer& _buffer);
    void    jacobi();
    void    subtractGradient();
    void    computeDivergence();
    
    void    applyImpulse(ofxSwapBuffer& _buffer, ofPoint _force, ofPoint _value, float _radio = 3.f);
    void    applyExternalVelocity(ofxSwapBuffer& _buffer, ofTexture velocityTex);
    void    applyExternalDye(ofxSwapBuffer& _buffer, ofTexture depthTex,  ofVec3f colourMult);
    
    void    applyBuoyancy();
    
    ofShader jacobiShader;
    ofShader subtractGradientShader;
    ofShader computeDivergenceShader;
    ofShader applyImpulseShader;
    ofShader applyExternalVelocityShader;
    ofShader applyExternalDyeShader;
    
    ofShader applyBuoyancyShader;
    
    ofxSwapBuffer velocityBuffer;
    ofxSwapBuffer externalVelocityBuffer;
    ofxSwapBuffer usersMaskBuffer;
    
    ofxSwapBuffer temperatureBuffer;
    ofxSwapBuffer pressureBuffer;
    
    ofFbo   divergenceFbo;
    ofFbo   obstaclesFbo;
    
    vector<pForce> constantForces;
    vector<pForce> temporalForces;
    ofPoint gForce;
    
    float   smokeBuoyancy;
    float   smokeWeight;
    float   gradientScale;
    float   ambientTemperature;
    
    ofVec3f dyeColour;
    
    float   gridWidth,gridHeight;
    float   timeStep;
    float   cellSize;
    float   scale;
    
    int     numJacobiIterations;
    
    
};
#endif /* defined(___Sonoromancer__FluidSim__) */


