//#######################################################################################################
//##                    This Plugin is only for use with the RFLink software package                   ##
//##                                     Plugin-065: SKYLINK PIR                               ##
//#######################################################################################################
/*********************************************************************************************\
 * This plugin provides support for Skylink HA-434TL PIR devices
 *
 * Author  : StormTeam 2020 - Cyril PAWELKO - Marc RIVES (aka Couin3)
 * Support : https://github.com/couin3/RFLink 
 * License : This code is free for use in any open source project when this header is included.
 *           Usage of any parts of this code in a commercial application is prohibited!
 *********************************************************************************************
 * Protocol information : 
 * Bitstream: 1011 1110 0011 1110 1   --> Button Press / Motion
 * Bitstream: 0101 1110 0011 1110 1   --> Battery Installation / No Motion
 *       
 *      Visonic Sensors Documentation Annex
 * 
 * Sample data:
 * 20;XX;DEBUG;Pulses=38;Pulses(uSec)=2080,1888,576,1440,608,416,576,1440,576,1440,576,1472,576,1440,576,1440,576,416,576,416,576,416,576,1472,576,1472,576,1472,576,1472,576,1472,576,448,576,1472,576,192;
 * Only tested with 
 \*********************************************************************************************/

#define Skylink_PLUGIN_ID 065
#define PLUGIN_DESC_065 "Skylink"
#define SKYLINK_PULSECOUNT 38

#define SKYLINK_PULSE_MID 1000 / RAWSIGNAL_SAMPLE_RATE
#define SKYLINK_PULSE_MIN 150 / RAWSIGNAL_SAMPLE_RATE
#define SKYLINK_PULSE_MAX 4500 / RAWSIGNAL_SAMPLE_RATE
#define SKYLINK_PULSE_SYNC 1500 / RAWSIGNAL_SAMPLE_RATE

#ifdef PLUGIN_065
#include "../4_Display.h"

boolean Plugin_065(byte function, char *string)
{
   if (RawSignal.Number != SKYLINK_PULSECOUNT)
      return false;

   unsigned long bitstream = 0L; // Only the 32 first bits are processed

   //==================================================================================
   // First 2 pulse's should be long
   //==================================================================================

   if (RawSignal.Pulses[1] < SKYLINK_PULSE_SYNC || RawSignal.Pulses[2] < SKYLINK_PULSE_SYNC)
      return false;

   //==================================================================================
   // Get first 32 bits : Sensor ID (24 bits) + 8 first bits of data
   // Bits are inverted !
   //==================================================================================
   
   //Serial.print("Bitstream: ");
   for (byte x = 4; x <= 36; x += 2)
   {
      if (RawSignal.Pulses[x] > SKYLINK_PULSE_MID)
      { // long pulse = 1
         if (RawSignal.Pulses[x] > SKYLINK_PULSE_MAX)
            return false; // pulse too long
         if (RawSignal.Pulses[x + 1] > SKYLINK_PULSE_MAX)
            return false; // invalid manchester code
         bitstream = (bitstream << 1) | 0x1;
         //Serial.print("1");
      }
      else
      { // short pulse = 0
         if (RawSignal.Pulses[x] < SKYLINK_PULSE_MIN)
            return false; // pulse too short
         if (RawSignal.Pulses[x + 1] > SKYLINK_PULSE_MAX)
            return false; // invalid manchester code
         bitstream = bitstream << 1;
         //Serial.print("0");
      }
   }
   //Serial.println("");
   //==================================================================================
   // Prevent repeating signals from showing up
   //==================================================================================
   if ((SignalHash != SignalHashPrevious) || ((RepeatingTimer) + 700 < millis()) || (SignalCRC != bitstream))
      SignalCRC = bitstream;
   else
      return true; // packet already seen
   //==================================================================================
   // Extract data
   //==================================================================================
   // char dataPrint[9];
   // sprintf(dataPrint, "%04lx", bitstream);
   // Serial.print("Datastream: ");
   // Serial.println(dataPrint);
   boolean alarm = (bitstream >> 14) & 0x01;
   unsigned long ID = ( bitstream & 0x3FFF);
   //Serial.print("alarm: ");
   //Serial.println(alarm);
   //Serial.print("ID: ");
   //Serial.println(ID);
   // ----------------------------------
   // Output
   // ----------------------------------
   display_Header();
   display_Name(PSTR("Skylink"));
   display_IDn(ID, 4);
   display_SWITCH(1);
   display_CMD(CMD_Single, alarm ? CMD_On : CMD_Off);
   display_Footer();
   //==================================================================================
   RawSignal.Repeats = true; // suppress repeats of the same RF packet
   RawSignal.Number = 0;
   return true;
}
#endif // Plugin_065
