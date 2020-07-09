#ifndef FFT_TELINK_H
#define FFT_TELINK_H
#include "typedef.h"
#include "telink_fft_types.h"
void BASOP_cfft(Word32* re, Word32* im, Word16 sizeOfFft, Word16 s, Word16* scale, Word32* x);

#endif // !FFT_TELINK_H

#pragma once
