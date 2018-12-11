#pragma once

#include "ofMain.h"
#include "guiScreen.h"
#include "pistaSettings.h"
#include "calSpeed.h"

/// interaction devices
#include "ofxThinkgear.h"
#include "ofxLeapMotion.h"
#include "ofxSerial.h"
#include "ofxSimpleTimer.h"

#include "ofxOSCSync.h"

//#define SERVER

class ofApp : public ofBaseApp{
private:
    int PLAYER_NUM;
    
public:
    void setup();
    void update();
    void draw();
    void exit();
    
    int mapExponencial(float in, float inMin, float inMax, float outMin, float outMax, float shaper);
    
    void keyPressed(int key);
    
    void setScreeSpheresData(float attention, float meditation, float power, float connection, float speedBar);
    

    void addLap();
    void newGame();
    void cuentAtras();
    void finishGame();
    
    
	// fondo fake
	ofImage fondoFake;
    
    // hace que la velocidad sea progresiva
    calSpeed calculaVelocidad;
    
    // gui
    guiScreen pantalla;
    
    
    bool conectado;
    
    // numero de vueltas que llevo
    int laps;
    
    // que index de player ha ganado
    int playerWinner;
    
    // esta la pista habilitada?
    int trackEnabled;
    
    // cual es el estado del juego
    int estadoJuego;
    
    // cuenta atras
    int countdown;
    
    // tiempo desde el ultimo mensaje
    long tiempoDesdeMsg;
    
    // ajustes de velocidad en la pista
    pistaSettings ajustesPista;
    

    
    // interaction device
    string interactionDevice;
    
    
    //////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////
    ofx::IO::SerialDevice arduino;
    
    ofxSimpleTimer timerCountDown;
    
    void timerOneSecond(int &args);
    
    void readSerialData();
    void writeSerialData(int _speed);
    
	//ofSerial arduinoParis;
	bool arduinoConectado;

    /// DEVICE LEAP
    ofxLeapMotion leap;
    vector <Hand> hands;
    
    
    /// DEVICE MINDWAVE
    float poorSignal;
    float attention;
    float meditation;
    float blink;
    
    ofxThinkgear mindWave;
    ofxThinkgearEventArgs mindWaveArgs;
    
    //
    void onThinkgearPoorSignal(ofxThinkgearEventArgs & args);
    void onThinkgearAttention(ofxThinkgearEventArgs & args);
    void onThinkgearMeditation(ofxThinkgearEventArgs & args);
    void onThinkgearBlinkStrength(ofxThinkgearEventArgs & args);
    
    /// OSC SYNC
    
    
#ifdef SERVER
    
    ofxServerOscManager* server;
#else
    ofxClientOSCManager* client;
    
#endif
    int clientPort;
    int uuid;
    int sendPort;
    int receivePort;
    string host;
    ofxCommonTimeOSC* commonTimeOsc;
    void                newData( DataPacket& _packet  );
};
