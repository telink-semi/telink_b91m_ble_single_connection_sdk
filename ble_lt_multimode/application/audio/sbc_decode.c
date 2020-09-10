/********************************************************************************************************
 * @file	sbc_decode.c
 *
 * @brief	This is the source file for B91
 *
 * @author	Ble Group
 * @date	2019
 *
 * @par     Copyright (c) 2019, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#include 	"../../tl_common.h"
#include 	"../../drivers.h"
#include	"audio_config.h"
#include	"sbc.h"



#if (TL_AUDIO_MODE == TL_AUDIO_DONGLE_SBC_HID)					//Dongle

#if 0
//
//#define SBC_SAMPLE    		  8
//
//#define CHAR_BIT              8
//
///* sampling frequency */
//#define SBC_FREQ_16000        0x00
//#define SBC_FREQ_32000        0x01
//#define SBC_FREQ_44100        0x02
//#define SBC_FREQ_48000        0x03
//
///* blocks */
//#define SBC_BLK_4             0x00
//#define SBC_BLK_8             0x01
//#define SBC_BLK_12            0x02
//#define SBC_BLK_16            0x03
//
///* channel mode */
//#define SBC_MODE_MONO         0x00
//#define SBC_MODE_DUAL_CHANNEL 0x01
//#define SBC_MODE_STEREO       0x02
//#define SBC_MODE_JOINT_STEREO 0x03
//
///* allocation method */
//#define SBC_AM_LOUDNESS       0x00
//#define SBC_AM_SNR            0x01
//
///* subbands */
//#define SBC_SB_4              0x00
//#define SBC_SB_8              0x01
//
///* data endianess */
//#define SBC_LE                0x00
//#define SBC_BE                0x01
//
//#define SBC_ERR               (-1)
//
//// SBC
//#define SBC_Streamline_Frame_Header		1
//
//#define MSBC_SYNCWORD         0x9C
//#define MSBC_CHANNELS         1
//#define MSBC_SUBBANDS         8
//
//// FRAME_LEN ---> 23 (MIC_SHORT_DEC_SIZE           80)
//#define MSBC_BLOCKS     	  10
//#define MSBC_BITPOOL		  12
//
////#define MSBC_BLOCKS     		15// SBC_SAMPLE == 12
////#define MSBC_BITPOOL			8
//
//
//#define SCALE_OUT_BITS        15
//#define SBC_X_BUFFER_SIZE     328
//
///* This structure contains an unpacked SBC frame.
//   Yes, there is probably quite some unused space herein */
//struct sbc_frame {
//    uint8_t frequency;
//    uint8_t block_mode;
//    uint8_t blocks;
//    uint8_t channels;
//    enum {
//        MONO          = SBC_MODE_MONO,
//        DUAL_CHANNEL  = SBC_MODE_DUAL_CHANNEL,
//        STEREO        = SBC_MODE_STEREO,
//        JOINT_STEREO  = SBC_MODE_JOINT_STEREO
//    } mode;
//    enum {
//        LOUDNESS      = SBC_AM_LOUDNESS,
//        SNR           = SBC_AM_SNR
//    } allocation;
//    uint8_t subband_mode;
//    uint8_t subbands;
//    uint8_t bitpool;
//
//    /* bit number x set means joint stereo has been used in subband x */
//    uint8_t joint;
//
//    uint16_t codesize;
//    uint16_t length;
//    uint16_t resv;
//
//    /* only the lower 4 bits of every element are to be used */
//    uint32_t scale_factor[MSBC_CHANNELS][MSBC_SUBBANDS];
//    /* modified subband samples */
//    int32_t sb_sample[MSBC_BLOCKS][MSBC_CHANNELS][MSBC_SUBBANDS];
//
//    /* original pcm audio samples */
//    int16_t pcm_sample[MSBC_CHANNELS][MSBC_BLOCKS * MSBC_SUBBANDS];
//
//};
//
//struct sbc_decoder_state {
//    int subbands;
//    int32_t V[MSBC_CHANNELS][170];
//    int offset[MSBC_CHANNELS][16];
//};
//
//struct sbc_priv {
//    char init;
//    char rsv[3];
//    struct sbc_frame frame;
//    struct sbc_decoder_state dec_state;
//    int (*unpack_frame)(const uint8_t *data, struct sbc_frame *frame,
//            size_t len);
//
//};
//
//struct sbc_struct {
//    uint8_t frequency;
//    uint8_t blocks;
//    uint8_t subbands;
//    uint8_t mode;
//    uint8_t allocation;
//    uint8_t bitpool;
//    uint8_t endian;
//    uint8_t rsv;
//
//    struct sbc_priv priv;
//};
//
//typedef struct sbc_struct sbc_t;
//
//_attribute_ram_code_ uint64_t mul64(uint64_t x, uint32_t y)
//{
//    uint8_t i;
//    uint64_t sum = 0;
//
//    for (i = 0; i < 32; i ++) {
//        if (y & (1 << i)) {
//            /*
//             * little endian
//             */
//            uint64_t temp;
//            uint32_t x1, x2;
//            x1 = *((uint32_t *)&x);  // low word of dword
//            x2 = *(((uint32_t *)&x) + 1); // high word of dword
//            x2 = x2 << i;
//            x2 = x2 | ((x1 >> (32 - i)) & ((1 << i) - 1));
//            x1 = x1 << i;
//            *(uint32_t *)(&temp) = x1;  // low word of dword
//            *((uint32_t *)(&temp) + 1) = x2;  // high word of dword
//            sum += temp;
//        }
//    }
//    return sum;
//}
//
//_attribute_ram_code_ uint64_t shift_left64(uint64_t x, uint8_t bits)
//{
//    uint64_t temp;
//    uint32_t x1, x2;
//    volatile unsigned long * ptr, * tmp_ptr;
//
//    ptr =(uint32_t *)&x;
//    tmp_ptr = (uint32_t *)&temp;
//    if (bits == 0)
//        return x;
//
//    if (bits < 32) {
//        x1 = *(ptr);  // low word of dword
//        x2 = *(ptr + 1); // high word of dword
//      //  my_printf("slt = %04x, %04x\n",x2,x1);
//        x2 = x2 << bits;
//      //  my_printf("slt = %04x, %04x\n",x2,x1);
//        x2 = x2 | ((x1 >> (32 - bits)) & ((1 << bits) - 1));
//      //  my_printf("slt = %04x, %04x\n",x2,x1);
//        x1 = x1 << bits;
//      //  my_printf("slt = %04x, %04x\n",x2,x1);
//    } else if (bits < 64){
//    	x1 = *(ptr);  // low word of dword
//    	x2 = 0;
//    	x1 = x1 << (bits - 32);
//    } else {
//    	x1 = 0;
//    	x2 = 0;
//    }
//    *(tmp_ptr) = x1;  // low word of dword
//    *(tmp_ptr + 1) = x2;  // high word of dword
//
//    return temp;
//}
//
//_attribute_ram_code_ uint64_t div64(uint64_t x, uint16_t y)
//{
//    uint64_t res = 0;
//    volatile unsigned long  * tmp_ptr;
//    volatile unsigned short * tmp16_ptr;
//    volatile unsigned long   tmp_long;
//    volatile uint16_t *x1 = (uint16_t *)&x;  // low word of dword
//    volatile uint32_t rest = *(((uint32_t *)&x) + 1);  // high word of dword
//
//    tmp_ptr = (uint32_t *)&res;
//    tmp16_ptr = (uint16_t *)&res;
//    //tmp_ptr++;
//    *(tmp_ptr+1) = rest/y;
//    //*(((uint32_t *)&res) + 1) = rest / y;
//    tmp_long = y;
//    //my_printf("rest = %04x, %04x\n",rest,tmp_long);
//
//    rest = rest % y;
//    rest = (rest << 16) | x1[1];
//    *(tmp16_ptr + 1) = rest / y;
//
//    rest = rest % y;
//    rest = (rest << 16) | x1[0];
//    *tmp16_ptr = rest / y;
//
//    //my_printf("resut = %04x, %04x\n",*(tmp_ptr+1),*tmp_ptr);
//    //while(1);
//    return res;
//}
//
//#define fabs(x) ((x) < 0 ? -(x) : (x))
///* C does not provide an explicit arithmetic shift right but this will
//   always be correct and every compiler *should* generate optimal code */
//#define ASR(val, bits) ((-2 >> 1 == -1) ? \
//         ((int32_t)(val)) >> (bits) : ((int32_t) (val)) / (1 << (bits)))
//
//#define SCALE_SPROTO4_TBL    	12
//#define SCALE_SPROTO8_TBL    	14
//#define SCALE_NPROTO4_TBL   	11
//#define SCALE_NPROTO8_TBL    	11
//#define SCALE4_STAGED1_BITS  	15
//#define SCALE4_STAGED2_BITS  	16
//#define SCALE8_STAGED1_BITS  	15
//#define SCALE8_STAGED2_BITS  	16
//
//#define SCALE4_STAGED1(src) 	ASR(src, SCALE4_STAGED1_BITS)
//#define SCALE4_STAGED2(src) 	ASR(src, SCALE4_STAGED2_BITS)
//#define SCALE8_STAGED1(src) 	ASR(src, SCALE8_STAGED1_BITS)
//#define SCALE8_STAGED2(src) 	ASR(src, SCALE8_STAGED2_BITS)
//
//#define SBC_FIXED_0(val) 		{ val = 0; }
//#define MUL(a, b)        		((a) * (b))
//#define MULA(a, b, res)  		((a) * (b) + (res))
//
//#ifdef __GNUC__
//#define SBC_ALWAYS_INLINE inline __attribute__((always_inline))
//#else
//#define SBC_ALWAYS_INLINE inline
//#endif
///* A2DP specification: Appendix B, page 69 */
////const
//static int sbc_offset8[4][8] = {
//    { -2, 0, 0, 0, 0, 0, 0, 1 },
//    { -3, 0, 0, 0, 0, 0, 1, 2 },
//    { -4, 0, 0, 0, 0, 0, 1, 2 },
//    { -4, 0, 0, 0, 0, 0, 1, 2 }
//};
//
///* extra bits of precision for the synthesis filter input data */
//#define SBCDEC_FIXED_EXTRA_BITS 2
//
//#define SS4(val) ASR(val, SCALE_SPROTO4_TBL)
//#define SS8(val) ASR(val, SCALE_SPROTO8_TBL)
//#define SN4(val) ASR(val, SCALE_NPROTO4_TBL + 1 + SBCDEC_FIXED_EXTRA_BITS)
//#define SN8(val) ASR(val, SCALE_NPROTO8_TBL + 1 + SBCDEC_FIXED_EXTRA_BITS)
//
////const
//static int32_t sbc_proto_8_80m0[] = {
//    SS8(0x00000000), SS8(0xfe8d1970), SS8(0xee979f00), SS8(0x11686100),
//    SS8(0x0172e690), SS8(0xfff5bd1a), SS8(0xfdf1c8d4), SS8(0xeac182c0),
//    SS8(0x0d9daee0), SS8(0x00e530da), SS8(0xffe9811d), SS8(0xfd52986c),
//    SS8(0xe7054ca0), SS8(0x0a00d410), SS8(0x006c1de4), SS8(0xffdba705),
//    SS8(0xfcbc98e8), SS8(0xe3889d20), SS8(0x06af2308), SS8(0x000bb7db),
//    SS8(0xffca00ed), SS8(0xfc3fbb68), SS8(0xe071bc00), SS8(0x03bf7948),
//    SS8(0xffc4e05c), SS8(0xffb54b3b), SS8(0xfbedadc0), SS8(0xdde26200),
//    SS8(0x0142291c), SS8(0xff960e94), SS8(0xff9f3e17), SS8(0xfbd8f358),
//    SS8(0xdbf79400), SS8(0xff405e01), SS8(0xff7d4914), SS8(0xff8b1a31),
//    SS8(0xfc1417b8), SS8(0xdac7bb40), SS8(0xfdbb828c), SS8(0xff762170)
//};
//
////const
//static int32_t sbc_proto_8_80m1[] = {
//    SS8(0xff7c272c), SS8(0xfcb02620), SS8(0xda612700), SS8(0xfcb02620),
//    SS8(0xff7c272c), SS8(0xff762170), SS8(0xfdbb828c), SS8(0xdac7bb40),
//    SS8(0xfc1417b8), SS8(0xff8b1a31), SS8(0xff7d4914), SS8(0xff405e01),
//    SS8(0xdbf79400), SS8(0xfbd8f358), SS8(0xff9f3e17), SS8(0xff960e94),
//    SS8(0x0142291c), SS8(0xdde26200), SS8(0xfbedadc0), SS8(0xffb54b3b),
//    SS8(0xffc4e05c), SS8(0x03bf7948), SS8(0xe071bc00), SS8(0xfc3fbb68),
//    SS8(0xffca00ed), SS8(0x000bb7db), SS8(0x06af2308), SS8(0xe3889d20),
//    SS8(0xfcbc98e8), SS8(0xffdba705), SS8(0x006c1de4), SS8(0x0a00d410),
//    SS8(0xe7054ca0), SS8(0xfd52986c), SS8(0xffe9811d), SS8(0x00e530da),
//    SS8(0x0d9daee0), SS8(0xeac182c0), SS8(0xfdf1c8d4), SS8(0xfff5bd1a)
//};
//
////const
//static int32_t synmatrix8[16][8] = {
//    { SN8(0x05a82798), SN8(0xfa57d868), SN8(0xfa57d868), SN8(0x05a82798),
//      SN8(0x05a82798), SN8(0xfa57d868), SN8(0xfa57d868), SN8(0x05a82798) },
//    { SN8(0x0471ced0), SN8(0xf8275a10), SN8(0x018f8b84), SN8(0x06a6d988),
//      SN8(0xf9592678), SN8(0xfe70747c), SN8(0x07d8a5f0), SN8(0xfb8e3130) },
//    { SN8(0x030fbc54), SN8(0xf89be510), SN8(0x07641af0), SN8(0xfcf043ac),
//      SN8(0xfcf043ac), SN8(0x07641af0), SN8(0xf89be510), SN8(0x030fbc54) },
//    { SN8(0x018f8b84), SN8(0xfb8e3130), SN8(0x06a6d988), SN8(0xf8275a10),
//      SN8(0x07d8a5f0), SN8(0xf9592678), SN8(0x0471ced0), SN8(0xfe70747c) },
//    { SN8(0x00000000), SN8(0x00000000), SN8(0x00000000), SN8(0x00000000),
//      SN8(0x00000000), SN8(0x00000000), SN8(0x00000000), SN8(0x00000000) },
//    { SN8(0xfe70747c), SN8(0x0471ced0), SN8(0xf9592678), SN8(0x07d8a5f0),
//      SN8(0xf8275a10), SN8(0x06a6d988), SN8(0xfb8e3130), SN8(0x018f8b84) },
//    { SN8(0xfcf043ac), SN8(0x07641af0), SN8(0xf89be510), SN8(0x030fbc54),
//      SN8(0x030fbc54), SN8(0xf89be510), SN8(0x07641af0), SN8(0xfcf043ac) },
//    { SN8(0xfb8e3130), SN8(0x07d8a5f0), SN8(0xfe70747c), SN8(0xf9592678),
//      SN8(0x06a6d988), SN8(0x018f8b84), SN8(0xf8275a10), SN8(0x0471ced0) },
//    { SN8(0xfa57d868), SN8(0x05a82798), SN8(0x05a82798), SN8(0xfa57d868),
//      SN8(0xfa57d868), SN8(0x05a82798), SN8(0x05a82798), SN8(0xfa57d868) },
//    { SN8(0xf9592678), SN8(0x018f8b84), SN8(0x07d8a5f0), SN8(0x0471ced0),
//      SN8(0xfb8e3130), SN8(0xf8275a10), SN8(0xfe70747c), SN8(0x06a6d988) },
//    { SN8(0xf89be510), SN8(0xfcf043ac), SN8(0x030fbc54), SN8(0x07641af0),
//      SN8(0x07641af0), SN8(0x030fbc54), SN8(0xfcf043ac), SN8(0xf89be510) },
//    { SN8(0xf8275a10), SN8(0xf9592678), SN8(0xfb8e3130), SN8(0xfe70747c),
//      SN8(0x018f8b84), SN8(0x0471ced0), SN8(0x06a6d988), SN8(0x07d8a5f0) },
//    { SN8(0xf8000000), SN8(0xf8000000), SN8(0xf8000000), SN8(0xf8000000),
//      SN8(0xf8000000), SN8(0xf8000000), SN8(0xf8000000), SN8(0xf8000000) },
//    { SN8(0xf8275a10), SN8(0xf9592678), SN8(0xfb8e3130), SN8(0xfe70747c),
//      SN8(0x018f8b84), SN8(0x0471ced0), SN8(0x06a6d988), SN8(0x07d8a5f0) },
//    { SN8(0xf89be510), SN8(0xfcf043ac), SN8(0x030fbc54), SN8(0x07641af0),
//      SN8(0x07641af0), SN8(0x030fbc54), SN8(0xfcf043ac), SN8(0xf89be510) },
//    { SN8(0xf9592678), SN8(0x018f8b84), SN8(0x07d8a5f0), SN8(0x0471ced0),
//      SN8(0xfb8e3130), SN8(0xf8275a10), SN8(0xfe70747c), SN8(0x06a6d988) }
//};
//
//#ifdef SBC_HIGH_PRECISION
//#define FIXED_A int64_t /* data type for fixed point accumulator */
//#define FIXED_T int32_t /* data type for fixed point constants */
//#define SBC_FIXED_EXTRA_BITS 16
//#else
//#define FIXED_A int32_t /* data type for fixed point accumulator */
//#define FIXED_T int16_t /* data type for fixed point constants */
//#define SBC_FIXED_EXTRA_BITS 0
//#endif
//
///* A2DP specification: Section 12.8 Tables
// *
// * Original values are premultiplied by 4 for better precision (that is the
// * maximum which is possible without overflows)
// *
// * Note: in each block of 16 numbers sign was changed for elements 4, 13, 14, 15
// * in order to compensate the same change applied to cos_table_fixed_8
// */
//#define SBC_PROTO_FIXED8_SCALE \
//    ((sizeof(FIXED_T) * CHAR_BIT - 1) - SBC_FIXED_EXTRA_BITS + 1)
//#define F_PROTO8(x) (FIXED_A) ((x * 2) * \
//    ((FIXED_A) 1 << (sizeof(FIXED_T) * CHAR_BIT - 1)) + 0.5)
//#define F(x) F_PROTO8(x)
////const
//static FIXED_T _sbc_proto_fixed8[80] = {
//    F(0.00000000E+00),  F(1.56575398E-04),
//    F(3.43256425E-04),  F(5.54620202E-04),
//    -F(8.23919506E-04),  F(1.13992507E-03),
//    F(1.47640169E-03),  F(1.78371725E-03),
//    F(2.01182542E-03),  F(2.10371989E-03),
//    F(1.99454554E-03),  F(1.61656283E-03),
//    F(9.02154502E-04),  F(1.78805361E-04),
//    F(1.64973098E-03),  F(3.49717454E-03),
//
//    F(5.65949473E-03),  F(8.02941163E-03),
//    F(1.04584443E-02),  F(1.27472335E-02),
//    -F(1.46525263E-02),  F(1.59045603E-02),
//    F(1.62208471E-02),  F(1.53184106E-02),
//    F(1.29371806E-02),  F(8.85757540E-03),
//    F(2.92408442E-03), -F(4.91578024E-03),
//    -F(1.46404076E-02),  F(2.61098752E-02),
//    F(3.90751381E-02),  F(5.31873032E-02),
//
//    F(6.79989431E-02),  F(8.29847578E-02),
//    F(9.75753918E-02),  F(1.11196689E-01),
//    -F(1.23264548E-01),  F(1.33264415E-01),
//    F(1.40753505E-01),  F(1.45389847E-01),
//    F(1.46955068E-01),  F(1.45389847E-01),
//    F(1.40753505E-01),  F(1.33264415E-01),
//    F(1.23264548E-01), -F(1.11196689E-01),
//    -F(9.75753918E-02), -F(8.29847578E-02),
//
//    -F(6.79989431E-02), -F(5.31873032E-02),
//    -F(3.90751381E-02), -F(2.61098752E-02),
//    F(1.46404076E-02), -F(4.91578024E-03),
//    F(2.92408442E-03),  F(8.85757540E-03),
//    F(1.29371806E-02),  F(1.53184106E-02),
//    F(1.62208471E-02),  F(1.59045603E-02),
//    F(1.46525263E-02), -F(1.27472335E-02),
//    -F(1.04584443E-02), -F(8.02941163E-03),
//
//    -F(5.65949473E-03), -F(3.49717454E-03),
//    -F(1.64973098E-03), -F(1.78805361E-04),
//    -F(9.02154502E-04),  F(1.61656283E-03),
//    F(1.99454554E-03),  F(2.10371989E-03),
//    F(2.01182542E-03),  F(1.78371725E-03),
//    F(1.47640169E-03),  F(1.13992507E-03),
//    F(8.23919506E-04), -F(5.54620202E-04),
//    -F(3.43256425E-04), -F(1.56575398E-04),
//};
//#undef F
//
///*
// * To produce this cosine matrix in Octave:
// *
// * b = zeros(8, 16);
// * for i = 0:7
// * for j = 0:15 b(i+1, j+1) = cos((i + 0.5) * (j - 4) * (pi/8))
// * endfor endfor;
// * printf("%.10f, ", b');
// *
// * Note: in each block of 16 numbers sign was changed for elements 4, 13, 14, 15
// *
// * Change of sign for element 4 allows to replace constant 1.0 (not
// * representable in Q15 format) with -1.0 (fine with Q15).
// * Changed signs for elements 13, 14, 15 allow to have more similar constants
// * and simplify subband filter function code.
// */
//#define SBC_COS_TABLE_FIXED8_SCALE \
//    ((sizeof(FIXED_T) * CHAR_BIT - 1) + SBC_FIXED_EXTRA_BITS)
//#define F_COS8(x) (FIXED_A) ((x) * \
//    ((FIXED_A) 1 << (sizeof(FIXED_T) * CHAR_BIT - 1)) + 0.5)
//#define F(x) F_COS8(x)
////const
//static FIXED_T cos_table_fixed_8[128] = {
//    F(0.7071067812),  F(0.8314696123),  F(0.9238795325),  F(0.9807852804),
//    -F(1.0000000000),  F(0.9807852804),  F(0.9238795325),  F(0.8314696123),
//    F(0.7071067812),  F(0.5555702330),  F(0.3826834324),  F(0.1950903220),
//    F(0.0000000000),  F(0.1950903220),  F(0.3826834324),  F(0.5555702330),
//
//    -F(0.7071067812), -F(0.1950903220),  F(0.3826834324),  F(0.8314696123),
//    -F(1.0000000000),  F(0.8314696123),  F(0.3826834324), -F(0.1950903220),
//    -F(0.7071067812), -F(0.9807852804), -F(0.9238795325), -F(0.5555702330),
//    -F(0.0000000000), -F(0.5555702330), -F(0.9238795325), -F(0.9807852804),
//
//    -F(0.7071067812), -F(0.9807852804), -F(0.3826834324),  F(0.5555702330),
//    -F(1.0000000000),  F(0.5555702330), -F(0.3826834324), -F(0.9807852804),
//    -F(0.7071067812),  F(0.1950903220),  F(0.9238795325),  F(0.8314696123),
//    F(0.0000000000),  F(0.8314696123),  F(0.9238795325),  F(0.1950903220),
//
//    F(0.7071067812), -F(0.5555702330), -F(0.9238795325),  F(0.1950903220),
//    -F(1.0000000000),  F(0.1950903220), -F(0.9238795325), -F(0.5555702330),
//    F(0.7071067812),  F(0.8314696123), -F(0.3826834324), -F(0.9807852804),
//    -F(0.0000000000), -F(0.9807852804), -F(0.3826834324),  F(0.8314696123),
//
//    F(0.7071067812),  F(0.5555702330), -F(0.9238795325), -F(0.1950903220),
//    -F(1.0000000000), -F(0.1950903220), -F(0.9238795325),  F(0.5555702330),
//    F(0.7071067812), -F(0.8314696123), -F(0.3826834324),  F(0.9807852804),
//    F(0.0000000000),  F(0.9807852804), -F(0.3826834324), -F(0.8314696123),
//
//    -F(0.7071067812),  F(0.9807852804), -F(0.3826834324), -F(0.5555702330),
//    -F(1.0000000000), -F(0.5555702330), -F(0.3826834324),  F(0.9807852804),
//    -F(0.7071067812), -F(0.1950903220),  F(0.9238795325), -F(0.8314696123),
//    -F(0.0000000000), -F(0.8314696123),  F(0.9238795325), -F(0.1950903220),
//
//    -F(0.7071067812),  F(0.1950903220),  F(0.3826834324), -F(0.8314696123),
//    -F(1.0000000000), -F(0.8314696123),  F(0.3826834324),  F(0.1950903220),
//    -F(0.7071067812),  F(0.9807852804), -F(0.9238795325),  F(0.5555702330),
//    -F(0.0000000000),  F(0.5555702330), -F(0.9238795325),  F(0.9807852804),
//
//    F(0.7071067812), -F(0.8314696123),  F(0.9238795325), -F(0.9807852804),
//    -F(1.0000000000), -F(0.9807852804),  F(0.9238795325), -F(0.8314696123),
//    F(0.7071067812), -F(0.5555702330),  F(0.3826834324), -F(0.1950903220),
//    -F(0.0000000000), -F(0.1950903220),  F(0.3826834324), -F(0.5555702330),
//};
//#undef F
//
///*
// * Enforce 16 byte alignment for the data, which is supposed to be used
// * with SIMD optimized code.
// */
//
//#define SBC_ALIGN_BITS 2
//#define SBC_ALIGN_MASK ((1 << (SBC_ALIGN_BITS)) - 1)
//
///*
// * Constant tables for the use in SIMD optimized analysis filters
// * Each table consists of two parts:
// * 1. reordered "proto" table
// * 2. reordered "cos" table
// *
// * Due to non-symmetrical reordering, separate tables for "even"
// * and "odd" cases are needed
// */
//
////const
//static FIXED_T analysis_consts_fixed8_simd_even[80 + 64] = {
//#define C0 2.7906148894
//#define C1 2.4270044280
//#define C2 2.8015616024
//#define C3 3.1710363741
//#define C4 2.5377944043
//#define C5 2.4270044280
//#define C6 2.8015616024
//#define C7 3.1710363741
//
//#define F(x) F_PROTO8(x)
//     F(0.00000000E+00 * C0),  F(2.01182542E-03 * C0),
//     F(1.56575398E-04 * C1),  F(1.78371725E-03 * C1),
//     F(3.43256425E-04 * C2),  F(1.47640169E-03 * C2),
//     F(5.54620202E-04 * C3),  F(1.13992507E-03 * C3),
//    -F(8.23919506E-04 * C4),  F(0.00000000E+00 * C4),
//     F(2.10371989E-03 * C5),  F(3.49717454E-03 * C5),
//     F(1.99454554E-03 * C6),  F(1.64973098E-03 * C6),
//     F(1.61656283E-03 * C7),  F(1.78805361E-04 * C7),
//     F(5.65949473E-03 * C0),  F(1.29371806E-02 * C0),
//     F(8.02941163E-03 * C1),  F(1.53184106E-02 * C1),
//     F(1.04584443E-02 * C2),  F(1.62208471E-02 * C2),
//     F(1.27472335E-02 * C3),  F(1.59045603E-02 * C3),
//    -F(1.46525263E-02 * C4),  F(0.00000000E+00 * C4),
//     F(8.85757540E-03 * C5),  F(5.31873032E-02 * C5),
//     F(2.92408442E-03 * C6),  F(3.90751381E-02 * C6),
//    -F(4.91578024E-03 * C7),  F(2.61098752E-02 * C7),
//     F(6.79989431E-02 * C0),  F(1.46955068E-01 * C0),
//     F(8.29847578E-02 * C1),  F(1.45389847E-01 * C1),
//     F(9.75753918E-02 * C2),  F(1.40753505E-01 * C2),
//     F(1.11196689E-01 * C3),  F(1.33264415E-01 * C3),
//    -F(1.23264548E-01 * C4),  F(0.00000000E+00 * C4),
//     F(1.45389847E-01 * C5), -F(8.29847578E-02 * C5),
//     F(1.40753505E-01 * C6), -F(9.75753918E-02 * C6),
//     F(1.33264415E-01 * C7), -F(1.11196689E-01 * C7),
//    -F(6.79989431E-02 * C0),  F(1.29371806E-02 * C0),
//    -F(5.31873032E-02 * C1),  F(8.85757540E-03 * C1),
//    -F(3.90751381E-02 * C2),  F(2.92408442E-03 * C2),
//    -F(2.61098752E-02 * C3), -F(4.91578024E-03 * C3),
//     F(1.46404076E-02 * C4),  F(0.00000000E+00 * C4),
//     F(1.53184106E-02 * C5), -F(8.02941163E-03 * C5),
//     F(1.62208471E-02 * C6), -F(1.04584443E-02 * C6),
//     F(1.59045603E-02 * C7), -F(1.27472335E-02 * C7),
//    -F(5.65949473E-03 * C0),  F(2.01182542E-03 * C0),
//    -F(3.49717454E-03 * C1),  F(2.10371989E-03 * C1),
//    -F(1.64973098E-03 * C2),  F(1.99454554E-03 * C2),
//    -F(1.78805361E-04 * C3),  F(1.61656283E-03 * C3),
//    -F(9.02154502E-04 * C4),  F(0.00000000E+00 * C4),
//     F(1.78371725E-03 * C5), -F(1.56575398E-04 * C5),
//     F(1.47640169E-03 * C6), -F(3.43256425E-04 * C6),
//     F(1.13992507E-03 * C7), -F(5.54620202E-04 * C7),
//#undef F
//#define F(x) F_COS8(x)
//     F(0.7071067812 / C0),  F(0.8314696123 / C1),
//    -F(0.7071067812 / C0), -F(0.1950903220 / C1),
//    -F(0.7071067812 / C0), -F(0.9807852804 / C1),
//     F(0.7071067812 / C0), -F(0.5555702330 / C1),
//     F(0.7071067812 / C0),  F(0.5555702330 / C1),
//    -F(0.7071067812 / C0),  F(0.9807852804 / C1),
//    -F(0.7071067812 / C0),  F(0.1950903220 / C1),
//     F(0.7071067812 / C0), -F(0.8314696123 / C1),
//     F(0.9238795325 / C2),  F(0.9807852804 / C3),
//     F(0.3826834324 / C2),  F(0.8314696123 / C3),
//    -F(0.3826834324 / C2),  F(0.5555702330 / C3),
//    -F(0.9238795325 / C2),  F(0.1950903220 / C3),
//    -F(0.9238795325 / C2), -F(0.1950903220 / C3),
//    -F(0.3826834324 / C2), -F(0.5555702330 / C3),
//     F(0.3826834324 / C2), -F(0.8314696123 / C3),
//     F(0.9238795325 / C2), -F(0.9807852804 / C3),
//    -F(1.0000000000 / C4),  F(0.5555702330 / C5),
//    -F(1.0000000000 / C4), -F(0.9807852804 / C5),
//    -F(1.0000000000 / C4),  F(0.1950903220 / C5),
//    -F(1.0000000000 / C4),  F(0.8314696123 / C5),
//    -F(1.0000000000 / C4), -F(0.8314696123 / C5),
//    -F(1.0000000000 / C4), -F(0.1950903220 / C5),
//    -F(1.0000000000 / C4),  F(0.9807852804 / C5),
//    -F(1.0000000000 / C4), -F(0.5555702330 / C5),
//     F(0.3826834324 / C6),  F(0.1950903220 / C7),
//    -F(0.9238795325 / C6), -F(0.5555702330 / C7),
//     F(0.9238795325 / C6),  F(0.8314696123 / C7),
//    -F(0.3826834324 / C6), -F(0.9807852804 / C7),
//    -F(0.3826834324 / C6),  F(0.9807852804 / C7),
//     F(0.9238795325 / C6), -F(0.8314696123 / C7),
//    -F(0.9238795325 / C6),  F(0.5555702330 / C7),
//     F(0.3826834324 / C6), -F(0.1950903220 / C7),
//#undef F
//
//#undef C0
//#undef C1
//#undef C2
//#undef C3
//#undef C4
//#undef C5
//#undef C6
//#undef C7
//};
//
////const
//static FIXED_T analysis_consts_fixed8_simd_odd[80 + 64] = {
//#define C0 2.5377944043
//#define C1 2.4270044280
//#define C2 2.8015616024
//#define C3 3.1710363741
//#define C4 2.7906148894
//#define C5 2.4270044280
//#define C6 2.8015616024
//#define C7 3.1710363741
//
//#define F(x) F_PROTO8(x)
//     F(0.00000000E+00 * C0), -F(8.23919506E-04 * C0),
//     F(1.56575398E-04 * C1),  F(1.78371725E-03 * C1),
//     F(3.43256425E-04 * C2),  F(1.47640169E-03 * C2),
//     F(5.54620202E-04 * C3),  F(1.13992507E-03 * C3),
//     F(2.01182542E-03 * C4),  F(5.65949473E-03 * C4),
//     F(2.10371989E-03 * C5),  F(3.49717454E-03 * C5),
//     F(1.99454554E-03 * C6),  F(1.64973098E-03 * C6),
//     F(1.61656283E-03 * C7),  F(1.78805361E-04 * C7),
//     F(0.00000000E+00 * C0), -F(1.46525263E-02 * C0),
//     F(8.02941163E-03 * C1),  F(1.53184106E-02 * C1),
//     F(1.04584443E-02 * C2),  F(1.62208471E-02 * C2),
//     F(1.27472335E-02 * C3),  F(1.59045603E-02 * C3),
//     F(1.29371806E-02 * C4),  F(6.79989431E-02 * C4),
//     F(8.85757540E-03 * C5),  F(5.31873032E-02 * C5),
//     F(2.92408442E-03 * C6),  F(3.90751381E-02 * C6),
//    -F(4.91578024E-03 * C7),  F(2.61098752E-02 * C7),
//     F(0.00000000E+00 * C0), -F(1.23264548E-01 * C0),
//     F(8.29847578E-02 * C1),  F(1.45389847E-01 * C1),
//     F(9.75753918E-02 * C2),  F(1.40753505E-01 * C2),
//     F(1.11196689E-01 * C3),  F(1.33264415E-01 * C3),
//     F(1.46955068E-01 * C4), -F(6.79989431E-02 * C4),
//     F(1.45389847E-01 * C5), -F(8.29847578E-02 * C5),
//     F(1.40753505E-01 * C6), -F(9.75753918E-02 * C6),
//     F(1.33264415E-01 * C7), -F(1.11196689E-01 * C7),
//     F(0.00000000E+00 * C0),  F(1.46404076E-02 * C0),
//    -F(5.31873032E-02 * C1),  F(8.85757540E-03 * C1),
//    -F(3.90751381E-02 * C2),  F(2.92408442E-03 * C2),
//    -F(2.61098752E-02 * C3), -F(4.91578024E-03 * C3),
//     F(1.29371806E-02 * C4), -F(5.65949473E-03 * C4),
//     F(1.53184106E-02 * C5), -F(8.02941163E-03 * C5),
//     F(1.62208471E-02 * C6), -F(1.04584443E-02 * C6),
//     F(1.59045603E-02 * C7), -F(1.27472335E-02 * C7),
//     F(0.00000000E+00 * C0), -F(9.02154502E-04 * C0),
//    -F(3.49717454E-03 * C1),  F(2.10371989E-03 * C1),
//    -F(1.64973098E-03 * C2),  F(1.99454554E-03 * C2),
//    -F(1.78805361E-04 * C3),  F(1.61656283E-03 * C3),
//     F(2.01182542E-03 * C4),  F(0.00000000E+00 * C4),
//     F(1.78371725E-03 * C5), -F(1.56575398E-04 * C5),
//     F(1.47640169E-03 * C6), -F(3.43256425E-04 * C6),
//     F(1.13992507E-03 * C7), -F(5.54620202E-04 * C7),
//#undef F
//#define F(x) F_COS8(x)
//    -F(1.0000000000 / C0),  F(0.8314696123 / C1),
//    -F(1.0000000000 / C0), -F(0.1950903220 / C1),
//    -F(1.0000000000 / C0), -F(0.9807852804 / C1),
//    -F(1.0000000000 / C0), -F(0.5555702330 / C1),
//    -F(1.0000000000 / C0),  F(0.5555702330 / C1),
//    -F(1.0000000000 / C0),  F(0.9807852804 / C1),
//    -F(1.0000000000 / C0),  F(0.1950903220 / C1),
//    -F(1.0000000000 / C0), -F(0.8314696123 / C1),
//     F(0.9238795325 / C2),  F(0.9807852804 / C3),
//     F(0.3826834324 / C2),  F(0.8314696123 / C3),
//    -F(0.3826834324 / C2),  F(0.5555702330 / C3),
//    -F(0.9238795325 / C2),  F(0.1950903220 / C3),
//    -F(0.9238795325 / C2), -F(0.1950903220 / C3),
//    -F(0.3826834324 / C2), -F(0.5555702330 / C3),
//     F(0.3826834324 / C2), -F(0.8314696123 / C3),
//     F(0.9238795325 / C2), -F(0.9807852804 / C3),
//     F(0.7071067812 / C4),  F(0.5555702330 / C5),
//    -F(0.7071067812 / C4), -F(0.9807852804 / C5),
//    -F(0.7071067812 / C4),  F(0.1950903220 / C5),
//     F(0.7071067812 / C4),  F(0.8314696123 / C5),
//     F(0.7071067812 / C4), -F(0.8314696123 / C5),
//    -F(0.7071067812 / C4), -F(0.1950903220 / C5),
//    -F(0.7071067812 / C4),  F(0.9807852804 / C5),
//     F(0.7071067812 / C4), -F(0.5555702330 / C5),
//     F(0.3826834324 / C6),  F(0.1950903220 / C7),
//    -F(0.9238795325 / C6), -F(0.5555702330 / C7),
//     F(0.9238795325 / C6),  F(0.8314696123 / C7),
//    -F(0.3826834324 / C6), -F(0.9807852804 / C7),
//    -F(0.3826834324 / C6),  F(0.9807852804 / C7),
//     F(0.9238795325 / C6), -F(0.8314696123 / C7),
//    -F(0.9238795325 / C6),  F(0.5555702330 / C7),
//     F(0.3826834324 / C6), -F(0.1950903220 / C7),
//#undef F
//
//#undef C0
//#undef C1
//#undef C2
//#undef C3
//#undef C4
//#undef C5
//#undef C6
//#undef C7
//};
//
//
//#define A2DP_SAMPLING_FREQ_16000        (1 << 3)
//#define A2DP_SAMPLING_FREQ_32000        (1 << 2)
//#define A2DP_SAMPLING_FREQ_44100        (1 << 1)
//#define A2DP_SAMPLING_FREQ_48000        (1 << 0)
//
//#define A2DP_CHANNEL_MODE_MONO          (1 << 3)
//#define A2DP_CHANNEL_MODE_DUAL_CHANNEL  (1 << 2)
//#define A2DP_CHANNEL_MODE_STEREO        (1 << 1)
//#define A2DP_CHANNEL_MODE_JOINT_STEREO  (1 << 0)
//
//#define A2DP_BLOCK_LENGTH_4             (1 << 3)
//#define A2DP_BLOCK_LENGTH_8             (1 << 2)
//#define A2DP_BLOCK_LENGTH_12            (1 << 1)
//#define A2DP_BLOCK_LENGTH_16            (1 << 0)
//
//#define A2DP_SUBBANDS_4                 (1 << 1)
//#define A2DP_SUBBANDS_8                 (1 << 0)
//
//#define A2DP_ALLOCATION_SNR             (1 << 1)
//#define A2DP_ALLOCATION_LOUDNESS        (1 << 0)
//
///*
// * Calculates the CRC-8 of the first len bits in data
// */
////const
//static uint8_t crc_table[256] = {
//    0x00, 0x1D, 0x3A, 0x27, 0x74, 0x69, 0x4E, 0x53,
//    0xE8, 0xF5, 0xD2, 0xCF, 0x9C, 0x81, 0xA6, 0xBB,
//    0xCD, 0xD0, 0xF7, 0xEA, 0xB9, 0xA4, 0x83, 0x9E,
//    0x25, 0x38, 0x1F, 0x02, 0x51, 0x4C, 0x6B, 0x76,
//    0x87, 0x9A, 0xBD, 0xA0, 0xF3, 0xEE, 0xC9, 0xD4,
//    0x6F, 0x72, 0x55, 0x48, 0x1B, 0x06, 0x21, 0x3C,
//    0x4A, 0x57, 0x70, 0x6D, 0x3E, 0x23, 0x04, 0x19,
//    0xA2, 0xBF, 0x98, 0x85, 0xD6, 0xCB, 0xEC, 0xF1,
//    0x13, 0x0E, 0x29, 0x34, 0x67, 0x7A, 0x5D, 0x40,
//    0xFB, 0xE6, 0xC1, 0xDC, 0x8F, 0x92, 0xB5, 0xA8,
//    0xDE, 0xC3, 0xE4, 0xF9, 0xAA, 0xB7, 0x90, 0x8D,
//    0x36, 0x2B, 0x0C, 0x11, 0x42, 0x5F, 0x78, 0x65,
//    0x94, 0x89, 0xAE, 0xB3, 0xE0, 0xFD, 0xDA, 0xC7,
//    0x7C, 0x61, 0x46, 0x5B, 0x08, 0x15, 0x32, 0x2F,
//    0x59, 0x44, 0x63, 0x7E, 0x2D, 0x30, 0x17, 0x0A,
//    0xB1, 0xAC, 0x8B, 0x96, 0xC5, 0xD8, 0xFF, 0xE2,
//    0x26, 0x3B, 0x1C, 0x01, 0x52, 0x4F, 0x68, 0x75,
//    0xCE, 0xD3, 0xF4, 0xE9, 0xBA, 0xA7, 0x80, 0x9D,
//    0xEB, 0xF6, 0xD1, 0xCC, 0x9F, 0x82, 0xA5, 0xB8,
//    0x03, 0x1E, 0x39, 0x24, 0x77, 0x6A, 0x4D, 0x50,
//    0xA1, 0xBC, 0x9B, 0x86, 0xD5, 0xC8, 0xEF, 0xF2,
//    0x49, 0x54, 0x73, 0x6E, 0x3D, 0x20, 0x07, 0x1A,
//    0x6C, 0x71, 0x56, 0x4B, 0x18, 0x05, 0x22, 0x3F,
//    0x84, 0x99, 0xBE, 0xA3, 0xF0, 0xED, 0xCA, 0xD7,
//    0x35, 0x28, 0x0F, 0x12, 0x41, 0x5C, 0x7B, 0x66,
//    0xDD, 0xC0, 0xE7, 0xFA, 0xA9, 0xB4, 0x93, 0x8E,
//    0xF8, 0xE5, 0xC2, 0xDF, 0x8C, 0x91, 0xB6, 0xAB,
//    0x10, 0x0D, 0x2A, 0x37, 0x64, 0x79, 0x5E, 0x43,
//    0xB2, 0xAF, 0x88, 0x95, 0xC6, 0xDB, 0xFC, 0xE1,
//    0x5A, 0x47, 0x60, 0x7D, 0x2E, 0x33, 0x14, 0x09,
//    0x7F, 0x62, 0x45, 0x58, 0x0B, 0x16, 0x31, 0x2C,
//    0x97, 0x8A, 0xAD, 0xB0, 0xE3, 0xFE, 0xD9, 0xC4
//};
//
//
//static sbc_t sbc_de_ctx;
//
//
//
//
//
//
//
//_attribute_ram_code_ uint8_t sbc_crc8(const uint8_t *data, size_t len)
//{
//    uint8_t crc = 0x0f;
//    uint16_t i;
//    uint8_t octet;
//
//    for (i = 0; i < len / 8; i++)
//        crc = crc_table[crc ^ data[i]];
//
//    octet = data[i];
//    for (i = 0; i < len % 8; i++) {
//        char bit = ((octet ^ crc) & 0x80) >> 7;
//
//        crc = ((crc & 0x7f) << 1) ^ (bit ? 0x1d : 0);
//
//        octet = octet << 1;
//    }
//
//    return crc;
//}
//
///*
// * Code straight from the spec to calculate the bits array
// * Takes a pointer to the frame in question, a pointer to the bits array and
// * the sampling frequency (as 2 bit integer)
// */
//_attribute_ram_code_ void sbc_calculate_bits_internal(
//        const struct sbc_frame *frame, int (*bits)[MSBC_SUBBANDS], int subbands)
//{
//    int bitneed[MSBC_CHANNELS][MSBC_SUBBANDS], loudness, max_bitneed, bitcount, slicecount, bitslice;
//    uint8_t sf = frame->frequency;
//
//    if (frame->mode == MONO || frame->mode == DUAL_CHANNEL) {
//        int8_t ch, sb;
//
//        for (ch = 0; ch < frame->channels; ch++) {
//            max_bitneed = 0;
//            if (frame->allocation == SNR) {
//                for (sb = 0; sb < subbands; sb++) {
//                    bitneed[ch][sb] = frame->scale_factor[ch][sb];
//                    if (bitneed[ch][sb] > max_bitneed)
//                        max_bitneed = bitneed[ch][sb];
//                }
//            } else {
//                for (sb = 0; sb < subbands; sb++) {
//                    if (frame->scale_factor[ch][sb] == 0)
//                        bitneed[ch][sb] = -5;
//                    else {
//                        loudness = frame->scale_factor[ch][sb] - sbc_offset8[sf][sb];
//                        if (loudness > 0)
//                            bitneed[ch][sb] = loudness / 2;
//                        else
//                            bitneed[ch][sb] = loudness;
//                    }
//                    if (bitneed[ch][sb] > max_bitneed)
//                        max_bitneed = bitneed[ch][sb];
//#if 0
//                    printf("bn[");putihex(sb, sizeof(sb) * CHAR_BIT);printf("]:");
//                    putihex(bitneed[ch][sb], sizeof(bitneed[ch][sb]) * CHAR_BIT);
//                    printf(", sf[]:");
//                    putihex(frame->scale_factor[ch][sb], sizeof(frame->scale_factor[0][0]) * CHAR_BIT);
//                    printf("\n");
//#endif
//                }
//            }
//
//            bitcount = 0;
//            slicecount = 0;
//            bitslice = max_bitneed + 1;
//
//            do {
//                bitslice--;
//                bitcount += slicecount;
//                slicecount = 0;
//                for (sb = 0; sb < subbands; sb++) {
//#if 0
//                	printf("bn[");putihex(sb, sizeof(sb) * CHAR_BIT);printf("]:");
//                	putihex(bitneed[ch][sb], sizeof(uint16_t) * CHAR_BIT);
//#endif
//                    if ((bitneed[ch][sb] > bitslice + 1) && (bitneed[ch][sb] < bitslice + 16))
//                        slicecount++;
//                    else if (bitneed[ch][sb] == bitslice + 1)
//                        slicecount += 2;
//                }
//#if 0
//                printf("\nb:");putihex(bitcount, sizeof(bitcount) * CHAR_BIT);
//                printf(",s:");putihex(slicecount, sizeof(slicecount) * CHAR_BIT);
//                printf(",p:");putihex(frame->bitpool, sizeof(frame->bitpool) * CHAR_BIT);
//                printf("\n");
//#endif
//            } while (bitcount + slicecount < frame->bitpool);
//
//            if (bitcount + slicecount == frame->bitpool) {
//                bitcount += slicecount;
//                bitslice--;
//            }
//
//            for (sb = 0; sb < subbands; sb++) {
//                if (bitneed[ch][sb] < bitslice + 2)
//                    bits[ch][sb] = 0;
//                else {
//                    bits[ch][sb] = bitneed[ch][sb] - bitslice;
//                    if (bits[ch][sb] > 16)
//                        bits[ch][sb] = 16;
//                }
//            }
//
//            for (sb = 0; bitcount < frame->bitpool &&
//                            sb < subbands; sb++) {
//                if ((bits[ch][sb] >= 2) && (bits[ch][sb] < 16)) {
//                    bits[ch][sb]++;
//                    bitcount++;
//                } else if ((bitneed[ch][sb] == bitslice + 1) && (frame->bitpool > bitcount + 1)) {
//                    bits[ch][sb] = 2;
//                    bitcount += 2;
//                }
//            }
//
//            for (sb = 0; bitcount < frame->bitpool &&
//                            sb < subbands; sb++) {
//                if (bits[ch][sb] < 16) {
//                    bits[ch][sb]++;
//                    bitcount++;
//                }
//            }
//        }
//    }
//}
//
//_attribute_ram_code_ void sbc_calculate_bits(const struct sbc_frame *frame, int (*bits)[MSBC_SUBBANDS])
//{
//    sbc_calculate_bits_internal(frame, bits, MSBC_SUBBANDS);
//}
//
//
///*
// * Unpacks a SBC frame at the beginning of the stream in data,
// * which has at most len bytes into frame.
// * Returns the length in bytes of the packed frame, or a negative
// * value on error. The error codes are:
// *
// *  -1   Data stream too short
// *  -2   Sync byte incorrect
// *  -3   CRC8 incorrect
// *  -4   Bitpool value out of bounds
// */
//
//_attribute_ram_code_ int sbc_unpack_frame_internal(const uint8_t *data,
//        struct sbc_frame *frame, size_t len)
//{
//    uint16_t consumed;
//    /* Will copy the parts of the header that are relevant to crc
//     * calculation here */
//    uint8_t crc_header[11] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
//    int16_t crc_pos = 0;
//    volatile uint64_t tmp;
//    volatile unsigned long * tmp_ptr;
//    uint32_t audio_sample;
//    int8_t ch, sb, blk;    /* channel, subband, block and bit standard counters */
//    int bit;
//    int bits[MSBC_CHANNELS][MSBC_SUBBANDS];        /* bits distribution */
//    uint32_t levels[MSBC_CHANNELS][MSBC_SUBBANDS];    /* levels derived from that */
//
//    consumed = 32;
//
//    crc_header[0] = data[1];
//    crc_header[1] = data[2];
//    crc_pos = 16;
//
//    if (len * 8 < consumed + (4 * frame->subbands * frame->channels))
//        return -1;
//
//    for (ch = 0; ch < frame->channels; ch++) {
//        for (sb = 0; sb < frame->subbands; sb++) {
//            /* FIXME assert(consumed % 4 == 0); */
//            frame->scale_factor[ch][sb] =
//                (data[consumed >> 3] >> (4 - (consumed & 0x7))) & 0x0F;
//            crc_header[crc_pos >> 3] |=
//                frame->scale_factor[ch][sb] << (4 - (crc_pos & 0x7));
//
//            consumed += 4;
//            crc_pos += 4;
//        }
//    }
//
//    if (data[3] != sbc_crc8(crc_header, crc_pos))
//        return -3;
//
//    sbc_calculate_bits(frame, bits);
//
//    for (ch = 0; ch < frame->channels; ch++) {
//        for (sb = 0; sb < frame->subbands; sb++)
//            levels[ch][sb] = (1 << bits[ch][sb]) - 1;
//    }
//
//    for (blk = 0; blk < frame->blocks; blk++) {
//        for (ch = 0; ch < frame->channels; ch++) {
//            for (sb = 0; sb < frame->subbands; sb++) {
//                uint32_t shift;
//
//                if (levels[ch][sb] == 0) {
//                    frame->sb_sample[blk][ch][sb] = 0;
//                    continue;
//                }
//
//                shift = frame->scale_factor[ch][sb] +
//                        1 + SBCDEC_FIXED_EXTRA_BITS;
//
//                audio_sample = 0;
//                for (bit = 0; bit < bits[ch][sb]; bit++) {
//                    if (consumed > len * 8)
//                        return -1;
//
//                    if ((data[consumed >> 3] >> (7 - (consumed & 0x7))) & 0x01)
//                        audio_sample |= 1 << (bits[ch][sb] - bit - 1);
//
//                    consumed++;
//                }
//#if 1
//                tmp_ptr = (int32_t *)&tmp;
//                tmp = audio_sample; //3f
////                my_printf("%04x %04x - %04x\n",*(tmp_ptr+1),*tmp_ptr,levels[ch][sb]);
//                tmp = shift_left64(tmp, 1); //7e
//                tmp = tmp | 1;
//
////                my_printf("%04x %04x - %04x\n",*(tmp_ptr+1),*tmp_ptr,levels[ch][sb]);
//
//                tmp = shift_left64(tmp, shift); //shift 7f000
//
////                my_printf("%04x %04x - %04x\n",*(tmp_ptr+1),*tmp_ptr,levels[ch][sb]);
//
//                tmp = div64(tmp, levels[ch][sb]); //0x1000
//                tmp = tmp - (1 << shift);  //0
//                //frame->sb_sample[blk][ch][sb] = *(int32_t *)&tmp;
////                my_printf("%04x %04x - %04x\n",*(tmp_ptr+1),*tmp_ptr,levels[ch][sb]);
//
//
//                frame->sb_sample[blk][ch][sb] = *tmp_ptr;
//
////                my_printf("[%d,%d,%d],%d\n",blk,ch,sb,frame->sb_sample[blk][ch][sb]);
//
////                if(blk==2)
////                {
////                	while(1);
////                }
////                while(1);
//
//#else
//                frame->sb_sample[blk][ch][sb] = (int32_t)
//                    (((((uint64_t) audio_sample << 1) | 1) << shift) /
//                    levels[ch][sb]) - (1 << shift);
//#endif
//            }
//        }
//    }
//
//    if ((consumed & 0x7) != 0)
//        consumed += 8 - (consumed & 0x7);
//
//    return consumed >> 3;
//}
//
//static int msbc_unpack_frame(const uint8_t *data,
//        struct sbc_frame *frame, size_t len)
//{
//    if (len < 4)
//        return -1;
//
//    if (data[0] != MSBC_SYNCWORD)
//        return -2;
//
////    if (data[1] != 0)
////        return -2;
////    if (data[2] != 0)
////        return -2;
//
//    frame->frequency = SBC_FREQ_16000;
//    frame->block_mode = SBC_BLK_4;
//    frame->blocks = MSBC_BLOCKS;
//    frame->allocation = LOUDNESS;
//    frame->mode = MONO;
//    frame->channels = MSBC_CHANNELS;
//    frame->subband_mode = 1;
//    frame->subbands = MSBC_SUBBANDS;
//    frame->bitpool = MSBC_BITPOOL;
//
//    return sbc_unpack_frame_internal(data, frame, len);
//}
//_attribute_ram_code_ int sbc_init_msbc(sbc_t *sbc)
//{
//    struct sbc_priv *priv;
//
//    if (!sbc)
//        return SBC_ERR;
//
//    tmemset(sbc, 0, sizeof(sbc_t));
//    priv = &sbc->priv;
//    priv->unpack_frame = msbc_unpack_frame;
//
//    sbc->frequency = SBC_FREQ_16000;
//    sbc->blocks = MSBC_BLOCKS;
//    sbc->subbands = SBC_SB_8;
//    sbc->mode = SBC_MODE_MONO;
//    sbc->allocation = SBC_AM_LOUDNESS;
//    sbc->bitpool = MSBC_BITPOOL;
//    sbc->endian = SBC_LE;
//
//    return 0;
//}
//
//void msbc_init_ctx()
//{
//    if (sbc_de_ctx.priv.init != 1)
//        sbc_init_msbc(&sbc_de_ctx);
//}
//
//_attribute_ram_code_ void sbc_decoder_init(struct sbc_decoder_state *state,
//                    const struct sbc_frame *frame)
//{
//    int8_t i, ch;
//
//    tmemset(state->V, 0, sizeof(state->V));
//    state->subbands = frame->subbands;
//
//    for (ch = 0; ch < frame->channels; ch++)
//        for (i = 0; i < frame->subbands * 2; i++)
//            state->offset[ch][i] = (10 * i + 10);
//}
//
//
////SBC_ALWAYS_INLINE int16_t sbc_clip16(int32_t s)
//_attribute_ram_code_ int16_t sbc_clip16(int32_t s)
//{
//    if (s > 0x7FFF)
//        return 0x7FFF;
//    else if (s < -0x8000)
//        return -0x8000;
//    else
//        return s;
//}
//
//_attribute_ram_code_  void sbc_synthesize_eight(struct sbc_decoder_state *state,
//                struct sbc_frame *frame, int ch, int blk)
//{
//    int8_t i, j, k, idx;
//    int *offset = state->offset[ch];
//
//    for (i = 0; i < 16; i++) {
//        /* Shifting */
//        offset[i]--;
//        if (offset[i] < 0) {
//            offset[i] = 159;
//            for (j = 0; j < 9; j++)
//                state->V[ch][j + 160] = state->V[ch][j];
//        }
//
//        /* Distribute the new matrix value to the shifted position */
//        state->V[ch][offset[i]] = SCALE8_STAGED1(
//            MULA(synmatrix8[i][0], frame->sb_sample[blk][ch][0],
//            MULA(synmatrix8[i][1], frame->sb_sample[blk][ch][1],
//            MULA(synmatrix8[i][2], frame->sb_sample[blk][ch][2],
//            MULA(synmatrix8[i][3], frame->sb_sample[blk][ch][3],
//            MULA(synmatrix8[i][4], frame->sb_sample[blk][ch][4],
//            MULA(synmatrix8[i][5], frame->sb_sample[blk][ch][5],
//            MULA(synmatrix8[i][6], frame->sb_sample[blk][ch][6],
//            MUL( synmatrix8[i][7], frame->sb_sample[blk][ch][7])))))))));
//    }
//
//    /* Compute the samples */
//    for (idx = 0, i = 0; i < 8; i++, idx += 5) {
//        k = (i + 8) & 0xf;
//
//        /* Store in output, Q0 */
//        frame->pcm_sample[ch][blk * 8 + i] = sbc_clip16(SCALE8_STAGED1(
//            MULA(state->V[ch][offset[i] + 0], sbc_proto_8_80m0[idx + 0],
//            MULA(state->V[ch][offset[k] + 1], sbc_proto_8_80m1[idx + 0],
//            MULA(state->V[ch][offset[i] + 2], sbc_proto_8_80m0[idx + 1],
//            MULA(state->V[ch][offset[k] + 3], sbc_proto_8_80m1[idx + 1],
//            MULA(state->V[ch][offset[i] + 4], sbc_proto_8_80m0[idx + 2],
//            MULA(state->V[ch][offset[k] + 5], sbc_proto_8_80m1[idx + 2],
//            MULA(state->V[ch][offset[i] + 6], sbc_proto_8_80m0[idx + 3],
//            MULA(state->V[ch][offset[k] + 7], sbc_proto_8_80m1[idx + 3],
//            MULA(state->V[ch][offset[i] + 8], sbc_proto_8_80m0[idx + 4],
//            MUL( state->V[ch][offset[k] + 9], sbc_proto_8_80m1[idx + 4]))))))))))));
//    }
//}
//
//_attribute_ram_code_ int sbc_synthesize_audio(struct sbc_decoder_state *state,
//                        struct sbc_frame *frame)
//{
//    int8_t ch, blk;
//
//    switch (frame->subbands) {
//    case 8:
//        for (ch = 0; ch < frame->channels; ch++) {
//            for (blk = 0; blk < frame->blocks; blk++)
//                sbc_synthesize_eight(state, frame, ch, blk);
//        }
//        return frame->blocks * 8;
//
//    default:
//        break;
//    }
//    return SBC_ERR;
//}
//
//
//
//
//_attribute_ram_code_ void msbc_release_ctx()
//{
//    tmemset(&sbc_de_ctx, 0, sizeof(sbc_t));
//
//}
//
//_attribute_ram_code_ uint32_t sbc_get_frame_length(int type)
//{
//    uint16_t ret;
//    uint8_t subbands, channels, blocks, bitpool;
//    sbc_t *sbc;
////if(type)
////    sbc = &sbc_en_ctx;
////else
//    sbc = &sbc_de_ctx;
//
//    struct sbc_priv *priv = &sbc->priv;
//
//    if (priv->init && priv->frame.bitpool == sbc->bitpool)
//        return priv->frame.length;
//
//    subbands = MSBC_SUBBANDS;
//    blocks = MSBC_BLOCKS;
//    channels = MSBC_CHANNELS;
//    bitpool = MSBC_BITPOOL;
//
//    ret = 4 + (4 * subbands * channels) / 8;
//    /* This term is not always evenly divide so we round it up */
//    ret += ((blocks * channels * bitpool) + 7) / 8;
//
//    return ret;
//}
//
//_attribute_ram_code_ uint32_t sbc_get_codesize(int type)
//{
//    uint8_t subbands, channels, blocks;
//    sbc_t *sbc;
////	if(type)
////		sbc = &sbc_en_ctx;
////	else
//    sbc = &sbc_de_ctx;
//
//    struct sbc_priv *priv = &sbc->priv;
//
//    if (priv->init != 1) {
//        subbands = MSBC_SUBBANDS;
//        blocks = MSBC_BLOCKS;
//        channels = MSBC_CHANNELS;
//    } else {
//        subbands = priv->frame.subbands;
//        blocks = priv->frame.blocks;
//        channels = priv->frame.channels;
//    }
//
//    return subbands * blocks * channels * 2;
//}
//
//_attribute_ram_code_ size_t sbc_decode(const void *input, size_t input_len,
//            void *output, size_t output_len, size_t *written)
//{
//    struct sbc_priv *priv;
//    char *ptr;
//    int8_t i, ch;
//    int16_t framelen, samples;
//
//    sbc_t *sbc = &sbc_de_ctx;
//
//    if (!input)
//        return SBC_ERR;
//
//    msbc_init_ctx();
//
//    priv = &sbc->priv;
//
//	#if SBC_Streamline_Frame_Header
//    u8		abuf_mic_Temp[MIC_ADPCM_FRAME_SIZE + 3];
//    u32		input_len_Temp;
//
//    input_len_Temp = MIC_ADPCM_FRAME_SIZE + 3;
//    abuf_mic_Temp[0] = MSBC_SYNCWORD;
//
//#if (SBC_SAMPLE == 8)
//    abuf_mic_Temp[1] = 0x11;	// Adjust according to actual parameters
//#elif (SBC_SAMPLE == 12)
//    abuf_mic_Temp[1] = 0x21;
//#endif
//
//    abuf_mic_Temp[2] = MSBC_BITPOOL;
//
//	tmemcpy (abuf_mic_Temp + 3, input, input_len);
//
//    framelen = priv->unpack_frame(abuf_mic_Temp, &priv->frame, input_len_Temp);
//	#else
//    framelen = priv->unpack_frame(input, &priv->frame, input_len);
//	#endif
//
////    framelen = priv->unpack_frame(input, &priv->frame, input_len);
//
//    if (!priv->init) {
//        sbc_decoder_init(&priv->dec_state, &priv->frame);
//        priv->init = true;
//
//        sbc->frequency = priv->frame.frequency;
//        sbc->mode = priv->frame.mode;
//        sbc->subbands = priv->frame.subband_mode;
//        sbc->blocks = priv->frame.block_mode;
//        sbc->allocation = priv->frame.allocation;
//        sbc->bitpool = priv->frame.bitpool;
//
//        priv->frame.codesize = sbc_get_codesize(0);
//        priv->frame.length = framelen;
//    } else if (priv->frame.bitpool != sbc->bitpool) {
//        priv->frame.length = framelen;
//        sbc->bitpool = priv->frame.bitpool;
//    }
//
//    if (!output)
//        return framelen;
//
//    if (written)
//        *written = 0;
//
//    if (framelen <= 0)
//        return framelen;
//
//    samples = sbc_synthesize_audio(&priv->dec_state, &priv->frame);
//
//    ptr = output;
//
//    if (output_len < (size_t) (samples * priv->frame.channels * 2))
//        samples = output_len / (priv->frame.channels * 2);
//
//    for (i = 0; i < samples; i++) {
//        for (ch = 0; ch < priv->frame.channels; ch++) {
//            int16_t s;
//            s = priv->frame.pcm_sample[ch][i];
//
//            if (sbc->endian == SBC_BE) {
//                *ptr++ = (s & 0xff00) >> 8;
//                *ptr++ = (s & 0x00ff);
//            } else {
//                *ptr++ = (s & 0x00ff);
//                *ptr++ = (s & 0xff00) >> 8;
//            }
//
//        }
//    }
//
//    if (written)
//        *written = samples * priv->frame.channels * 2;
//
//    return framelen;
//}
#else
#define SBC_SAMPLE    8




