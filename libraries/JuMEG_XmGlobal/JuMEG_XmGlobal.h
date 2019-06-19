/**************************************************************************/
/*! 
   JuMEG XmGlobal
    global container cls
   
    Hardware:
     Arduino Mega
     MusicMaker Shield 
     SPI communication
     SD card
     Pin 5 Output      
     PortC Cue Tone Event Code  

     @file     JuMEG_XmGlobal
     @author   F.Boers (f.boers@fz-juelich.de)
     update 05.04.2016
     v1.0  - First release
*/	
/**************************************************************************/

#ifndef JuMEG_XMGLOBAL_H
#define JuMEG_XMGLOBAL_H 

#include <JuMEG_XmBase.h>

//---

class JuMEG_XmGlobal: public JuMEG_XmBase{

  protected: 
   
   unsigned long         _duration;
   
   volatile bool          _ison;
   volatile unsigned long _onset;
   volatile unsigned long _offset;

  public:
   JuMEG_XmGlobal();
   ~JuMEG_XmGlobal();

   virtual bool begin();
   virtual bool reset();

   virtual void switchOn();
   virtual void switchOn(uint8_t code);
  
   void switchOff();
   bool isOn();
   virtual unsigned long getOnset();
   virtual unsigned long getOffset();

   unsigned long getDuration();
   void setDuration( unsigned long tms);

  private:

}; // end of cls JuMEG_XmGlobal


#endif //JUMEG_XMGLOBAL_H



