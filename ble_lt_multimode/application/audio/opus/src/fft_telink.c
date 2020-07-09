#include "fft_telink.h"
#include "intrinsic.h"
#ifndef ANDES_DSP_LIB_FFT
#define L_shr_pos L_shr
#define L_shl_pos L_shl
#define WORD32_BITS 32
#define WORD16_BITS 16

#define WORD322WORD16(val)                                                                                             \
    ((((((val) >> (WORD32_BITS - WORD16_BITS - 1)) + 1) > (((long)1 << WORD16_BITS) - 1)) && ((long)(val) > 0))      \
         ? (Word16)(short)(((long)1 << (WORD16_BITS - 1)) - 1)                                                       \
         : (Word16)(short)((((val) >> (WORD32_BITS - WORD16_BITS - 1)) + 1) >> 1))

#define FFTC(x) WORD322WORD16((Word32)x)

#define C31 (FFTC(0x91261468)) /* FL2WORD32( -0.86602540) -sqrt(3)/2 */
#define C31_32 0x91261468
#define C51 (FFTC(0x79bc3854)) /* FL2WORD32( 0.95105652)   */
#define C52 (FFTC(0x9d839db0)) /* FL2WORD32(-1.53884180/2) */
#define C53 (FFTC(0xd18053ce)) /* FL2WORD32(-0.36327126)   */
#define C54 (FFTC(0x478dde64)) /* FL2WORD32( 0.55901699)   */
#define C55 (FFTC(0xb0000001)) /* FL2WORD32(-1.25/2)       */
#define C81 (FFTC(0x5a82799a)) /* FL2WORD32( 7.071067811865475e-1) */
#define C82 (FFTC(0xa57d8666)) /* FL2WORD32(-7.071067811865475e-1) */
#define C51_32 (0x79bc3854) /* FL2WORD32( 0.95105652)   */
#define C52_32 (0x9d839db0) /* FL2WORD32(-1.53884180/2) */
#define C53_32 (0xd18053ce) /* FL2WORD32(-0.36327126)   */
#define C54_32 (0x478dde64) /* FL2WORD32( 0.55901699)   */
#define C55_32 (0xb0000001) /* FL2WORD32(-1.25/2)       */
#define C81_32 (0x5a82799a) /* FL2WORD32( 7.071067811865475e-1) */
#define C82_32 (0xa57d8666) /* FL2WORD32(-7.071067811865475e-1) */

#define SCALEFACTOR15 (4)
#define SCALEFACTOR32_1 (4)
#define SCALEFACTOR32_2 (1)
#define SCALEFACTOR480 (9)
const opus_int16 RotVector_480[896] = { 32767,0,32765,-429,32757,-858,32743,-1286,32723,-1715,32698,-2143,32667,-2571,32631,-2998,32588,-3425,32541,-3851,32488,-4277,32429,-4702,32365,-5126,32295,-5549,32219,-5971,32138,-6393,32052,-6813,31960,-7232,31863,-7650,31760,-8066,31651,-8481,31538,-8895,31419,-9307,31294,-9717,31164,-10126,31029,-10533,30888,-10938,30743,-11342,30592,-11743,30435,-12142,30274,-12540,30107,-12935,32767,0,32757,-858,32723,-1715,32667,-2571,32588,-3425,32488,-4277,32365,-5126,32219,-5971,32052,-6813,31863,-7650,31651,-8481,31419,-9307,31164,-10126,30888,-10938,30592,-11743,30274,-12540,29935,-13328,29576,-14107,29197,-14876,28797,-15636,28378,-16384,27939,-17121,27482,-17847,27005,-18560,26510,-19261,25997,-19948,25466,-20622,24917,-21281,24351,-21926,23769,-22556,23170,-23170,22556,-23769,32767,0,32743,-1286,32667,-2571,32541,-3851,32365,-5126,32138,-6393,31863,-7650,31538,-8895,31164,-10126,30743,-11342,30274,-12540,29758,-13719,29197,-14876,28590,-16011,27939,-17121,27246,-18205,26510,-19261,25733,-20286,24917,-21281,24062,-22243,23170,-23170,22243,-24062,21281,-24917,20286,-25733,19261,-26510,18205,-27246,17121,-27939,16011,-28590,14876,-29197,13719,-29758,12540,-30274,11342,-30743,32767,0,32723,-1715,32588,-3425,32365,-5126,32052,-6813,31651,-8481,31164,-10126,30592,-11743,29935,-13328,29197,-14876,28378,-16384,27482,-17847,26510,-19261,25466,-20622,24351,-21926,23170,-23170,21926,-24351,20622,-25466,19261,-26510,17847,-27482,16384,-28378,14876,-29197,13328,-29935,11743,-30592,10126,-31164,8481,-31651,6813,-32052,5126,-32365,3425,-32588,1715,-32723,0,-32768,-1715,-32723,32767,0,32698,-2143,32488,-4277,32138,-6393,31651,-8481,31029,-10533,30274,-12540,29389,-14493,28378,-16384,27246,-18205,25997,-19948,24636,-21605,23170,-23170,21605,-24636,19948,-25997,18205,-27246,16384,-28378,14493,-29389,12540,-30274,10533,-31029,8481,-31651,6393,-32138,4277,-32488,2143,-32698,0,-32768,-2143,-32698,-4277,-32488,-6393,-32138,-8481,-31651,-10533,-31029,-12540,-30274,-14493,-29389,32767,0,32667,-2571,32365,-5126,31863,-7650,31164,-10126,30274,-12540,29197,-14876,27939,-17121,26510,-19261,24917,-21281,23170,-23170,21281,-24917,19261,-26510,17121,-27939,14876,-29197,12540,-30274,10126,-31164,7650,-31863,5126,-32365,2571,-32667,0,-32768,-2571,-32667,-5126,-32365,-7650,-31863,-10126,-31164,-12540,-30274,-14876,-29197,-17121,-27939,-19261,-26510,-21281,-24917,-23170,-23170,-24917,-21281,32767,0,32631,-2998,32219,-5971,31538,-8895,30592,-11743,29389,-14493,27939,-17121,26255,-19606,24351,-21926,22243,-24062,19948,-25997,17485,-27713,14876,-29197,12142,-30435,9307,-31419,6393,-32138,3425,-32588,429,-32765,-2571,-32667,-5549,-32295,-8481,-31651,-11342,-30743,-14107,-29576,-16754,-28161,-19261,-26510,-21605,-24636,-23769,-22556,-25733,-20286,-27482,-17847,-28999,-15257,-30274,-12540,-31294,-9717,32767,0,32588,-3425,32052,-6813,31164,-10126,29935,-13328,28378,-16384,26510,-19261,24351,-21926,21926,-24351,19261,-26510,16384,-28378,13328,-29935,10126,-31164,6813,-32052,3425,-32588,0,-32768,-3425,-32588,-6813,-32052,-10126,-31164,-13328,-29935,-16384,-28378,-19261,-26510,-21926,-24351,-24351,-21926,-26510,-19261,-28378,-16384,-29935,-13328,-31164,-10126,-32052,-6813,-32588,-3425,-32768,0,-32588,3425,32767,0,32541,-3851,31863,-7650,30743,-11342,29197,-14876,27246,-18205,24917,-21281,22243,-24062,19261,-26510,16011,-28590,12540,-30274,8895,-31538,5126,-32365,1286,-32743,-2571,-32667,-6393,-32138,-10126,-31164,-13719,-29758,-17121,-27939,-20286,-25733,-23170,-23170,-25733,-20286,-27939,-17121,-29758,-13719,-31164,-10126,-32138,-6393,-32667,-2571,-32743,1286,-32365,5126,-31538,8895,-30274,12540,-28590,16011,32767,0,32488,-4277,31651,-8481,30274,-12540,28378,-16384,25997,-19948,23170,-23170,19948,-25997,16384,-28378,12540,-30274,8481,-31651,4277,-32488,0,-32768,-4277,-32488,-8481,-31651,-12540,-30274,-16384,-28378,-19948,-25997,-23170,-23170,-25997,-19948,-28378,-16384,-30274,-12540,-31651,-8481,-32488,-4277,-32768,0,-32488,4277,-31651,8481,-30274,12540,-28378,16384,-25997,19948,-23170,23170,-19948,25997,32767,0,32429,-4702,31419,-9307,29758,-13719,27482,-17847,24636,-21605,21281,-24917,17485,-27713,13328,-29935,8895,-31538,4277,-32488,-429,-32765,-5126,-32365,-9717,-31294,-14107,-29576,-18205,-27246,-21926,-24351,-25193,-20953,-27939,-17121,-30107,-12935,-31651,-8481,-32541,-3851,-32757,858,-32295,5549,-31164,10126,-29389,14493,-27005,18560,-24062,22243,-20622,25466,-16754,28161,-12540,30274,-8066,31760,32767,0,32365,-5126,31164,-10126,29197,-14876,26510,-19261,23170,-23170,19261,-26510,14876,-29197,10126,-31164,5126,-32365,0,-32768,-5126,-32365,-10126,-31164,-14876,-29197,-19261,-26510,-23170,-23170,-26510,-19261,-29197,-14876,-31164,-10126,-32365,-5126,-32768,0,-32365,5126,-31164,10126,-29197,14876,-26510,19261,-23170,23170,-19261,26510,-14876,29197,-10126,31164,-5126,32365,0,32767,5126,32365,32767,0,32295,-5549,30888,-10938,28590,-16011,25466,-20622,21605,-24636,17121,-27939,12142,-30435,6813,-32052,1286,-32743,-4277,-32488,-9717,-31294,-14876,-29197,-19606,-26255,-23769,-22556,-27246,-18205,-29935,-13328,-31760,-8066,-32667,-2571,-32631,2998,-31651,8481,-29758,13719,-27005,18560,-23472,22865,-19261,26510,-14493,29389,-9307,31419,-3851,32541,1715,32723,7232,31960,12540,30274,17485,27713,32767,0,32219,-5971,30592,-11743,27939,-17121,24351,-21926,19948,-25997,14876,-29197,9307,-31419,3425,-32588,-2571,-32667,-8481,-31651,-14107,-29576,-19261,-26510,-23769,-22556,-27482,-17847,-30274,-12540,-32052,-6813,-32757,-858,-32365,5126,-30888,10938,-28378,16384,-24917,21281,-20622,25466,-15636,28797,-10126,31164,-4277,32488,1715,32723,7650,31863,13328,29935,18560,27005,23170,23170,27005,18560, };
const opus_int32 RotVector_32_32[2 * 20] = {
    /**
     * \brief    Twiddle factors are unscaled
     */
      0x7d8a5f00, 0xe70747c0, 0x7641b000, 0xcf043ac0, 0x6a6d9900, 0xb8e31300, 0x5a827980, 0xa57d8680,
      0x471ced80, 0x95926780, 0x30fbc600, 0x89be5100, 0x18f8b800, 0x8275a100, 0x7641b000, 0xcf043ac0,
      0x5a827980, 0xa57d8680, 0x30fbc600, 0x89be5100, 0xcf043b00, 0x89be5100, 0xa57d8680, 0xa57d8680,
      0x89be5100, 0xcf043ac0, 0x6a6d9900, 0xb8e31300, 0x30fbc600, 0x89be5100, 0xe7074800, 0x8275a100,
      0xa57d8680, 0xa57d8680, 0x8275a100, 0xe70747c0, 0x89be5000, 0x30fbc540, 0xb8e31280, 0x6a6d9880
};

