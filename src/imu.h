//
//  imu.h
//  acuraGui09
//
//  Created by Natxo Pedreira gonzalez on 2/12/15.
//
//

#ifndef acuraGui09_imu_h
#define acuraGui09_imu_h


class imu {
    ofSerial serial;
    std::vector <ofSerialDeviceInfo> deviceList;
    
    float roll, pitch, yaw;
    string buf;
    
    //--------------------------------------------------------------
    void serial_init(){
        serial.close();
        serial.listDevices();
        deviceList = serial.getDeviceList();
        for(int i=0; i<deviceList.size(); i++){
            cout << i << ": "
            << deviceList[i].getDeviceID()   << ", "
            << deviceList[i].getDeviceName() << ", "
            << deviceList[i].getDevicePath() << endl;
        }
    }
    
    //--------------------------------------------------------------
    void serial_connect(int com_id, int baud){
        serial.setup(com_id,baud);// 0 is to open the first device
        nTimesRead = 0;
        while(!serial.available() ){
            nTimesRead++;
            //Sleep(10);
            if(nTimesRead>timeout_count) return;
        }
    }
    
    //--------------------------------------------------------------
    void serial_connect(string com_name, int baud) {
        serial.setup(com_name,baud);
        //serial.setup("COM4", baud); // windows example
        //serial.setup("/dev/tty.usbserial-A4001JEC", baud); // mac osx example
        //serial.setup("/dev/ttyUSB0", baud); //linux example
        nTimesRead = 0;
        while(!serial.available()){
            nTimesRead++;
            //Sleep(10);
            if(nTimesRead>timeout_count) return;
        }
    }
    
    //--------------------------------------------------------------
    void serial_readypr(float &y, float &p, float &r) {
        int nRead  = 0;  // a temp variable to keep count per read
        serial.drain();
        buf.clear();
        
        // header align
        nTimesRead = 0;
        while( (nRead = serial.readByte()) != '#'){
            nTimesRead++;
            if(nTimesRead>timeout_count) return;
        }
        // readline
        nTimesRead = 0;
        while( (nRead = serial.readByte()) != '\n'){
            nTimesRead++;
            if(nRead > 0){
                buf.push_back(nRead);
            }
            if( nTimesRead>timeout_count) return;
        }
        
        
        if( (sscanf(buf.data(), "YPR=%f,%f,%f",  &y, &p, &r)) == 3){
           // cerr << buf.data() << endl;
            //cerr << y << ", " << p << ", " << r << endl;
        }
    }
    
    //--------------------------------------------------------------
    int dafault_baud;
    int timeout_count;
    int timeout_wdt;
    
    string puerto;
    
public:
    int watchDogTimer;
    int nTimesRead;// how many times did we read?
    float readTime;// when did we last read?
    
    //--------------------------------------------------------------
    imu(){
        dafault_baud = 57600;
        timeout_count = 100000;
        timeout_wdt = 200;
    }
    
    //--------------------------------------------------------------
    void setup(string port){
        roll = pitch = yaw = 0.0f;
        readTime = 0;
        nTimesRead = 0;
        watchDogTimer = 0;
        
        serial_init();
        serial_connect(port,dafault_baud);
        
        puerto = port;
    }
    
    //--------------------------------------------------------------
    void update(){
        watchDogTimer++;
        if(serial.isInitialized()){
            if(serial.available()){
                serial_readypr(yaw, pitch, roll);
                readTime = ofGetElapsedTimef()-readTime;
                watchDogTimer = 0;
            }
        }
        
        if(watchDogTimer>timeout_wdt){
            serial_init();
            serial_connect(puerto, dafault_baud);
            watchDogTimer = 0;
        }
    }
    
    //--------------------------------------------------------------
    int getRoll(){
        return ofWrap(roll, -180, 180);
    }
    
    //--------------------------------------------------------------
    int getPitch(){
        return ofWrap(pitch, -180, 180);
    }
    
    //--------------------------------------------------------------
    int getYaw(){
        return ofWrap(yaw, -180, 180);
    }
};
#endif
