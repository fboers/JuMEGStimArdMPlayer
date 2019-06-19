/**************************************************************************/
/*! 
   JuMEG XmBase
    global container cls
   
    Hardware:
     Arduino Mega
     MusicMaker Shield 
     SPI communication
     SD card
     Pin 5 Output      
     PortC Cue Tone Event Code  

     @file     JuMEG_XmBase
     @author   F.Boers (f.boers@fz-juelich.de)
     update 31.03.2016
     v1.0  - First release
*/	
/**************************************************************************/

#ifndef JuMEG_XMBASE_H
#define JuMEG_XMBASE_H 

#include <Arduino.h>

#include <stdint.h> 

#include <DirectIO.h>

#define XM_PULSE_WIDTH_MS 20

//---

class JuMEG_XmBase{
 protected: 

//--- init DirectIO LPST ports A;
  OutputPort<PORT_A> portADDR;
//--- init DirectIO LPST ports C; 
  OutputPort<PORT_C> portEVENTCODE; 

//--- init response port
  InputPort <PORT_L> portResponse;

  uint8_t       _event_code; 
  unsigned long _pgr_start_time = 0;
  bool          _verbose        = false;


 public:
  JuMEG_XmBase();
  ~JuMEG_XmBase();

  virtual bool begin();  
  virtual bool setup();
  virtual bool reset(); 
  virtual bool update();
 
  virtual uint8_t getEventCode();
  virtual void    setEventCode(uint8_t code);
 
  virtual void sendEventCode();
  virtual void sendEventCode(uint8_t code);

  unsigned long getPgrStartTime();
  void setPgrStartTime();
  void setPgrStartTime(unsigned long dt);

  void setVerbose(bool v);
  bool getVerbose();
 
 private:

}; // end of cls JuMEG_XmGlobal


#endif //JUMEG_XMBASE_H