#define Mpy_32_xx mult_32_Q15_32

void Mpy_32_16_ss(Word32 L_var1, Word16 var2, Word32* L_varout_h, UWord16* varout_l) {
    Word16 var1_h;
    UWord16 uvar1_l;
    Word64 L64_var1;

    if ((L_var1 == (Word32)0x80000000)
        && (var2 == (Word16)0x8000)) {
        *L_varout_h = 0x7fffffff;
        *varout_l = (UWord16)0xffff;

    }
    else {
        uvar1_l = (Word16)(L_var1);
        var1_h = (L_var1) >> 16;
        L64_var1 = ((Word64)((Word32)var2 * (Word32)uvar1_l)) << 1;

        *varout_l = (Word16)(L64_var1);

        L64_var1 = (L64_var1 >> 16);
        L64_var1 = L64_var1 + (((Word64)var2 * (Word64)var1_h) << 1);

        *L_varout_h = (Word32)(L64_var1);
    }
    return;
}

Word32 Mpy_32_16(Word32 x, Word16 y)
{
    Word32  mh;
    UWord16 ml;

    Mpy_32_16_ss(x, y, &mh, &ml);

    return (mh);
}
void cplxMpy_32_16(Word32* c_Re, Word32* c_Im, const Word32 a_Re, const Word32 a_Im, const Word16 b_Re,
    const Word16 b_Im)
{
    *c_Re = L_sub(Mpy_32_16(a_Re, b_Re), Mpy_32_16(a_Im, b_Im));
    *c_Im = L_add(Mpy_32_16(a_Re, b_Im), Mpy_32_16(a_Im, b_Re));
}

void Mpy_32_32_ss(Word32 L_var1, Word32 L_var2, Word32* L_varout_h, UWord32* L_varout_l) {
    UWord16 uvar1_l, uvar2_l;
    Word16   var1_h, var2_h;
    Word64 L64_var1;

    if ((L_var1 == (Word32)0x80000000)
        && (L_var2 == (Word32)0x80000000))
    {
        *L_varout_h = 0x7fffffff;
        *L_varout_l = (UWord32)0xffffffff;
    }
    else {

        uvar1_l = (Word16)(L_var1);
        var1_h = (L_var1) >> 16;
        uvar2_l = (Word16)(L_var2);
        var2_h = (L_var2) >> 16;

        /* Below line can not overflow, so we can use << instead of L40_shl.  */
        L64_var1 = ((Word64)((UWord32)uvar2_l * (UWord32)uvar1_l)) << 1;

        *L_varout_l = 0x0000ffff & (Word32)(L64_var1);

        L64_var1 = (L64_var1 >> 16);
        L64_var1 = (L64_var1 + (((Word64)((Word32)var2_h * (Word32)uvar1_l)) << 1));
        L64_var1 = (L64_var1 + (((Word64)((Word32)var1_h * (Word32)uvar2_l)) << 1));
        *L_varout_l |= ((Word32)(L64_var1)) << 16;

        L64_var1 = (L64_var1 >> 16);
        L64_var1 = L64_var1 + (((Word64)var1_h * (Word64)var2_h) << 1);

        *L_varout_h = (Word32)(L64_var1);
    }

}

Word32 Mpy_32_32(Word32 x, Word32 y)
{
    Word32  mh;
    UWord32 ml;

    Mpy_32_32_ss(x, y, &mh, &ml);

    return (mh);
}