#define CHAR_BIT             (8)

/* sampling frequency */
#define SBC_FREQ_16000        0x00
#define SBC_FREQ_32000        0x01
#define SBC_FREQ_44100        0x02
#define SBC_FREQ_48000        0x03

/* blocks */
#define SBC_BLK_4             0x00
#define SBC_BLK_8             0x01
#define SBC_BLK_12            0x02
#define SBC_BLK_16            0x03

/* channel mode */
#define SBC_MODE_MONO         0x00
#define SBC_MODE_DUAL_CHANNEL 0x01
#define SBC_MODE_STEREO       0x02
#define SBC_MODE_JOINT_STEREO 0x03

/* allocation method */
#define SBC_AM_LOUDNESS       0x00
#define SBC_AM_SNR            0x01

/* subbands */
#define SBC_SB_4              0x00
#define SBC_SB_8              0x01

/* data endianess */
#define SBC_LE                0x00
#define SBC_BE                0x01

#define SBC_ERR               (-1)



// mSBC
//	#define MSBC_SYNCWORD         0xAD
//	#define MSBC_CHANNELS         1
//	#define MSBC_SUBBANDS         8
//	#define MSBC_BLOCKS           15
//	#define MSBC_BITPOOL          26


// SBC
	#define SBC_Streamline_Frame_Header		1

	#define MSBC_SYNCWORD         0x9C
	#define MSBC_CHANNELS         1
	#define MSBC_SUBBANDS         8

	// FRAME_LEN ---> 57
