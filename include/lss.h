/*
This file is part of CanFestival, a library implementing CanOpen Stack. 

Copyright (C): Jorge Berzosa

See COPYING file for copyrights details.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifndef __LSS_h__
#define __LSS_h__

#define SLSS_ADRESS	0x7E4
#define MLSS_ADRESS	0x7E5

#define LSS_MSG_TIMER 0
#define LSS_SWITCH_DELAY_TIMER 1

#define SDELAY_OFF		0
#define SDELAY_FIRST 	1
#define SDELAY_SECOND 	2

#define LSS_WAITING_MODE 0
#define LSS_CONFIGURATION_MODE 1

/* Switch mode services */
#define LSS_SM_GLOBAL 				 4
#define LSS_SM_SELECTIVE_VENDOR 	64
#define LSS_SM_SELECTIVE_PRODUCT 	65
#define LSS_SM_SELECTIVE_REVISION 	66
#define LSS_SM_SELECTIVE_SERIAL 	67
#define LSS_SM_SELECTIVE_RESP 		68
/* Configuration services */
#define LSS_CONF_NODE_ID 			17
#define LSS_CONF_BIT_TIMING 		19
#define LSS_CONF_ACT_BIT_TIMING 	21
#define LSS_CONF_STORE 				23
/* Inquire services */
#define LSS_INQ_VENDOR_ID 		90
#define LSS_INQ_PRODUCT_CODE 	91
#define LSS_INQ_REV_NUMBER 		92
#define LSS_INQ_SERIAL_NUMBER 	93
#define LSS_INQ_NODE_ID 		94
/* Identification services */
#define LSS_IDENT_REMOTE_VENDOR 		70
#define LSS_IDENT_REMOTE_PRODUCT 		71
#define LSS_IDENT_REMOTE_REV_LOW 		72
#define LSS_IDENT_REMOTE_REV_HIGH 		73
#define LSS_IDENT_REMOTE_SERIAL_LOW 	74
#define LSS_IDENT_REMOTE_SERIAL_HIGH 	75
#define LSS_IDENT_REMOTE_NON_CONF 		76
#define LSS_IDENT_SLAVE 				79
#define LSS_IDENT_NON_CONF_SLAVE 		80
#define LSS_IDENT_FASTSCAN				81

/*FastScan State Machine*/
#define LSS_FS_RESET	0


typedef void (*LSSCallback_t)(CO_Data* d, UNS8 command); 

typedef void (*lss_StoreConfiguration_t)(UNS8*,UNS8*);
//void _lss_StoreConfiguration(UNS8 *error, UNS8 *spec_error);

typedef void (*lss_ChangeBaudRate_t)(char*);
//void _lss_ChangeBaudRate(char *BaudRate);


struct struct_lss_transfer;

//#include "timer.h"

/* The Transfer structure
* Used to store the different fields of the internal state of the LSS  
*/

struct struct_lss_transfer {
  UNS8          state;      /* state of the transmission : Takes the values LSS_... */
  UNS8 			command;     /* the LSS command of the transmision */
  UNS8			mode;       /* LSS mode */
  
  UNS32 		dat1;		/* the data from the last msg received */
  UNS8			dat2; 
  
  e_nodeState currentState; /* the state of the node before switching to LSSTimingDelay*/
  UNS8 nodeID;              /* the new nodeid stored to update the nodeid when switching to LSS operational*/
  UNS8 addr_sel_match;    	/* the matching mask for the LSS Switch Mode Selective service */
  UNS8 addr_ident_match;    /* the matching mask for the LSS Identify Remote Slaves service*/

  char *baudRate;           /* the new baudrate stored to update the node baudrate when a Activate Bit 
  							 * Timing Parameters is received*/
  UNS16 switchDelay;		/* the period of the two delay */
  UNS8  switchDelayState;   /* the state machine for the switchDelay */