#define cplxMpy4_12_0(re, im, a, b, c, d)                                                                              \
    do                                                                                                                 \
    {                                                                                                                  \
        re = L_sub(Mpy_32_xx(a, c), Mpy_32_xx(b, d));                                                                  \
        im = L_add(Mpy_32_xx(a, d), Mpy_32_xx(b, c));                                                                  \
    } while (0)

#define cplxMpy4_12_1(re, im, a, b)                                                                                    \
    do                                                                                                                 \
    {                                                                                                                  \
        re = a;                                                                                                        \
        im = b;                                                                                                        \
    } while (0)

#define cplxMpy3_0(a, b, c, d)                                                                                         \
    do                                                                                                                 \
    {                                                                                                                  \
        as = L_shr_pos(a, 1);                                                                                          \
        bs = L_shr_pos(b, 1);                                                                                          \
        a  = L_sub(Mpy_32_32(as, c), Mpy_32_32(bs, d));                                                                \
        b  = L_add(Mpy_32_32(as, d), Mpy_32_32(bs, c));                                                                \
    } while (0)

#define Mpy3_0(s12, s13, s14, s15, t0, t1, t2, t3)                                                                     \
    do                                                                                                                 \
    {                                                                                                                  \
        s12 = Mpy_32_32(L_add(t0, t2), C81_32);                                                                        \
        s14 = Mpy_32_32(L_sub(t0, t2), C81_32);                                                                        \
        s13 = Mpy_32_32(L_sub(t3, t1), C81_32);                                                                        \
        s15 = Mpy_32_32(L_add(t1, t3), C82_32);                                                                        \
    } while (0)