//	#define MSBC_BLOCKS           15
//	#define MSBC_BITPOOL          26

	// FRAME_LEN ---> 28
//	#define MSBC_BLOCKS           16
//	#define MSBC_BITPOOL          10

	// FRAME_LEN ---> 29
//	#define MSBC_BLOCKS           15
//	#define MSBC_BITPOOL          11

	// FRAME_LEN ---> 20
//	#define MSBC_BLOCKS           15
//	#define MSBC_BITPOOL          6

	// FRAME_LEN ---> 40
//	#define MSBC_BLOCKS           15
//	#define MSBC_BITPOOL          17

	// FRAME_LEN ---> 39
//	#define MSBC_BLOCKS           15
//	#define MSBC_BITPOOL          16



	// FRAME_LEN ---> 43 (MIC_SHORT_DEC_SIZE           160)
//	#define MSBC_BLOCKS     		20
//	#define MSBC_BITPOOL    		14

	// FRAME_LEN ---> 23 (MIC_SHORT_DEC_SIZE           80)
#if (SBC_SAMPLE == 8)
	#define MSBC_BLOCKS     		10
	#define MSBC_BITPOOL			12
#elif (SBC_SAMPLE == 12)
	#define MSBC_BLOCKS     		15
	#define MSBC_BITPOOL			8