  TIMER_HANDLE timers[2];      /* Time counters to implement a timeout in milliseconds.
                              * LSS_MSG_TIMER (index 0) is automatically incremented whenever 
                              * the lss state is in LSS_TRANS_IN_PROGRESS, and reseted to 0 
                              * when the response LSS have been received.
                              * LSS_SWITCH_DELAY_TIMER (index 1) is automatically incremented whenever 
                              * the lss switchDelayState is in SDELAY_FIRST or SDELAY_SECOND, and reseted to 0 
                              * when the two periods have been expired.
                              */
  LSSCallback_t Callback;   /* The user callback func to be called at LSS transaction end */
  
  UNS32 IDNumber;
  UNS8 BitChecked;
  UNS8 LSSSub;
  UNS8 LSSNext;
  UNS8 LSSPos;
  UNS8 FastScan_SM;
};

#ifdef CO_ENABLE_LSS
typedef struct struct_lss_transfer lss_transfer_t;
#else
typedef UNS8 lss_transfer_t;
#endif
  
  
void startLSS(CO_Data* d);
void stopLSS(CO_Data* d);


/** transmit a LSS message 
 * Checks if the msg can be transmited (i.e. we are not in LssTimingDelay state)
 * command is the LSS command specifier
 * dat1 and dat2 are pointers to optional data (depend on command)
 * return sendLSSMessage(d,command,dat1,dat2)
 */
UNS8 sendLSS (CO_Data* d, UNS8 command,void *dat1, void *dat2);

/** transmit a LSS message on CAN bus
 * comamnd is the LSS command specifier
 * bus_id is MLSS_ADRESS or SLSS_ADRESS depending in d->iam_a_slave. 
 * dat1 and dat2 are pointers to optional data (depend on command).
 * return canSend(bus_id,&m)
 */
 
UNS8 sendLSSMessage(CO_Data* d, UNS8 command,  void *dat1, void *dat2);

/** This function is called when the node is receiving a Master LSS message (cob-id = 0x7E5).
 *  - Check if there is a callback which will take care of the response. If not return 0 but does nothing.
 *  - Stops the timer so the alarm wont raise an error.
 *  - return 0 if OK
 */
UNS8 proceedLSS_Master (CO_Data* d, Message* m );

/** This function is called when the node is receiving a Slave LSS message (cob-id = 0x7E4).
 *  - Call the callback function or send the response message depending on the LSS comand within m.
 *  - return 0 if OK
 */
UNS8 proceedLSS_Slave (CO_Data* d, Message* m );

/** Used by the Master application to send a LSS command, WITHOUT response, to the slave. 
 * command: the LSS command. LSS_...
 * dat1 and dat2: pointers to optional data (depend on command).
 * return sendLSS(d,command,dat1,dat2)
 */
UNS8 configNetworkNode(CO_Data* d, UNS8 command, void *dat1, void* dat2);

/** Used by the Master application to send a LSS command, WITH response, to the slave. 
 * The function Callback, which must be defined in the user code, is called at the
 * end of the exchange (on succes or abort) and can be NULL.
 * The LSS_MSG_TIMER timer is started to control the timeout
 * return sendLSS(d,command,dat1,dat2)
 */
UNS8 configNetworkNodeCallBack (CO_Data* d, UNS8 command, void *dat1, void* dat2, LSSCallback_t Callback);

/** Use this function after a configNetworkNode or configNetworkNodeCallBack to get the result.
  Returns : LSS_RESET				// Transmission not started. Init state.
			LSS_FINISHED			// data are available                           
			LSS_ABORTED_INTERNAL	// Aborted but not because of an abort message. 
			LSS_TRANS_IN_PROGRESS	// Data not yet available

  * command: the LSS command (unused).
  * example:
  * UNS32 dat1;
  * UNS8 dat2;
  res=configNetworkNodeCallBack(&_Data,LSS_INQ_NODE_ID,0,0,NULL); // inquire the nodeID
  while (getConfigResultNetworkNode (&_Data, LSS_INQ_NODE_ID, &dat1, &dat2) != LSS_TRANS_IN_PROGRESS);
*/
UNS8 getConfigResultNetworkNode (CO_Data* d, UNS8 command, UNS32* dat1, UNS8* dat2);

#endif