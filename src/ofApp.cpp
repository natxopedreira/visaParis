#include "ofApp.h"
/*
con respecto a la version de laracha aqui se han cambiado las imagenes de los slices y los ladrillos por punteros para evitar car
gar varias veces la misma imagen

*/

//--------------------------------------------------------------
void ofApp::setup(){
    //
    ofHideCursor();
    
    ofSetLogLevel(OF_LOG_ERROR);
    
    
    ofBackground(0, 0, 0);
    
    estadoJuego=laps=playerWinner=trackEnabled=countdown=0;
    
    // quitamos el blur al texto de conecion
    pantalla.barraConnection.setBlur(false);
    
    // blur para las esferas
    pantalla.esferaVelocidad.setBlur(true);
    pantalla.esferaMeditacion.setBlur(false);
    pantalla.esferaAtencion.setBlur(false);
    
    // textos para mostrar
    pantalla.setMeditacionTxt("MEDITATION");
    pantalla.setAtencionTxt("ATENTION");
    pantalla.setBarraConectionTxt("CONEXION");
    
    // inicamos el skin
    pantalla.setup();
    
    
    // iniciamos el gui con los settings
    ajustesPista.setup();
    ajustesPista.setVisible(false);
    
    ///////////////////////////////////
    /////// INTERACTION DEVICE ////////
    ///////////////////////////////////
    
    //// **************************** ////
    ////// ***** LEAP MOTION ***** //////
    
    poorSignal = 200;
    attention = meditation = 0;
    
    if (ajustesPista.getInteractionMode() == "INTERACTION MINDWAVE"){
        mindWave.setup("/dev/tty.MindWaveMobile-DevA", 57600, TG_COMMS_DRIVER);
        mindWave.addEventListener(this);
    }
    
    //// ************************ ////
    ////// ***** MINDWAVE ***** //////
    
    if (ajustesPista.getInteractionMode() == "INTERACTION LEAP MOTION"){
        /// leap motion
        leap.open();
        
        pantalla.setMeditacionTxt("POSITION Z");
        pantalla.setAtencionTxt("POSITION X");
        pantalla.setBarraConectionTxt("HAND");
    }
    
    
    //////
    /// dame el index del vector
    PLAYER_NUM = ajustesPista.getPlayerIndex();
    
    /////
   
	fondoFake.loadImage("pantallaFake.png");


	//conectamos al arduino
	try {
		arduinoConectado = arduino.setup("/dev/tty.usbmodem1411", 9600);
        ajustesPista.setPistaConectada(arduinoConectado);
	}
	catch (exception& e) {
		ofLogError("imposible conectarse al arduinos");
	}

    timerCountDown.setup(1000);
    ofAddListener( timerCountDown.TIMER_COMPLETE , this, &ofApp::timerOneSecond ) ;
    
    
    /// OSC SYNC
    
    ofxXmlSettings XML;
    bool loadedFile = XML.loadFile( "sync-settings.xml" );
    if( loadedFile )
    {
        sendPort = XML.getValue("settings:server:send", 7778);
        receivePort = XML.getValue("settings:server:receive", 7777);
        host = XML.getValue("settings:server:host", "127.0.0.1");
        clientPort = XML.getValue("settings:client:receive", 7778);
        uuid = XML.getValue("settings:client:uuid", 0);
        
        ajustesPista.setOscId(ofToString(uuid));
        ajustesPista.setPlayerId(ofToString(PLAYER_NUM));
    }
    
    
#ifdef SERVER
    
    server = new ofxServerOscManager();
    server->init(host, sendPort, receivePort);
    
    ofAddListener( server->newDataEvent, this, &ofApp::newData );
#else
    client = new ofxClientOSCManager();
    client->init( uuid, clientPort);
    
    commonTimeOsc = client->getCommonTimeOscObj();
    commonTimeOsc->setEaseOffset( true );
    
    ofAddListener( client->newDataEvent, this, &ofApp::newData );
#endif
    
    
    
}