#else

#endif


#define SCALE_OUT_BITS        15
#define SBC_X_BUFFER_SIZE     328



#define MSBC_ENCODE           0

/* This structure contains an unpacked SBC frame.
   Yes, there is probably quite some unused space herein */
struct sbc_frame {
    uint8_t frequency;
    uint8_t block_mode;
    uint8_t blocks;
    uint8_t channels;
    enum {
        MONO          = SBC_MODE_MONO,
        DUAL_CHANNEL  = SBC_MODE_DUAL_CHANNEL,
        STEREO        = SBC_MODE_STEREO,
        JOINT_STEREO  = SBC_MODE_JOINT_STEREO
    } mode;
    enum {
        LOUDNESS      = SBC_AM_LOUDNESS,
        SNR           = SBC_AM_SNR
    } allocation;
    uint8_t subband_mode;
    uint8_t subbands;
    uint8_t bitpool;

    /* bit number x set means joint stereo has been used in subband x */
    uint8_t joint;

    uint16_t codesize;
    uint16_t length;
    uint16_t resv;

    /* only the lower 4 bits of every element are to be used */
    uint32_t scale_factor[MSBC_CHANNELS][MSBC_SUBBANDS];
#if MSBC_ENCODE
    /* raw integer subband samples in the frame */
    int32_t sb_sample_f[MSBC_BLOCKS][MSBC_CHANNELS][MSBC_SUBBANDS];
#else
    /* modified subband samples */
    int32_t sb_sample[MSBC_BLOCKS][MSBC_CHANNELS][MSBC_SUBBANDS];

