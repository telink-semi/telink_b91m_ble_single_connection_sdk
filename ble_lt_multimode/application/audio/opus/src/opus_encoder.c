/* Copyright (c) 2010-2011 Xiph.Org Foundation, Skype Limited
   Written by Jean-Marc Valin and Koen Vos */
/*
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

   - Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.

   - Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
   OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifdef HAVE_CONFIG_H
#include "opus_config.h"
#endif

#include <stdarg.h>
#include "celt.h"
#include "entenc.h"
#include "modes.h"
#include "API.h"
#include "stack_alloc.h"
#include "float_cast.h"
#include "opus.h"
#include "arch.h"
#include "pitch.h"
#include "opus_private.h"
#include "os_support.h"
#include "cpu_support.h"
#include "analysis.h"
#include "mathops.h"
#include "tuning_parameters.h"
#ifdef FIXED_POINT
#include "fixed/structs_FIX.h"
#else
#include "float/structs_FLP.h"
#endif
#include "../../../../tl_common.h"
#define MAX_ENCODER_BUFFER 480

#ifndef DISABLE_FLOAT_API
#define PSEUDO_SNR_THRESHOLD 316.23f    /* 10^(25/10) */
#endif

typedef struct {
   opus_val32 XX, XY, YY;
   opus_val16 smoothed_width;
   opus_val16 max_follower;
} StereoWidthState;

struct OpusEncoder {
    int          celt_enc_offset;
//    int          application;
//    int          channels;
//    int          delay_compensation;
//    int          force_channels;
//    int          signal_type;
//    int          user_bandwidth;
//    int          max_bandwidth;
//    int          user_forced_mode;
//    int          voice_ratio;
//    opus_int32   Fs;
//    int          use_vbr;
//    int          vbr_constraint;
//    int          variable_duration;
//    opus_int32   bitrate_bps;
//    opus_int32   user_bitrate_bps;
//    int          lsb_depth;
//    int          encoder_buffer;
//    int          lfe;
//    int          arch;
//    int          use_dtx;                 /* general DTX for both SILK and CELT */
#ifndef DISABLE_FLOAT_API
    TonalityAnalysisState analysis;
#endif

//#define OPUS_ENCODER_RESET_START stream_channels
//    int          stream_channels;
#if(OPUS_ENC_CHANNELS == 2)	
    opus_int16   hybrid_stereo_width_Q14;
    opus_val16   prev_HB_gain;
#endif
//    opus_int32   variable_HP_smth2_Q15;
    opus_val32   hp_mem[4];
//    int          mode;
//    int          prev_mode;
//    int          prev_channels;
//    int          prev_framesize;
//    int          bandwidth;
    /* Bandwidth determined automatically from the rate (before any other adjustment) */
//    int          auto_bandwidth;
//    int          silk_bw_switch;
    /* Sampling rate (at the API level) */
    int          first;
#if(OPUS_ENC_CHANNELS == 2)	
    opus_val16 * energy_masking;
    StereoWidthState width_mem;
#endif
    opus_val16   delay_buffer[OPUS_ENC_ENCODERBUFFER*OPUS_ENC_CHANNELS];
#ifndef DISABLE_FLOAT_API
    int          detected_bandwidth;
    int          nb_no_activity_frames;
    opus_val32   peak_signal_energy;
#endif
    int          nonfinal_frame; /* current frame is not the final in a packet */
    opus_uint32  rangeFinal;
};

/* Transition tables for the voice and music. First column is the
   middle (memoriless) threshold. The second column is the hysteresis
   (difference with the middle) */
static const opus_int32 mono_voice_bandwidth_thresholds[8] = {
         9000,  700, /* NB<->MB */
         9000,  700, /* MB<->WB */
        13500, 1000, /* WB<->SWB */
        14000, 2000, /* SWB<->FB */
};
static const opus_int32 mono_music_bandwidth_thresholds[8] = {
         9000,  700, /* NB<->MB */
         9000,  700, /* MB<->WB */
        11000, 1000, /* WB<->SWB */
        12000, 2000, /* SWB<->FB */
};
static const opus_int32 stereo_voice_bandwidth_thresholds[8] = {
         9000,  700, /* NB<->MB */
         9000,  700, /* MB<->WB */
        13500, 1000, /* WB<->SWB */
        14000, 2000, /* SWB<->FB */
};
static const opus_int32 stereo_music_bandwidth_thresholds[8] = {
         9000,  700, /* NB<->MB */
         9000,  700, /* MB<->WB */
        11000, 1000, /* WB<->SWB */
        12000, 2000, /* SWB<->FB */
};
/* Threshold bit-rates for switching between mono and stereo */
static const opus_int32 stereo_voice_threshold = 19000;
static const opus_int32 stereo_music_threshold = 17000;

/* Threshold bit-rate for switching between SILK/hybrid and CELT-only */
static const opus_int32 mode_thresholds[2][2] = {
      /* voice */ /* music */
      {  64000,      10000}, /* mono */
      {  44000,      10000}, /* stereo */
};

static const opus_int32 fec_thresholds[] = {
        12000, 1000, /* NB */
        14000, 1000, /* MB */
        16000, 1000, /* WB */
        20000, 1000, /* SWB */
        22000, 1000, /* FB */
};

static int stBuf[ (ALIGN4(sizeof(OpusEncoder)) + 4888) / 4 ] = {0};
int opus_encoder_init(OpusEncoder* st, opus_int32 Fs, int channels, int application)
{
    void *silk_enc;
    CELTEncoder *celt_enc;
    int err;
    int ret, silkEncSizeBytes;

    st->celt_enc_offset = align(sizeof(OpusEncoder));
    celt_enc = (CELTEncoder*)((char*)st+st->celt_enc_offset);
	
//    st->stream_channels = st->channels = channels;

//    OPUS_ENC_FS = Fs;

    /* Create CELT encoder */
    /* Initialize CELT encoder */
    err = celt_encoder_init(celt_enc, OPUS_ENC_FS, OPUS_ENC_CHANNELS, OPUS_ENC_ARCH);
    if(err!=OPUS_OK)return OPUS_INTERNAL_ERROR;

//	celt_enc->signalling = 0;
//	celt_enc->complexity = 0;

//    st->use_vbr = 1;
    /* Makes constrained VBR the default (safer for real-time use) */
//    st->vbr_constraint = 1;
//    st->user_bitrate_bps = OPUS_AUTO;
//    st->bitrate_bps = 3000+OPUS_ENC_FS*OPUS_ENC_CHANNELS;
//    st->application = application;
//    st->signal_type = OPUS_AUTO;
//    st->user_bandwidth = OPUS_AUTO;
//    st->max_bandwidth = OPUS_BANDWIDTH_FULLBAND;
//    st->force_channels = OPUS_AUTO;
//    st->user_forced_mode = OPUS_AUTO;
//    st->voice_ratio = -1;
//    st->encoder_buffer = OPUS_ENC_FS/100;
//    st->lsb_depth = 24;
//    st->variable_duration = OPUS_FRAMESIZE_ARG;

    /* Delay compensation of 4 ms (2.5 ms for SILK's extra look-ahead
       + 1.5 ms for SILK resamplers and stereo prediction) */
//    st->delay_compensation = st->Fs/250;

//    st->hybrid_stereo_width_Q14 = 1 << 14;
//    st->prev_HB_gain = Q15ONE;

    st->first = 1;
//    st->mode = MODE_HYBRID;
//    st->bandwidth = OPUS_BANDWIDTH_FULLBAND;

#ifndef DISABLE_FLOAT_API
    tonality_analysis_init(&st->analysis, OPUS_ENC_FS);
//    st->analysis.application = st->application;
#endif

    return OPUS_OK;
}

static unsigned char gen_toc(int mode, int framerate, int bandwidth, int channels)
{
   int period;
   unsigned char toc;
   period = 0;
   while (framerate < 400)
   {
       framerate <<= 1;
       period++;
   }
   if (mode == MODE_SILK_ONLY)
   {
       toc = (bandwidth-OPUS_BANDWIDTH_NARROWBAND)<<5;
       toc |= (period-2)<<3;
   } else if (mode == MODE_CELT_ONLY)
   {
       int tmp = bandwidth-OPUS_BANDWIDTH_MEDIUMBAND;
       if (tmp < 0)
           tmp = 0;
       toc = 0x80;
       toc |= tmp << 5;
       toc |= period<<3;
   } else /* Hybrid */
   {
       toc = 0x60;
       toc |= (bandwidth-OPUS_BANDWIDTH_SUPERWIDEBAND)<<4;
       toc |= (period-2)<<3;
   }
   toc |= (OPUS_ENC_CHANNELS==2)<<2;
   return toc;
}

#ifndef FIXED_POINT
static void silk_biquad_float(
    const opus_val16      *in,            /* I:    Input signal                   */
    const opus_int32      *B_Q28,         /* I:    MA coefficients [3]            */
    const opus_int32      *A_Q28,         /* I:    AR coefficients [2]            */
    opus_val32            *S,             /* I/O:  State vector [2]               */
    opus_val16            *out,           /* O:    Output signal                  */
    const opus_int32      len,            /* I:    Signal length (must be even)   */
    int stride
)
{
    /* DIRECT FORM II TRANSPOSED (uses 2 element state vector) */
    opus_int   k;
    opus_val32 vout;
    opus_val32 inval;
    opus_val32 A[2], B[3];

    A[0] = (opus_val32)(A_Q28[0] * (1.f/((opus_int32)1<<28)));
    A[1] = (opus_val32)(A_Q28[1] * (1.f/((opus_int32)1<<28)));
    B[0] = (opus_val32)(B_Q28[0] * (1.f/((opus_int32)1<<28)));
    B[1] = (opus_val32)(B_Q28[1] * (1.f/((opus_int32)1<<28)));
    B[2] = (opus_val32)(B_Q28[2] * (1.f/((opus_int32)1<<28)));

    /* Negate A_Q28 values and split in two parts */

    for( k = 0; k < len; k++ ) {
        /* S[ 0 ], S[ 1 ]: Q12 */
        inval = in[ k*stride ];
        vout = S[ 0 ] + B[0]*inval;

        S[ 0 ] = S[1] - vout*A[0] + B[1]*inval;

        S[ 1 ] = - vout*A[1] + B[2]*inval + VERY_SMALL;

        /* Scale back to Q0 and saturate */
        out[ k*stride ] = vout;
    }
}
#endif

