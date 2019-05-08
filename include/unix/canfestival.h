#ifndef UNIX_H_
#define UNIX_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "timerscfg.h"
#include "can_driver.h"
#include "data.h"
#include "timers_driver.h"

#ifndef __KERNEL__
#include <dlfcn.h>
#endif

typedef void* LIB_HANDLE;

/** @defgroup userapi User API */

/** @defgroup can CAN management
 *  @ingroup userapi
 */

/**
 * @ingroup can
 * @brief Unload CAN driver interface
 * @param handle The library handle
 * @return
 *       -  0 is returned upon success.
 *       - -1 is returned if the CAN driver interface can't be unloaded.
 */
 /*Ğ¶ÔØcandriverÇı¶¯µÄ½Ó¿Ú*/
UNS8 UnLoadCanDriver(LIB_HANDLE handle);

/**
 * @ingroup can
 * @brief Load CAN driver interface.
 * @param *driver_name The location of the library to load
 * @return
 *       - handle of the CAN driver interface is returned upon success.
 *       - NULL is returned if the CAN driver interface can't be loaded.
 */
 /*×°ÔØcanÇı¶¯*/
LIB_HANDLE LoadCanDriver(const char* driver_name);

/**
 * @brief Send a CAN message
 * @param port CanFestival file descriptor
 * @param *m The CAN message to send
 * @return 0 if succes
 */
 /*·¢ËÍcanÏûÏ¢º¯Êı
²ÎÊı1ÎÄ¼şÖ¸Õë 2ÏûÏ¢ÄÚÈİ
³É¹¦·µ»Ø0*/
UNS8 canSend(CAN_PORT port, Message *m);

/**
 * @ingroup can
 * @brief Open a CANOpen device
 * @param *board Pointer to the board structure that contains busname and baudrate 
 * @param *d Pointer to the CAN object data structure
 * @return
 *       - CanFestival file descriptor is returned upon success.
 *       - NULL is returned if the CANOpen board can't be opened.
 */
 /*´ò¿ªcanopenÉè±¸
1ºÅ²ÎÊıÊÇÖ¸ÏòbusnameºÍbaudratreµÄÖ¸Õë
2ºÅ²ÎÊıÊÇÖ¸Ïòcan½á¹¹ÌåµÄÖ¸Õë
·µ»ØÖµ 1.ÎÄ¼şÖ¸Õë2NULL*/
CAN_PORT canOpen(s_BOARD *board, CO_Data * d);

/**
 * @ingroup can
 * @brief Close a CANOpen device
 * @param *d Pointer to the CAN object data structure
 * @return
 *       - 0 is returned upon success.
 *       - errorcode if error. (if implemented)  
 */
 å/*¹Ø±ÕcanopenÉè±¸*/
int canClose(CO_Data * d);

/**
 * @ingroup can
 * @brief Change the CANOpen device baudrate 
 * @param port CanFestival file descriptor 
 * @param *baud The new baudrate to assign
 * @return
 *       - 0 is returned upon success or if not supported by the CAN driver.
 *       - errorcode from the CAN driver is returned if an error occurs. (if implemented in the CAN driver)
 */
 /*¸Ä±äcanopenÉè±¸µÄ²¨ÌØÂÊ ²ÎÊı1.ÎÄ¼şÃèÊö·û 2.ĞÂµÄ²¨ÌØÂÊ
·µ»Ø 0--success errcode---fail*/
UNS8 canChangeBaudRate(CAN_PORT port, char* baud);



#ifdef __cplusplus
};
#endif

#endif /*UNIX_H_*/