    /* original pcm audio samples */
    int16_t pcm_sample[MSBC_CHANNELS][MSBC_BLOCKS * MSBC_SUBBANDS];
#endif
};
#if !MSBC_ENCODE
struct sbc_decoder_state {
    int subbands;
    int32_t V[MSBC_CHANNELS][170];
    int offset[MSBC_CHANNELS][16];
};
#else
struct sbc_encoder_state {
    int16_t position;
    /* Number of consecutive blocks handled by the encoder */
    uint8_t increment;
    uint8_t rsv[1];
    int16_t X[MSBC_CHANNELS][SBC_X_BUFFER_SIZE];
    /* Polyphase analysis filter for 4 subbands configuration,
     * it handles "increment" blocks at once */
    /* Polyphase analysis filter for 8 subbands configuration,
     * it handles "increment" blocks at once */
    void (*sbc_analyze_8s)(struct sbc_encoder_state *state,
            int16_t *x, int32_t *out, int out_stride);
    /* Process input data (deinterleave, endian conversion, reordering),
     * depending on the number of subbands and input data byte order */
    int (*sbc_enc_process_input_8s_le)(int position,
            const uint8_t *pcm, int16_t X[MSBC_CHANNELS][SBC_X_BUFFER_SIZE],
            int nsamples, int nchannels);
    /* Scale factors calculation */
    void (*sbc_calc_scalefactors)(int32_t sb_sample_f[MSBC_BLOCKS][MSBC_CHANNELS][MSBC_SUBBANDS],
            uint32_t scale_factor[MSBC_CHANNELS][MSBC_SUBBANDS],
            int blocks, int channels, int subbands);
};
#endif

struct sbc_priv {
    char init;
    char rsv[3];
    struct sbc_frame frame;
#if MSBC_ENCODE
    struct sbc_encoder_state enc_state;
    size_t (*pack_frame)(uint8_t *data, struct sbc_frame *frame,
            size_t len, int joint);
#else
    struct sbc_decoder_state dec_state;
    int (*unpack_frame)(const uint8_t *data, struct sbc_frame *frame,
            size_t len);
#endif
};

struct sbc_struct {
    uint8_t frequency;
    uint8_t blocks;
    uint8_t subbands;
    uint8_t mode;
    uint8_t allocation;
    uint8_t bitpool;
    uint8_t endian;
    uint8_t rsv;

    struct sbc_priv priv;
};

typedef struct sbc_struct sbc_t;
#define fabs(x) ((x) < 0 ? -(x) : (x))
/* C does not provide an explicit arithmetic shift right but this will
   always be correct and every compiler *should* generate optimal code */
#define ASR(val, bits) ((-2 >> 1 == -1) ? \
         ((int32_t)(val)) >> (bits) : ((int32_t) (val)) / (1 << (bits)))

#define SCALE_SPROTO4_TBL    12
#define SCALE_SPROTO8_TBL    14
#define SCALE_NPROTO4_TBL    11
#define SCALE_NPROTO8_TBL    11
#define SCALE4_STAGED1_BITS  15
#define SCALE4_STAGED2_BITS  16
#define SCALE8_STAGED1_BITS  15
#define SCALE8_STAGED2_BITS  16

#define SCALE4_STAGED1(src) ASR(src, SCALE4_STAGED1_BITS)
#define SCALE4_STAGED2(src) ASR(src, SCALE4_STAGED2_BITS)
#define SCALE8_STAGED1(src) ASR(src, SCALE8_STAGED1_BITS)
#define SCALE8_STAGED2(src) ASR(src, SCALE8_STAGED2_BITS)

#define SBC_FIXED_0(val) { val = 0; }
#define MUL(a, b)        ((a) * (b))
#define MULA(a, b, res)  ((a) * (b) + (res))

/* A2DP specification: Appendix B, page 69 */
//const
static int sbc_offset8[4][8] = {
    { -2, 0, 0, 0, 0, 0, 0, 1 },
    { -3, 0, 0, 0, 0, 0, 1, 2 },
    { -4, 0, 0, 0, 0, 0, 1, 2 },
    { -4, 0, 0, 0, 0, 0, 1, 2 }
};

/* extra bits of precision for the synthesis filter input data */
#define SBCDEC_FIXED_EXTRA_BITS 2

#define SS4(val) ASR(val, SCALE_SPROTO4_TBL)
#define SS8(val) ASR(val, SCALE_SPROTO8_TBL)
#define SN4(val) ASR(val, SCALE_NPROTO4_TBL + 1 + SBCDEC_FIXED_EXTRA_BITS)
#define SN8(val) ASR(val, SCALE_NPROTO8_TBL + 1 + SBCDEC_FIXED_EXTRA_BITS)

//const
static int32_t sbc_proto_8_80m0[] = {
    SS8(0x00000000), SS8(0xfe8d1970), SS8(0xee979f00), SS8(0x11686100),
    SS8(0x0172e690), SS8(0xfff5bd1a), SS8(0xfdf1c8d4), SS8(0xeac182c0),
    SS8(0x0d9daee0), SS8(0x00e530da), SS8(0xffe9811d), SS8(0xfd52986c),
    SS8(0xe7054ca0), SS8(0x0a00d410), SS8(0x006c1de4), SS8(0xffdba705),
    SS8(0xfcbc98e8), SS8(0xe3889d20), SS8(0x06af2308), SS8(0x000bb7db),
    SS8(0xffca00ed), SS8(0xfc3fbb68), SS8(0xe071bc00), SS8(0x03bf7948),
    SS8(0xffc4e05c), SS8(0xffb54b3b), SS8(0xfbedadc0), SS8(0xdde26200),
    SS8(0x0142291c), SS8(0xff960e94), SS8(0xff9f3e17), SS8(0xfbd8f358),
    SS8(0xdbf79400), SS8(0xff405e01), SS8(0xff7d4914), SS8(0xff8b1a31),
    SS8(0xfc1417b8), SS8(0xdac7bb40), SS8(0xfdbb828c), SS8(0xff762170)
};

//const
static int32_t sbc_proto_8_80m1[] = {
    SS8(0xff7c272c), SS8(0xfcb02620), SS8(0xda612700), SS8(0xfcb02620),
    SS8(0xff7c272c), SS8(0xff762170), SS8(0xfdbb828c), SS8(0xdac7bb40),
    SS8(0xfc1417b8), SS8(0xff8b1a31), SS8(0xff7d4914), SS8(0xff405e01),
    SS8(0xdbf79400), SS8(0xfbd8f358), SS8(0xff9f3e17), SS8(0xff960e94),
    SS8(0x0142291c), SS8(0xdde26200), SS8(0xfbedadc0), SS8(0xffb54b3b),
    SS8(0xffc4e05c), SS8(0x03bf7948), SS8(0xe071bc00), SS8(0xfc3fbb68),
    SS8(0xffca00ed), SS8(0x000bb7db), SS8(0x06af2308), SS8(0xe3889d20),
    SS8(0xfcbc98e8), SS8(0xffdba705), SS8(0x006c1de4), SS8(0x0a00d410),
    SS8(0xe7054ca0), SS8(0xfd52986c), SS8(0xffe9811d), SS8(0x00e530da),
    SS8(0x0d9daee0), SS8(0xeac182c0), SS8(0xfdf1c8d4), SS8(0xfff5bd1a)
};

//const
static int32_t synmatrix8[16][8] = {
    { SN8(0x05a82798), SN8(0xfa57d868), SN8(0xfa57d868), SN8(0x05a82798),
      SN8(0x05a82798), SN8(0xfa57d868), SN8(0xfa57d868), SN8(0x05a82798) },
    { SN8(0x0471ced0), SN8(0xf8275a10), SN8(0x018f8b84), SN8(0x06a6d988),
      SN8(0xf9592678), SN8(0xfe70747c), SN8(0x07d8a5f0), SN8(0xfb8e3130) },
    { SN8(0x030fbc54), SN8(0xf89be510), SN8(0x07641af0), SN8(0xfcf043ac),
      SN8(0xfcf043ac), SN8(0x07641af0), SN8(0xf89be510), SN8(0x030fbc54) },
    { SN8(0x018f8b84), SN8(0xfb8e3130), SN8(0x06a6d988), SN8(0xf8275a10),
      SN8(0x07d8a5f0), SN8(0xf9592678), SN8(0x0471ced0), SN8(0xfe70747c) },
    { SN8(0x00000000), SN8(0x00000000), SN8(0x00000000), SN8(0x00000000),
      SN8(0x00000000), SN8(0x00000000), SN8(0x00000000), SN8(0x00000000) },
    { SN8(0xfe70747c), SN8(0x0471ced0), SN8(0xf9592678), SN8(0x07d8a5f0),
      SN8(0xf8275a10), SN8(0x06a6d988), SN8(0xfb8e3130), SN8(0x018f8b84) },
    { SN8(0xfcf043ac), SN8(0x07641af0), SN8(0xf89be510), SN8(0x030fbc54),
      SN8(0x030fbc54), SN8(0xf89be510), SN8(0x07641af0), SN8(0xfcf043ac) },
    { SN8(0xfb8e3130), SN8(0x07d8a5f0), SN8(0xfe70747c), SN8(0xf9592678),
      SN8(0x06a6d988), SN8(0x018f8b84), SN8(0xf8275a10), SN8(0x0471ced0) },
    { SN8(0xfa57d868), SN8(0x05a82798), SN8(0x05a82798), SN8(0xfa57d868),
      SN8(0xfa57d868), SN8(0x05a82798), SN8(0x05a82798), SN8(0xfa57d868) },
    { SN8(0xf9592678), SN8(0x018f8b84), SN8(0x07d8a5f0), SN8(0x0471ced0),
      SN8(0xfb8e3130), SN8(0xf8275a10), SN8(0xfe70747c), SN8(0x06a6d988) },
    { SN8(0xf89be510), SN8(0xfcf043ac), SN8(0x030fbc54), SN8(0x07641af0),
      SN8(0x07641af0), SN8(0x030fbc54), SN8(0xfcf043ac), SN8(0xf89be510) },
    { SN8(0xf8275a10), SN8(0xf9592678), SN8(0xfb8e3130), SN8(0xfe70747c),
      SN8(0x018f8b84), SN8(0x0471ced0), SN8(0x06a6d988), SN8(0x07d8a5f0) },
    { SN8(0xf8000000), SN8(0xf8000000), SN8(0xf8000000), SN8(0xf8000000),
      SN8(0xf8000000), SN8(0xf8000000), SN8(0xf8000000), SN8(0xf8000000) },
    { SN8(0xf8275a10), SN8(0xf9592678), SN8(0xfb8e3130), SN8(0xfe70747c),
      SN8(0x018f8b84), SN8(0x0471ced0), SN8(0x06a6d988), SN8(0x07d8a5f0) },
    { SN8(0xf89be510), SN8(0xfcf043ac), SN8(0x030fbc54), SN8(0x07641af0),
      SN8(0x07641af0), SN8(0x030fbc54), SN8(0xfcf043ac), SN8(0xf89be510) },
    { SN8(0xf9592678), SN8(0x018f8b84), SN8(0x07d8a5f0), SN8(0x0471ced0),
      SN8(0xfb8e3130), SN8(0xf8275a10), SN8(0xfe70747c), SN8(0x06a6d988) }
};

#ifdef SBC_HIGH_PRECISION
#define FIXED_A int64_t /* data type for fixed point accumulator */
#define FIXED_T int32_t /* data type for fixed point constants */
#define SBC_FIXED_EXTRA_BITS 16
#else
#define FIXED_A int32_t /* data type for fixed point accumulator */
#define FIXED_T int16_t /* data type for fixed point constants */
#define SBC_FIXED_EXTRA_BITS 0
#endif

/* A2DP specification: Section 12.8 Tables
 *
 * Original values are premultiplied by 4 for better precision (that is the
 * maximum which is possible without overflows)
 *
 * Note: in each block of 16 numbers sign was changed for elements 4, 13, 14, 15
 * in order to compensate the same change applied to cos_table_fixed_8
 */
#define SBC_PROTO_FIXED8_SCALE \
    ((sizeof(FIXED_T) * CHAR_BIT - 1) - SBC_FIXED_EXTRA_BITS + 1)
#define F_PROTO8(x) (FIXED_A) ((x * 2) * \
    ((FIXED_A) 1 << (sizeof(FIXED_T) * CHAR_BIT - 1)) + 0.5)
#define F(x) F_PROTO8(x)
//const
static FIXED_T _sbc_proto_fixed8[80] = {
    F(0.00000000E+00),  F(1.56575398E-04),
    F(3.43256425E-04),  F(5.54620202E-04),
    -F(8.23919506E-04),  F(1.13992507E-03),
    F(1.47640169E-03),  F(1.78371725E-03),
    F(2.01182542E-03),  F(2.10371989E-03),
    F(1.99454554E-03),  F(1.61656283E-03),
    F(9.02154502E-04),  F(1.78805361E-04),
    F(1.64973098E-03),  F(3.49717454E-03),

    F(5.65949473E-03),  F(8.02941163E-03),
    F(1.04584443E-02),  F(1.27472335E-02),
    -F(1.46525263E-02),  F(1.59045603E-02),
    F(1.62208471E-02),  F(1.53184106E-02),
    F(1.29371806E-02),  F(8.85757540E-03),
    F(2.92408442E-03), -F(4.91578024E-03),
    -F(1.46404076E-02),  F(2.61098752E-02),
    F(3.90751381E-02),  F(5.31873032E-02),

    F(6.79989431E-02),  F(8.29847578E-02),
    F(9.75753918E-02),  F(1.11196689E-01),
    -F(1.23264548E-01),  F(1.33264415E-01),
    F(1.40753505E-01),  F(1.45389847E-01),
    F(1.46955068E-01),  F(1.45389847E-01),
    F(1.40753505E-01),  F(1.33264415E-01),
    F(1.23264548E-01), -F(1.11196689E-01),
    -F(9.75753918E-02), -F(8.29847578E-02),

    -F(6.79989431E-02), -F(5.31873032E-02),
    -F(3.90751381E-02), -F(2.61098752E-02),
    F(1.46404076E-02), -F(4.91578024E-03),
    F(2.92408442E-03),  F(8.85757540E-03),
    F(1.29371806E-02),  F(1.53184106E-02),
    F(1.62208471E-02),  F(1.59045603E-02),
    F(1.46525263E-02), -F(1.27472335E-02),
    -F(1.04584443E-02), -F(8.02941163E-03),

    -F(5.65949473E-03), -F(3.49717454E-03),
    -F(1.64973098E-03), -F(1.78805361E-04),
    -F(9.02154502E-04),  F(1.61656283E-03),
    F(1.99454554E-03),  F(2.10371989E-03),
    F(2.01182542E-03),  F(1.78371725E-03),
    F(1.47640169E-03),  F(1.13992507E-03),
    F(8.23919506E-04), -F(5.54620202E-04),
    -F(3.43256425E-04), -F(1.56575398E-04),
};
#undef F

/*
 * To produce this cosine matrix in Octave:
 *
 * b = zeros(8, 16);
 * for i = 0:7
 * for j = 0:15 b(i+1, j+1) = cos((i + 0.5) * (j - 4) * (pi/8))
 * endfor endfor;
 * printf("%.10f, ", b');
 *
 * Note: in each block of 16 numbers sign was changed for elements 4, 13, 14, 15
 *
 * Change of sign for element 4 allows to replace constant 1.0 (not
 * representable in Q15 format) with -1.0 (fine with Q15).
 * Changed signs for elements 13, 14, 15 allow to have more similar constants
 * and simplify subband filter function code.
 */
#define SBC_COS_TABLE_FIXED8_SCALE \
    ((sizeof(FIXED_T) * CHAR_BIT - 1) + SBC_FIXED_EXTRA_BITS)
#define F_COS8(x) (FIXED_A) ((x) * \
    ((FIXED_A) 1 << (sizeof(FIXED_T) * CHAR_BIT - 1)) + 0.5)