static void fft15(Word32* re, Word32* im, Word16 s)
{
    Word32 t;
    Word32 r1, r2, r3, r4;
    Word32 s1, s2, s3, s4;
    Word32 x00, x01, x02, x03, x04, x05, x06, x07, x08, x09;
    Word32 x10, x11, x12, x13, x14, x15, x16, x17, x18, x19;
    Word32 x20, x21, x22, x23, x24, x25, x26, x27, x28, x29;
    Word32 y00, y01, y02, y03, y04, y05, y06, y07, y08, y09;
    Word32 y10, y11, y12, y13, y14, y15, y16, y17, y18, y19;
    Word32 y20, y21, y22, y23, y24, y25, y26, y27, y28, y29;


    x00 = L_shr_pos(re[s * 0], SCALEFACTOR15);
    x01 = L_shr_pos(im[s * 0], SCALEFACTOR15);
    x02 = L_shr_pos(re[s * 3], SCALEFACTOR15);
    x03 = L_shr_pos(im[s * 3], SCALEFACTOR15);
    x04 = L_shr_pos(re[s * 6], SCALEFACTOR15);
    x05 = L_shr_pos(im[s * 6], SCALEFACTOR15);
    x06 = L_shr_pos(re[s * 9], SCALEFACTOR15);
    x07 = L_shr_pos(im[s * 9], SCALEFACTOR15);
    x08 = L_shr_pos(re[s * 12], SCALEFACTOR15);
    x09 = L_shr_pos(im[s * 12], SCALEFACTOR15);

    x10 = L_shr_pos(re[s * 5], SCALEFACTOR15);
    x11 = L_shr_pos(im[s * 5], SCALEFACTOR15);
    x12 = L_shr_pos(re[s * 8], SCALEFACTOR15);
    x13 = L_shr_pos(im[s * 8], SCALEFACTOR15);
    x14 = L_shr_pos(re[s * 11], SCALEFACTOR15);
    x15 = L_shr_pos(im[s * 11], SCALEFACTOR15);
    x16 = L_shr_pos(re[s * 14], SCALEFACTOR15);
    x17 = L_shr_pos(im[s * 14], SCALEFACTOR15);
    x18 = L_shr_pos(re[s * 2], SCALEFACTOR15);
    x19 = L_shr_pos(im[s * 2], SCALEFACTOR15);

    x20 = L_shr_pos(re[s * 10], SCALEFACTOR15);
    x21 = L_shr_pos(im[s * 10], SCALEFACTOR15);
    x22 = L_shr_pos(re[s * 13], SCALEFACTOR15);
    x23 = L_shr_pos(im[s * 13], SCALEFACTOR15);
    x24 = L_shr_pos(re[s * 1], SCALEFACTOR15);
    x25 = L_shr_pos(im[s * 1], SCALEFACTOR15);
    x26 = L_shr_pos(re[s * 4], SCALEFACTOR15);
    x27 = L_shr_pos(im[s * 4], SCALEFACTOR15);
    x28 = L_shr_pos(re[s * 7], SCALEFACTOR15);
    x29 = L_shr_pos(im[s * 7], SCALEFACTOR15);

    /* 1. FFT5 stage */

    /* real part */
    r1 = L_add(x02, x08);
    r4 = L_sub(x02, x08);
    r3 = L_add(x04, x06);
    r2 = L_sub(x04, x06);
    t = Mpy_32_xx(L_sub(r1, r3), C54);
    r1 = L_add(r1, r3);
    y00 = L_add(x00, r1);
    r1 = L_add(y00, (L_shl_pos(Mpy_32_xx(r1, C55), 1)));
    r3 = L_sub(r1, t);
    r1 = L_add(r1, t);
    t = Mpy_32_xx((L_add(r4, r2)), C51);
    r4 = L_add(t, L_shl_pos(Mpy_32_xx(r4, C52), 1));
    r2 = L_add(t, Mpy_32_xx(r2, C53));

    /* imaginary part */
    s1 = L_add(x03, x09);
    s4 = L_sub(x03, x09);
    s3 = L_add(x05, x07);
    s2 = L_sub(x05, x07);
    t = Mpy_32_xx(L_sub(s1, s3), C54);
    s1 = L_add(s1, s3);
    y01 = L_add(x01, s1);
    s1 = L_add(y01, L_shl_pos(Mpy_32_xx(s1, C55), 1));
    s3 = L_sub(s1, t);
    s1 = L_add(s1, t);
    t = Mpy_32_xx(L_add(s4, s2), C51);
    s4 = L_add(t, L_shl_pos(Mpy_32_xx(s4, C52), 1));
    s2 = L_add(t, Mpy_32_xx(s2, C53));

    /* combination */
    y02 = L_add(r1, s2);
    y08 = L_sub(r1, s2);
    y04 = L_sub(r3, s4);
    y06 = L_add(r3, s4);

    y03 = L_sub(s1, r2);
    y09 = L_add(s1, r2);
    y05 = L_add(s3, r4);
    y07 = L_sub(s3, r4);

    /* 2. FFT5 stage */

    /* real part */
    r1 = L_add(x12, x18);
    r4 = L_sub(x12, x18);
    r3 = L_add(x14, x16);
    r2 = L_sub(x14, x16);
    t = Mpy_32_xx(L_sub(r1, r3), C54);
    r1 = L_add(r1, r3);
    y10 = L_add(x10, r1);
    r1 = L_add(y10, (L_shl_pos(Mpy_32_xx(r1, C55), 1)));
    r3 = L_sub(r1, t);
    r1 = L_add(r1, t);
    t = Mpy_32_xx((L_add(r4, r2)), C51);
    r4 = L_add(t, L_shl_pos(Mpy_32_xx(r4, C52), 1));
    r2 = L_add(t, Mpy_32_xx(r2, C53));

    /* imaginary part */
    s1 = L_add(x13, x19);
    s4 = L_sub(x13, x19);
    s3 = L_add(x15, x17);
    s2 = L_sub(x15, x17);
    t = Mpy_32_xx(L_sub(s1, s3), C54);
    s1 = L_add(s1, s3);
    y11 = L_add(x11, s1);
    s1 = L_add(y11, L_shl_pos(Mpy_32_xx(s1, C55), 1));
    s3 = L_sub(s1, t);
    s1 = L_add(s1, t);
    t = Mpy_32_xx(L_add(s4, s2), C51);
    s4 = L_add(t, L_shl_pos(Mpy_32_xx(s4, C52), 1));
    s2 = L_add(t, Mpy_32_xx(s2, C53));

    /* combination */
    y12 = L_add(r1, s2);
    y18 = L_sub(r1, s2);
    y14 = L_sub(r3, s4);
    y16 = L_add(r3, s4);

    y13 = L_sub(s1, r2);
    y19 = L_add(s1, r2);
    y15 = L_add(s3, r4);
    y17 = L_sub(s3, r4);

    /* 3. FFT5 stage */

    /* real part */
    r1 = L_add(x22, x28);
    r4 = L_sub(x22, x28);
    r3 = L_add(x24, x26);
    r2 = L_sub(x24, x26);
    t = Mpy_32_xx(L_sub(r1, r3), C54);
    r1 = L_add(r1, r3);
    y20 = L_add(x20, r1);
    r1 = L_add(y20, (L_shl_pos(Mpy_32_xx(r1, C55), 1)));
    r3 = L_sub(r1, t);
    r1 = L_add(r1, t);
    t = Mpy_32_xx((L_add(r4, r2)), C51);
    r4 = L_add(t, L_shl_pos(Mpy_32_xx(r4, C52), 1));
    r2 = L_add(t, Mpy_32_xx(r2, C53));

    /* imaginary part */
    s1 = L_add(x23, x29);
    s4 = L_sub(x23, x29);
    s3 = L_add(x25, x27);
    s2 = L_sub(x25, x27);
    t = Mpy_32_xx(L_sub(s1, s3), C54);
    s1 = L_add(s1, s3);
    y21 = L_add(x21, s1);
    s1 = L_add(y21, L_shl_pos(Mpy_32_xx(s1, C55), 1));
    s3 = L_sub(s1, t);
    s1 = L_add(s1, t);
    t = Mpy_32_xx(L_add(s4, s2), C51);
    s4 = L_add(t, L_shl_pos(Mpy_32_xx(s4, C52), 1));
    s2 = L_add(t, Mpy_32_xx(s2, C53));

    /* combination */
    y22 = L_add(r1, s2);
    y28 = L_sub(r1, s2);
    y24 = L_sub(r3, s4);
    y26 = L_add(r3, s4);

    y23 = L_sub(s1, r2);
    y29 = L_add(s1, r2);
    y25 = L_add(s3, r4);
    y27 = L_sub(s3, r4);

    /* 1. FFT3 stage */

    /* real part */
    r1 = L_add(y10, y20);
    r2 = Mpy_32_xx(L_sub(y10, y20), C31);
    re[s * 0] = L_add(y00, r1);
    r1 = L_sub(y00, L_shr_pos(r1, 1));

    /* imaginary part */
    s1 = L_add(y11, y21);
    s2 = Mpy_32_xx(L_sub(y11, y21), C31);
    im[s * 0] = L_add(y01, s1);
    s1 = L_sub(y01, L_shr_pos(s1, 1));

    /* combination */
    re[s * 10] = L_sub(r1, s2);
    re[s * 5] = L_add(r1, s2);
    im[s * 10] = L_add(s1, r2);
    im[s * 5] = L_sub(s1, r2);

    /* 2. FFT3 stage */

    /* real part */
    r1 = L_add(y12, y22);
    r2 = Mpy_32_xx(L_sub(y12, y22), C31);
    re[s * 6] = L_add(y02, r1);
    r1 = L_sub(y02, L_shr_pos(r1, 1));

    /* imaginary part */
    s1 = L_add(y13, y23);
    s2 = Mpy_32_xx(L_sub(y13, y23), C31);
    im[s * 6] = L_add(y03, s1);
    s1 = L_sub(y03, L_shr_pos(s1, 1));

    /* combination */
    re[s * 1] = L_sub(r1, s2);
    re[s * 11] = L_add(r1, s2);
    im[s * 1] = L_add(s1, r2);
    im[s * 11] = L_sub(s1, r2);

    /* 3. FFT3 stage */

    /* real part */
    r1 = L_add(y14, y24);
    r2 = Mpy_32_xx(L_sub(y14, y24), C31);
    re[s * 12] = L_add(y04, r1);
    r1 = L_sub(y04, L_shr_pos(r1, 1));

    /* imaginary part */
    s1 = L_add(y15, y25);
    s2 = Mpy_32_xx(L_sub(y15, y25), C31);
    im[s * 12] = L_add(y05, s1);
    s1 = L_sub(y05, L_shr_pos(s1, 1));

    /* combination */
    re[s * 7] = L_sub(r1, s2);
    re[s * 2] = L_add(r1, s2);
    im[s * 7] = L_add(s1, r2);
    im[s * 2] = L_sub(s1, r2);

    /* 4. FFT3 stage */

    /* real part */
    r1 = L_add(y16, y26);
    r2 = Mpy_32_xx(L_sub(y16, y26), C31);
    re[s * 3] = L_add(y06, r1);
    r1 = L_sub(y06, L_shr_pos(r1, 1));

    /* imaginary part */
    s1 = L_add(y17, y27);
    s2 = Mpy_32_xx(L_sub(y17, y27), C31);
    im[s * 3] = L_add(y07, s1);
    s1 = L_sub(y07, L_shr_pos(s1, 1));

    /* combination */
    re[s * 13] = L_sub(r1, s2);
    re[s * 8] = L_add(r1, s2);
    im[s * 13] = L_add(s1, r2);
    im[s * 8] = L_sub(s1, r2);

    /* 5. FFT3 stage */

    /* real part */
    r1 = L_add(y18, y28);
    r2 = Mpy_32_xx(L_sub(y18, y28), C31);
    re[s * 9] = L_add(y08, r1);
    r1 = L_sub(y08, L_shr_pos(r1, 1));

    /* imaginary part */
    s1 = L_add(y19, y29);
    s2 = Mpy_32_xx(L_sub(y19, y29), C31);
    im[s * 9] = L_add(y09, s1);
    s1 = L_sub(y09, L_shr_pos(s1, 1));

    /* combination */
    re[s * 4] = L_sub(r1, s2);
    re[s * 14] = L_add(r1, s2);
    im[s * 4] = L_add(s1, r2);
    im[s * 14] = L_sub(s1, r2);

}