static void hp_cutoff(const opus_val16 *in, opus_int32 cutoff_Hz, opus_val16 *out, opus_val32 *hp_mem, int len, int channels, opus_int32 Fs, int arch)
{
   opus_int32 B_Q28[ 3 ], A_Q28[ 2 ];
   opus_int32 Fc_Q19, r_Q28, r_Q22;
   (void)arch;

   silk_assert( cutoff_Hz <= silk_int32_MAX / SILK_FIX_CONST( 1.5 * 3.14159 / 1000, 19 ) );
   Fc_Q19 = silk_DIV32_16( silk_SMULBB( SILK_FIX_CONST( 1.5 * 3.14159 / 1000, 19 ), cutoff_Hz ), Fs/1000 );
   silk_assert( Fc_Q19 > 0 && Fc_Q19 < 32768 );

   r_Q28 = SILK_FIX_CONST( 1.0, 28 ) - silk_MUL( SILK_FIX_CONST( 0.92, 9 ), Fc_Q19 );

   /* b = r * [ 1; -2; 1 ]; */
   /* a = [ 1; -2 * r * ( 1 - 0.5 * Fc^2 ); r^2 ]; */
   B_Q28[ 0 ] = r_Q28;
   B_Q28[ 1 ] = silk_LSHIFT( -r_Q28, 1 );
   B_Q28[ 2 ] = r_Q28;

   /* -r * ( 2 - Fc * Fc ); */
   r_Q22  = silk_RSHIFT( r_Q28, 6 );
   A_Q28[ 0 ] = silk_SMULWW( r_Q22, silk_SMULWW( Fc_Q19, Fc_Q19 ) - SILK_FIX_CONST( 2.0,  22 ) );
   A_Q28[ 1 ] = silk_SMULWW( r_Q22, r_Q22 );

#ifdef FIXED_POINT
   if( OPUS_ENC_CHANNELS == 1 ) {
      //silk_biquad_alt_stride1( in, B_Q28, A_Q28, hp_mem, out, len );
   } else {
      silk_biquad_alt_stride2( in, B_Q28, A_Q28, hp_mem, out, len, OPUS_ENC_ARCH );
   }
#else
   silk_biquad_float( in, B_Q28, A_Q28, hp_mem, out, len, OPUS_ENC_CHANNELS );
   if( OPUS_ENC_CHANNELS == 2 ) {
       silk_biquad_float( in+1, B_Q28, A_Q28, hp_mem+2, out+1, len, OPUS_ENC_CHANNELS );
   }
#endif
}

#ifdef FIXED_POINT
static void dc_reject(const opus_val16 *in, opus_int32 cutoff_Hz, opus_val16 *out, opus_val32 *hp_mem, int len, int channels, opus_int32 Fs)
{
   int c, i;
   int shift;

   /* Approximates -round(log2(6.3*cutoff_Hz/Fs)) */
   shift=celt_ilog2(OPUS_ENC_FS/(cutoff_Hz*4));
   for (c=0;c<OPUS_ENC_CHANNELS;c++)
   {
      for (i=0;i<len;i++)
      {
         opus_val32 x, y;
         x = SHL32(EXTEND32(in[OPUS_ENC_CHANNELS*i+c]), 14);
         y = x-hp_mem[2*c];
         hp_mem[2*c] = hp_mem[2*c] + PSHR32(x - hp_mem[2*c], shift);
         out[OPUS_ENC_CHANNELS*i+c] = EXTRACT16(SATURATE(PSHR32(y, 14), 32767));
      }
   }
}

#else
static void dc_reject(const opus_val16 *in, opus_int32 cutoff_Hz, opus_val16 *out, opus_val32 *hp_mem, int len, int channels, opus_int32 Fs)
{
   int i;
   float coef, coef2;
   coef = 6.3f*cutoff_Hz/OPUS_ENC_FS;
   coef2 = 1-coef;
   if (OPUS_ENC_CHANNELS==2)
   {
      float m0, m2;
      m0 = hp_mem[0];
      m2 = hp_mem[2];
      for (i=0;i<len;i++)
      {
         opus_val32 x0, x1, out0, out1;
         x0 = in[2*i+0];
         x1 = in[2*i+1];
         out0 = x0-m0;
         out1 = x1-m2;
         m0 = coef*x0 + VERY_SMALL + coef2*m0;
         m2 = coef*x1 + VERY_SMALL + coef2*m2;
         out[2*i+0] = out0;
         out[2*i+1] = out1;
      }
      hp_mem[0] = m0;
      hp_mem[2] = m2;
   } else {
      float m0;
      m0 = hp_mem[0];
      for (i=0;i<len;i++)
      {
         opus_val32 x, y;
         x = in[i];
         y = x-m0;
         m0 = coef*x + VERY_SMALL + coef2*m0;
         out[i] = y;
      }
      hp_mem[0] = m0;
   }
}
#endif

static void stereo_fade(const opus_val16 *in, opus_val16 *out, opus_val16 g1, opus_val16 g2,
        int overlap48, int frame_size, int channels, const opus_val16 *window, opus_int32 Fs)
{
    int i;
    int overlap;
    int inc;
    inc = 48000/Fs;
    overlap=overlap48/inc;
    g1 = Q15ONE-g1;
    g2 = Q15ONE-g2;
    for (i=0;i<overlap;i++)
    {
       opus_val32 diff;
       opus_val16 g, w;
       w = MULT16_16_Q15(window[i*inc], window[i*inc]);
       g = SHR32(MAC16_16(MULT16_16(w,g2),
             Q15ONE-w, g1), 15);
       diff = EXTRACT16(HALF32((opus_val32)in[i*OPUS_ENC_CHANNELS] - (opus_val32)in[i*OPUS_ENC_CHANNELS+1]));
       diff = MULT16_16_Q15(g, diff);
       out[i*OPUS_ENC_CHANNELS] = out[i*OPUS_ENC_CHANNELS] - diff;
       out[i*OPUS_ENC_CHANNELS+1] = out[i*OPUS_ENC_CHANNELS+1] + diff;
    }
    for (;i<frame_size;i++)
    {
       opus_val32 diff;
       diff = EXTRACT16(HALF32((opus_val32)in[i*OPUS_ENC_CHANNELS] - (opus_val32)in[i*OPUS_ENC_CHANNELS+1]));
       diff = MULT16_16_Q15(g2, diff);
       out[i*OPUS_ENC_CHANNELS] = out[i*OPUS_ENC_CHANNELS] - diff;
       out[i*OPUS_ENC_CHANNELS+1] = out[i*OPUS_ENC_CHANNELS+1] + diff;
    }
}

static void gain_fade(const opus_val16 *in, opus_val16 *out, opus_val16 g1, opus_val16 g2,
        int overlap48, int frame_size, int channels, const opus_val16 *window, opus_int32 Fs)
{
    int i;
    int inc;
    int overlap;
    int c;
    inc = 48000/Fs;
    overlap=overlap48/inc;
    if (OPUS_ENC_CHANNELS==1)
    {
       for (i=0;i<overlap;i++)
       {
          opus_val16 g, w;
          w = MULT16_16_Q15(window[i*inc], window[i*inc]);
          g = SHR32(MAC16_16(MULT16_16(w,g2),
                Q15ONE-w, g1), 15);
          out[i] = MULT16_16_Q15(g, in[i]);
       }
    } else {
       for (i=0;i<overlap;i++)
       {
          opus_val16 g, w;
          w = MULT16_16_Q15(window[i*inc], window[i*inc]);
          g = SHR32(MAC16_16(MULT16_16(w,g2),
                Q15ONE-w, g1), 15);
          out[i*2] = MULT16_16_Q15(g, in[i*2]);
          out[i*2+1] = MULT16_16_Q15(g, in[i*2+1]);
       }
    }
    c=0;do {
       for (i=overlap;i<frame_size;i++)
       {
          out[i*OPUS_ENC_CHANNELS+c] = MULT16_16_Q15(g2, in[i*OPUS_ENC_CHANNELS+c]);
       }
    }
    while (++c<OPUS_ENC_CHANNELS);
}

OpusEncoder *opus_encoder_create(opus_int32 Fs, int channels, int application, int *error)
{
   int ret;
   OpusEncoder *st;
   st = (OpusEncoder *)(stBuf);
   if((OPUS_ENC_FS!=48000&&OPUS_ENC_FS!=24000&&OPUS_ENC_FS!=16000&&OPUS_ENC_FS!=12000&&OPUS_ENC_FS!=8000)||(OPUS_ENC_CHANNELS!=1&&OPUS_ENC_CHANNELS!=2)||
       (OPUS_ENC_APPLICATION != OPUS_APPLICATION_VOIP && OPUS_ENC_APPLICATION != OPUS_APPLICATION_AUDIO
       && OPUS_ENC_APPLICATION != OPUS_APPLICATION_RESTRICTED_LOWDELAY))
   {
      if (error)
         *error = OPUS_BAD_ARG;
      return NULL;
   }
//   st = (OpusEncoder *)opus_alloc(opus_encoder_get_size(OPUS_ENC_CHANNELS));
   if (st == NULL)
   {
      if (error)
         *error = OPUS_ALLOC_FAIL;
      return NULL;
   }
   ret = opus_encoder_init(st, OPUS_ENC_FS, OPUS_ENC_CHANNELS, OPUS_ENC_APPLICATION);
   if (error)
      *error = ret;
   if (ret != OPUS_OK)
   {
//      opus_free(st);
      st = NULL;
   }
   return st;
}

