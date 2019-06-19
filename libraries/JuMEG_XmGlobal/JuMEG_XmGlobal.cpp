/**************************************************************************/
/*! 

  JuMEG XmGlobal
    @author F.Boers (f.boers@fz-juelich.de)
    update 05.04.2016
    v1.0  - First release
*/
/**************************************************************************/
#include <JuMEG_XmGlobal.h>


/**************************************************************************/
/*! 
    @brief  Instantiates a new JuMEG_XmGlobal class
*/
/**************************************************************************/
JuMEG_XmGlobal::JuMEG_XmGlobal() {
}

// destructor
JuMEG_XmGlobal::~JuMEG_XmGlobal() {
}
//---------------------------------------------
// begin
//---------------------------------------------
bool JuMEG_XmGlobal::begin() {return reset();}// end of begin
 
//---------------------------------------------
// reset
//---------------------------------------------
bool JuMEG_XmGlobal::reset(){
  portEVENTCODE = 0; 
  _onset    = 0; 
  _duration = 0;
  _offset   = 0;
  _ison     = false;
  return true;
} // end of reset

//---------------------------------------------
// switchOn
//---------------------------------------------
void JuMEG_XmGlobal::switchOn(){
  portEVENTCODE = _event_code;
  _onset  = millis();
  _offset = 0;
  _ison   = true;
} // end of switchOn

//---------------------------------------------
// switchOn
//---------------------------------------------
void JuMEG_XmGlobal::switchOn(uint8_t code){
  setEventCode( code );
  portEVENTCODE = _event_code;
  _onset  = millis();
  _ison   = true;
} // end of switchOn

//---------------------------------------------
// switchOff
//---------------------------------------------
void JuMEG_XmGlobal::switchOff(){
 reset();
 _offset = millis() - _pgr_start_time;
} // end of switchOff

//---------------------------------------------
// isOn
//---------------------------------------------
bool JuMEG_XmGlobal::isOn(){ return _ison; }

//---------------------------------------------
// getOnset
//---------------------------------------------
unsigned long JuMEG_XmGlobal::getOnset(){
     return _onset - _pgr_start_time;
}

//---------------------------------------------
// getOffset
//---------------------------------------------
unsigned long JuMEG_XmGlobal::getOffset(){
     return _offset;
}

//---------------------------------------------
// getDuration
//---------------------------------------------
unsigned long JuMEG_XmGlobal::getDuration(){
     return _duration;
}
//---------------------------------------------
// setDuration
//---------------------------------------------
void JuMEG_XmGlobal::setDuration(unsigned long tms){
     _duration = tms;
}
