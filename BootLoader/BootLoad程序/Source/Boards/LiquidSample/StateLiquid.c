#include "Liquid.h"
#include "math.h"
#include "Adc.h"
#include "LiquidPwm.h"
#include "StateLiquid.h"
#include "Gpio.h"
#include "Board.h"
#include "Timer.h"
#include "Tim4.h"
#include "console.h"
#define     IDEAL_TEM       (38.6)
/********************************************************************
 *
 *******************************************************************/
VOID SetPwmDutyRatio(DOUBLE ratio)
{
//	PwmStop();
//	PwmSetDutyRatio ((UINT32)(ratio*1000));
//	PwmStart();
}
/********************************************************************
 *
 *******************************************************************/
STATIC INT8 tag = 0;
DOUBLE NtcStateReady(VOID)
{
   if(NtcApp.state != NTC_STATE_READY)
       NtcApp.state = NTC_STATE_READY;
    DOUBLE temp;
    temp = ReadNtcValue();
    DOUBLE pwm = 0;
    DOUBLE ek;
//    ek   = IDEAL_TEM - temp;
//    pwm = PIDCalc (&(NtcApp.pid), ek);         
//    //SetPwmDutyRatio (pwm);
//    NtcApp.pwm = pwm; 
//   PwmSetDutyRatio(pwm);
////   DebugLog(tag,"catch:%8.4f",
////		temp);
   return pwm;
}
/********************************************************************
 *
 *******************************************************************/
DOUBLE NtcStateRun(UINT16 _pwm)
{
    if(NtcApp.state != NTC_STATE_RUN){
        NtcApp.state = NTC_STATE_RUN;
    }
    DOUBLE pwm = (DOUBLE)_pwm/1000;
//    NtcApp.pwm = pwm;
//    PwmSetDutyRatio(pwm);
    return pwm;
}
/********************************************************************
 *
 *******************************************************************/
DOUBLE NtcStateStop(VOID)
{
    NtcApp.state = NTC_STATE_STOP;
    DOUBLE pwm = 0;
    NtcApp.pwm = pwm;
//    PwmSetDutyRatio(pwm);
    return pwm;
}

/********************************************************************
 end of file
 *******************************************************************/