static opus_int32 user_bitrate_to_bitrate(OpusEncoder *st, int frame_size, int max_data_bytes)
{
  if(!frame_size)frame_size=OPUS_ENC_FS/400;
  if (OPUS_ENC_BITRATEBPS==OPUS_AUTO)
    return 60*OPUS_ENC_FS/frame_size + OPUS_ENC_FS*OPUS_ENC_CHANNELS;
  else if (OPUS_ENC_BITRATEBPS==OPUS_BITRATE_MAX)
    return max_data_bytes*8*OPUS_ENC_FS/frame_size;
  else
    return OPUS_ENC_BITRATEBPS;
}

#ifndef DISABLE_FLOAT_API
#ifdef FIXED_POINT
#define PCM2VAL(x) FLOAT2INT16(x)
#else
#define PCM2VAL(x) SCALEIN(x)
#endif

void downmix_float(const void *_x, opus_val32 *y, int subframe, int offset, int c1, int c2, int C)
{
   const float *x;
   int j;

   x = (const float *)_x;
   for (j=0;j<subframe;j++)
      y[j] = PCM2VAL(x[(j+offset)*C+c1]);
   if (c2>-1)
   {
      for (j=0;j<subframe;j++)
         y[j] += PCM2VAL(x[(j+offset)*C+c2]);
   } else if (c2==-2)
   {
      int c;
      for (c=1;c<C;c++)
      {
         for (j=0;j<subframe;j++)
            y[j] += PCM2VAL(x[(j+offset)*C+c]);
      }
   }
}
#endif

void downmix_int(const void *_x, opus_val32 *y, int subframe, int offset, int c1, int c2, int C)
{
   const opus_int16 *x;
   int j;

   x = (const opus_int16 *)_x;
   for (j=0;j<subframe;j++)
      y[j] = x[(j+offset)*C+c1];
   if (c2>-1)
   {
      for (j=0;j<subframe;j++)
         y[j] += x[(j+offset)*C+c2];
   } else if (c2==-2)
   {
      int c;
      for (c=1;c<C;c++)
      {
         for (j=0;j<subframe;j++)
            y[j] += x[(j+offset)*C+c];
      }
   }
}

opus_int32 frame_size_select(opus_int32 frame_size, int variable_duration, opus_int32 Fs)
{
   int new_size;
   if (frame_size<OPUS_ENC_FS/400)
      return -1;
   if (OPUS_ENC_VARIABLEDURATION == OPUS_FRAMESIZE_ARG)
      new_size = frame_size;
   else if (OPUS_ENC_VARIABLEDURATION >= OPUS_FRAMESIZE_2_5_MS && OPUS_ENC_VARIABLEDURATION <= OPUS_FRAMESIZE_120_MS)
   {
      if (OPUS_ENC_VARIABLEDURATION <= OPUS_FRAMESIZE_40_MS)
         new_size = (OPUS_ENC_FS/400)<<(OPUS_ENC_VARIABLEDURATION-OPUS_FRAMESIZE_2_5_MS);
      else
         new_size = (OPUS_ENC_VARIABLEDURATION-OPUS_FRAMESIZE_2_5_MS-2)*OPUS_ENC_FS/50;
   }
   else
      return -1;
   if (new_size>frame_size)
      return -1;
   if (400*new_size!=OPUS_ENC_FS   && 200*new_size!=OPUS_ENC_FS   && 100*new_size!=OPUS_ENC_FS   &&
        50*new_size!=OPUS_ENC_FS   &&  25*new_size!=OPUS_ENC_FS   &&  50*new_size!=3*OPUS_ENC_FS &&
        50*new_size!=4*OPUS_ENC_FS &&  50*new_size!=5*OPUS_ENC_FS &&  50*new_size!=6*OPUS_ENC_FS)
      return -1;
   return new_size;
}

opus_val16 compute_stereo_width(const opus_val16 *pcm, int frame_size, opus_int32 Fs, StereoWidthState *mem)
{
   opus_val32 xx, xy, yy;
   opus_val16 sqrt_xx, sqrt_yy;
   opus_val16 qrrt_xx, qrrt_yy;
   int frame_rate;
   int i;
   opus_val16 short_alpha;

   frame_rate = Fs/frame_size;
   short_alpha = Q15ONE - MULT16_16(25, Q15ONE)/IMAX(50,frame_rate);
   xx=xy=yy=0;
   /* Unroll by 4. The frame size is always a multiple of 4 *except* for
      2.5 ms frames at 12 kHz. Since this setting is very rare (and very
      stupid), we just discard the last two samples. */
   for (i=0;i<frame_size-3;i+=4)
   {
      opus_val32 pxx=0;
      opus_val32 pxy=0;
      opus_val32 pyy=0;
      opus_val16 x, y;
      x = pcm[2*i];
      y = pcm[2*i+1];
      pxx = SHR32(MULT16_16(x,x),2);
      pxy = SHR32(MULT16_16(x,y),2);
      pyy = SHR32(MULT16_16(y,y),2);
      x = pcm[2*i+2];
      y = pcm[2*i+3];
      pxx += SHR32(MULT16_16(x,x),2);
      pxy += SHR32(MULT16_16(x,y),2);
      pyy += SHR32(MULT16_16(y,y),2);
      x = pcm[2*i+4];
      y = pcm[2*i+5];
      pxx += SHR32(MULT16_16(x,x),2);
      pxy += SHR32(MULT16_16(x,y),2);
      pyy += SHR32(MULT16_16(y,y),2);
      x = pcm[2*i+6];
      y = pcm[2*i+7];
      pxx += SHR32(MULT16_16(x,x),2);
      pxy += SHR32(MULT16_16(x,y),2);
      pyy += SHR32(MULT16_16(y,y),2);

      xx += SHR32(pxx, 10);
      xy += SHR32(pxy, 10);
      yy += SHR32(pyy, 10);
   }
#ifndef FIXED_POINT
   if (!(xx < 1e9f) || celt_isnan(xx) || !(yy < 1e9f) || celt_isnan(yy))
   {
      xy = xx = yy = 0;
   }
#endif
   mem->XX += MULT16_32_Q15(short_alpha, xx-mem->XX);
   mem->XY += MULT16_32_Q15(short_alpha, xy-mem->XY);
   mem->YY += MULT16_32_Q15(short_alpha, yy-mem->YY);
   mem->XX = MAX32(0, mem->XX);
   mem->XY = MAX32(0, mem->XY);
   mem->YY = MAX32(0, mem->YY);
   if (MAX32(mem->XX, mem->YY)>QCONST16(8e-4f, 18))
   {
      opus_val16 corr;
      opus_val16 ldiff;
      opus_val16 width;
      sqrt_xx = celt_sqrt(mem->XX);
      sqrt_yy = celt_sqrt(mem->YY);
      qrrt_xx = celt_sqrt(sqrt_xx);
      qrrt_yy = celt_sqrt(sqrt_yy);
      /* Inter-channel correlation */
      mem->XY = MIN32(mem->XY, sqrt_xx*sqrt_yy);
      corr = SHR32(frac_div32(mem->XY,EPSILON+MULT16_16(sqrt_xx,sqrt_yy)),16);
      /* Approximate loudness difference */
      ldiff = MULT16_16(Q15ONE, ABS16(qrrt_xx-qrrt_yy))/(EPSILON+qrrt_xx+qrrt_yy);
      width = MULT16_16_Q15(celt_sqrt(QCONST32(1.f,30)-MULT16_16(corr,corr)), ldiff);
      /* Smoothing over one second */
      mem->smoothed_width += (width-mem->smoothed_width)/frame_rate;
      /* Peak follower */
      mem->max_follower = MAX16(mem->max_follower-QCONST16(.02f,15)/frame_rate, mem->smoothed_width);
   }
   /*printf("%f %f %f %f %f ", corr/(float)Q15ONE, ldiff/(float)Q15ONE, width/(float)Q15ONE, mem->smoothed_width/(float)Q15ONE, mem->max_follower/(float)Q15ONE);*/
   return EXTRACT16(MIN32(Q15ONE, MULT16_16(20, mem->max_follower)));
}

static int decide_fec(int useInBandFEC, int PacketLoss_perc, int last_fec, int mode, int *bandwidth, opus_int32 rate)
{
   int orig_bandwidth;
   if (!useInBandFEC || PacketLoss_perc == 0 || mode == MODE_CELT_ONLY)
      return 0;
   orig_bandwidth = *bandwidth;
   for (;;)
   {
      opus_int32 hysteresis;
      opus_int32 LBRR_rate_thres_bps;
      /* Compute threshold for using FEC at the current bandwidth setting */
      LBRR_rate_thres_bps = fec_thresholds[2*(*bandwidth - OPUS_BANDWIDTH_NARROWBAND)];
      hysteresis = fec_thresholds[2*(*bandwidth - OPUS_BANDWIDTH_NARROWBAND) + 1];
      if (last_fec == 1) LBRR_rate_thres_bps -= hysteresis;
      if (last_fec == 0) LBRR_rate_thres_bps += hysteresis;
      LBRR_rate_thres_bps = silk_SMULWB( silk_MUL( LBRR_rate_thres_bps,
            125 - silk_min( PacketLoss_perc, 25 ) ), SILK_FIX_CONST( 0.01, 16 ) );
      /* If loss <= 5%, we look at whether we have enough rate to enable FEC.
         If loss > 5%, we decrease the bandwidth until we can enable FEC. */
      if (rate > LBRR_rate_thres_bps)
         return 1;
      else if (PacketLoss_perc <= 5)
         return 0;
      else if (*bandwidth > OPUS_BANDWIDTH_NARROWBAND)
         (*bandwidth)--;
      else
         break;
   }
   /* Couldn't find any bandwidth to enable FEC, keep original bandwidth. */
   *bandwidth = orig_bandwidth;
   return 0;
}