//--------------------------------------------------------------
void ofApp::update(){
#ifdef SERVER
    timerCountDown.update();
    
    if (server->isInitialised() && !ajustesPista.getOscStatus()) {
        ajustesPista.setOscStatus(server->isInitialised());
    }
#else
    timerCountDown.update(commonTimeOsc->getTimeMillis());
    
    if (client->isReady() && !ajustesPista.getOscStatus()) {
        ajustesPista.setOscStatus(client->isReady());
    }
    
#endif
    
    
    readSerialData();
    
    
    
    
    float finalSpeed = 0.0;
    
    //interaction device
    if (interactionDevice != ajustesPista.getInteractionMode()) {
        interactionDevice = ajustesPista.getInteractionMode();
    }
    
    
    //// ************************ ////
    ////// ***** MINDWAVE ***** //////
    
    if (interactionDevice == "INTERACTION MINDWAVE") {
        // estamos usando el mindwaves
        mindWave.update();
        
        // si la conexion esta ook
        if(poorSignal == 1){
            
            // mira si quieres usar un valor solo o los dos juntos
            if(ajustesPista.getAttentionPlusMeditation()){
                float v = (attention+meditation)*.5;
                calculaVelocidad.setDesiredSpeed(v);
            }else{
                calculaVelocidad.setDesiredSpeed(attention);
                
                ///cout << "setDesiredSpeed " << attention << endl;
            }
            
        }else{
            calculaVelocidad.setDesiredSpeed(0);
            
            attention = 0;
            meditation = 0;
        }
        
        // actualiza el calculo de velocidad
        calculaVelocidad.update();
        
        // setea el valor que envias a las esferas de velocidad por el calculo de velocidad
        finalSpeed = calculaVelocidad.getSpeed();
        //finalSpeed = attention;
    }
    
    
    
    //// **************************** ////
    ////// ***** LEAP MOTION ***** //////
    
    if (interactionDevice == "INTERACTION LEAP MOTION") {
        // usamos el leap
        
        hands = leap.getLeapHands();
        leap.markFrameAsOld();
        
        //
        if(hands.size() > 0){
            int posX = hands.at(0).palmPosition().x;
            int posY = hands.at(0).palmPosition().y;
            int posZ = hands.at(0).palmPosition().z;
            
            finalSpeed = ofMap(posY, ajustesPista.getLeapMin(), ajustesPista.getLeapMax(), 1, 0, true);
            
            attention = ofMap(posX, -100, 100, 0, 1, true);
            meditation = ofMap(posZ, -100, 100, 0, 1, true);
            
            poorSignal = 1;
        }else{
            poorSignal = 0;
        }
    }
    
    
    
    

    // panel de debug enviar data
    if (ajustesPista.getSendData()) {
        finalSpeed = ajustesPista.getSpeedNorm();
    }
    
    
    
    // mandamos los datos para las esferas
    setScreeSpheresData(attention, meditation, finalSpeed, poorSignal, finalSpeed);

    
    // update el skin
    pantalla.update();
    
    
    
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////
    /// enviamos el dato de velocidad a la pista
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////
    

	int velocidadPlayerPista = mapExponencial(finalSpeed, 0, 1, ajustesPista.getVelocidadMinima(), ajustesPista.getVelocidadMaxima(), ajustesPista.getCurva());
	
    if(!ajustesPista.getPistaActivada()){
        velocidadPlayerPista = 0;
    }
    
   // cout << velocidadPlayerPista << endl;
    writeSerialData(velocidadPlayerPista);
    /*
    if (arduinoConectado) {

		try {
			unsigned char speedCmdChar = (unsigned char)velocidadPlayerPista;
			arduino.writeByte(speedCmdChar);

		}
		catch (exception& e) {
			ofLogError("imposible envidar data al bluetooos");
		}

	}*/

    //cout << velocidadPlayerPista << endl;
}
//--------------------------------------------------------------
void ofApp::draw() {
    ofSetColor(255,255, 255, 255);
	fondoFake.draw(0, 0, 1920,1080);

	ofPushMatrix();
	ofTranslate(0,0);
    if (interactionDevice == "INTERACTION LEAP MOTION") {
        pantalla.draw(true);
    }else{
        pantalla.draw(false);
    }
	
	ofPopMatrix();
    
#ifdef SERVER
   
    //cout << ajustesPista.getOscStatus() << endl;
    if (!ajustesPista.getOscStatus()) {
        ofDrawBitmapStringHighlight("ERROR: SERVIDOR OSC DESCONECTADO", 200, 200);
    }
#else
    
    if (!ajustesPista.getOscStatus()) {
        ofDrawBitmapStringHighlight("ERROR: CLIENTE OSC DESCONECTADO", 200, 200);
    }
    
#endif
    
    if (!ajustesPista.getPistaConectada()) {
        ofDrawBitmapStringHighlight("ERROR: ARDUINO PISTA NO CONECTADO", 200, 250);
    }
    
	//ofDrawBitmapString(ofToString(ofGetFrameRate()), 50, 50);
}
//--------------------------------------------------------------
//--------------------------------------------------------------
int ofApp::mapExponencial(float in, float inMin, float inMax, float outMin, float outMax, float shaper){
    float pct = ofMap(in, inMin, inMax, 0, 1, true);
    pct = powf(pct, shaper);
    
    int out = ofMap(pct, 0, 1, outMin, outMax, true);
    return out;
}


