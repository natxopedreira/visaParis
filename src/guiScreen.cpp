//
//  guiScreen.cpp
//  accuraGUI
//
//  Created by Natxo Pedreira gonzalez on 28/11/15.
//
//

#include "guiScreen.h"

//--------------------------------------------------------------
void guiScreen::startCrono(){
    cronometro.startTime();
}

//--------------------------------------------------------------
void guiScreen::stopCrono(){
    cronometro.stopTimer();
}


//--------------------------------------------------------------
void guiScreen::addLap(){
    laps++;
}

//--------------------------------------------------------------
void guiScreen::setLapCounter(int c){
    laps = c;
}

//--------------------------------------------------------------
void guiScreen::setAtencionTxt(string f){
    esferaAtencionTxt = f;
}

//--------------------------------------------------------------
void guiScreen::setMeditacionTxt(string f){
    esferaMeditacionTxt = f;
}

//--------------------------------------------------------------
void guiScreen::setBarraConectionTxt(string f){
    barraConectionTxt = f;
}



//--------------------------------------------------------------
void guiScreen::setVelocidadPower(float f){
    velocidadEsferaPower = f;
}

//--------------------------------------------------------------
void guiScreen::setMeditacionPower(float f){
    meditacionEsferaPower = f;
}

//--------------------------------------------------------------
void guiScreen::setAtencionPower(float f){
    atencionEsferaPower = f;
}

//--------------------------------------------------------------
void guiScreen::setBarraPower(float f){
    powerBarra = f;
}

//--------------------------------------------------------------
void guiScreen::setConectionPower(float f){
    conectionBarraPower = f;
}

//--------------------------------------------------------------
void guiScreen::newGame(){
    //estadoJuego = NEWGAME;
    winner = false;
    laps = 0;
    //cronometro.startTime();
}

//--------------------------------------------------------------
void guiScreen::finishGame(bool w){
    estadoJuego = FINISHED;
    winner = w;
    //playerWinner
}

void guiScreen::setGameStatus(gameStatus g){
    estadoJuego = g;
}
gameStatus guiScreen::getGameStatus(){
    return estadoJuego;
}

int guiScreen::getnumLaps(){
    return cuentaVueltas.getLap();
}
//--------------------------------------------------------------
void guiScreen::setup(){
    
	pastilla.loadImage("pastilla.png");
	pastillaOn.loadImage("pastillaOnBlanca.png");
	pastillaLuz.loadImage("pastillaLuzBlanca.png");
	// pastillaOn.loadImage("pastillaOnBlanca.png");

	pastilla.setAnchorPoint(435, 18);
	pastillaOn.setAnchorPoint(435 + 3, 18 + 3);
	pastillaLuz.setAnchorPoint(435 + 3, 18 + 3);

	pastillaOnladrillo.loadImage("ladrilloOn.png");
	pastillaGlowladrillo.loadImage("ladrilloGlow.png");
	pastillaladrillo.loadImage("ladrillo.png");

    winner = false;
    
    setupEsferas();
    
    cronometro.setup(108);
    cronometro.setColor(ofColor(255,255,255));
    
    
    //backgroundImage.load("pantalla.png");
    banda.load("barraVisa.png");
    fondoGris.load("fondoGris.png");
    
    fuenteMediana.load("HelveticaNeue-CondensedBold.ttf", 20);
    fuentePorciento.load("HelveticaNeue-CondensedBold.ttf", 83);
    fuenteWinner.load("HelveticaNeue-CondensedBold.ttf", 120);
    
    estadoJuego = RUNNING;
    
    countDown.setup();

    mano.loadImage("mano.png");
    cabeza.load("cabeza.png");
}

//--------------------------------------------------------------
void guiScreen::update(){
    
    
    if (estadoJuego == RUNNING || estadoJuego == NEWGAME) {
        // indicador de velocidad
        esferaVelocidad.setPotenciaNorm(velocidadEsferaPower);
        
    }else{
        // indicador de velocidad
        esferaVelocidad.setPotenciaNorm(0);
        
    }
    
    // indicador de meditacion
    esferaMeditacion.setPotenciaNorm(meditacionEsferaPower);
    
    // indicador de atencion
    esferaAtencion.setPotenciaNorm(atencionEsferaPower);
    
    // barra de power
    barraPower.setPotencia(powerBarra);
    
    // barra de conection
    barraConnection.setPotencia(conectionBarraPower);
    
    //
    esferaVelocidad.update();
    esferaMeditacion.update();
    esferaAtencion.update();
    barraPower.update();
    barraConnection.update();
    
    // cuentavueltas
	if (getnumLaps() != laps) {
		cuentaVueltas.setLap(laps);
		
	}
	cuentaVueltas.update();
    
    //
    cronometro.update();
    
    //
    countDown.update();
}

