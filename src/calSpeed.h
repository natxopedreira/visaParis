//
//  calSpeed.h
//  acuraGui09
//
//  Created by Natxo Pedreira gonzalez on 1/12/15.
//
//

#ifndef acuraGui09_calSpeed_h
#define acuraGui09_calSpeed_h

class calSpeed {
    float speed, speedDest;
    
public:
    
    //--------------------------------------------------------------
    calSpeed(){
        speed = 0.0;
        speedDest = 0.0;
    }
    
    //--------------------------------------------------------------
    void update(){
        //if(speedDest-speed<.1) return;
        
        if (speed < speedDest) {
            float dif = speedDest-speed;
            
            speed +=dif*.5;
        }else if(speed > speedDest){
            
            float difMenos = speed-speedDest;
            speed -=difMenos*.05;
        }
    }
    
    //--------------------------------------------------------------
    void setDesiredSpeed(float f){
        speedDest = f;
    }
    
    //--------------------------------------------------------------
    void setTurbo(float f){
        speed = f;
    }
    
    //--------------------------------------------------------------
    float getSpeed(){
        //cout << speed << endl;
        //float f = ofMap(speed,0,100,0,1,true);
        return speed;
    }
};

#endif