//// MINDWAVES CALLBACK
//--------------------------------------------------------------
void ofApp::onThinkgearPoorSignal(ofxThinkgearEventArgs & args){
    int f = (int) args.poorSignal;
    poorSignal = ofMap(f, 200, 0, 0, 1, true);
}
//--------------------------------------------------------------
void ofApp::onThinkgearAttention(ofxThinkgearEventArgs & args){
    int a = (int) args.attention;
    attention = ofMap(a, 0, 100, 0, 1, true);
}
//--------------------------------------------------------------
void ofApp::onThinkgearMeditation(ofxThinkgearEventArgs & args){
    int m = (int) args.meditation;
    meditation = ofMap(m, 0, 100, 0, 1, true);
}
//--------------------------------------------------------------
void ofApp::onThinkgearBlinkStrength(ofxThinkgearEventArgs & args){
    int b = (int) args.blinkStrength;
    
    blink = ofMap(b, 0, 255, 0.7, 1.0, true);
    
    
   // cout << blink << blink << endl;
   if(poorSignal==1.0) calculaVelocidad.setTurbo(1.0);
}


//--------------------------------------------------------------
void ofApp::setScreeSpheresData(float attention, float meditation, float power, float connection, float speedBar){
    // nivel de atencion
    pantalla.setAtencionPower(attention);
    
    // nivel de power // si es solo atencion o solo meditacion o ambos
    pantalla.setBarraPower(power);
    
    // calidad de la conexion
    pantalla.setConectionPower(connection);
    
    // nivel de meditacion
    pantalla.setMeditacionPower(meditation);
    
    // nivel de velocidad = power
    pantalla.setVelocidadPower(speedBar);
}



//--------------------------------------------------------------
void ofApp::writeSerialData(int _speed){
    if (arduinoConectado) {
        
        try {
            /// hay que comprobar que esta dentro de 0-254
            int d = (int)ofClamp(_speed, 0, 254);
            
            /// escribimos la velocidad
            arduino.writeByte(d);
            arduino.writeByte(255);
            
        } catch (exception& e) {
            ofLogError("ofApp::arduino write ") << e.what();
        
        }

    }
}

//--------------------------------------------------------------
void ofApp::readSerialData(){

    try {
        uint8_t buff[1];
        while (arduino.available()>0) {
            
            // ta el buffer listo
            arduino.readBytes(buff,1);
            
            if (buff[0] == 'L') {
                // new lap
                addLap();
                
            }else if (buff[0] == 'X'){
                //// error en la pista
                cout << "error en el shield del motor" << endl;
            
            }
            
            
            
        }
        
    } catch (const std::exception& exc) {
        ofLogError("ofApp::arduino read ") << exc.what();
    }

}
//--------------e-----------------------------------------------
void ofApp::exit(){

}
//--------------e-----------------------------------------------
void ofApp::addLap(){
    pantalla.addLap();
    //cout << "new lap" << endl;
    
    // miramos cuantas llevamos
    if(pantalla.getnumLaps() > 9){
        /// se acabo
        finishGame();
    }
}
//--------------e-----------------------------------------------
void ofApp::newGame(){
    
#ifdef SERVER
    DataPacket data;
    data.valuesString.push_back("NEWGAME");
    
    server->sendData(data);
#endif
    
    estadoJuego = NEWGAME;
    
    /// valor para el gui
    ajustesPista.setGameStatus(NEWGAME);
    
    // valor para la pantalla
    pantalla.setGameStatus(NEWGAME);
    
    // iniciamos la partida
    pantalla.newGame();
    
    ajustesPista.setPistaActivada(false);
    
    
    
}
//--------------e-----------------------------------------------
void ofApp::cuentAtras(){
#ifdef SERVER
    timerCountDown.start(false);
    
    DataPacket data;
    data.valuesString.push_back("COUNTDOWN");
    
    server->sendData(data);
#else
    timerCountDown.start(false, false, commonTimeOsc->getTimeMillis());
#endif
    
    
    // valor para el gui
    ajustesPista.setGameStatus(COUNTDOWN);
    
    // valor para la pantalla
    pantalla.setGameStatus(COUNTDOWN);
    
    countdown = 0;
    
    pantalla.countDown.setData(5 - countdown);
    
    ajustesPista.setPistaActivada(false);
}
//--------------e-----------------------------------------------
void ofApp::finishGame(){
    //cout << "finish partida" << endl;
    
    /// valor para el gui
    ajustesPista.setGameStatus(FINISHED);
    
    // valor para la pantalla
    pantalla.setGameStatus(FINISHED);
    
    // terminamos la partida y ganamos
    pantalla.finishGame(true);
    
    // desactivamos la pista
    ajustesPista.setPistaActivada(false);
    
    // datos del ganador
    DataPacket data;
    data.valuesInt.push_back(PLAYER_NUM);
    data.valuesString.push_back("FINISHED");
    
#ifdef SERVER
    //timerCountDown.start(false);
    server->sendData(data);
    server->sendData(data);
#else
    //timerCountDown.start(false, false, commonTimeOsc->getTimeMillis());
    
    client->sendData(data);
#endif
    
    pantalla.stopCrono();
    
}