//--------------------------------------------------------------
void guiScreen::draw(bool drawMano){
    
    //backgroundImage.draw(0, 0);
    /*
    ofSetColor(140, 140, 140);
    
    /////////////////////////
    ////// lineas ///////////
    /////////////////////////
    ofRect(1100, 540, 1920, 2);
    ofRect(1570, 540, 2, 500);
    ofRect(1630, 540, 2, -500);
    */
    ofSetColor(255,255, 255, 255);
    
    
    //// barra con mind icono
    ofPushMatrix();
    ofTranslate(125, 1080);
    ofRotate(-90);
  //  banda.draw(0, 0);
    ofPopMatrix();
    
    
    if (estadoJuego == RUNNING || estadoJuego == NEWGAME) {
    
        /////////////////////////
        // cuentavueltas //
        /////////////////////////
    
        ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
        
        ofPushMatrix();
        ofTranslate(190, 380);
        ofRotate(-90);
        cuentaVueltas.draw();
        cuentaVueltas.drawBlur();
    
        ofDisableBlendMode();
        ofSetColor(255,255,255,200);
        ofScale(1.4,1.4);
        fuenteMediana.drawString("TOUR", 120, 30);
        ofSetColor(255,255,255,255);
        ofPopMatrix();
    
    }
    
    /////////////////////////
    // cronometro //
    /////////////////////////
    ofPushMatrix();
    
    ofTranslate(1550, 515);
    ofRotate(-90);
    //cronometro.draw();
    cronometro.drawBlur();
    ofPopMatrix();
 
    
    
    
    /// cabeza o mano
    if (estadoJuego != COUNTDOWN && estadoJuego != FINISHED){
        ofPushMatrix();
        if(drawMano){
            mano.draw(550, 480);
        }else{
            cabeza.draw(570, 470);
        }
        //
        
        ofPopMatrix();
    }
    
    /////////////////////////
    // esfera de velocidad //
    /////////////////////////
    ofPushMatrix();
    ofTranslate(845, 540);
    ofRotate(-90);
    
    

    
    
    
    
    if (estadoJuego == RUNNING || estadoJuego == NEWGAME) {
        esferaVelocidad.draw(ofPoint(0,0), &pastilla, &pastillaLuz, &pastillaOn);
        
        ofPushStyle();
        ofSetColor(77,11,4,80);
        fuentePorciento.drawString("%",-44, 240);
        ofPopStyle();
        
    }else{
        esferaVelocidad.drawSinTexto(ofPoint(0,0), &pastilla, &pastillaLuz, &pastillaOn);
        
        if (estadoJuego == FINISHED) {
            // se acabo el juego pinta si ganaste o perdise

            fuenteWinner.drawString("YOU", -140, -70);
            
            if(winner){
                fuenteWinner.drawString("WIN", -130, 100);
            }else{
                fuenteWinner.drawString("LOOSE", -220, 100);
            }
            
            
        }else if (estadoJuego == COUNTDOWN){
            // pintas la cuenta atras
            countDown.draw();
        }
    }
    
    
    //esferaVelocidad.draw(ofPoint(0,0));
    

    
    ofPopMatrix();
    
    ////////////////////////
    // esfera de atencion //
    ////////////////////////
    ofPushMatrix();
    ofTranslate(1455, 697);
    ofRotate(-90);
    esferaAtencion.draw(ofPoint(0,0), &pastilla, &pastillaLuz, &pastillaOn);
    
    ofSetColor(77,11,4);
    fuenteMediana.drawString(esferaAtencionTxt, -62, -115);
    ofSetColor(255,255, 255);
    ofPopMatrix();
    
    //////////////////////////
    // esfera de meditacion //
    //////////////////////////
    ofPushMatrix();
    ofTranslate(1720, 697);
    ofRotate(-90);
    esferaMeditacion.draw(ofPoint(0,0), &pastilla, &pastillaLuz, &pastillaOn);
    
    ofSetColor(77,11,4);
    fuenteMediana.drawString(esferaMeditacionTxt, -68, -115);
    ofSetColor(255,255, 255);
    ofPopMatrix();
    
    ////////////////////
    // barra de power //
    ////////////////////
    ofPushMatrix();
    ofTranslate(1005, 937);
    ofRotate(-90);
    barraPower.draw(&pastillaladrillo, &pastillaOnladrillo, &pastillaGlowladrillo);
    ofPopMatrix();
    
    /////////////////////////
    // barra de connection //
    /////////////////////////
    ofPushMatrix();
    ofTranslate(1580, 510);
    ofRotate(-90);
    barraConnection.draw(&pastillaladrillo,&pastillaOnladrillo, &pastillaGlowladrillo);
    
    ofSetColor(77,11,4);
    fuenteMediana.drawString(barraConectionTxt, 205, -240);
    ofSetColor(255,255, 255);
    ofPopMatrix();
    
    
    /////////////////////////////
    // mini % para las esferas //
    /////////////////////////////
	/*ofPushMatrix();
    ofTranslate(1510, 710);
    ofRotate(-90);
    ofScale(1.1,1.1);
    fuenteMediana.drawString("%", 0, 0);
    fuenteMediana.drawString("%", 492, -100);
    fuenteMediana.drawString("%", 0, 240);
    ofPopMatrix();
    
    
    //////////////////////////////////////
    // imagen con los bordes sombreados //
    /////////////////////////////////////
	
    ofPushStyle();
    
    ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
    fondoGris.draw(0, 0);
    ofDisableBlendMode();
    
    ofPopStyle();
	*/
}