static void fft32(Word32* re, Word32* im, Word16 s)
{
    Word32 as, bs;
    Word32 x00, x01, x02, x03, x04, x05, x06, x07;
    Word32 x08, x09, x10, x11, x12, x13, x14, x15;
    Word32 t00, t01, t02, t03, t04, t05, t06, t07;
    Word32 t08, t09, t10, t11, t12, t13, t14, t15;
    Word32 s00, s01, s02, s03, s04, s05, s06, s07;
    Word32 s08, s09, s10, s11, s12, s13, s14, s15;

    Word32 y00, y01, y02, y03, y04, y05, y06, y07;
    Word32 y08, y09, y10, y11, y12, y13, y14, y15;
    Word32 y16, y17, y18, y19, y20, y21, y22, y23;
    Word32 y24, y25, y26, y27, y28, y29, y30, y31;
    Word32 y32, y33, y34, y35, y36, y37, y38, y39;
    Word32 y40, y41, y42, y43, y44, y45, y46, y47;
    Word32 y48, y49, y50, y51, y52, y53, y54, y55;
    Word32 y56, y57, y58, y59, y60, y61, y62, y63;

    /* 1. FFT8 stage */
    x00 = L_shr_pos(re[s * 0], SCALEFACTOR32_1);
    x01 = L_shr_pos(im[s * 0], SCALEFACTOR32_1);
    x02 = L_shr_pos(re[s * 4], SCALEFACTOR32_1);
    x03 = L_shr_pos(im[s * 4], SCALEFACTOR32_1);
    x04 = L_shr_pos(re[s * 8], SCALEFACTOR32_1);
    x05 = L_shr_pos(im[s * 8], SCALEFACTOR32_1);
    x06 = L_shr_pos(re[s * 12], SCALEFACTOR32_1);
    x07 = L_shr_pos(im[s * 12], SCALEFACTOR32_1);
    x08 = L_shr_pos(re[s * 16], SCALEFACTOR32_1);
    x09 = L_shr_pos(im[s * 16], SCALEFACTOR32_1);
    x10 = L_shr_pos(re[s * 20], SCALEFACTOR32_1);
    x11 = L_shr_pos(im[s * 20], SCALEFACTOR32_1);
    x12 = L_shr_pos(re[s * 24], SCALEFACTOR32_1);
    x13 = L_shr_pos(im[s * 24], SCALEFACTOR32_1);
    x14 = L_shr_pos(re[s * 28], SCALEFACTOR32_1);
    x15 = L_shr_pos(im[s * 28], SCALEFACTOR32_1);

    t00 = L_add(x00, x08);
    t02 = L_sub(x00, x08);
    t01 = L_add(x01, x09);
    t03 = L_sub(x01, x09);
    t04 = L_add(x02, x10);
    t06 = L_sub(x02, x10);
    t05 = L_add(x03, x11);
    t07 = L_sub(x03, x11);
    t08 = L_add(x04, x12);
    t10 = L_sub(x04, x12);
    t09 = L_add(x05, x13);
    t11 = L_sub(x05, x13);
    t12 = L_add(x06, x14);
    t14 = L_sub(x06, x14);
    t13 = L_add(x07, x15);
    t15 = L_sub(x07, x15);

    /* Pre-additions and core multiplications */
    s00 = L_add(t00, t08);
    s04 = L_sub(t00, t08);
    s01 = L_add(t01, t09);
    s05 = L_sub(t01, t09);
    s08 = L_sub(t02, t11);
    s10 = L_add(t02, t11);
    s09 = L_add(t03, t10);
    s11 = L_sub(t03, t10);
    s02 = L_add(t04, t12);
    s07 = L_sub(t04, t12);
    s03 = L_add(t05, t13);
    s06 = L_sub(t13, t05);
    t01 = L_add(t06, t14);
    t02 = L_sub(t06, t14);
    t00 = L_add(t07, t15);
    t03 = L_sub(t07, t15);

    Mpy3_0(s12, s13, s14, s15, t00, t01, t02, t03);

    /* Post-additions */
    y00 = L_add(s00, s02);
    y08 = L_sub(s00, s02);
    y01 = L_add(s01, s03);
    y09 = L_sub(s01, s03);
    y04 = L_sub(s04, s06);
    y12 = L_add(s04, s06);
    y05 = L_sub(s05, s07);
    y13 = L_add(s05, s07);
    y06 = L_add(s08, s14);
    y14 = L_sub(s08, s14);
    y07 = L_add(s09, s15);
    y15 = L_sub(s09, s15);
    y02 = L_add(s10, s12);
    y10 = L_sub(s10, s12);
    y03 = L_add(s11, s13);
    y11 = L_sub(s11, s13);

    /* 2. FFT8 stage */
    x00 = L_shr_pos(re[s * 1], SCALEFACTOR32_1);
    x01 = L_shr_pos(im[s * 1], SCALEFACTOR32_1);
    x02 = L_shr_pos(re[s * 5], SCALEFACTOR32_1);
    x03 = L_shr_pos(im[s * 5], SCALEFACTOR32_1);
    x04 = L_shr_pos(re[s * 9], SCALEFACTOR32_1);
    x05 = L_shr_pos(im[s * 9], SCALEFACTOR32_1);
    x06 = L_shr_pos(re[s * 13], SCALEFACTOR32_1);
    x07 = L_shr_pos(im[s * 13], SCALEFACTOR32_1);
    x08 = L_shr_pos(re[s * 17], SCALEFACTOR32_1);
    x09 = L_shr_pos(im[s * 17], SCALEFACTOR32_1);
    x10 = L_shr_pos(re[s * 21], SCALEFACTOR32_1);
    x11 = L_shr_pos(im[s * 21], SCALEFACTOR32_1);
    x12 = L_shr_pos(re[s * 25], SCALEFACTOR32_1);
    x13 = L_shr_pos(im[s * 25], SCALEFACTOR32_1);
    x14 = L_shr_pos(re[s * 29], SCALEFACTOR32_1);
    x15 = L_shr_pos(im[s * 29], SCALEFACTOR32_1);

    t00 = L_add(x00, x08);
    t02 = L_sub(x00, x08);
    t01 = L_add(x01, x09);
    t03 = L_sub(x01, x09);
    t04 = L_add(x02, x10);
    t06 = L_sub(x02, x10);
    t05 = L_add(x03, x11);
    t07 = L_sub(x03, x11);
    t08 = L_add(x04, x12);
    t10 = L_sub(x04, x12);
    t09 = L_add(x05, x13);
    t11 = L_sub(x05, x13);
    t12 = L_add(x06, x14);
    t14 = L_sub(x06, x14);
    t13 = L_add(x07, x15);
    t15 = L_sub(x07, x15);

    /* Pre-additions and core multiplications */
    s00 = L_add(t00, t08);
    s04 = L_sub(t00, t08);
    s01 = L_add(t01, t09);
    s05 = L_sub(t01, t09);
    s08 = L_sub(t02, t11);
    s10 = L_add(t02, t11);
    s09 = L_add(t03, t10);
    s11 = L_sub(t03, t10);
    s02 = L_add(t04, t12);
    s07 = L_sub(t04, t12);
    s03 = L_add(t05, t13);
    s06 = L_sub(t13, t05);
    t01 = L_add(t06, t14);
    t02 = L_sub(t06, t14);
    t00 = L_add(t07, t15);
    t03 = L_sub(t07, t15);

    Mpy3_0(s12, s13, s14, s15, t00, t01, t02, t03);

    /* Post-additions */
    y16 = L_add(s00, s02);
    y24 = L_sub(s00, s02);
    y17 = L_add(s01, s03);
    y25 = L_sub(s01, s03);
    y20 = L_sub(s04, s06);
    y28 = L_add(s04, s06);
    y21 = L_sub(s05, s07);
    y29 = L_add(s05, s07);
    y22 = L_add(s08, s14);
    y30 = L_sub(s08, s14);
    y23 = L_add(s09, s15);
    y31 = L_sub(s09, s15);
    y18 = L_add(s10, s12);
    y26 = L_sub(s10, s12);
    y19 = L_add(s11, s13);
    y27 = L_sub(s11, s13);

    /* 3. FFT8 stage */
    x00 = L_shr_pos(re[s * 2], SCALEFACTOR32_1);
    x01 = L_shr_pos(im[s * 2], SCALEFACTOR32_1);
    x02 = L_shr_pos(re[s * 6], SCALEFACTOR32_1);
    x03 = L_shr_pos(im[s * 6], SCALEFACTOR32_1);
    x04 = L_shr_pos(re[s * 10], SCALEFACTOR32_1);
    x05 = L_shr_pos(im[s * 10], SCALEFACTOR32_1);
    x06 = L_shr_pos(re[s * 14], SCALEFACTOR32_1);
    x07 = L_shr_pos(im[s * 14], SCALEFACTOR32_1);
    x08 = L_shr_pos(re[s * 18], SCALEFACTOR32_1);
    x09 = L_shr_pos(im[s * 18], SCALEFACTOR32_1);
    x10 = L_shr_pos(re[s * 22], SCALEFACTOR32_1);
    x11 = L_shr_pos(im[s * 22], SCALEFACTOR32_1);
    x12 = L_shr_pos(re[s * 26], SCALEFACTOR32_1);
    x13 = L_shr_pos(im[s * 26], SCALEFACTOR32_1);
    x14 = L_shr_pos(re[s * 30], SCALEFACTOR32_1);
    x15 = L_shr_pos(im[s * 30], SCALEFACTOR32_1);

    t00 = L_add(x00, x08);
    t02 = L_sub(x00, x08);
    t01 = L_add(x01, x09);
    t03 = L_sub(x01, x09);
    t04 = L_add(x02, x10);
    t06 = L_sub(x02, x10);
    t05 = L_add(x03, x11);
    t07 = L_sub(x03, x11);
    t08 = L_add(x04, x12);
    t10 = L_sub(x04, x12);
    t09 = L_add(x05, x13);
    t11 = L_sub(x05, x13);
    t12 = L_add(x06, x14);
    t14 = L_sub(x06, x14);
    t13 = L_add(x07, x15);
    t15 = L_sub(x07, x15);

    /* Pre-additions and core multiplications */
    s00 = L_add(t00, t08);
    s04 = L_sub(t00, t08);
    s01 = L_add(t01, t09);
    s05 = L_sub(t01, t09);
    s08 = L_sub(t02, t11);
    s10 = L_add(t02, t11);
    s09 = L_add(t03, t10);
    s11 = L_sub(t03, t10);
    s02 = L_add(t04, t12);
    s07 = L_sub(t04, t12);
    s03 = L_add(t05, t13);
    s06 = L_sub(t13, t05);
    t01 = L_add(t06, t14);
    t02 = L_sub(t06, t14);
    t00 = L_add(t07, t15);
    t03 = L_sub(t07, t15);

    Mpy3_0(s12, s13, s14, s15, t00, t01, t02, t03);

    /* Post-additions */
    y32 = L_add(s00, s02);
    y40 = L_sub(s00, s02);
    y33 = L_add(s01, s03);
    y41 = L_sub(s01, s03);
    y36 = L_sub(s04, s06);
    y44 = L_add(s04, s06);
    y37 = L_sub(s05, s07);
    y45 = L_add(s05, s07);
    y38 = L_add(s08, s14);
    y46 = L_sub(s08, s14);
    y39 = L_add(s09, s15);
    y47 = L_sub(s09, s15);
    y34 = L_add(s10, s12);
    y42 = L_sub(s10, s12);
    y35 = L_add(s11, s13);
    y43 = L_sub(s11, s13);

    /* 4. FFT8 stage */
    x00 = L_shr_pos(re[s * 3], SCALEFACTOR32_1);
    x01 = L_shr_pos(im[s * 3], SCALEFACTOR32_1);
    x02 = L_shr_pos(re[s * 7], SCALEFACTOR32_1);
    x03 = L_shr_pos(im[s * 7], SCALEFACTOR32_1);
    x04 = L_shr_pos(re[s * 11], SCALEFACTOR32_1);
    x05 = L_shr_pos(im[s * 11], SCALEFACTOR32_1);
    x06 = L_shr_pos(re[s * 15], SCALEFACTOR32_1);
    x07 = L_shr_pos(im[s * 15], SCALEFACTOR32_1);
    x08 = L_shr_pos(re[s * 19], SCALEFACTOR32_1);
    x09 = L_shr_pos(im[s * 19], SCALEFACTOR32_1);
    x10 = L_shr_pos(re[s * 23], SCALEFACTOR32_1);
    x11 = L_shr_pos(im[s * 23], SCALEFACTOR32_1);
    x12 = L_shr_pos(re[s * 27], SCALEFACTOR32_1);
    x13 = L_shr_pos(im[s * 27], SCALEFACTOR32_1);
    x14 = L_shr_pos(re[s * 31], SCALEFACTOR32_1);
    x15 = L_shr_pos(im[s * 31], SCALEFACTOR32_1);

    t00 = L_add(x00, x08);
    t02 = L_sub(x00, x08);
    t01 = L_add(x01, x09);
    t03 = L_sub(x01, x09);
    t04 = L_add(x02, x10);
    t06 = L_sub(x02, x10);
    t05 = L_add(x03, x11);
    t07 = L_sub(x03, x11);
    t08 = L_add(x04, x12);
    t10 = L_sub(x04, x12);
    t09 = L_add(x05, x13);
    t11 = L_sub(x05, x13);
    t12 = L_add(x06, x14);
    t14 = L_sub(x06, x14);
    t13 = L_add(x07, x15);
    t15 = L_sub(x07, x15);

    /* Pre-additions and core multiplications */
    s00 = L_add(t00, t08);
    s04 = L_sub(t00, t08);
    s01 = L_add(t01, t09);
    s05 = L_sub(t01, t09);
    s08 = L_sub(t02, t11);
    s10 = L_add(t02, t11);
    s09 = L_add(t03, t10);
    s11 = L_sub(t03, t10);
    s02 = L_add(t04, t12);
    s07 = L_sub(t04, t12);
    s03 = L_add(t05, t13);
    s06 = L_sub(t13, t05);
    t01 = L_add(t06, t14);
    t02 = L_sub(t06, t14);
    t00 = L_add(t07, t15);
    t03 = L_sub(t07, t15);

    Mpy3_0(s12, s13, s14, s15, t00, t01, t02, t03);

    /* Post-additions */
    y48 = L_add(s00, s02);
    y56 = L_sub(s00, s02);
    y49 = L_add(s01, s03);
    y57 = L_sub(s01, s03);
    y52 = L_sub(s04, s06);
    y60 = L_add(s04, s06);
    y53 = L_sub(s05, s07);
    y61 = L_add(s05, s07);
    y54 = L_add(s08, s14);
    y62 = L_sub(s08, s14);
    y55 = L_add(s09, s15);
    y63 = L_sub(s09, s15);
    y50 = L_add(s10, s12);
    y58 = L_sub(s10, s12);
    y51 = L_add(s11, s13);
    y59 = L_sub(s11, s13);

    /* apply twiddle factors */
    y00 = L_shr_pos(y00, SCALEFACTOR32_2);
    y01 = L_shr_pos(y01, SCALEFACTOR32_2);
    y02 = L_shr_pos(y02, SCALEFACTOR32_2);
    y03 = L_shr_pos(y03, SCALEFACTOR32_2);
    y04 = L_shr_pos(y04, SCALEFACTOR32_2);
    y05 = L_shr_pos(y05, SCALEFACTOR32_2);
    y06 = L_shr_pos(y06, SCALEFACTOR32_2);
    y07 = L_shr_pos(y07, SCALEFACTOR32_2);
    y08 = L_shr_pos(y08, SCALEFACTOR32_2);
    y09 = L_shr_pos(y09, SCALEFACTOR32_2);
    y10 = L_shr_pos(y10, SCALEFACTOR32_2);
    y11 = L_shr_pos(y11, SCALEFACTOR32_2);
    y12 = L_shr_pos(y12, SCALEFACTOR32_2);
    y13 = L_shr_pos(y13, SCALEFACTOR32_2);
    y14 = L_shr_pos(y14, SCALEFACTOR32_2);
    y15 = L_shr_pos(y15, SCALEFACTOR32_2);
    y16 = L_shr_pos(y16, SCALEFACTOR32_2);
    y17 = L_shr_pos(y17, SCALEFACTOR32_2);
    y32 = L_shr_pos(y32, SCALEFACTOR32_2);
    y33 = L_shr_pos(y33, SCALEFACTOR32_2);
    y48 = L_shr_pos(y48, SCALEFACTOR32_2);
    y49 = L_shr_pos(y49, SCALEFACTOR32_2);
    y40 = L_shr_pos(y40, SCALEFACTOR32_2);
    y41 = L_shr_pos(y41, SCALEFACTOR32_2);

    cplxMpy3_0(y18, y19, RotVector_32_32[2 * 0 + 0], RotVector_32_32[2 * 0 + 1]);
    cplxMpy3_0(y20, y21, RotVector_32_32[2 * 1 + 0], RotVector_32_32[2 * 1 + 1]);
    cplxMpy3_0(y22, y23, RotVector_32_32[2 * 2 + 0], RotVector_32_32[2 * 2 + 1]);
    cplxMpy3_0(y24, y25, RotVector_32_32[2 * 3 + 0], RotVector_32_32[2 * 3 + 1]);
    cplxMpy3_0(y26, y27, RotVector_32_32[2 * 4 + 0], RotVector_32_32[2 * 4 + 1]);
    cplxMpy3_0(y28, y29, RotVector_32_32[2 * 5 + 0], RotVector_32_32[2 * 5 + 1]);
    cplxMpy3_0(y30, y31, RotVector_32_32[2 * 6 + 0], RotVector_32_32[2 * 6 + 1]);
    cplxMpy3_0(y34, y35, RotVector_32_32[2 * 7 + 0], RotVector_32_32[2 * 7 + 1]);
    cplxMpy3_0(y36, y37, RotVector_32_32[2 * 8 + 0], RotVector_32_32[2 * 8 + 1]);
    cplxMpy3_0(y38, y39, RotVector_32_32[2 * 9 + 0], RotVector_32_32[2 * 9 + 1]);
    cplxMpy3_0(y42, y43, RotVector_32_32[2 * 10 + 0], RotVector_32_32[2 * 10 + 1]);
    cplxMpy3_0(y44, y45, RotVector_32_32[2 * 11 + 0], RotVector_32_32[2 * 11 + 1]);
    cplxMpy3_0(y46, y47, RotVector_32_32[2 * 12 + 0], RotVector_32_32[2 * 12 + 1]);
    cplxMpy3_0(y50, y51, RotVector_32_32[2 * 13 + 0], RotVector_32_32[2 * 13 + 1]);
    cplxMpy3_0(y52, y53, RotVector_32_32[2 * 14 + 0], RotVector_32_32[2 * 14 + 1]);
    cplxMpy3_0(y54, y55, RotVector_32_32[2 * 15 + 0], RotVector_32_32[2 * 15 + 1]);
    cplxMpy3_0(y56, y57, RotVector_32_32[2 * 16 + 0], RotVector_32_32[2 * 16 + 1]);
    cplxMpy3_0(y58, y59, RotVector_32_32[2 * 17 + 0], RotVector_32_32[2 * 17 + 1]);
    cplxMpy3_0(y60, y61, RotVector_32_32[2 * 18 + 0], RotVector_32_32[2 * 18 + 1]);
    cplxMpy3_0(y62, y63, RotVector_32_32[2 * 19 + 0], RotVector_32_32[2 * 19 + 1]);

    /* 1. FFT4 stage */

    /* Pre-additions */
    t00 = L_add(y00, y32);
    t02 = L_sub(y00, y32);
    t01 = L_add(y01, y33);
    t03 = L_sub(y01, y33);
    t04 = L_add(y16, y48);
    t07 = L_sub(y16, y48);
    t05 = L_add(y49, y17);
    t06 = L_sub(y49, y17);

    /* Post-additions */
    re[s * 0] = L_add(t00, t04);  
    im[s * 0] = L_add(t01, t05);  
    re[s * 8] = L_sub(t02, t06); 
    im[s * 8] = L_sub(t03, t07);  
    re[s * 16] = L_sub(t00, t04);  
    im[s * 16] = L_sub(t01, t05);  
    re[s * 24] = L_add(t02, t06);  
    im[s * 24] = L_add(t03, t07);  

    /* 2. FFT4 stage */

    /* Pre-additions */
    t00 = L_add(y02, y34);
    t02 = L_sub(y02, y34);
    t01 = L_add(y03, y35);
    t03 = L_sub(y03, y35);
    t04 = L_add(y18, y50);
    t07 = L_sub(y18, y50);
    t05 = L_add(y51, y19);
    t06 = L_sub(y51, y19);

    /* Post-additions */
    re[s * 1] = L_add(t00, t04);  
    im[s * 1] = L_add(t01, t05); 
    re[s * 9] = L_sub(t02, t06); 
    im[s * 9] = L_sub(t03, t07);  
    re[s * 17] = L_sub(t00, t04); 
    im[s * 17] = L_sub(t01, t05);  
    re[s * 25] = L_add(t02, t06);  
    im[s * 25] = L_add(t03, t07); 

    /* 3. FFT4 stage */

    /* Pre-additions */
    t00 = L_add(y04, y36);
    t02 = L_sub(y04, y36);
    t01 = L_add(y05, y37);
    t03 = L_sub(y05, y37);
    t04 = L_add(y20, y52);
    t07 = L_sub(y20, y52);
    t05 = L_add(y53, y21);
    t06 = L_sub(y53, y21);

    /* Post-additions */
    re[s * 2] = L_add(t00, t04); 
    im[s * 2] = L_add(t01, t05); 
    re[s * 10] = L_sub(t02, t06);  
    im[s * 10] = L_sub(t03, t07);  
    re[s * 18] = L_sub(t00, t04);  
    im[s * 18] = L_sub(t01, t05); 
    re[s * 26] = L_add(t02, t06);  
    im[s * 26] = L_add(t03, t07);  

    /* 4. FFT4 stage */

    /* Pre-additions */
    t00 = L_add(y06, y38);
    t02 = L_sub(y06, y38);
    t01 = L_add(y07, y39);
    t03 = L_sub(y07, y39);
    t04 = L_add(y22, y54);
    t07 = L_sub(y22, y54);
    t05 = L_add(y55, y23);
    t06 = L_sub(y55, y23);

    /* Post-additions */
    re[s * 3] = L_add(t00, t04); 
    im[s * 3] = L_add(t01, t05); 
    re[s * 11] = L_sub(t02, t06);
    im[s * 11] = L_sub(t03, t07);  
    re[s * 19] = L_sub(t00, t04);  
    im[s * 19] = L_sub(t01, t05);  
    re[s * 27] = L_add(t02, t06);  
    im[s * 27] = L_add(t03, t07);

    /* 5. FFT4 stage */

    /* Pre-additions */
    t00 = L_add(y08, y41);
    t02 = L_sub(y08, y41);
    t01 = L_sub(y09, y40);
    t03 = L_add(y09, y40);
    t04 = L_add(y24, y56);
    t07 = L_sub(y24, y56);
    t05 = L_add(y57, y25);
    t06 = L_sub(y57, y25);

    /* Post-additions */
    re[s * 4] = L_add(t00, t04); 
    im[s * 4] = L_add(t01, t05); 
    re[s * 12] = L_sub(t02, t06);  
    im[s * 12] = L_sub(t03, t07); 
    re[s * 20] = L_sub(t00, t04);  
    im[s * 20] = L_sub(t01, t05); 
    re[s * 28] = L_add(t02, t06);  
    im[s * 28] = L_add(t03, t07); 

    /* 6. FFT4 stage */

    /* Pre-additions */
    t00 = L_add(y10, y42);
    t02 = L_sub(y10, y42);
    t01 = L_add(y11, y43);
    t03 = L_sub(y11, y43);
    t04 = L_add(y26, y58);
    t07 = L_sub(y26, y58);
    t05 = L_add(y59, y27);
    t06 = L_sub(y59, y27);

    /* Post-additions */
    re[s * 5] = L_add(t00, t04); 
    im[s * 5] = L_add(t01, t05); 
    re[s * 13] = L_sub(t02, t06); 
    im[s * 13] = L_sub(t03, t07);  
    re[s * 21] = L_sub(t00, t04);  
    im[s * 21] = L_sub(t01, t05);
    re[s * 29] = L_add(t02, t06);  
    im[s * 29] = L_add(t03, t07);  

    /* 7. FFT4 stage */

    /* Pre-additions */
    t00 = L_add(y12, y44);
    t02 = L_sub(y12, y44);
    t01 = L_add(y13, y45);
    t03 = L_sub(y13, y45);
    t04 = L_add(y28, y60);
    t07 = L_sub(y28, y60);
    t05 = L_add(y61, y29);
    t06 = L_sub(y61, y29);

    /* Post-additions */
    re[s * 6] = L_add(t00, t04); 
    im[s * 6] = L_add(t01, t05); 
    re[s * 14] = L_sub(t02, t06);
    im[s * 14] = L_sub(t03, t07); 
    re[s * 22] = L_sub(t00, t04);  
    im[s * 22] = L_sub(t01, t05); 
    re[s * 30] = L_add(t02, t06);  
    im[s * 30] = L_add(t03, t07); 

    /* 8. FFT4 stage */

    /* Pre-additions */
    t00 = L_add(y14, y46);
    t02 = L_sub(y14, y46);
    t01 = L_add(y15, y47);
    t03 = L_sub(y15, y47);
    t04 = L_add(y30, y62);
    t07 = L_sub(y30, y62);
    t05 = L_add(y63, y31);
    t06 = L_sub(y63, y31);

    /* Post-additions */
    re[s * 7] = L_add(t00, t04); 
    im[s * 7] = L_add(t01, t05); 
    re[s * 15] = L_sub(t02, t06); 
    im[s * 15] = L_sub(t03, t07);
    re[s * 23] = L_sub(t00, t04);  
    im[s * 23] = L_sub(t01, t05);  
    re[s * 31] = L_add(t02, t06);  
    im[s * 31] = L_add(t03, t07);  

}