static int compute_silk_rate_for_hybrid(int rate, int bandwidth, int frame20ms, int vbr, int fec, int channels) {
   int entry;
   int i;
   int N;
   int silk_rate;
   static int rate_table[][5] = {
  /*  |total| |-------- SILK------------|
              |-- No FEC -| |--- FEC ---|
               10ms   20ms   10ms   20ms */
      {    0,     0,     0,     0,     0},
      {12000, 10000, 10000, 11000, 11000},
      {16000, 13500, 13500, 15000, 15000},
      {20000, 16000, 16000, 18000, 18000},
      {24000, 18000, 18000, 21000, 21000},
      {32000, 22000, 22000, 28000, 28000},
      {64000, 38000, 38000, 50000, 50000}
   };
   /* Do the allocation per-channel. */
   rate /= OPUS_ENC_CHANNELS;
   entry = 1 + frame20ms + 2*fec;
   N = sizeof(rate_table)/sizeof(rate_table[0]);
   for (i=1;i<N;i++)
   {
      if (rate_table[i][0] > rate) break;
   }
   if (i == N)
   {
      silk_rate = rate_table[i-1][entry];
      /* For now, just give 50% of the extra bits to SILK. */
      silk_rate += (rate-rate_table[i-1][0])/2;
   } else {
      opus_int32 lo, hi, x0, x1;
      lo = rate_table[i-1][entry];
      hi = rate_table[i][entry];
      x0 = rate_table[i-1][0];
      x1 = rate_table[i][0];
      silk_rate = (lo*(x1-rate) + hi*(rate-x0))/(x1-x0);
   }
   if (!OPUS_ENC_USE_VBR)
   {
      /* Tiny boost to SILK for CBR. We should probably tune this better. */
      silk_rate += 100;
   }
   if (bandwidth==OPUS_BANDWIDTH_SUPERWIDEBAND)
      silk_rate += 300;
   silk_rate *= OPUS_ENC_CHANNELS;
   /* Small adjustment for stereo (calibrated for 32 kb/s, haven't tried other bitrates). */
   if (OPUS_ENC_CHANNELS == 2 && rate >= 12000)
      silk_rate -= 1000;
   return silk_rate;
}

/* Returns the equivalent bitrate corresponding to 20 ms frames,
   complexity 10 VBR operation. */
static opus_int32 compute_equiv_rate(opus_int32 bitrate, int channels,
      int frame_rate, int vbr, int mode, int complexity, int loss)
{
   opus_int32 equiv;
   equiv = bitrate;
   /* Take into account overhead from smaller frames. */
   if (frame_rate > 50)
      equiv -= (40*OPUS_ENC_CHANNELS+20)*(frame_rate - 50);
   /* CBR is about a 8% penalty for both SILK and CELT. */
   if (!OPUS_ENC_USE_VBR)
      equiv -= equiv/12;
   /* Complexity makes about 10% difference (from 0 to 10) in general. */
   equiv = equiv * (90+OPUS_ENC_COMPLEXITY)/100;
   if (mode == MODE_SILK_ONLY || mode == MODE_HYBRID)
   {
      /* SILK complexity 0-1 uses the non-delayed-decision NSQ, which
         costs about 20%. */
      if (OPUS_ENC_COMPLEXITY<2)
         equiv = equiv*4/5;
      equiv -= equiv*loss/(6*loss + 10);
   } else if (mode == MODE_CELT_ONLY) {
      /* CELT complexity 0-4 doesn't have the pitch filter, which costs
         about 10%. */
      if (OPUS_ENC_COMPLEXITY<5)
         equiv = equiv*9/10;
   } else {
      /* Mode not known yet */
      /* Half the SILK loss*/
      equiv -= equiv*loss/(12*loss + 20);
   }
   return equiv;
}

#ifndef DISABLE_FLOAT_API

int is_digital_silence(const opus_val16* pcm, int frame_size, int channels, int lsb_depth)
{
   int silence = 0;
   opus_val32 sample_max = 0;
#ifdef MLP_TRAINING
   return 0;
#endif
   sample_max = celt_maxabs16(pcm, frame_size*OPUS_ENC_CHANNELS);

#ifdef FIXED_POINT
   silence = (sample_max == 0);
   (void)lsb_depth;
#else
   silence = (sample_max <= (opus_val16) 1 / (1 << OPUS_ENC_LSBDEPTH));
#endif

   return silence;
}

#ifdef FIXED_POINT
static opus_val32 compute_frame_energy(const opus_val16 *pcm, int frame_size, int channels, int arch)
{
   int i;
   opus_val32 sample_max;
   int max_shift;
   int shift;
   opus_val32 energy = 0;
   int len = frame_size*OPUS_ENC_CHANNELS;
   (void)arch;
   /* Max amplitude in the signal */
   sample_max = celt_maxabs16(pcm, len);

   /* Compute the right shift required in the MAC to avoid an overflow */
   max_shift = celt_ilog2(len);
   shift = IMAX(0, (celt_ilog2(sample_max) << 1) + max_shift - 28);

   /* Compute the energy */
   for (i=0; i<len; i++)
      energy += SHR32(MULT16_16(pcm[i], pcm[i]), shift);

   /* Normalize energy by the frame size and left-shift back to the original position */
   energy /= len;
   energy = SHL32(energy, shift);

   return energy;
}
#else
static opus_val32 compute_frame_energy(const opus_val16 *pcm, int frame_size, int channels, int arch)
{
   int len = OPUS_ENC_FRAMESIZE*OPUS_ENC_CHANNELS;
   return celt_inner_prod(pcm, pcm, len, OPUS_ENC_ARCH)/OPUS_ENC_FRAMESIZE;
}
#endif

/* Decides if DTX should be turned on (=1) or off (=0) */
static int decide_dtx_mode(float activity_probability,    /* probability that current frame contains speech/music */
                           int *nb_no_activity_frames,    /* number of consecutive frames with no activity */
                           opus_val32 peak_signal_energy, /* peak energy of desired signal detected so far */
                           const opus_val16 *pcm,         /* input pcm signal */
                           int frame_size,                /* frame size */
                           int channels,
                           int is_silence,                 /* only digital silence detected in this frame */
                           int arch
                          )
{
   opus_val32 noise_energy;

   if (!is_silence)
   {
      if (activity_probability < DTX_ACTIVITY_THRESHOLD)  /* is noise */
      {
         noise_energy = compute_frame_energy(pcm, OPUS_ENC_FRAMESIZE, OPUS_ENC_CHANNELS, OPUS_ENC_ARCH);

         /* but is sufficiently quiet */
         is_silence = peak_signal_energy >= (PSEUDO_SNR_THRESHOLD * noise_energy);
      }
   }

   if (is_silence)
   {
      /* The number of consecutive DTX frames should be within the allowed bounds */
      (*nb_no_activity_frames)++;

      if (*nb_no_activity_frames > NB_SPEECH_FRAMES_BEFORE_DTX)
      {
         if (*nb_no_activity_frames <= (NB_SPEECH_FRAMES_BEFORE_DTX + MAX_CONSECUTIVE_DTX))
            /* Valid frame for DTX! */
            return 1;
         else
            (*nb_no_activity_frames) = NB_SPEECH_FRAMES_BEFORE_DTX;
      }
   } else
      (*nb_no_activity_frames) = 0;

   return 0;
}

#endif

static opus_int32 encode_multiframe_packet(OpusEncoder *st,
                                           const opus_val16 *pcm,
                                           int nb_frames,
                                           int frame_size,
                                           unsigned char *data,
                                           opus_int32 out_data_bytes,
                                           int to_celt,
                                           int lsb_depth,
                                           int float_api)
{
   int i;
   int ret = 0;
   VARDECL(unsigned char, tmp_data);
   int bak_mode, bak_bandwidth, bak_channels, bak_to_mono;
   VARDECL(OpusRepacketizer, rp);
   int max_header_bytes;
   opus_int32 bytes_per_frame;
   opus_int32 cbr_bytes;
   opus_int32 repacketize_len;
   int tmp_len;
   ALLOC_STACK;

   /* Worst cases:
    * 2 frames: Code 2 with different compressed sizes
    * >2 frames: Code 3 VBR */
   max_header_bytes = nb_frames == 2 ? 3 : (2+(nb_frames-1)*2);

   if (OPUS_ENC_USE_VBR || OPUS_ENC_BITRATEBPS==OPUS_BITRATE_MAX)
      repacketize_len = out_data_bytes;
   else {
      cbr_bytes = 3*OPUS_ENC_BITRATEBPS/(3*8*OPUS_ENC_FS/(OPUS_ENC_FRAMESIZE*nb_frames));
      repacketize_len = IMIN(cbr_bytes, out_data_bytes);
   }
   bytes_per_frame = IMIN(1276, 1+(repacketize_len-max_header_bytes)/nb_frames);

   ALLOC(tmp_data, nb_frames*bytes_per_frame, unsigned char);
   ALLOC(rp, 1, OpusRepacketizer);
   opus_repacketizer_init(rp);

   for (i=0;i<nb_frames;i++)
   {
      st->nonfinal_frame = i<(nb_frames-1);

      tmp_len = opus_encode_native(st, pcm+i*(OPUS_ENC_CHANNELS*OPUS_ENC_FRAMESIZE), OPUS_ENC_FRAMESIZE,
         tmp_data+i*bytes_per_frame, bytes_per_frame, OPUS_ENC_LSBDEPTH, NULL, 0, 0, 0, 0,
         NULL, float_api);

      if (tmp_len<0)
      {
         RESTORE_STACK;
         return OPUS_INTERNAL_ERROR;
      }

      ret = opus_repacketizer_cat(rp, tmp_data+i*bytes_per_frame, tmp_len);

      if (ret<0)
      {
         RESTORE_STACK;
         return OPUS_INTERNAL_ERROR;
      }
   }

   ret = opus_repacketizer_out_range_impl(rp, 0, nb_frames, data, repacketize_len, 0, !OPUS_ENC_USE_VBR);

   if (ret<0)
   {
      RESTORE_STACK;
      return OPUS_INTERNAL_ERROR;
   }

   RESTORE_STACK;
   return ret;
}