//--------------------------------------------------------------
void ofApp::timerOneSecond(int &args){
    countdown++;
    pantalla.countDown.setData(5 - countdown);
    
    if(countdown<5){
#ifdef SERVER
        timerCountDown.start(false);
#else
        timerCountDown.start(false, false, commonTimeOsc->getTimeMillis());
#endif
        
    }else if(countdown == 5){
        /// valor para el gui
        ajustesPista.setGameStatus(RUNNING);
        
        // valor para la pantalla
        pantalla.setGameStatus(RUNNING);
        
        ajustesPista.setPistaActivada(true);
        
        pantalla.startCrono();
    }
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 'f') {
        ofToggleFullscreen();
    }
    if (key == 'd') {
        ajustesPista.setVisible(!ajustesPista.getVisible());
        if(ajustesPista.getVisible()){
            ofShowCursor();
        }else{
            ofHideCursor();
        }
    }
    
#ifdef SERVER
    if(key == 'L'){
        DataPacket data;
        data.valuesInt.push_back(1);
        data.valuesString.push_back("GAMEMODE");
        
        server->sendData(data);
        
        
        ajustesPista.setInteractionMode(1);
        
    }
    if(key == 'M'){
        DataPacket data;
        data.valuesInt.push_back(0);
        data.valuesString.push_back("GAMEMODE");
        
        server->sendData(data);
        
        ajustesPista.setInteractionMode(0);
        
    }
#endif
    
    // debug del juego
    if (key == 'l') {
        addLap();
    }
    
    if(key == 'n'){
        newGame();
    }
    
    if(key == 'g'){
        cuentAtras();
    }
    
}

//--------------------------------------------------------------
void ofApp::newData( DataPacket& _packet  ){

    //cout << "datta" << endl;
    //cout << _packet.valuesString[0] << endl;
    
    
    if (_packet.valuesString.size()>0) {
        if (_packet.valuesString[0] == "GAMEMODE") {
            if (_packet.valuesInt.size()>0) {
                ajustesPista.setInteractionMode(_packet.valuesInt[0]);
            }
            
        }if (_packet.valuesString[0] == "NEWGAME") {
            newGame();
        }else if (_packet.valuesString[0] == "COUNTDOWN") {
            cuentAtras();
        }else if (_packet.valuesString[0] == "FINISHED") {
            if (_packet.valuesInt.size()>0) {
               // cout << _packet.valuesInt[0] << endl;
                // si el que gana tiene tu numero de player
                if(_packet.valuesInt[0] == PLAYER_NUM){
                    // has ganado y me lo has dicho tu mismo
                    
                }else{
                    /// valor para el gui
                    ajustesPista.setGameStatus(FINISHED);
                    
                    // valor para la pantalla
                    pantalla.setGameStatus(FINISHED);
                    
                    // terminamos la partida y ganamos
                    pantalla.finishGame(false);
                    
                    // desactivamos la pista
                    ajustesPista.setPistaActivada(false);
                    pantalla.stopCrono();
                    
                 #ifdef SERVER
                    DataPacket data;
                    data.valuesInt.push_back(_packet.valuesInt[0]);
                    data.valuesString.push_back("FINISHED");
                    server->sendData(data);
                #endif
                }
             
                
            }
            
        }
    }
    
}