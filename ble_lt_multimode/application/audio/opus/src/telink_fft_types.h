#ifndef TELINK_FFT_TYPES_H
#define TELINK_FFT_TYPES_H
#include "opus_types.h"
#define Word64 opus_int64
#define Word32 opus_int32
#define Word16 opus_int16
#define UWord32 opus_uint32
#define UWord16 opus_uint16
#ifndef MAX_32
#define MAX_32 (0x7fffffffL)
#endif // !MAX_32

#ifndef MIN_32
#define MIN_32 (0x80000000L)
#endif // !MIN_32

#ifndef MAX_16
#define MAX_16     (32767)
#endif

#ifndef MIN_16
#define MIN_16     (-32768)
#endif
#endif // !TELINK_FFT_TYPES_H

#pragma once