static int compute_redundancy_bytes(opus_int32 max_data_bytes, opus_int32 bitrate_bps, int frame_rate, int channels)
{
   int redundancy_bytes_cap;
   int redundancy_bytes;
   opus_int32 redundancy_rate;
   int base_bits;
   opus_int32 available_bits;
   base_bits = (40*OPUS_ENC_CHANNELS+20);

   /* Equivalent rate for 5 ms frames. */
   redundancy_rate = bitrate_bps + base_bits*(200 - frame_rate);
   /* For VBR, further increase the bitrate if we can afford it. It's pretty short
      and we'll avoid artefacts. */
   redundancy_rate = 3*redundancy_rate/2;
   redundancy_bytes = redundancy_rate/1600;

   /* Compute the max rate we can use given CBR or VBR with cap. */
   available_bits = max_data_bytes*8 - 2*base_bits;
   redundancy_bytes_cap = (available_bits*240/(240+48000/frame_rate) + base_bits)/8;
   redundancy_bytes = IMIN(redundancy_bytes, redundancy_bytes_cap);
   /* It we can't get enough bits for redundancy to be worth it, rely on the decoder PLC. */
   if (redundancy_bytes > 4 + 8*OPUS_ENC_CHANNELS)
      redundancy_bytes = IMIN(257, redundancy_bytes);
   else
      redundancy_bytes = 0;
   return redundancy_bytes;
}

extern void memcpy4(void * d, const void * s, int len);
void * memmove4(void * out, const void * in, int len);
void * memset4(void * dest, int val, int len);