//--------------------------------------------------------------
void guiScreen::setupEsferas(){
    
    // cuenta vueltas
    cuentaVueltas.setup(440);
    cuentaVueltas.setColor(ofColor(77,11,4));
    
    // barra vertical
    barraPower.setNumeroLadrillos(24);
    barraPower.setScale(1.01);
    barraPower.setOrigen(ofPoint(20, 800));
    barraPower.setColor(ofColor(77,11,4));
    barraPower.setup();
    
    // barra de conexion
    barraConnection.setScale(1.01);
    barraConnection.setOrigen(ofPoint(0, 0));
    barraConnection.setColor(ofColor(77,11,4));
    barraConnection.setup();
    
    
    // donde ponemos la esfera
    esferaVelocidad.setOrigenEsfera(ofPoint(ofGetWidth()/2, ofGetHeight()/2));
    esferaMeditacion.setOrigenEsfera(ofPoint(ofGetWidth()/2, ofGetHeight()/2));
    esferaAtencion.setOrigenEsfera(ofPoint(ofGetWidth()/2, ofGetHeight()/2));
    
    
    // punto de rotacion para la imagen de las casillas
    esferaVelocidad.setImgPivotPoint(ofPoint(435, 18));
    esferaMeditacion.setImgPivotPoint(ofPoint(435, 18));
    esferaAtencion.setImgPivotPoint(ofPoint(435, 18));
    
    // la escala a la que dibujamos la esfera
    esferaVelocidad.setScale(0.9);
    esferaMeditacion.setScale(0.2053);
    esferaAtencion.setScale(0.2053);
    esferaVelocidad.setNumPastillas(50);
    
    
    // colores para las esferas
    esferaVelocidad.setPastillaColor(ofColor(77,11,4));
    esferaMeditacion.setPastillaColor(ofColor(77,11,4));
    esferaAtencion.setPastillaColor(ofColor(77,11,4));
    

    
	esferaVelocidad.setNumPastillas(34);
    
    // customice esferas pequeñas atencion y meditacion
    esferaAtencion.setNumPastillas(24);
    esferaAtencion.setGradosDial(310);
    esferaAtencion.rotationInicial = 60;
    esferaAtencion.desfaseLineasRotacion = 60;
    esferaAtencion.speedText.setColor(ofColor(77,11,4,80));
    esferaAtencion.setFontScale(1.0);
    
    esferaMeditacion.setNumPastillas(24);
    esferaMeditacion.setGradosDial(310);
    esferaMeditacion.rotationInicial = 60;
    esferaMeditacion.desfaseLineasRotacion = 60;
    esferaMeditacion.speedText.setColor(ofColor(77,11,4,80));
    esferaMeditacion.setFontScale(1.0);

	// cargamos las imagenes
	
	//ofImage pastilla;
	//pastilla.loadImage("pastilla.png");



	//esferaVelocidad.pastilla = &pastilla;
	//esferaVelocidad.pastillaOn = &pastillaOn;
	//esferaVelocidad.pastillaLuz = &pastillaLuz;

	//esferaMeditacion.pastilla = &pastilla;
	//esferaMeditacion.pastillaOn = &pastillaOn;
	//esferaMeditacion.pastillaLuz = &pastillaLuz;

	//esferaAtencion.pastilla = &pastilla;
	//esferaAtencion.pastillaOn = &pastillaOn;
	//esferaAtencion.pastillaLuz = &pastillaLuz;

	// pastilla.loadImage("pastilla.png");
	//pastilla->setAnchorPoint(imgPivotPoint.x, imgPivotPoint.y);

	//pastillaOn.loadImage("pastillaOnBlanca.png");
	//pastillaOn->setAnchorPoint(imgPivotPoint.x + 3, imgPivotPoint.y + 3);

	//pastillaLuz.loadImage("pastillaLuzBlanca.png");
	//pastillaLuz->setAnchorPoint(imgPivotPoint.x + 3, imgPivotPoint.y + 3);



	esferaVelocidad.setup(ofColor(77,11,4));
	esferaMeditacion.setup(ofColor(255,255,255));
	esferaAtencion.setup(ofColor(255,255,255));
}