static void fftN2(Word32* re, Word32* im, const Word16* W, Word16 dim1, Word16 dim2, Word16 sx, Word16 sc, Word32* x,
    Word16 Woff)
{
    Word32 i, j;


    for (i = 0; i < dim2; i++)
    {
        for (j = 0; j < dim1; j++)
        {
            x[2 * i * dim1 + 2 * j] = re[sx * i + sx * j * dim2];
            x[2 * i * dim1 + 2 * j + 1] = im[sx * i + sx * j * dim2];
        }
    }

    switch (dim1)
    {

    case 32:
        for (i = 0; i < dim2; i++)
        {
            fft32(&x[i * 2 * dim1], &x[i * 2 * dim1 + 1], 2);
        }
        break;

    default:
        break;
    }

    switch (dim2)
    {
    
    Word32 y[2 * 20];
    case 15:

        for (j = 0; j < dim2; j++)
        {
            cplxMpy4_12_1(y[2 * j], y[2 * j + 1], x[2 * 0 + 2 * j * dim1], x[2 * 0 + 2 * j * dim1 + 1]);
        }

        fft15(y,&y[1],2);

        for (j = 0; j < dim2; j++)
        {
            re[sx * 0 + sx * j * dim1] = y[2 * j];
            im[sx * 0 + sx * j * dim1] = y[2 * j + 1];
        }

        for (i = 1; i < dim1; i++)
        {
            cplxMpy4_12_1(y[2 * (0 + 0)], y[2 * (0 + 0) + 1], x[2 * i + 2 * (0 + 0) * dim1],
                x[2 * i + 2 * (0 + 0) * dim1 + 1]);
            cplxMpy4_12_0(y[2 * (0 + 1)], y[2 * (0 + 1) + 1], x[2 * i + 2 * (0 + 1) * dim1],
                x[2 * i + 2 * (0 + 1) * dim1 + 1], W[sc * i + sc * (0 + 1) * dim1 - Woff],
                W[sc * i + sc * (0 + 1) * dim1 + 1 - Woff]);
            for (j = 2; j < dim2; j = j + 2)
            {
                cplxMpy4_12_0(y[2 * (j + 0)], y[2 * (j + 0) + 1], x[2 * i + 2 * (j + 0) * dim1],
                    x[2 * i + 2 * (j + 0) * dim1 + 1], W[sc * i + sc * (j + 0) * dim1 - Woff],
                    W[sc * i + sc * (j + 0) * dim1 + 1 - Woff]);
                cplxMpy4_12_0(y[2 * (j + 1)], y[2 * (j + 1) + 1], x[2 * i + 2 * (j + 1) * dim1],
                    x[2 * i + 2 * (j + 1) * dim1 + 1], W[sc * i + sc * (j + 1) * dim1 - Woff],
                    W[sc * i + sc * (j + 1) * dim1 + 1 - Woff]);
            }
            fft15(y, &y[1], 2);
            for (j = 0; j < dim2; j++)
            {
                re[sx * i + sx * j * dim1] = y[2 * j];
                im[sx * i + sx * j * dim1] = y[2 * j + 1];
            }
        }
        break;
    default:
        break;
    }
}

/**
 * \brief Complex valued FFT
 *
 * \param    [i/o] re          real part
 * \param    [i/o] im          imag part
 * \param    [i  ] sizeOfFft   length of fft
 * \param    [i  ] s           stride real and imag part
 * \param    [i  ] scale       scalefactor
 *
 * \return void
 */
 /* x is the scratch buffer */
void BASOP_cfft(Word32* re, Word32* im, Word16 sizeOfFft, Word16 s, Word16* scale, Word32* x)
{
    switch (sizeOfFft)
    {
    case 480:/* 48k 10ms */
        fftN2(re, im, RotVector_480, 32, 15, s, 2, x, 64);
        s = (*scale + SCALEFACTOR480);
        break;
    default:
        break;
    }
    *scale = s;
}


#else

#endif // ANDES_DSP_LIB_FFT