opus_int32 opus_encode_native(OpusEncoder *st, const opus_val16 *pcm, int frame_size,
                unsigned char *data, opus_int32 out_data_bytes, int lsb_depth,
                const void *analysis_pcm, opus_int32 analysis_size, int c1, int c2,
                int analysis_channels, downmix_func downmix, int float_api)
{
//    void *silk_enc;
    OpusCustomEncoder *celt_enc;
    int i;
    int ret=0;
    opus_int32 nBytes;
    ec_enc enc;
    int bytes_target;
    int prefill=0;
    int start_band = 0;
    int redundancy = 0;
    int redundancy_bytes = 0; /* Number of bytes to use for redundancy frame */
    int celt_to_silk = 0;
    VARDECL(opus_val16, pcm_buf);
    int nb_compr_bytes;
    int to_celt = 0;
    opus_uint32 redundant_rng = 0;
    int cutoff_Hz, hp_freq_smth1;
    int voice_est; /* Probability of voice in Q7 */
    opus_int32 equiv_rate;
    int delay_compensation;
    int frame_rate;
    opus_int32 max_rate; /* Max bitrate we're allowed to use */
//    int curr_bandwidth;
    opus_val16 HB_gain;
    opus_int32 max_data_bytes; /* Max number of bytes we're allowed to use */
//    int total_buffer;
    opus_val16 stereo_width;
    const CELTMode *celt_mode;
#ifndef DISABLE_FLOAT_API
    AnalysisInfo analysis_info;
    int analysis_read_pos_bak=-1;
    int analysis_read_subframe_bak=-1;
    int is_silence = 0;
#endif
    VARDECL(opus_val16, tmp_prefill);

    ALLOC_STACK;

    max_data_bytes = IMIN(1276, out_data_bytes);

    st->rangeFinal = 0;
    if (OPUS_ENC_FRAMESIZE <= 0 || max_data_bytes <= 0)
    {
       RESTORE_STACK;
       return OPUS_BAD_ARG;
    }

    /* Cannot encode 100 ms in 1 byte */
    if (max_data_bytes==1 && OPUS_ENC_FS==(OPUS_ENC_FRAMESIZE*10))
    {
      RESTORE_STACK;
      return OPUS_BUFFER_TOO_SMALL;
    }

    celt_enc = (CELTEncoder*)((char*)st+st->celt_enc_offset);
    if (OPUS_ENC_APPLICATION == OPUS_APPLICATION_RESTRICTED_LOWDELAY)
       delay_compensation = 0;
    else
       delay_compensation = OPUS_ENC_DELYCOMPENSATION;

//    lsb_depth = IMIN(lsb_depth, st->lsb_depth);

	celt_mode = celt_enc->mode;
#ifndef DISABLE_FLOAT_API
    analysis_info.valid = 0;
#ifdef FIXED_POINT
    if (OPUS_ENC_COMPLEXITY >= 10 && OPUS_ENC_FS>=16000)
#else
    if (OPUS_ENC_COMPLEXITY>= 7 && OPUS_ENC_FS>=16000)
#endif
    {
       is_silence = is_digital_silence(pcm, OPUS_ENC_FRAMESIZE, OPUS_ENC_CHANNELS, OPUS_ENC_LSBDEPTH);
       analysis_read_pos_bak = st->analysis.read_pos;
       analysis_read_subframe_bak = st->analysis.read_subframe;
       run_analysis(&st->analysis, celt_mode, analysis_pcm, OPUS_ENC_FRAMESIZE, OPUS_ENC_FRAMESIZE,
             c1, c2, analysis_channels, OPUS_ENC_FS,
             OPUS_ENC_LSBDEPTH, downmix, &analysis_info);

       /* Track the peak signal energy */
       if (!is_silence && analysis_info.activity_probability > DTX_ACTIVITY_THRESHOLD)
          st->peak_signal_energy = MAX32(MULT16_32_Q15(QCONST16(0.999f, 15), st->peak_signal_energy),
                compute_frame_energy(pcm, OPUS_ENC_FRAMESIZE, OPUS_ENC_CHANNELS, OPUS_ENC_ARCH));
    } else if (st->analysis.initialized) {
       tonality_analysis_reset(&st->analysis);
    }
#else
    (void)analysis_pcm;
    (void)analysis_size;
    (void)c1;
    (void)c2;
    (void)analysis_channels;
    (void)downmix;
#endif

#ifndef DISABLE_FLOAT_API
    /* Reset voice_ratio if this frame is not silent or if analysis is disabled.
     * Otherwise, preserve voice_ratio from the last non-silent frame */
    if (!is_silence)
      st->voice_ratio = -1;

    st->detected_bandwidth = 0;
    if (analysis_info.valid)
    {
       int analysis_bandwidth;
       if (OPUS_ENC_SIGNALTYPE == OPUS_AUTO)
       {
          float prob;
          if (OPUS_ENC_MODE == 0)
             prob = analysis_info.music_prob;
          else if (OPUS_ENC_MODE == MODE_CELT_ONLY)
             prob = analysis_info.music_prob_max;
          else
             prob = analysis_info.music_prob_min;
          st->voice_ratio = (int)floor(.5+100*(1-prob));
       }

       analysis_bandwidth = analysis_info.bandwidth;
       if (analysis_bandwidth<=12)
          st->detected_bandwidth = OPUS_BANDWIDTH_NARROWBAND;
       else if (analysis_bandwidth<=14)
          st->detected_bandwidth = OPUS_BANDWIDTH_MEDIUMBAND;
       else if (analysis_bandwidth<=16)
          st->detected_bandwidth = OPUS_BANDWIDTH_WIDEBAND;
       else if (analysis_bandwidth<=18)
          st->detected_bandwidth = OPUS_BANDWIDTH_SUPERWIDEBAND;
       else
          st->detected_bandwidth = OPUS_BANDWIDTH_FULLBAND;
    }
#else
//    st->voice_ratio = -1;
#endif

    #if(OPUS_ENC_CHANNELS==2 && OPUS_ENC_FORCECHANNELS!=1)
       stereo_width = compute_stereo_width(pcm, OPUS_ENC_FRAMESIZE, OPUS_ENC_FS, &st->width_mem);
    #else
       stereo_width = 0;
    #endif
//    total_buffer = delay_compensation;
//    st->bitrate_bps = user_bitrate_to_bitrate(st, frame_size, max_data_bytes);

    frame_rate = OPUS_ENC_FS/OPUS_ENC_FRAMESIZE;
    if (!OPUS_ENC_USE_VBR)
    {
       int cbrBytes;
       /* Multiply by 12 to make sure the division is exact. */
       int frame_rate12 = 12*OPUS_ENC_FS/OPUS_ENC_FRAMESIZE;
       /* We need to make sure that "int" values always fit in 16 bits. */
       cbrBytes = IMIN( (12*OPUS_ENC_BITRATEBPS/8 + frame_rate12/2)/frame_rate12, max_data_bytes);
//       st->bitrate_bps = cbrBytes*(opus_int32)frame_rate12*8/12;
       /* Make sure we provide at least one byte to avoid failing. */
       max_data_bytes = IMAX(1, cbrBytes);
    }
    if (max_data_bytes<3 || OPUS_ENC_BITRATEBPS < 3*frame_rate*8
       || (frame_rate<50 && (max_data_bytes*frame_rate<300 || OPUS_ENC_BITRATEBPS < 2400)))
    {
       /*If the space is too low to do something useful, emit 'PLC' frames.*/
       int tocmode = OPUS_ENC_MODE;
       int bw = OPUS_ENC_BANDWIDTH == 0 ? OPUS_BANDWIDTH_NARROWBAND : OPUS_ENC_BANDWIDTH;
       int packet_code = 0;
       int num_multiframes = 0;

       if (tocmode==0)
          tocmode = MODE_SILK_ONLY;
       if (frame_rate>100)
          tocmode = MODE_CELT_ONLY;
       /* 40 ms -> 2 x 20 ms if in CELT_ONLY or HYBRID mode */
       if (frame_rate==25 && tocmode!=MODE_SILK_ONLY)
       {
          frame_rate = 50;
          packet_code = 1;
       }

       /* >= 60 ms frames */
       if (frame_rate<=16)
       {
          /* 1 x 60 ms, 2 x 40 ms, 2 x 60 ms */
          if (out_data_bytes==1 || (tocmode==MODE_SILK_ONLY && frame_rate!=10))
          {
             tocmode = MODE_SILK_ONLY;

             packet_code = frame_rate <= 12;
             frame_rate = frame_rate == 12 ? 25 : 16;
          }
          else
          {
             num_multiframes = 50/frame_rate;
             frame_rate = 50;
             packet_code = 3;
          }
       }

       if(tocmode==MODE_SILK_ONLY&&bw>OPUS_BANDWIDTH_WIDEBAND)
          bw=OPUS_BANDWIDTH_WIDEBAND;
       else if (tocmode==MODE_CELT_ONLY&&bw==OPUS_BANDWIDTH_MEDIUMBAND)
          bw=OPUS_BANDWIDTH_NARROWBAND;
       else if (tocmode==MODE_HYBRID&&bw<=OPUS_BANDWIDTH_SUPERWIDEBAND)
          bw=OPUS_BANDWIDTH_SUPERWIDEBAND;

       data[0] = gen_toc(tocmode, frame_rate, bw, OPUS_ENC_STREAMCHANNELS);
       data[0] |= packet_code;

       ret = packet_code <= 1 ? 1 : 2;

       max_data_bytes = IMAX(max_data_bytes, ret);

       if (packet_code==3)
          data[1] = num_multiframes;

       if (!OPUS_ENC_USE_VBR)
       {
          ret = opus_packet_pad(data, ret, max_data_bytes);
          if (ret == OPUS_OK)
             ret = max_data_bytes;
          else
             ret = OPUS_INTERNAL_ERROR;
       }
       RESTORE_STACK;
       return ret;
    }
    max_rate = frame_rate*max_data_bytes*8;

    /* Equivalent 20-ms rate for mode/channel/bandwidth decisions */
    equiv_rate = compute_equiv_rate(OPUS_ENC_BITRATEBPS, OPUS_ENC_CHANNELS, OPUS_ENC_FS/OPUS_ENC_FRAMESIZE,
          OPUS_ENC_USE_VBR, 0, OPUS_ENC_COMPLEXITY, OPUS_ENC_PACKETLOSSPERCENTAGE);

#if(OPUS_ENC_BANDWIDTH != OPUS_BANDWIDTH_WIDEBAND)
    if (OPUS_ENC_SIGNALTYPE == OPUS_SIGNAL_VOICE)
       voice_est = 127;
    else if (OPUS_ENC_SIGNALTYPE == OPUS_SIGNAL_MUSIC)
       voice_est = 0;
    else if (OPUS_ENC_VOICERATIO >= 0)
    {
       voice_est = OPUS_ENC_VOICERATIO*327>>8;
       /* For AUDIO, never be more than 90% confident of having speech */
       if (OPUS_ENC_APPLICATION == OPUS_APPLICATION_AUDIO)
          voice_est = IMIN(voice_est, 115);
    } else if (OPUS_ENC_APPLICATION == OPUS_APPLICATION_VOIP)
       voice_est = 115;
    else
       voice_est = 48;
#endif

    if (OPUS_ENC_FORCECHANNELS!=OPUS_AUTO && OPUS_ENC_CHANNELS == 2)
    {
//        OPUS_ENC_STREAMCHANNELS = OPUS_ENC_FORCECHANNELS;
    } else {
#ifdef FUZZING
       /* Random mono/stereo decision */
       if (OPUS_ENC_CHANNELS == 2 && (rand()&0x1F)==0){
//          st->stream_channels = 3-OPUS_ENC_STREAMCHANNELS;
       }
#else
       /* Rate-dependent mono-stereo decision */
       if (OPUS_ENC_CHANNELS == 2)
       {
          opus_int32 stereo_threshold;
          stereo_threshold = stereo_music_threshold + ((voice_est*voice_est*(stereo_voice_threshold-stereo_music_threshold))>>14);
          if (OPUS_ENC_STREAMCHANNELS == 2)
             stereo_threshold -= 1000;
          else
             stereo_threshold += 1000;
//          st->stream_channels = (equiv_rate > stereo_threshold) ? 2 : 1;
       } else {
//          st->stream_channels = OPUS_ENC_CHANNELS;
       }
#endif
    }
    /* Update equivalent rate for channels decision. */
    equiv_rate = compute_equiv_rate(OPUS_ENC_BITRATEBPS, OPUS_ENC_STREAMCHANNELS, OPUS_ENC_FS/OPUS_ENC_FRAMESIZE,
          OPUS_ENC_USE_VBR, 0, OPUS_ENC_COMPLEXITY, OPUS_ENC_PACKETLOSSPERCENTAGE);

//    st->mode = MODE_CELT_ONLY;

    /* Update equivalent rate with mode decision. */
    equiv_rate = compute_equiv_rate(OPUS_ENC_BITRATEBPS, OPUS_ENC_STREAMCHANNELS, OPUS_ENC_FS/OPUS_ENC_FRAMESIZE,
          OPUS_ENC_USE_VBR, OPUS_ENC_MODE, OPUS_ENC_COMPLEXITY, OPUS_ENC_PACKETLOSSPERCENTAGE);
	
#if(OPUS_ENC_BANDWIDTH != OPUS_BANDWIDTH_WIDEBAND)
    /* Automatic (rate-dependent) bandwidth selection */
    if (OPUS_ENC_MODE == MODE_CELT_ONLY /* || st->first || st->silk_mode.allowBandwidthSwitch */)
    {
        const opus_int32 *voice_bandwidth_thresholds, *music_bandwidth_thresholds;
        opus_int32 bandwidth_thresholds[8];
        int bandwidth = OPUS_BANDWIDTH_FULLBAND;

        if (OPUS_ENC_CHANNELS==2 && OPUS_ENC_FORCECHANNELS!=1)
        {
           voice_bandwidth_thresholds = stereo_voice_bandwidth_thresholds;
           music_bandwidth_thresholds = stereo_music_bandwidth_thresholds;
        } else {
           voice_bandwidth_thresholds = mono_voice_bandwidth_thresholds;
           music_bandwidth_thresholds = mono_music_bandwidth_thresholds;
        }
        /* Interpolate bandwidth thresholds depending on voice estimation */
        for (i=0;i<8;i++)
        {
           bandwidth_thresholds[i] = music_bandwidth_thresholds[i]
                    + ((voice_est*voice_est*(voice_bandwidth_thresholds[i]-music_bandwidth_thresholds[i]))>>14);
        }
        do {
            int threshold, hysteresis;
            threshold = bandwidth_thresholds[2*(bandwidth-OPUS_BANDWIDTH_MEDIUMBAND)];
            hysteresis = bandwidth_thresholds[2*(bandwidth-OPUS_BANDWIDTH_MEDIUMBAND)+1];
            if (!st->first)
            {
                if (st->auto_bandwidth >= bandwidth)
                    threshold -= hysteresis;
                else
                    threshold += hysteresis;
            }
            if (equiv_rate >= threshold)
                break;
        } while (--bandwidth>OPUS_BANDWIDTH_NARROWBAND);
        /* We don't use mediumband anymore, except when explicitly requested or during
           mode transitions. */
        if (bandwidth == OPUS_BANDWIDTH_MEDIUMBAND)
           bandwidth = OPUS_BANDWIDTH_WIDEBAND;
        st->bandwidth = st->auto_bandwidth = bandwidth;
    }

    if (st->bandwidth>st->max_bandwidth)
       st->bandwidth = st->max_bandwidth;

    if (OPUS_ENC_BANDWIDTH != OPUS_AUTO)
        st->bandwidth = st->user_bandwidth;

    /* Prevents Opus from wasting bits on frequencies that are above
       the Nyquist rate of the input signal */
    if (OPUS_ENC_FS <= 24000 && st->bandwidth > OPUS_BANDWIDTH_SUPERWIDEBAND)
        st->bandwidth = OPUS_BANDWIDTH_SUPERWIDEBAND;
    if (OPUS_ENC_FS <= 16000 && st->bandwidth > OPUS_BANDWIDTH_WIDEBAND)
        st->bandwidth = OPUS_BANDWIDTH_WIDEBAND;
    if (OPUS_ENC_FS <= 12000 && st->bandwidth > OPUS_BANDWIDTH_MEDIUMBAND)
        st->bandwidth = OPUS_BANDWIDTH_MEDIUMBAND;
    if (OPUS_ENC_FS <= 8000 && st->bandwidth > OPUS_BANDWIDTH_NARROWBAND)
        st->bandwidth = OPUS_BANDWIDTH_NARROWBAND;
#ifndef DISABLE_FLOAT_API
    /* Use detected bandwidth to reduce the encoded bandwidth. */
    if (st->detected_bandwidth && OPUS_ENC_BANDWIDTH == OPUS_AUTO)
    {
       int min_detected_bandwidth;
       /* Makes bandwidth detection more conservative just in case the detector
          gets it wrong when we could have coded a high bandwidth transparently.
          When operating in SILK/hybrid mode, we don't go below wideband to avoid
          more complicated switches that require redundancy. */
       if (equiv_rate <= 18000*OPUS_ENC_STREAMCHANNELS && OPUS_ENC_MODE == MODE_CELT_ONLY)
          min_detected_bandwidth = OPUS_BANDWIDTH_NARROWBAND;
       else if (equiv_rate <= 24000*OPUS_ENC_STREAMCHANNELS && OPUS_ENC_MODE == MODE_CELT_ONLY)
          min_detected_bandwidth = OPUS_BANDWIDTH_MEDIUMBAND;
       else if (equiv_rate <= 30000*OPUS_ENC_STREAMCHANNELS)
          min_detected_bandwidth = OPUS_BANDWIDTH_WIDEBAND;
       else if (equiv_rate <= 44000*OPUS_ENC_STREAMCHANNELS)
          min_detected_bandwidth = OPUS_BANDWIDTH_SUPERWIDEBAND;
       else
          min_detected_bandwidth = OPUS_BANDWIDTH_FULLBAND;

       st->detected_bandwidth = IMAX(st->detected_bandwidth, min_detected_bandwidth);
       st->bandwidth = IMIN(st->bandwidth, st->detected_bandwidth);
    }
#endif
#endif

//	celt_enc->lsb_depth = lsb_depth;

#if(OPUS_ENC_BANDWIDTH != OPUS_BANDWIDTH_WIDEBAND)
    /* CELT mode doesn't support mediumband, use wideband instead */
    if (OPUS_ENC_MODE == MODE_CELT_ONLY && st->bandwidth == OPUS_BANDWIDTH_MEDIUMBAND)
        st->bandwidth = OPUS_BANDWIDTH_WIDEBAND;
    if (OPUS_ENC_LFE)
       st->bandwidth = OPUS_BANDWIDTH_NARROWBAND;
#endif

    const int curr_bandwidth = OPUS_ENC_BANDWIDTH;

    /* Chooses the appropriate mode for speech
       *NEVER* switch to/from CELT-only mode here as this will invalidate some assumptions */
//    if (OPUS_ENC_MODE == MODE_SILK_ONLY && curr_bandwidth > OPUS_BANDWIDTH_WIDEBAND)
//        st->mode = MODE_HYBRID;
//    if (OPUS_ENC_MODE == MODE_HYBRID && curr_bandwidth <= OPUS_BANDWIDTH_WIDEBAND)
//        st->mode = MODE_SILK_ONLY;

    /* Can't support higher than >60 ms frames, and >20 ms when in Hybrid or CELT-only modes */
    if ((OPUS_ENC_FRAMESIZE > OPUS_ENC_FS/50 && (OPUS_ENC_MODE != MODE_SILK_ONLY)) || OPUS_ENC_FRAMESIZE > 3*OPUS_ENC_FS/50)
    {
       int enc_frame_size;
       int nb_frames;

       if (OPUS_ENC_MODE == MODE_SILK_ONLY)
       {
         if (OPUS_ENC_FRAMESIZE == 2*OPUS_ENC_FS/25)  /* 80 ms -> 2x 40 ms */
           enc_frame_size = OPUS_ENC_FS/25;
         else if (OPUS_ENC_FRAMESIZE == 3*OPUS_ENC_FS/25)  /* 120 ms -> 2x 60 ms */
           enc_frame_size = 3*OPUS_ENC_FS/50;
         else                            /* 100 ms -> 5x 20 ms */
           enc_frame_size = OPUS_ENC_FS/50;
       }
       else
         enc_frame_size = OPUS_ENC_FS/50;

       nb_frames = OPUS_ENC_FRAMESIZE/OPUS_ENC_FRAMESIZE;

#ifndef DISABLE_FLOAT_API
       if (analysis_read_pos_bak!= -1)
       {
          st->analysis.read_pos = analysis_read_pos_bak;
          st->analysis.read_subframe = analysis_read_subframe_bak;
       }
#endif

       ret = encode_multiframe_packet(st, pcm, nb_frames, OPUS_ENC_FRAMESIZE, data,
                                      out_data_bytes, to_celt, OPUS_ENC_LSBDEPTH, float_api);

       RESTORE_STACK;
       return ret;
    }

    /* printf("%d %d %d %d\n", st->bitrate_bps, st->stream_channels, st->mode, curr_bandwidth); */
    bytes_target = IMIN(max_data_bytes-redundancy_bytes, OPUS_ENC_BITRATEBPS * OPUS_ENC_FRAMESIZE / (OPUS_ENC_FS * 8)) - 1;

    data += 1;

    ec_enc_init(&enc, data, max_data_bytes-1);

    ALLOC(pcm_buf, (OPUS_ENC_TOTALBUFFER+OPUS_ENC_FRAMESIZE)*OPUS_ENC_CHANNELS, opus_val16);
    OPUS_COPY4(pcm_buf, &st->delay_buffer[(OPUS_ENC_ENCODERBUFFER-OPUS_ENC_TOTALBUFFER)*OPUS_ENC_CHANNELS], OPUS_ENC_TOTALBUFFER*OPUS_ENC_CHANNELS);
//    if (OPUS_ENC_MODE == MODE_CELT_ONLY)
//       hp_freq_smth1 = silk_LSHIFT( silk_lin2log( VARIABLE_HP_MIN_CUTOFF_HZ ), 8 );

    dc_reject(pcm, 3, &pcm_buf[OPUS_ENC_TOTALBUFFER*OPUS_ENC_CHANNELS], st->hp_mem, OPUS_ENC_FRAMESIZE, OPUS_ENC_CHANNELS, OPUS_ENC_FS);
#if DEBUG_LOG
    for(unsigned char i=0;i<160;i++)
    {
    	printf("%d,",pcm_buf[i]);
    }
#endif
#ifndef FIXED_POINT
    if (float_api)
    {
       opus_val32 sum;
       sum = celt_inner_prod(&pcm_buf[OPUS_ENC_TOTALBUFFER*OPUS_ENC_CHANNELS], &pcm_buf[OPUS_ENC_TOTALBUFFER*OPUS_ENC_CHANNELS], OPUS_ENC_FRAMESIZE*OPUS_ENC_CHANNELS, OPUS_ENC_ARCH);
       /* This should filter out both NaNs and ridiculous signals that could
          cause NaNs further down. */
       if (!(sum < 1e9f) || celt_isnan(sum))
       {
          OPUS_CLEAR(&pcm_buf[OPUS_ENC_TOTALBUFFER*OPUS_ENC_CHANNELS], OPUS_ENC_FRAMESIZE*OPUS_ENC_CHANNELS);
          st->hp_mem[0] = st->hp_mem[1] = st->hp_mem[2] = st->hp_mem[3] = 0;
       }
    }
#endif


    /* SILK processing */
    HB_gain = Q15ONE;
    /* CELT processing */
    {
        int endband=21;

        switch(curr_bandwidth)
        {
            case OPUS_BANDWIDTH_NARROWBAND:
                endband = 13;
                break;
            case OPUS_BANDWIDTH_MEDIUMBAND:
            case OPUS_BANDWIDTH_WIDEBAND:
                endband = 17;
                break;
            case OPUS_BANDWIDTH_SUPERWIDEBAND:
                endband = 19;
                break;
            case OPUS_BANDWIDTH_FULLBAND:
                endband = 21;
                break;
        }
//		celt_enc->end = endband;
//		celt_enc->stream_channels = OPUS_ENC_STREAMCHANNELS;
    }
//	celt_enc->bitrate = IMIN(OPUS_BITRATE_MAX, 260000*OPUS_ENC_CHANNELS);
    if (OPUS_ENC_MODE != MODE_SILK_ONLY)
    {
//		celt_enc->vbr = 0;
//		celt_enc->disable_pf = 0;
//		celt_enc->force_intra = 0;

        {
            if (OPUS_ENC_USE_VBR)
            {
//				celt_enc->vbr = 1;
//				celt_enc->constrained_vbr = st->vbr_constraint;
//				celt_enc->bitrate = IMIN(OPUS_ENC_BITRATEBPS, 260000*OPUS_ENC_CHANNELS);
            }
        }
    }

    ALLOC(tmp_prefill, OPUS_ENC_CHANNELS*OPUS_ENC_FS/400, opus_val16);
    if (OPUS_ENC_MODE != MODE_SILK_ONLY && OPUS_ENC_MODE != OPUS_ENC_MODE && OPUS_ENC_MODE > 0)
    {
       OPUS_COPY4(tmp_prefill, &st->delay_buffer[(OPUS_ENC_ENCODERBUFFER-OPUS_ENC_TOTALBUFFER-OPUS_ENC_FS/400)*OPUS_ENC_CHANNELS], OPUS_ENC_CHANNELS*OPUS_ENC_FS/400);
    }

    if (OPUS_ENC_CHANNELS*(OPUS_ENC_ENCODERBUFFER-(OPUS_ENC_FRAMESIZE+OPUS_ENC_TOTALBUFFER)) > 0)
    {
       OPUS_MOVE(st->delay_buffer, &st->delay_buffer[OPUS_ENC_CHANNELS*OPUS_ENC_FRAMESIZE], OPUS_ENC_CHANNELS*((opus_uint32)(OPUS_ENC_ENCODERBUFFER-OPUS_ENC_FRAMESIZE-OPUS_ENC_TOTALBUFFER)));
       OPUS_COPY4(&st->delay_buffer[OPUS_ENC_CHANNELS*(OPUS_ENC_ENCODERBUFFER-OPUS_ENC_FRAMESIZE-OPUS_ENC_TOTALBUFFER)],
             &pcm_buf[0],
             (OPUS_ENC_FRAMESIZE+OPUS_ENC_TOTALBUFFER)*OPUS_ENC_CHANNELS);
    } else {
       OPUS_COPY4(st->delay_buffer, &pcm_buf[(OPUS_ENC_FRAMESIZE+OPUS_ENC_TOTALBUFFER-OPUS_ENC_ENCODERBUFFER)*OPUS_ENC_CHANNELS], OPUS_ENC_ENCODERBUFFER*OPUS_ENC_CHANNELS);
    }
    /* gain_fade() and stereo_fade() need to be after the buffer copying
       because we don't want any of this to affect the SILK part */
    #if(OPUS_ENC_MODE != MODE_CELT_ONLY)       
    if( st->prev_HB_gain < Q15ONE || HB_gain < Q15ONE ) {
       gain_fade(pcm_buf, pcm_buf,
             st->prev_HB_gain, HB_gain, celt_mode->overlap, OPUS_ENC_FRAMESIZE, OPUS_ENC_CHANNELS, celt_mode->window, OPUS_ENC_FS);
    }
    st->prev_HB_gain = HB_gain;
	#endif
#if(OPUS_ENC_CHANNELS == 2)	
    if( !st->energy_masking && OPUS_ENC_CHANNELS == 2 ) {
        /* Apply stereo width reduction (at low bitrates) */
        if( st->hybrid_stereo_width_Q14 < (1 << 14) || st->silk_mode.stereoWidth_Q14 < (1 << 14) ) {
            opus_val16 g1, g2;
            g1 = st->hybrid_stereo_width_Q14;
            g2 = (opus_val16)(st->silk_mode.stereoWidth_Q14);
#ifdef FIXED_POINT
            g1 = g1==16384 ? Q15ONE : SHL16(g1,1);
            g2 = g2==16384 ? Q15ONE : SHL16(g2,1);
#else
            g1 *= (1.f/16384);
            g2 *= (1.f/16384);
#endif
            stereo_fade(pcm_buf, pcm_buf, g1, g2, celt_mode->overlap,
                  OPUS_ENC_FRAMESIZE, OPUS_ENC_CHANNELS, celt_mode->window, OPUS_ENC_FS);
            st->hybrid_stereo_width_Q14 = st->silk_mode.stereoWidth_Q14;
        }
    }
#endif

    {
	   redundancy = 0;
       nb_compr_bytes = (max_data_bytes-1)-redundancy_bytes;
       ec_enc_shrink(&enc, nb_compr_bytes);
    }

//	celt_enc->start = start_band;

    if (OPUS_ENC_MODE != MODE_SILK_ONLY)
    {
        /* If false, we already busted the budget and we'll end up with a "PLC frame" */
        if (ec_tell(&enc) <= 8*nb_compr_bytes)
        {
//		   celt_enc->vbr = OPUS_ENC_USE_VBR;
#if DEBUG_LOG
       for(unsigned short i=0;i<320;i++)
       {
    	   printf("%d",pcm_buf[i]);
       }
#endif
           ret = celt_encode_with_ec(celt_enc, pcm_buf, OPUS_ENC_FRAMESIZE, NULL, nb_compr_bytes, &enc);
           if (ret < 0)
           {
              RESTORE_STACK;
              return OPUS_INTERNAL_ERROR;
           }
           /* Put CELT->SILK redundancy data in the right place. */
           if (redundancy && celt_to_silk && OPUS_ENC_MODE==MODE_HYBRID && OPUS_ENC_USE_VBR)
           {
              OPUS_MOVE(data+ret, data+nb_compr_bytes, redundancy_bytes);
              nb_compr_bytes = nb_compr_bytes+redundancy_bytes;
           }
        }
    }

    /* Signalling the mode in the first byte */
    data--;
    data[0] = gen_toc(OPUS_ENC_MODE, OPUS_ENC_FS/OPUS_ENC_FRAMESIZE, curr_bandwidth, OPUS_ENC_STREAMCHANNELS);

    st->rangeFinal = enc.rng ^ redundant_rng;
#if(0)
    if (to_celt)
        st->mode = MODE_CELT_ONLY;
    else
		st->mode = OPUS_ENC_MODE;
#endif
//    st->prev_channels = OPUS_ENC_STREAMCHANNELS;
//    st->prev_framesize = OPUS_ENC_FRAMESIZE;

    st->first = 0;

    /* DTX decision */
#ifndef DISABLE_FLOAT_API
    if (OPUS_ENC_DTX && (analysis_info.valid || is_silence))
    {
       if (decide_dtx_mode(analysis_info.activity_probability, &st->nb_no_activity_frames,
             st->peak_signal_energy, pcm, OPUS_ENC_FRAMESIZE, OPUS_ENC_CHANNELS, is_silence, OPUS_ENC_ARCH))
       {
          st->rangeFinal = 0;
          data[0] = gen_toc(OPUS_ENC_MODE, OPUS_ENC_FS/OPUS_ENC_FRAMESIZE, curr_bandwidth, OPUS_ENC_STREAMCHANNELS);
          RESTORE_STACK;
          return 1;
       }
    } else {
       st->nb_no_activity_frames = 0;
    }
#endif

    /* In the unlikely case that the SILK encoder busted its target, tell
       the decoder to call the PLC */
    if (ec_tell(&enc) > (max_data_bytes-1)*8)
    {
       if (max_data_bytes < 2)
       {
          RESTORE_STACK;
          return OPUS_BUFFER_TOO_SMALL;
       }
       data[1] = 0;
       ret = 1;
       st->rangeFinal = 0;
    } else if (OPUS_ENC_MODE==MODE_SILK_ONLY&&!redundancy)
    {
       /*When in LPC only mode it's perfectly
         reasonable to strip off trailing zero bytes as
         the required range decoder behavior is to
         fill these in. This can't be done when the MDCT
         modes are used because the decoder needs to know
         the actual length for allocation purposes.*/
       while(ret>2&&data[ret]==0)ret--;
    }
    /* Count ToC and redundancy */
    ret += 1+redundancy_bytes;
    if (!OPUS_ENC_USE_VBR)
    {
       if (opus_packet_pad(data, ret, max_data_bytes) != OPUS_OK)
       {
          RESTORE_STACK;
          return OPUS_INTERNAL_ERROR;
       }
       ret = max_data_bytes;
    }
    RESTORE_STACK;
    return ret;
}