#define F(x) F_COS8(x)
//const
static FIXED_T cos_table_fixed_8[128] = {
    F(0.7071067812),  F(0.8314696123),  F(0.9238795325),  F(0.9807852804),
    -F(1.0000000000),  F(0.9807852804),  F(0.9238795325),  F(0.8314696123),
    F(0.7071067812),  F(0.5555702330),  F(0.3826834324),  F(0.1950903220),
    F(0.0000000000),  F(0.1950903220),  F(0.3826834324),  F(0.5555702330),

    -F(0.7071067812), -F(0.1950903220),  F(0.3826834324),  F(0.8314696123),
    -F(1.0000000000),  F(0.8314696123),  F(0.3826834324), -F(0.1950903220),
    -F(0.7071067812), -F(0.9807852804), -F(0.9238795325), -F(0.5555702330),
    -F(0.0000000000), -F(0.5555702330), -F(0.9238795325), -F(0.9807852804),

    -F(0.7071067812), -F(0.9807852804), -F(0.3826834324),  F(0.5555702330),
    -F(1.0000000000),  F(0.5555702330), -F(0.3826834324), -F(0.9807852804),
    -F(0.7071067812),  F(0.1950903220),  F(0.9238795325),  F(0.8314696123),
    F(0.0000000000),  F(0.8314696123),  F(0.9238795325),  F(0.1950903220),

    F(0.7071067812), -F(0.5555702330), -F(0.9238795325),  F(0.1950903220),
    -F(1.0000000000),  F(0.1950903220), -F(0.9238795325), -F(0.5555702330),
    F(0.7071067812),  F(0.8314696123), -F(0.3826834324), -F(0.9807852804),
    -F(0.0000000000), -F(0.9807852804), -F(0.3826834324),  F(0.8314696123),

    F(0.7071067812),  F(0.5555702330), -F(0.9238795325), -F(0.1950903220),
    -F(1.0000000000), -F(0.1950903220), -F(0.9238795325),  F(0.5555702330),
    F(0.7071067812), -F(0.8314696123), -F(0.3826834324),  F(0.9807852804),
    F(0.0000000000),  F(0.9807852804), -F(0.3826834324), -F(0.8314696123),

    -F(0.7071067812),  F(0.9807852804), -F(0.3826834324), -F(0.5555702330),
    -F(1.0000000000), -F(0.5555702330), -F(0.3826834324),  F(0.9807852804),
    -F(0.7071067812), -F(0.1950903220),  F(0.9238795325), -F(0.8314696123),
    -F(0.0000000000), -F(0.8314696123),  F(0.9238795325), -F(0.1950903220),

    -F(0.7071067812),  F(0.1950903220),  F(0.3826834324), -F(0.8314696123),
    -F(1.0000000000), -F(0.8314696123),  F(0.3826834324),  F(0.1950903220),
    -F(0.7071067812),  F(0.9807852804), -F(0.9238795325),  F(0.5555702330),
    -F(0.0000000000),  F(0.5555702330), -F(0.9238795325),  F(0.9807852804),

    F(0.7071067812), -F(0.8314696123),  F(0.9238795325), -F(0.9807852804),
    -F(1.0000000000), -F(0.9807852804),  F(0.9238795325), -F(0.8314696123),
    F(0.7071067812), -F(0.5555702330),  F(0.3826834324), -F(0.1950903220),
    -F(0.0000000000), -F(0.1950903220),  F(0.3826834324), -F(0.5555702330),
};
#undef F

/*
 * Enforce 16 byte alignment for the data, which is supposed to be used
 * with SIMD optimized code.
 */

#define SBC_ALIGN_BITS 2
#define SBC_ALIGN_MASK ((1 << (SBC_ALIGN_BITS)) - 1)

/*
 * Constant tables for the use in SIMD optimized analysis filters
 * Each table consists of two parts:
 * 1. reordered "proto" table
 * 2. reordered "cos" table
 *
 * Due to non-symmetrical reordering, separate tables for "even"
 * and "odd" cases are needed
 */

//const
static FIXED_T analysis_consts_fixed8_simd_even[80 + 64] = {
#define C0 2.7906148894
#define C1 2.4270044280
#define C2 2.8015616024
#define C3 3.1710363741
#define C4 2.5377944043
#define C5 2.4270044280
#define C6 2.8015616024
#define C7 3.1710363741

#define F(x) F_PROTO8(x)
     F(0.00000000E+00 * C0),  F(2.01182542E-03 * C0),
     F(1.56575398E-04 * C1),  F(1.78371725E-03 * C1),
     F(3.43256425E-04 * C2),  F(1.47640169E-03 * C2),
     F(5.54620202E-04 * C3),  F(1.13992507E-03 * C3),
    -F(8.23919506E-04 * C4),  F(0.00000000E+00 * C4),
     F(2.10371989E-03 * C5),  F(3.49717454E-03 * C5),
     F(1.99454554E-03 * C6),  F(1.64973098E-03 * C6),
     F(1.61656283E-03 * C7),  F(1.78805361E-04 * C7),
     F(5.65949473E-03 * C0),  F(1.29371806E-02 * C0),
     F(8.02941163E-03 * C1),  F(1.53184106E-02 * C1),
     F(1.04584443E-02 * C2),  F(1.62208471E-02 * C2),
     F(1.27472335E-02 * C3),  F(1.59045603E-02 * C3),
    -F(1.46525263E-02 * C4),  F(0.00000000E+00 * C4),
     F(8.85757540E-03 * C5),  F(5.31873032E-02 * C5),
     F(2.92408442E-03 * C6),  F(3.90751381E-02 * C6),
    -F(4.91578024E-03 * C7),  F(2.61098752E-02 * C7),
     F(6.79989431E-02 * C0),  F(1.46955068E-01 * C0),
     F(8.29847578E-02 * C1),  F(1.45389847E-01 * C1),
     F(9.75753918E-02 * C2),  F(1.40753505E-01 * C2),
     F(1.11196689E-01 * C3),  F(1.33264415E-01 * C3),
    -F(1.23264548E-01 * C4),  F(0.00000000E+00 * C4),
     F(1.45389847E-01 * C5), -F(8.29847578E-02 * C5),
     F(1.40753505E-01 * C6), -F(9.75753918E-02 * C6),
     F(1.33264415E-01 * C7), -F(1.11196689E-01 * C7),
    -F(6.79989431E-02 * C0),  F(1.29371806E-02 * C0),
    -F(5.31873032E-02 * C1),  F(8.85757540E-03 * C1),
    -F(3.90751381E-02 * C2),  F(2.92408442E-03 * C2),
    -F(2.61098752E-02 * C3), -F(4.91578024E-03 * C3),
     F(1.46404076E-02 * C4),  F(0.00000000E+00 * C4),
     F(1.53184106E-02 * C5), -F(8.02941163E-03 * C5),
     F(1.62208471E-02 * C6), -F(1.04584443E-02 * C6),
     F(1.59045603E-02 * C7), -F(1.27472335E-02 * C7),
    -F(5.65949473E-03 * C0),  F(2.01182542E-03 * C0),
    -F(3.49717454E-03 * C1),  F(2.10371989E-03 * C1),
    -F(1.64973098E-03 * C2),  F(1.99454554E-03 * C2),
    -F(1.78805361E-04 * C3),  F(1.61656283E-03 * C3),
    -F(9.02154502E-04 * C4),  F(0.00000000E+00 * C4),
     F(1.78371725E-03 * C5), -F(1.56575398E-04 * C5),
     F(1.47640169E-03 * C6), -F(3.43256425E-04 * C6),
     F(1.13992507E-03 * C7), -F(5.54620202E-04 * C7),
#undef F
#define F(x) F_COS8(x)
     F(0.7071067812 / C0),  F(0.8314696123 / C1),
    -F(0.7071067812 / C0), -F(0.1950903220 / C1),
    -F(0.7071067812 / C0), -F(0.9807852804 / C1),
     F(0.7071067812 / C0), -F(0.5555702330 / C1),
     F(0.7071067812 / C0),  F(0.5555702330 / C1),
    -F(0.7071067812 / C0),  F(0.9807852804 / C1),
    -F(0.7071067812 / C0),  F(0.1950903220 / C1),
     F(0.7071067812 / C0), -F(0.8314696123 / C1),
     F(0.9238795325 / C2),  F(0.9807852804 / C3),
     F(0.3826834324 / C2),  F(0.8314696123 / C3),
    -F(0.3826834324 / C2),  F(0.5555702330 / C3),
    -F(0.9238795325 / C2),  F(0.1950903220 / C3),
    -F(0.9238795325 / C2), -F(0.1950903220 / C3),
    -F(0.3826834324 / C2), -F(0.5555702330 / C3),
     F(0.3826834324 / C2), -F(0.8314696123 / C3),
     F(0.9238795325 / C2), -F(0.9807852804 / C3),
    -F(1.0000000000 / C4),  F(0.5555702330 / C5),
    -F(1.0000000000 / C4), -F(0.9807852804 / C5),
    -F(1.0000000000 / C4),  F(0.1950903220 / C5),
    -F(1.0000000000 / C4),  F(0.8314696123 / C5),
    -F(1.0000000000 / C4), -F(0.8314696123 / C5),
    -F(1.0000000000 / C4), -F(0.1950903220 / C5),
    -F(1.0000000000 / C4),  F(0.9807852804 / C5),
    -F(1.0000000000 / C4), -F(0.5555702330 / C5),
     F(0.3826834324 / C6),  F(0.1950903220 / C7),
    -F(0.9238795325 / C6), -F(0.5555702330 / C7),
     F(0.9238795325 / C6),  F(0.8314696123 / C7),
    -F(0.3826834324 / C6), -F(0.9807852804 / C7),
    -F(0.3826834324 / C6),  F(0.9807852804 / C7),
     F(0.9238795325 / C6), -F(0.8314696123 / C7),
    -F(0.9238795325 / C6),  F(0.5555702330 / C7),
     F(0.3826834324 / C6), -F(0.1950903220 / C7),
#undef F

#undef C0
#undef C1
#undef C2
#undef C3
#undef C4
#undef C5
#undef C6
#undef C7
};

//const
static FIXED_T analysis_consts_fixed8_simd_odd[80 + 64] = {
#define C0 2.5377944043
#define C1 2.4270044280
#define C2 2.8015616024
#define C3 3.1710363741
#define C4 2.7906148894
#define C5 2.4270044280
#define C6 2.8015616024
#define C7 3.1710363741

#define F(x) F_PROTO8(x)
     F(0.00000000E+00 * C0), -F(8.23919506E-04 * C0),
     F(1.56575398E-04 * C1),  F(1.78371725E-03 * C1),
     F(3.43256425E-04 * C2),  F(1.47640169E-03 * C2),
     F(5.54620202E-04 * C3),  F(1.13992507E-03 * C3),
     F(2.01182542E-03 * C4),  F(5.65949473E-03 * C4),
     F(2.10371989E-03 * C5),  F(3.49717454E-03 * C5),
     F(1.99454554E-03 * C6),  F(1.64973098E-03 * C6),
     F(1.61656283E-03 * C7),  F(1.78805361E-04 * C7),
     F(0.00000000E+00 * C0), -F(1.46525263E-02 * C0),
     F(8.02941163E-03 * C1),  F(1.53184106E-02 * C1),
     F(1.04584443E-02 * C2),  F(1.62208471E-02 * C2),
     F(1.27472335E-02 * C3),  F(1.59045603E-02 * C3),
     F(1.29371806E-02 * C4),  F(6.79989431E-02 * C4),
     F(8.85757540E-03 * C5),  F(5.31873032E-02 * C5),
     F(2.92408442E-03 * C6),  F(3.90751381E-02 * C6),
    -F(4.91578024E-03 * C7),  F(2.61098752E-02 * C7),
     F(0.00000000E+00 * C0), -F(1.23264548E-01 * C0),
     F(8.29847578E-02 * C1),  F(1.45389847E-01 * C1),
     F(9.75753918E-02 * C2),  F(1.40753505E-01 * C2),
     F(1.11196689E-01 * C3),  F(1.33264415E-01 * C3),
     F(1.46955068E-01 * C4), -F(6.79989431E-02 * C4),
     F(1.45389847E-01 * C5), -F(8.29847578E-02 * C5),
     F(1.40753505E-01 * C6), -F(9.75753918E-02 * C6),
     F(1.33264415E-01 * C7), -F(1.11196689E-01 * C7),
     F(0.00000000E+00 * C0),  F(1.46404076E-02 * C0),
    -F(5.31873032E-02 * C1),  F(8.85757540E-03 * C1),
    -F(3.90751381E-02 * C2),  F(2.92408442E-03 * C2),
    -F(2.61098752E-02 * C3), -F(4.91578024E-03 * C3),
     F(1.29371806E-02 * C4), -F(5.65949473E-03 * C4),
     F(1.53184106E-02 * C5), -F(8.02941163E-03 * C5),
     F(1.62208471E-02 * C6), -F(1.04584443E-02 * C6),
     F(1.59045603E-02 * C7), -F(1.27472335E-02 * C7),
     F(0.00000000E+00 * C0), -F(9.02154502E-04 * C0),
    -F(3.49717454E-03 * C1),  F(2.10371989E-03 * C1),
    -F(1.64973098E-03 * C2),  F(1.99454554E-03 * C2),
    -F(1.78805361E-04 * C3),  F(1.61656283E-03 * C3),
     F(2.01182542E-03 * C4),  F(0.00000000E+00 * C4),
     F(1.78371725E-03 * C5), -F(1.56575398E-04 * C5),
     F(1.47640169E-03 * C6), -F(3.43256425E-04 * C6),
     F(1.13992507E-03 * C7), -F(5.54620202E-04 * C7),
#undef F
#define F(x) F_COS8(x)
    -F(1.0000000000 / C0),  F(0.8314696123 / C1),
    -F(1.0000000000 / C0), -F(0.1950903220 / C1),
    -F(1.0000000000 / C0), -F(0.9807852804 / C1),
    -F(1.0000000000 / C0), -F(0.5555702330 / C1),
    -F(1.0000000000 / C0),  F(0.5555702330 / C1),
    -F(1.0000000000 / C0),  F(0.9807852804 / C1),
    -F(1.0000000000 / C0),  F(0.1950903220 / C1),
    -F(1.0000000000 / C0), -F(0.8314696123 / C1),
     F(0.9238795325 / C2),  F(0.9807852804 / C3),
     F(0.3826834324 / C2),  F(0.8314696123 / C3),
    -F(0.3826834324 / C2),  F(0.5555702330 / C3),
    -F(0.9238795325 / C2),  F(0.1950903220 / C3),
    -F(0.9238795325 / C2), -F(0.1950903220 / C3),
    -F(0.3826834324 / C2), -F(0.5555702330 / C3),
     F(0.3826834324 / C2), -F(0.8314696123 / C3),
     F(0.9238795325 / C2), -F(0.9807852804 / C3),
     F(0.7071067812 / C4),  F(0.5555702330 / C5),
    -F(0.7071067812 / C4), -F(0.9807852804 / C5),
    -F(0.7071067812 / C4),  F(0.1950903220 / C5),
     F(0.7071067812 / C4),  F(0.8314696123 / C5),
     F(0.7071067812 / C4), -F(0.8314696123 / C5),
    -F(0.7071067812 / C4), -F(0.1950903220 / C5),
    -F(0.7071067812 / C4),  F(0.9807852804 / C5),
     F(0.7071067812 / C4), -F(0.5555702330 / C5),
     F(0.3826834324 / C6),  F(0.1950903220 / C7),
    -F(0.9238795325 / C6), -F(0.5555702330 / C7),
     F(0.9238795325 / C6),  F(0.8314696123 / C7),
    -F(0.3826834324 / C6), -F(0.9807852804 / C7),
    -F(0.3826834324 / C6),  F(0.9807852804 / C7),
     F(0.9238795325 / C6), -F(0.8314696123 / C7),
    -F(0.9238795325 / C6),  F(0.5555702330 / C7),
     F(0.3826834324 / C6), -F(0.1950903220 / C7),
#undef F

#undef C0
#undef C1
#undef C2
#undef C3
#undef C4
#undef C5
#undef C6
#undef C7
};

#ifdef __GNUC__
#define SBC_ALWAYS_INLINE inline __attribute__((always_inline))
#else
#define SBC_ALWAYS_INLINE inline
#endif

#if (PRINT_DEBUG_INFO)
#include "../common/myprintf.h"
#endif

#define A2DP_SAMPLING_FREQ_16000        (1 << 3)
#define A2DP_SAMPLING_FREQ_32000        (1 << 2)
#define A2DP_SAMPLING_FREQ_44100        (1 << 1)
#define A2DP_SAMPLING_FREQ_48000        (1 << 0)

#define A2DP_CHANNEL_MODE_MONO          (1 << 3)
#define A2DP_CHANNEL_MODE_DUAL_CHANNEL  (1 << 2)
#define A2DP_CHANNEL_MODE_STEREO        (1 << 1)
#define A2DP_CHANNEL_MODE_JOINT_STEREO  (1 << 0)

#define A2DP_BLOCK_LENGTH_4             (1 << 3)
#define A2DP_BLOCK_LENGTH_8             (1 << 2)
#define A2DP_BLOCK_LENGTH_12            (1 << 1)
#define A2DP_BLOCK_LENGTH_16            (1 << 0)

#define A2DP_SUBBANDS_4                 (1 << 1)
#define A2DP_SUBBANDS_8                 (1 << 0)

#define A2DP_ALLOCATION_SNR             (1 << 1)
#define A2DP_ALLOCATION_LOUDNESS        (1 << 0)

/*
 * Calculates the CRC-8 of the first len bits in data
 */
