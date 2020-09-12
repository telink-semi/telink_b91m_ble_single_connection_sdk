/********************************************************************************************************
 * @file	app_audio.c
 *
 * @brief	for TLSR chips
 *
 * @author	BLE GROUP
 * @date	2020.06
 *
 * @par     Copyright (c) 2020, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
 *          
 *          Redistribution and use in source and binary forms, with or without
 *          modification, are permitted provided that the following conditions are met:
 *          
 *              1. Redistributions of source code must retain the above copyright
 *              notice, this list of conditions and the following disclaimer.
 *          
 *              2. Unless for usage inside a TELINK integrated circuit, redistributions 
 *              in binary form must reproduce the above copyright notice, this list of 
 *              conditions and the following disclaimer in the documentation and/or other
 *              materials provided with the distribution.
 *          
 *              3. Neither the name of TELINK, nor the names of its contributors may be 
 *              used to endorse or promote products derived from this software without 
 *              specific prior written permission.
 *          
 *              4. This software, with or without modification, must only be used with a
 *              TELINK integrated circuit. All other usages are subject to written permission
 *              from TELINK and different commercial license may apply.
 *
 *              5. Licensee shall be solely responsible for any claim to the extent arising out of or 
 *              relating to such deletion(s), modification(s) or alteration(s).
 *         
 *          THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 *          ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *          WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *          DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDER BE LIABLE FOR ANY
 *          DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *          (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *          LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *          ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *          (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *          SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *         
 *******************************************************************************************************/
#include "tl_common.h"
#include "drivers.h"
#include "stack/ble/ble.h"
#include "application/audio/tl_audio.h"
#include "application/audio/audio_config.h"
#include "app_audio.h"

#if (TL_AUDIO_MODE == TL_AUDIO_DONGLE_ADPCM_GATT_TELINK)			//GATT Telink

u8		att_mic_rcvd = 0;
u32		tick_adpcm;
u8		buff_mic_adpcm[MIC_ADPCM_FRAME_SIZE];

u32		tick_iso_in;
int		mode_iso_in;



_attribute_ram_code_ void  usb_endpoints_irq_handler (void)
{
	u32 t = clock_time ();
	/////////////////////////////////////
	// ISO IN
	/////////////////////////////////////
	if (reg_usb_irq & BIT(7)) {
		mode_iso_in = 1;
		tick_iso_in = t;
		reg_usb_irq = BIT(7);	//clear interrupt flag of endpoint 7

		/////// get MIC input data ///////////////////////////////
		//usb_iso_in_1k_square ();
		//usb_iso_in_from_mic ();
		DBG_CHN6_TOGGLE;

		abuf_dec_usb ();
	}

}

void	att_mic (u16 conn, u8 *p)
{
	att_mic_rcvd = 1;
	memcpy (buff_mic_adpcm, p, MIC_ADPCM_FRAME_SIZE);
	abuf_mic_add ((u32 *)buff_mic_adpcm);
}

_attribute_ram_code_ void proc_audio (void)
{
	if (att_mic_rcvd)
	{
		tick_adpcm = clock_time ();
		att_mic_rcvd = 0;
	}
	if (clock_time_exceed (tick_adpcm, 200000))
	{
		tick_adpcm = clock_time ();
		abuf_init ();
	}

	//DBG_CHN5_TOGGLE;
	abuf_mic_dec ();
}

#endif