#ifdef FIXED_POINT

#ifndef DISABLE_FLOAT_API
opus_int32 opus_encode_float(OpusEncoder *st, const float *pcm, int analysis_frame_size,
      unsigned char *data, opus_int32 max_data_bytes)
{
   int i, ret;
   int frame_size;
   VARDECL(opus_int16, in);
   ALLOC_STACK;

   frame_size = frame_size_select(analysis_frame_size, OPUS_ENC_VARIABLEDURATION, OPUS_ENC_FS);
   if (frame_size <= 0)
   {
      RESTORE_STACK;
      return OPUS_BAD_ARG;
   }
   ALLOC(in, frame_size*OPUS_ENC_CHANNELS, opus_int16);

   for (i=0;i<frame_size*OPUS_ENC_CHANNELS;i++)
      in[i] = FLOAT2INT16(pcm[i]);
   ret = opus_encode_native(st, in, frame_size, data, max_data_bytes, 16,
                            pcm, analysis_frame_size, 0, -2, OPUS_ENC_CHANNELS, downmix_float, 1);
   RESTORE_STACK;
   return ret;
}
#endif

opus_int32 opus_encode(OpusEncoder *st, const opus_int16 *pcm, int analysis_frame_size,
                unsigned char *data, opus_int32 out_data_bytes)
{
//   int frame_size;
//   frame_size = frame_size_select(analysis_frame_size, OPUS_ENC_VARIABLEDURATION, OPUS_ENC_FS);
   return opus_encode_native(st, pcm, OPUS_ENC_FRAMESIZE, data, out_data_bytes, 16,
                             pcm, OPUS_ENC_FRAMESIZE, 0, -2, OPUS_ENC_CHANNELS, 0, 0);
}
#else
opus_int32 opus_encode(OpusEncoder *st, const opus_int16 *pcm, int analysis_frame_size,
      unsigned char *data, opus_int32 max_data_bytes)
{
   int i, ret;
   int frame_size;
   VARDECL(float, in);
   ALLOC_STACK;

   frame_size = frame_size_select(analysis_frame_size, OPUS_ENC_VARIABLEDURATION, OPUS_ENC_FS);
   if (frame_size <= 0)
   {
      RESTORE_STACK;
      return OPUS_BAD_ARG;
   }
   ALLOC(in, frame_size*OPUS_ENC_CHANNELS, float);

   for (i=0;i<frame_size*OPUS_ENC_CHANNELS;i++)
      in[i] = (1.0f/32768)*pcm[i];
   ret = opus_encode_native(st, in, frame_size, data, max_data_bytes, 16,
                            pcm, analysis_frame_size, 0, -2, OPUS_ENC_CHANNELS, downmix_int, 0);
   RESTORE_STACK;
   return ret;
}
opus_int32 opus_encode_float(OpusEncoder *st, const float *pcm, int analysis_frame_size,
                      unsigned char *data, opus_int32 out_data_bytes)
{
   int frame_size;
   frame_size = frame_size_select(analysis_frame_size, OPUS_ENC_VARIABLEDURATION, OPUS_ENC_FS);
   return opus_encode_native(st, pcm, frame_size, data, out_data_bytes, 24,
                             pcm, analysis_frame_size, 0, -2, OPUS_ENC_CHANNELS, downmix_float, 1);
}
#endif


void opus_encoder_get_final_rng(OpusEncoder *st, opus_uint32 *rng){
	*rng = st->rangeFinal;
}


