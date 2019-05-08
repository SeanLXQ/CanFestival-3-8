/*
This file is part of CanFestival, a library implementing CanOpen Stack. 

Copyright (C): Edouard TISSERANT and Francis DUPIN

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

#ifndef __can_h__
#define __can_h__

#include "applicfg.h"

/** 
 * @brief The CAN message structure 
 * @ingroup can
 */

/*can 消息结构体*/
/*cob id:消息id
rtr:远程帧请求 0不是rtr消息，1是
len:消息长度
data:消息数据
*/
typedef struct {
  UNS16 cob_id;	/**< message's ID */
  UNS8 rtr;		/**< remote transmission request. (0 if not rtr message, 1 if rtr message) */
  UNS8 len;		/**< message's length (0 to 8) */
  UNS8 data[8]; /**< message's datas */
} Message;

/*定义一个初始化的消息结构体*/
#define Message_Initializer {0,0,0,{0,0,0,0,0,0,0,0}}

/*可重入函数邋canSend-t*/
typedef UNS8 (*canSend_t)(Message *);

#endif /* __can_h__ */
