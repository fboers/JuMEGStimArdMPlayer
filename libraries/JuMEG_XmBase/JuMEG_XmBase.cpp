/**************************************************************************/
/*! 

  JuMEG XmBase
    @author F.Boers (f.boers@fz-juelich.de)
    update 05.04.2016
    v1.0  - First release
*/
/**************************************************************************/
#include <JuMEG_XmBase.h>


/**************************************************************************/
/*! 
    @brief  Instantiates a new JuMEG_XmBase class
*/
/**************************************************************************/
JuMEG_XmBase::JuMEG_XmBase() {
}

// destructor
JuMEG_XmBase::~JuMEG_XmBase() {
}

//---------------------------------------------
// begin
//---------------------------------------------
bool JuMEG_XmBase::begin() {return true;}// end of begin
//---------------------------------------------
// setup
//---------------------------------------------
bool JuMEG_XmBase::setup() {return true;}// end of begin
//---------------------------------------------
// reset
//---------------------------------------------
bool JuMEG_XmBase::reset() {return true;}// end of reset
//---------------------------------------------
// update
//---------------------------------------------
bool JuMEG_XmBase::update() {return true;}// end of reset


//---------------------------------------------
// get_event_code
//---------------------------------------------
uint8_t JuMEG_XmBase::getEventCode(){
     return _event_code;
}
//---------------------------------------------
// setEventCode
//---------------------------------------------
void JuMEG_XmBase::setEventCode(uint8_t code){
     _event_code = code;
}

//===== !!!! and not or
// portEVENTCODE = portEVENTCODE & _event_code;
// portEVENTCODE = portEVENTCODE & ~_event_code;
// portEVENTCODE = portEVENTCODE | _event_code;

//---------------------------------------------
// send_event_code
//---------------------------------------------
void JuMEG_XmBase::sendEventCode() {
// portEVENTCODE = portEVENTCODE | _event_code;
 portEVENTCODE = _event_code;

}// end of send_event_code
//---------------------------------------------
// send_event_code
//---------------------------------------------
void JuMEG_XmBase::sendEventCode(uint8_t code) {
 _event_code = code;
// portEVENTCODE = portEVENTCODE | _event_code;
 portEVENTCODE = _event_code;
}// end of send_event_code


//---------------------------------------------
// getPGRStartTime
//---------------------------------------------
unsigned long JuMEG_XmBase::getPgrStartTime(){
   return _pgr_start_time;
}

//---------------------------------------------
// setPGRStartTime
//---------------------------------------------
void JuMEG_XmBase::setPgrStartTime(){
     _pgr_start_time = millis();
}
//---------------------------------------------
// setPGRStartTime
//---------------------------------------------
void JuMEG_XmBase::setPgrStartTime(unsigned long dt){
     _pgr_start_time = dt;
}

//---------------------------------------------------------------
// setVerbose
//---------------------------------------------------------------
void JuMEG_XmBase::setVerbose(bool v){_verbose = v;}
//---------------------------------------------------------------
// getVerbose
//---------------------------------------------------------------
bool JuMEG_XmBase::getVerbose(){return _verbose;}
 

