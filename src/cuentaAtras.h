//
//  cuentaAtras.h
//  accuraGUI
//
//  Created by Natxo Pedreira gonzalez on 28/11/15.
//
//

#ifndef accuraGUI_cuentaAtras_h
#define accuraGUI_cuentaAtras_h

#include "txtCountDown.h"
#include "ofxBlur.h"

class cuentaAtras {
private:
    //ofxBlur blur;
    //ofFbo areaTexto;
    txtCountDown speedText;
    int data;
    
    ofSoundPlayer cuentaAtrasSound, cuentaAtrasUltimoSound;
    
public:
    
    void setup(){
        // texto
        speedText.setup(540);
        speedText.setColor(ofColor(77,11,4));

        // filtro blur
        // void setup(int width, int height, int radius = 32, float shape = .2, int passes = 1, float downsample = .5);
        //blur.setup(320, 560, 2, .12, 1);
            
        // fbo para el blur del texto
        //areaTexto.allocate(320,560);
        //areaTexto.begin();
        //ofClear(130,255,222);
        //areaTexto.end();
        
        cuentaAtrasSound.load("beepCuentrAtras.mp3");
        cuentaAtrasSound.setVolume(.8);
        cuentaAtrasUltimoSound.load("beepCuentrAtrasUltimo.mp3");
    }
    
    void setData(int d){
        if(data!=d){
            data = d;
            if (data==0) {
                cuentaAtrasUltimoSound.play();
                //cout << countdown << endl;
            }else{
                cuentaAtrasSound.play();
            }
        }
        
        

    }
    
    void update(){
        // texto con la velocidad
        speedText.setCountDownNumber(data);
        speedText.update();
        

        //areaTexto.begin();
        //ofClear(130,255,222);
        //speedText.drawBlur();
        
        //areaTexto.end();

    }
    
    void draw(){
        ofPushMatrix();
        ofTranslate(-160, -250);
        speedText.draw();
        speedText.drawBlur();
        //drawVelocidadTextBlur();
        ofPopMatrix();
    }
    //--------------------------------------------------------------
   /*
    void drawVelocidadTextBlur(){
        
        speedText.draw();
        
        speedText.drawWhite();
        
        blur.begin();
        ofClear(130,255,222);
        ofEnableAlphaBlending();
        areaTexto.draw(0, 0);
        ofDisableAlphaBlending();
        blur.end();
        
        ofEnableAlphaBlending();
        blur.draw();
        
    }
    */
};

#endif