//const
static uint8_t crc_table[256] = {
    0x00, 0x1D, 0x3A, 0x27, 0x74, 0x69, 0x4E, 0x53,
    0xE8, 0xF5, 0xD2, 0xCF, 0x9C, 0x81, 0xA6, 0xBB,
    0xCD, 0xD0, 0xF7, 0xEA, 0xB9, 0xA4, 0x83, 0x9E,
    0x25, 0x38, 0x1F, 0x02, 0x51, 0x4C, 0x6B, 0x76,
    0x87, 0x9A, 0xBD, 0xA0, 0xF3, 0xEE, 0xC9, 0xD4,
    0x6F, 0x72, 0x55, 0x48, 0x1B, 0x06, 0x21, 0x3C,
    0x4A, 0x57, 0x70, 0x6D, 0x3E, 0x23, 0x04, 0x19,
    0xA2, 0xBF, 0x98, 0x85, 0xD6, 0xCB, 0xEC, 0xF1,
    0x13, 0x0E, 0x29, 0x34, 0x67, 0x7A, 0x5D, 0x40,
    0xFB, 0xE6, 0xC1, 0xDC, 0x8F, 0x92, 0xB5, 0xA8,
    0xDE, 0xC3, 0xE4, 0xF9, 0xAA, 0xB7, 0x90, 0x8D,
    0x36, 0x2B, 0x0C, 0x11, 0x42, 0x5F, 0x78, 0x65,
    0x94, 0x89, 0xAE, 0xB3, 0xE0, 0xFD, 0xDA, 0xC7,
    0x7C, 0x61, 0x46, 0x5B, 0x08, 0x15, 0x32, 0x2F,
    0x59, 0x44, 0x63, 0x7E, 0x2D, 0x30, 0x17, 0x0A,
    0xB1, 0xAC, 0x8B, 0x96, 0xC5, 0xD8, 0xFF, 0xE2,
    0x26, 0x3B, 0x1C, 0x01, 0x52, 0x4F, 0x68, 0x75,
    0xCE, 0xD3, 0xF4, 0xE9, 0xBA, 0xA7, 0x80, 0x9D,
    0xEB, 0xF6, 0xD1, 0xCC, 0x9F, 0x82, 0xA5, 0xB8,
    0x03, 0x1E, 0x39, 0x24, 0x77, 0x6A, 0x4D, 0x50,
    0xA1, 0xBC, 0x9B, 0x86, 0xD5, 0xC8, 0xEF, 0xF2,
    0x49, 0x54, 0x73, 0x6E, 0x3D, 0x20, 0x07, 0x1A,
    0x6C, 0x71, 0x56, 0x4B, 0x18, 0x05, 0x22, 0x3F,
    0x84, 0x99, 0xBE, 0xA3, 0xF0, 0xED, 0xCA, 0xD7,
    0x35, 0x28, 0x0F, 0x12, 0x41, 0x5C, 0x7B, 0x66,
    0xDD, 0xC0, 0xE7, 0xFA, 0xA9, 0xB4, 0x93, 0x8E,
    0xF8, 0xE5, 0xC2, 0xDF, 0x8C, 0x91, 0xB6, 0xAB,
    0x10, 0x0D, 0x2A, 0x37, 0x64, 0x79, 0x5E, 0x43,
    0xB2, 0xAF, 0x88, 0x95, 0xC6, 0xDB, 0xFC, 0xE1,
    0x5A, 0x47, 0x60, 0x7D, 0x2E, 0x33, 0x14, 0x09,
    0x7F, 0x62, 0x45, 0x58, 0x0B, 0x16, 0x31, 0x2C,
    0x97, 0x8A, 0xAD, 0xB0, 0xE3, 0xFE, 0xD9, 0xC4
};

_attribute_ram_code_ uint64_t mul64(uint64_t x, uint32_t y)
{
    uint8_t i;
    uint64_t sum = 0;

    for (i = 0; i < 32; i ++) {
        if (y & (1 << i)) {
            /*
             * little endian
             */
            uint64_t temp;
            uint32_t x1, x2;
            x1 = *((uint32_t *)&x);  // low word of dword
            x2 = *(((uint32_t *)&x) + 1); // high word of dword
            x2 = x2 << i;
            x2 = x2 | ((x1 >> (32 - i)) & ((1 << i) - 1));
            x1 = x1 << i;
            *(uint32_t *)(&temp) = x1;  // low word of dword
            *((uint32_t *)(&temp) + 1) = x2;  // high word of dword
            sum += temp;
        }
    }
    return sum;
}

_attribute_ram_code_ uint64_t shift_left64(uint64_t x, uint8_t bits)
{
    uint64_t temp;
    uint32_t x1, x2;
    volatile unsigned long * ptr, * tmp_ptr;

    ptr =(uint32_t *)&x;
    tmp_ptr = (uint32_t *)&temp;
    if (bits == 0)
        return x;

    if (bits < 32) {
        x1 = *(ptr);  // low word of dword
        x2 = *(ptr + 1); // high word of dword
      //  my_printf("slt = %04x, %04x\n",x2,x1);
        x2 = x2 << bits;
      //  my_printf("slt = %04x, %04x\n",x2,x1);
        x2 = x2 | ((x1 >> (32 - bits)) & ((1 << bits) - 1));
      //  my_printf("slt = %04x, %04x\n",x2,x1);
        x1 = x1 << bits;
      //  my_printf("slt = %04x, %04x\n",x2,x1);
    } else if (bits < 64){
    	x1 = *(ptr);  // low word of dword
    	x2 = 0;
    	x1 = x1 << (bits - 32);
    } else {
    	x1 = 0;
    	x2 = 0;
    }
    *(tmp_ptr) = x1;  // low word of dword
    *(tmp_ptr + 1) = x2;  // high word of dword

    return temp;
}

_attribute_ram_code_ uint64_t div64(uint64_t x, uint16_t y)
{
    uint64_t res = 0;
    volatile unsigned long  * tmp_ptr;
    volatile unsigned short * tmp16_ptr;
    volatile unsigned long   tmp_long;
    volatile uint16_t *x1 = (uint16_t *)&x;  // low word of dword
    volatile uint32_t rest = *(((uint32_t *)&x) + 1);  // high word of dword

    tmp_ptr = (uint32_t *)&res;
    tmp16_ptr = (uint16_t *)&res;
    //tmp_ptr++;
    *(tmp_ptr+1) = rest/y;
    //*(((uint32_t *)&res) + 1) = rest / y;
    tmp_long = y;
    //my_printf("rest = %04x, %04x\n",rest,tmp_long);

    rest = rest % y;
    rest = (rest << 16) | x1[1];
    *(tmp16_ptr + 1) = rest / y;

    rest = rest % y;
    rest = (rest << 16) | x1[0];
    *tmp16_ptr = rest / y;

    //my_printf("resut = %04x, %04x\n",*(tmp_ptr+1),*tmp_ptr);
    //while(1);
    return res;
}


//#if MSBC_ENCODE
static sbc_t sbc_en_ctx;
//#else
static sbc_t sbc_de_ctx;
//#endif

static int sbc_init_msbc(sbc_t *sbc);


// ****************************************************




// *****************************************************


void msbc_init_ctx()
{
#if MSBC_ENCODE
    if (sbc_en_ctx.priv.init != 1)
        sbc_init_msbc(&sbc_en_ctx);
#else
    if (sbc_de_ctx.priv.init != 1)
        sbc_init_msbc(&sbc_de_ctx);
#endif
}

_attribute_ram_code_ uint8_t sbc_crc8(const uint8_t *data, size_t len)
{
    uint8_t crc = 0x0f;
    uint16_t i;
    uint8_t octet;

    for (i = 0; i < len / 8; i++)
        crc = crc_table[crc ^ data[i]];

    octet = data[i];
    for (i = 0; i < len % 8; i++) {
        char bit = ((octet ^ crc) & 0x80) >> 7;

        crc = ((crc & 0x7f) << 1) ^ (bit ? 0x1d : 0);

        octet = octet << 1;
    }

    return crc;
}

/*
 * Code straight from the spec to calculate the bits array
 * Takes a pointer to the frame in question, a pointer to the bits array and
 * the sampling frequency (as 2 bit integer)
 */
_attribute_ram_code_ void sbc_calculate_bits_internal(
        const struct sbc_frame *frame, int (*bits)[MSBC_SUBBANDS], int subbands)
{
    int bitneed[MSBC_CHANNELS][MSBC_SUBBANDS], loudness, max_bitneed, bitcount, slicecount, bitslice;
    uint8_t sf = frame->frequency;

    if (frame->mode == MONO || frame->mode == DUAL_CHANNEL) {
        int8_t ch, sb;

        for (ch = 0; ch < frame->channels; ch++) {
            max_bitneed = 0;
            if (frame->allocation == SNR) {
                for (sb = 0; sb < subbands; sb++) {
                    bitneed[ch][sb] = frame->scale_factor[ch][sb];
                    if (bitneed[ch][sb] > max_bitneed)
                        max_bitneed = bitneed[ch][sb];
                }
            } else {
                for (sb = 0; sb < subbands; sb++) {
                    if (frame->scale_factor[ch][sb] == 0)
                        bitneed[ch][sb] = -5;
                    else {
                        loudness = frame->scale_factor[ch][sb] - sbc_offset8[sf][sb];
                        if (loudness > 0)
                            bitneed[ch][sb] = loudness / 2;
                        else
                            bitneed[ch][sb] = loudness;
                    }
                    if (bitneed[ch][sb] > max_bitneed)
                        max_bitneed = bitneed[ch][sb];
#if 0
                    printf("bn[");putihex(sb, sizeof(sb) * CHAR_BIT);printf("]:");
                    putihex(bitneed[ch][sb], sizeof(bitneed[ch][sb]) * CHAR_BIT);
                    printf(", sf[]:");
                    putihex(frame->scale_factor[ch][sb], sizeof(frame->scale_factor[0][0]) * CHAR_BIT);
                    printf("\n");
#endif
                }
            }

            bitcount = 0;
            slicecount = 0;
            bitslice = max_bitneed + 1;

            do {
                bitslice--;
                bitcount += slicecount;
                slicecount = 0;
                for (sb = 0; sb < subbands; sb++) {
#if 0
                	printf("bn[");putihex(sb, sizeof(sb) * CHAR_BIT);printf("]:");
                	putihex(bitneed[ch][sb], sizeof(uint16_t) * CHAR_BIT);
#endif
                    if ((bitneed[ch][sb] > bitslice + 1) && (bitneed[ch][sb] < bitslice + 16))
                        slicecount++;
                    else if (bitneed[ch][sb] == bitslice + 1)
                        slicecount += 2;
                }
#if 0
                printf("\nb:");putihex(bitcount, sizeof(bitcount) * CHAR_BIT);
                printf(",s:");putihex(slicecount, sizeof(slicecount) * CHAR_BIT);
                printf(",p:");putihex(frame->bitpool, sizeof(frame->bitpool) * CHAR_BIT);
                printf("\n");
#endif
            } while (bitcount + slicecount < frame->bitpool);

            if (bitcount + slicecount == frame->bitpool) {
                bitcount += slicecount;
                bitslice--;
            }

            for (sb = 0; sb < subbands; sb++) {
                if (bitneed[ch][sb] < bitslice + 2)
                    bits[ch][sb] = 0;
                else {
                    bits[ch][sb] = bitneed[ch][sb] - bitslice;
                    if (bits[ch][sb] > 16)
                        bits[ch][sb] = 16;
                }
            }

            for (sb = 0; bitcount < frame->bitpool &&
                            sb < subbands; sb++) {
                if ((bits[ch][sb] >= 2) && (bits[ch][sb] < 16)) {
                    bits[ch][sb]++;
                    bitcount++;
                } else if ((bitneed[ch][sb] == bitslice + 1) && (frame->bitpool > bitcount + 1)) {
                    bits[ch][sb] = 2;
                    bitcount += 2;
                }
            }

            for (sb = 0; bitcount < frame->bitpool &&
                            sb < subbands; sb++) {
                if (bits[ch][sb] < 16) {
                    bits[ch][sb]++;
                    bitcount++;
                }
            }
        }
    }
}

_attribute_ram_code_ void sbc_calculate_bits(const struct sbc_frame *frame, int (*bits)[MSBC_SUBBANDS])
{
    sbc_calculate_bits_internal(frame, bits, MSBC_SUBBANDS);
}

#if !MSBC_ENCODE
/*
 * Unpacks a SBC frame at the beginning of the stream in data,
 * which has at most len bytes into frame.
 * Returns the length in bytes of the packed frame, or a negative
 * value on error. The error codes are:
 *
 *  -1   Data stream too short
 *  -2   Sync byte incorrect
 *  -3   CRC8 incorrect
 *  -4   Bitpool value out of bounds
 */

_attribute_ram_code_ int sbc_unpack_frame_internal(const uint8_t *data,
        struct sbc_frame *frame, size_t len)
{
    uint16_t consumed;
    /* Will copy the parts of the header that are relevant to crc
     * calculation here */
    uint8_t crc_header[11] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t crc_pos = 0;
    volatile uint64_t tmp;
    volatile unsigned long * tmp_ptr;
    uint32_t audio_sample;
    int8_t ch, sb, blk;    /* channel, subband, block and bit standard counters */
    int bit;
    int bits[MSBC_CHANNELS][MSBC_SUBBANDS];        /* bits distribution */
    uint32_t levels[MSBC_CHANNELS][MSBC_SUBBANDS];    /* levels derived from that */

    consumed = 32;

    crc_header[0] = data[1];
    crc_header[1] = data[2];
    crc_pos = 16;

    if (len * 8 < consumed + (4 * frame->subbands * frame->channels))
        return -1;

    for (ch = 0; ch < frame->channels; ch++) {
        for (sb = 0; sb < frame->subbands; sb++) {
            /* FIXME assert(consumed % 4 == 0); */
            frame->scale_factor[ch][sb] =
                (data[consumed >> 3] >> (4 - (consumed & 0x7))) & 0x0F;
            crc_header[crc_pos >> 3] |=
                frame->scale_factor[ch][sb] << (4 - (crc_pos & 0x7));

            consumed += 4;
            crc_pos += 4;
        }
    }

    if (data[3] != sbc_crc8(crc_header, crc_pos))
        return -3;

    sbc_calculate_bits(frame, bits);

    for (ch = 0; ch < frame->channels; ch++) {
        for (sb = 0; sb < frame->subbands; sb++)
            levels[ch][sb] = (1 << bits[ch][sb]) - 1;
    }

    for (blk = 0; blk < frame->blocks; blk++) {
        for (ch = 0; ch < frame->channels; ch++) {
            for (sb = 0; sb < frame->subbands; sb++) {
                uint32_t shift;

                if (levels[ch][sb] == 0) {
                    frame->sb_sample[blk][ch][sb] = 0;
                    continue;
                }

                shift = frame->scale_factor[ch][sb] +
                        1 + SBCDEC_FIXED_EXTRA_BITS;

                audio_sample = 0;
                for (bit = 0; bit < bits[ch][sb]; bit++) {
                    if (consumed > len * 8)
                        return -1;

                    if ((data[consumed >> 3] >> (7 - (consumed & 0x7))) & 0x01)
                        audio_sample |= 1 << (bits[ch][sb] - bit - 1);

                    consumed++;
                }
#if 1
                tmp_ptr = (int32_t *)&tmp;
                tmp = audio_sample; //3f
//                my_printf("%04x %04x - %04x\n",*(tmp_ptr+1),*tmp_ptr,levels[ch][sb]);
                tmp = shift_left64(tmp, 1); //7e
                tmp = tmp | 1;

//                my_printf("%04x %04x - %04x\n",*(tmp_ptr+1),*tmp_ptr,levels[ch][sb]);

                tmp = shift_left64(tmp, shift); //shift 7f000

//                my_printf("%04x %04x - %04x\n",*(tmp_ptr+1),*tmp_ptr,levels[ch][sb]);

                tmp = div64(tmp, levels[ch][sb]); //0x1000
                tmp = tmp - (1 << shift);  //0
                //frame->sb_sample[blk][ch][sb] = *(int32_t *)&tmp;
//                my_printf("%04x %04x - %04x\n",*(tmp_ptr+1),*tmp_ptr,levels[ch][sb]);


                frame->sb_sample[blk][ch][sb] = *tmp_ptr;

//                my_printf("[%d,%d,%d],%d\n",blk,ch,sb,frame->sb_sample[blk][ch][sb]);

//                if(blk==2)
//                {
//                	while(1);
//                }
//                while(1);

#else
                frame->sb_sample[blk][ch][sb] = (int32_t)
                    (((((uint64_t) audio_sample << 1) | 1) << shift) /
                    levels[ch][sb]) - (1 << shift);
#endif
            }
        }
    }

    if ((consumed & 0x7) != 0)
        consumed += 8 - (consumed & 0x7);

    return consumed >> 3;
}

_attribute_ram_code_ static int msbc_unpack_frame(const uint8_t *data,
        struct sbc_frame *frame, size_t len)
{
    if (len < 4)
        return -1;

    if (data[0] != MSBC_SYNCWORD)
        return -2;
// LLJ
//    if (data[1] != 0)
//        return -2;
//    if (data[2] != 0)
//        return -2;

    frame->frequency = SBC_FREQ_16000;
    frame->block_mode = SBC_BLK_4;
    frame->blocks = MSBC_BLOCKS;
    frame->allocation = LOUDNESS;
    frame->mode = MONO;
    frame->channels = MSBC_CHANNELS;
    frame->subband_mode = 1;
    frame->subbands = MSBC_SUBBANDS;
    frame->bitpool = MSBC_BITPOOL;

    return sbc_unpack_frame_internal(data, frame, len);
}

_attribute_ram_code_ void sbc_decoder_init(struct sbc_decoder_state *state,
                    const struct sbc_frame *frame)
{
    int8_t i, ch;

    tmemset(state->V, 0, sizeof(state->V));
    state->subbands = frame->subbands;

    for (ch = 0; ch < frame->channels; ch++)
        for (i = 0; i < frame->subbands * 2; i++)
            state->offset[ch][i] = (10 * i + 10);
}


//SBC_ALWAYS_INLINE int16_t sbc_clip16(int32_t s)
_attribute_ram_code_ int16_t sbc_clip16(int32_t s)
{
    if (s > 0x7FFF)
        return 0x7FFF;
    else if (s < -0x8000)
        return -0x8000;
    else
        return s;
}

_attribute_ram_code_  void sbc_synthesize_eight(struct sbc_decoder_state *state,
                struct sbc_frame *frame, int ch, int blk)
{
    int8_t i, j, k, idx;
    int *offset = state->offset[ch];

    for (i = 0; i < 16; i++) {
        /* Shifting */
        offset[i]--;
        if (offset[i] < 0) {
            offset[i] = 159;
            for (j = 0; j < 9; j++)
                state->V[ch][j + 160] = state->V[ch][j];
        }

        /* Distribute the new matrix value to the shifted position */
        state->V[ch][offset[i]] = SCALE8_STAGED1(
            MULA(synmatrix8[i][0], frame->sb_sample[blk][ch][0],
            MULA(synmatrix8[i][1], frame->sb_sample[blk][ch][1],
            MULA(synmatrix8[i][2], frame->sb_sample[blk][ch][2],
            MULA(synmatrix8[i][3], frame->sb_sample[blk][ch][3],
            MULA(synmatrix8[i][4], frame->sb_sample[blk][ch][4],
            MULA(synmatrix8[i][5], frame->sb_sample[blk][ch][5],
            MULA(synmatrix8[i][6], frame->sb_sample[blk][ch][6],
            MUL( synmatrix8[i][7], frame->sb_sample[blk][ch][7])))))))));
    }

    /* Compute the samples */
    for (idx = 0, i = 0; i < 8; i++, idx += 5) {
        k = (i + 8) & 0xf;

        /* Store in output, Q0 */
        frame->pcm_sample[ch][blk * 8 + i] = sbc_clip16(SCALE8_STAGED1(
            MULA(state->V[ch][offset[i] + 0], sbc_proto_8_80m0[idx + 0],
            MULA(state->V[ch][offset[k] + 1], sbc_proto_8_80m1[idx + 0],
            MULA(state->V[ch][offset[i] + 2], sbc_proto_8_80m0[idx + 1],
            MULA(state->V[ch][offset[k] + 3], sbc_proto_8_80m1[idx + 1],
            MULA(state->V[ch][offset[i] + 4], sbc_proto_8_80m0[idx + 2],
            MULA(state->V[ch][offset[k] + 5], sbc_proto_8_80m1[idx + 2],
            MULA(state->V[ch][offset[i] + 6], sbc_proto_8_80m0[idx + 3],
            MULA(state->V[ch][offset[k] + 7], sbc_proto_8_80m1[idx + 3],
            MULA(state->V[ch][offset[i] + 8], sbc_proto_8_80m0[idx + 4],
            MUL( state->V[ch][offset[k] + 9], sbc_proto_8_80m1[idx + 4]))))))))))));
    }
}

_attribute_ram_code_ int sbc_synthesize_audio(struct sbc_decoder_state *state,
                        struct sbc_frame *frame)
{
    int8_t ch, blk;

    switch (frame->subbands) {
    case 8:
        for (ch = 0; ch < frame->channels; ch++) {
            for (blk = 0; blk < frame->blocks; blk++)
                sbc_synthesize_eight(state, frame, ch, blk);
        }
        return frame->blocks * 8;

    default:
        break;
    }
    return SBC_ERR;
}
#endif

#if MSBC_ENCODE
static int sbc_analyze_audio(struct sbc_encoder_state *state,
                        struct sbc_frame *frame)
{
    int8_t ch, blk;
    int16_t *x;

    switch (frame->subbands) {
    case 8:
        for (ch = 0; ch < frame->channels; ch++) {
            x = &state->X[ch][state->position - 8 *
                    state->increment + frame->blocks * 8];
            for (blk = 0; blk < frame->blocks;
                        blk += state->increment) {
                state->sbc_analyze_8s(
                    state, x,
                    frame->sb_sample_f[blk][ch],
                    frame->sb_sample_f[blk + 1][ch] -
                    frame->sb_sample_f[blk][ch]);
                x -= 8 * state->increment;
            }
        }
        return frame->blocks * 8;

    default:
        return SBC_ERR;
    }
}

/* Supplementary bitstream writing macros for 'sbc_pack_frame' */

#define PUT_BITS(data_ptr, bits_cache, bits_count, v, n)        \
    do {                                \
        bits_cache = (v) | (bits_cache << (n));            \
        bits_count += (n);                    \
        if (bits_count >= 16) {                    \
            bits_count -= 8;                \
            *data_ptr++ = (uint8_t)                \
                (bits_cache >> bits_count);        \
            bits_count -= 8;                \
            *data_ptr++ = (uint8_t)                \
                (bits_cache >> bits_count);        \
        }                            \
    } while (0)

#define FLUSH_BITS(data_ptr, bits_cache, bits_count)            \
    do {                                \
        while (bits_count >= 8) {                \
            bits_count -= 8;                \
            *data_ptr++ = (uint8_t)                \
                (bits_cache >> bits_count);        \
        }                            \
        if (bits_count > 0)                    \
            *data_ptr++ = (uint8_t)                \
                (bits_cache << (8 - bits_count));    \
    } while (0)


/*
 * Packs the SBC frame from frame into the memory at data. At most len
 * bytes will be used, should more memory be needed an appropriate
 * error code will be returned. Returns the length of the packed frame
 * on success or a negative value on error.
 *
 * The error codes are:
 * -1 Not enough memory reserved
 * -2 Unsupported sampling rate
 * -3 Unsupported number of blocks
 * -4 Unsupported number of subbands
 * -5 Bitpool value out of bounds
 * -99 not implemented
 */

static SBC_ALWAYS_INLINE size_t sbc_pack_frame_internal(uint8_t *data,
                    struct sbc_frame *frame, size_t len,
                    int frame_subbands, int frame_channels,
                    int joint)
{
    /* Bitstream writer starts from the fourth byte */
    uint8_t *data_ptr = data + 4;
    uint32_t bits_cache = 0;
    uint32_t bits_count = 0;

    /* Will copy the header parts for CRC-8 calculation here */
    uint8_t crc_header[11] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t crc_pos = 0;

    uint32_t audio_sample;

    int8_t ch, sb, blk;    /* channel, subband, block and bit counters */
    int bits[MSBC_CHANNELS][MSBC_SUBBANDS];        /* bits distribution */
    uint32_t levels[MSBC_CHANNELS][MSBC_SUBBANDS];    /* levels are derived from that */
    uint32_t sb_sample_delta[MSBC_CHANNELS][MSBC_SUBBANDS];

    /* Can't fill in crc yet */

    crc_header[0] = data[1];
    crc_header[1] = data[2];
    crc_pos = 16;

    for (ch = 0; ch < frame_channels; ch++) {
        for (sb = 0; sb < frame_subbands; sb++) {
            PUT_BITS(data_ptr, bits_cache, bits_count,
                frame->scale_factor[ch][sb] & 0x0F, 4);
            crc_header[crc_pos >> 3] <<= 4;
            crc_header[crc_pos >> 3] |= frame->scale_factor[ch][sb] & 0x0F;
            crc_pos += 4;
        }
    }

    /* align the last crc byte */
    if (crc_pos % 8)
        crc_header[crc_pos >> 3] <<= 8 - (crc_pos % 8);

    data[3] = sbc_crc8(crc_header, crc_pos);

    sbc_calculate_bits(frame, bits);

    for (ch = 0; ch < frame_channels; ch++) {
        for (sb = 0; sb < frame_subbands; sb++) {
            levels[ch][sb] = ((1 << bits[ch][sb]) - 1) <<
                (32 - (frame->scale_factor[ch][sb] +
                    SCALE_OUT_BITS + 2));
            sb_sample_delta[ch][sb] = (uint32_t) 1 <<
                (frame->scale_factor[ch][sb] +
                    SCALE_OUT_BITS + 1);
        }
    }

    for (blk = 0; blk < frame->blocks; blk++) {
        for (ch = 0; ch < frame_channels; ch++) {
            for (sb = 0; sb < frame_subbands; sb++) {

                if (bits[ch][sb] == 0)
                    continue;
#if 1
                volatile uint64_t tmp = sb_sample_delta[ch][sb];
                tmp += frame->sb_sample_f[blk][ch][sb];
                tmp = mul64(tmp, levels[ch][sb]);
                audio_sample = *(uint32_t *)(((uint32_t *)&tmp) + 1);
#else
                audio_sample = ((uint64_t) levels[ch][sb] *
                    (sb_sample_delta[ch][sb] +
                    frame->sb_sample_f[blk][ch][sb])) >> 32;
#endif
                PUT_BITS(data_ptr, bits_cache, bits_count,
                    audio_sample, bits[ch][sb]);
            }
        }
    }

    FLUSH_BITS(data_ptr, bits_cache, bits_count);

    return data_ptr - data;
}

static size_t msbc_pack_frame(uint8_t *data, struct sbc_frame *frame,
                        size_t len, int joint)
{
    data[0] = MSBC_SYNCWORD;
    data[1] = 0;
    data[2] = 0;

    return sbc_pack_frame_internal(data, frame, len, 8, 1, joint);
}

static void sbc_encoder_init(struct sbc_encoder_state *state,
                        const struct sbc_frame *frame)
{
    tmemset(&state->X, 0, sizeof(state->X));
    state->position = (SBC_X_BUFFER_SIZE - frame->subbands * 9) & ~7;
    state->increment = 1;

    sbc_init_primitives(state);
}
#endif
_attribute_ram_code_ int sbc_init_msbc(sbc_t *sbc)
{
    struct sbc_priv *priv;

    if (!sbc)
        return SBC_ERR;

    tmemset(sbc, 0, sizeof(sbc_t));
    priv = &sbc->priv;
#if MSBC_ENCODE
    priv->pack_frame = msbc_pack_frame;
#else
    priv->unpack_frame = msbc_unpack_frame;
#endif
    sbc->frequency = SBC_FREQ_16000;
    sbc->blocks = MSBC_BLOCKS;
    sbc->subbands = SBC_SB_8;
    sbc->mode = SBC_MODE_MONO;
    sbc->allocation = SBC_AM_LOUDNESS;
    sbc->bitpool = MSBC_BITPOOL;
    sbc->endian = SBC_LE;

    return 0;
}

#if !MSBC_ENCODE
_attribute_ram_code_ size_t sbc_decode(const void *input, size_t input_len,
            void *output, size_t output_len, size_t *written)
{
    struct sbc_priv *priv;
    char *ptr;
    int8_t i, ch;
    int16_t framelen, samples;

    sbc_t *sbc = &sbc_de_ctx;

    if (!input)
        return SBC_ERR;

    msbc_init_ctx();

    priv = &sbc->priv;

	#if SBC_Streamline_Frame_Header
    u8		abuf_mic_Temp[MIC_ADPCM_FRAME_SIZE + 3];
    u32		input_len_Temp;

    input_len_Temp = MIC_ADPCM_FRAME_SIZE + 3;
    abuf_mic_Temp[0] = MSBC_SYNCWORD;
#if (SBC_SAMPLE == 8)
    abuf_mic_Temp[1] = 0x11;	// Adjust according to actual parameters
#elif (SBC_SAMPLE == 12)
    abuf_mic_Temp[1] = 0x21;
#endif
    abuf_mic_Temp[2] = MSBC_BITPOOL;

	tmemcpy (abuf_mic_Temp + 3, input, input_len);

    framelen = priv->unpack_frame(abuf_mic_Temp, &priv->frame, input_len_Temp);
	#else
    framelen = priv->unpack_frame(input, &priv->frame, input_len);
	#endif

//    framelen = priv->unpack_frame(input, &priv->frame, input_len);

    if (!priv->init) {
        sbc_decoder_init(&priv->dec_state, &priv->frame);
        priv->init = true;

        sbc->frequency = priv->frame.frequency;
        sbc->mode = priv->frame.mode;
        sbc->subbands = priv->frame.subband_mode;
        sbc->blocks = priv->frame.block_mode;
        sbc->allocation = priv->frame.allocation;
        sbc->bitpool = priv->frame.bitpool;

        priv->frame.codesize = sbc_get_codesize(0);
        priv->frame.length = framelen;
    } else if (priv->frame.bitpool != sbc->bitpool) {
        priv->frame.length = framelen;
        sbc->bitpool = priv->frame.bitpool;
    }

    if (!output)
        return framelen;

    if (written)
        *written = 0;

    if (framelen <= 0)
        return framelen;

    samples = sbc_synthesize_audio(&priv->dec_state, &priv->frame);

    ptr = output;

    if (output_len < (size_t) (samples * priv->frame.channels * 2))
        samples = output_len / (priv->frame.channels * 2);

    for (i = 0; i < samples; i++) {
        for (ch = 0; ch < priv->frame.channels; ch++) {
            int16_t s;
            s = priv->frame.pcm_sample[ch][i];

            if (sbc->endian == SBC_BE) {
                *ptr++ = (s & 0xff00) >> 8;
                *ptr++ = (s & 0x00ff);
            } else {
                *ptr++ = (s & 0x00ff);
                *ptr++ = (s & 0xff00) >> 8;
            }

        }
    }

    if (written)
        *written = samples * priv->frame.channels * 2;

    return framelen;
}

#else
size_t sbc_encode(const void *input, size_t input_len,
            void *output, size_t output_len, size_t *written)
{
    struct sbc_priv *priv;
    int16_t samples;
    uint16_t framelen;

    sbc_t *sbc = &sbc_en_ctx;

    if (!input)
        return SBC_ERR;

    priv = &sbc->priv;

    if (written)
        *written = 0;

    msbc_init_ctx();

    if (priv->init != 1) {
        priv->frame.frequency = sbc->frequency;
        priv->frame.mode = sbc->mode;
        priv->frame.channels = sbc->mode == SBC_MODE_MONO ? 1 : 2;
        priv->frame.allocation = sbc->allocation;
        priv->frame.subband_mode = sbc->subbands;
        priv->frame.subbands = sbc->subbands ? 8 : 4;
        priv->frame.block_mode = sbc->blocks;
        priv->frame.blocks = MSBC_BLOCKS;
        priv->frame.bitpool = sbc->bitpool;
        priv->frame.codesize = sbc_get_codesize(1);
        priv->frame.length = sbc_get_frame_length(1);

        sbc_encoder_init(&priv->enc_state, &priv->frame);
        priv->init = 1;
    } else if (priv->frame.bitpool != sbc->bitpool) {
        priv->frame.length = sbc_get_frame_length(1);
        priv->frame.bitpool = sbc->bitpool;
    }

    /* input must be large enough to encode a complete frame */
    if (input_len < priv->frame.codesize)
        return 0;

    /* output must be large enough to receive the encoded frame */
    if (!output || output_len < priv->frame.length)
        return SBC_ERR;

    /* Select the needed input data processing function and call it */
    priv->enc_state.position = priv->enc_state.sbc_enc_process_input_8s_le(
        priv->enc_state.position, (const uint8_t *) input,
        priv->enc_state.X, priv->frame.subbands * priv->frame.blocks,
        priv->frame.channels);

    samples = sbc_analyze_audio(&priv->enc_state, &priv->frame);

    priv->enc_state.sbc_calc_scalefactors(
        priv->frame.sb_sample_f, priv->frame.scale_factor,
        priv->frame.blocks, priv->frame.channels,
        priv->frame.subbands);
    framelen = priv->pack_frame(output, &priv->frame, output_len, 0);

    if (written)
        *written = framelen;

    return samples * priv->frame.channels * 2;
}
#endif
_attribute_ram_code_ void msbc_release_ctx()
{
#if MSBC_ENCODE
    tmemset(&sbc_en_ctx, 0, sizeof(sbc_t));
#else
    tmemset(&sbc_de_ctx, 0, sizeof(sbc_t));
#endif
}

_attribute_ram_code_ size_t sbc_get_frame_length(int type)
{
    uint16_t ret;
    uint8_t subbands, channels, blocks, bitpool;
    sbc_t *sbc;
if(type)
    sbc = &sbc_en_ctx;
else
    sbc = &sbc_de_ctx;

    struct sbc_priv *priv = &sbc->priv;

    if (priv->init && priv->frame.bitpool == sbc->bitpool)
        return priv->frame.length;

    subbands = MSBC_SUBBANDS;
    blocks = MSBC_BLOCKS;
    channels = MSBC_CHANNELS;
    bitpool = MSBC_BITPOOL;

    ret = 4 + (4 * subbands * channels) / 8;
    /* This term is not always evenly divide so we round it up */
    ret += ((blocks * channels * bitpool) + 7) / 8;

    return ret;
}

_attribute_ram_code_ size_t sbc_get_codesize(int type)
{
    uint8_t subbands, channels, blocks;
    sbc_t *sbc;
	if(type)
		sbc = &sbc_en_ctx;
	else
    sbc = &sbc_de_ctx;

    struct sbc_priv *priv = &sbc->priv;

    if (priv->init != 1) {
        subbands = MSBC_SUBBANDS;
        blocks = MSBC_BLOCKS;
        channels = MSBC_CHANNELS;
    } else {
        subbands = priv->frame.subbands;
        blocks = priv->frame.blocks;
        channels = priv->frame.channels;
    }

    return subbands * blocks * channels * 2;
}

#endif


#endif
