/********************************************************************************************************
 * @file    hadm.h
 *
 * @brief   This is the header file for B91
 *
 * @author  Driver Group
 * @date    2023
 *
 * @par     Copyright (c) 2023, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *
 *          Licensed under the Apache License, Version 2.0 (the "License");
 *          you may not use this file except in compliance with the License.
 *          You may obtain a copy of the License at
 *
 *              http://www.apache.org/licenses/LICENSE-2.0
 *
 *          Unless required by applicable law or agreed to in writing, software
 *          distributed under the License is distributed on an "AS IS" BASIS,
 *          WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *          See the License for the specific language governing permissions and
 *          limitations under the License.
 *
 *******************************************************************************************************/
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#define HADM_VERSION_INT(major, minor, micro) (((major) << 16) | ((minor) << 8) | (micro))
#define HADM_VERSION HADM_VERSION_INT(1, 1, 10)

#ifdef __riscv
#define ANDES 1
#else
#define ANDES 0
#endif

//The amount of IQ data used in the Fcs phase for frequency offset estimation.
#define MAXFCSLEN (100*4) //80us in standard

//Each TONE interaction at both ends of the TES phase is used to estimate the
//amount of IQ data at a distance.
#define MAXTESLEN (40*4) // 40/20/10 us in standard

//The PES stage is used for distance estimation of the valid sample data bit length.
#define MAXPNLEN (32) //4 oct in standard
#define MAXFMCLEN ((MAXPNLEN + 8) * 4) //4 oct in standard

//tesCalcDistance
#define MAXCHANNUM 80
#define CHANNUMSTART 0
#define CHANNUM 40
#define FSTEP 2e6

#if (CHANNUM == 80)
#define MATLEN 40
#define NUMSEP 8
#if FLOATPOINT
#define MEANDIVNORM 1.0
#define MEANDIV 0
#else
#define MEANDIVNORM 32
#define MEANDIV 5
#endif
#elif (CHANNUM == 40)
#define MATLEN 20
#define NUMSEP 4
#define MEANDIVNORM 16
#define MEANDIV 4
#elif (CHANNUM == 20)
#define MATLEN 10
#define NUMSEP 4
#define MEANDIVNORM 8
#define MEANDIV 3
#endif
#define HMATLEN  (((MATLEN+1)*MATLEN)/2) //length of hermitiann matrix


#define FLOATPOINT 0

#if FLOATPOINT
#define PRECBITS 0
#define PRECBITSNORM 1 //2^22
#define PRECBITSSQRT 0
#define PRECBITSSQRTNORM 1
#define DIGITTYPE float
#define DIGITTYPELONG float
#define DIGITTYPESHORT float
#define TOL1 1e-7
#else
#define PRECBITS 28
#define PRECBITSNORM  268435456//2^PRECBITS
#define PRECBITSSQRT 14
#define PRECBITSSQRTNORM 16384
#define PRECQ15NORM 32767
#define TOL1 (1e-7*PRECBITSNORM)

#if(ANDES == 1)
#include <nds_math_types.h>
#include <nds_intrinsic.h>
#include <nds_utils_math.h>
#include <nds_basic_math.h>
#include <nds_statistics_math.h>
#include <nds_complex_math.h>

//#define DIGITTYPE q31_t
#define DIGITTYPE int
#define DIGITTYPELONG q63_t
#define DIGITTYPESHORT q15_t
#else
//#define DIGITTYPE __int32
//#define DIGITTYPELONG __int64
//#define DIGITTYPESHORT __int16
#define DIGITTYPE __INT32_TYPE__
#define DIGITTYPELONG __INT64_TYPE__
#define DIGITTYPESHORT __INT16_TYPE__

#endif
#endif

//Define the IQ data type.
struct complex {
	DIGITTYPE real;
	DIGITTYPE imag;
};
typedef struct complex complex;

//It is used for the collection of IQ data at both ends of the tes phase.
struct tesInfo {
	float distanceEst;  //for simulation only
	float distanceReal; //for simulation only
	float likeliness;
	float RSSI_initiator;
	float RSSI_reflector;
	complex I[MAXCHANNUM]; // iq data collected from initator
	complex Q[MAXCHANNUM]; // iq data collected from reflector
};
typedef struct tesInfo tesInfo;

//Receive the return value of the TES initialization stage and provide it to the
//algorithm for distance calculation.
struct paramterConstTes {
	float cPhase2R;
	float firstPeakTolx;
	DIGITTYPE invDen;
	DIGITTYPE firstPeakMinLevel;
	float tol2;
	float invPREC;
	float invPRECSQRT;
	float l2s2R;
	float x[CHANNUM];
};
typedef struct paramterConstTes paramterConstTes;

//Used to record RSSI and FTE information on both ends.
struct pesInfo {
	float RSSI_initiator;
	float RSSI_reflector;
	int fteInitiator;
	int fteReflector;
};
typedef struct pesInfo pesInfo;

//Receive the return value of the PES initialization stage and provide it to the
//algorithm for distance calculation.
struct paramterConstPes {
	float xTick;
	float fteOffseta;
	float fteOffsetb;
	float tofCalInitiator;
	float tofCalReflector;
};
typedef struct paramterConstPes paramterConstPes;


#define PI (3.141592653589793)
#define SPEEDOFLIGHT (299792458)
#define SAMPLERATE (4e6)
#define FCLK (24e6)
#define FIF (1e6)

/*
 * @brief			This function is mainly used to return the version number of the algorithm library.
 * @param[in]		none.
 * @return			The version of hadm.a.
 */
int get_version();

/*
 * @brief			This function is mainly used to estimate the frequency offset information through IQ data.
 * @param[in]		IQData	-	IQ data collected during the FCS phase.
 * @param[in]		IQLen	-	The number of groups of IQ data collected during the FCS phase.
 * @param[in]		cfoCoarse	-	coarse CFO£¨carrier frequency offset£© estimation value.
 * @param[in]		sampleRate	-	Sampling frequency of the FCS stage.
 * @return			The frequency offset between initiator and reflector.
 */
float calcFreq(float IQData[], int IQLen, float cfoCoarse, float sampleRate);

/*
 * @brief			This function is mainly used to estimate the frequency offset information through IQ data.
 * @param[in]		pnData	-	IQ data collected during the PES phase.
 * @param[in]		pnLen	-	The number of groups of IQ data collected during the FCS phase.
 * @param[in]		fmcData	-	fmc data.
 * @param[in]		fmcLen	-	The number of fmc data.
 * @return			0:success; others:false.
 */
float calcPesInfo(int pnData[], int pnLen, float fmcData[], int fmcLen);

/*
 * @brief			This function is mainly used for initializing the distance calculation algorithm in PES mode.
 * @param[in]		none.
 * @return			The paramter of distance calculate.
 */
paramterConstPes pesInit(float sampleRate);

/*
 * @brief			This function is mainly used to calculate the compensation value.
 * @param[in]		compArr		-	The address where the compensation value is stored.
 * @param[in]		IQLen		-	The number of groups of IQ data collected.
 * @param[in]		angleStep	-	The step value of the angle.
 * @return			0:success; others:false.
 */
int calcCompensate(float compArr[], int IQLen, float angleStep);

/*
 * @brief			This function is mainly used to calculate the information of Tes.
 * @param[in]		IQData		-	The address where the compensation value is stored.
 * @param[in]		compArr		-	The number of groups of IQ data collected.
 * @param[in]		IQLen		-	The step value of the angle.
 * @param[in]		iq_timeStamp	-	The timestamp when the IQ data collection begins.
 * @param[in]		trx_timeStamp	-	TX RX conversion time.
 * @param[in]		cfo				-	The frequency offset at both ends of initiator and reflect, plus and minus signs are determined by the character.
 * @param[in]		if_adjustment	-	Frequency compensation value.
 * @param[in]		output			-	Processed IQ data.
 * @return			0:success; others:false.
 */
int calcTesInfo(int IQData[], const float compArr[], int IQLen, int iq_timeStamp, int trx_timeStamp, float cfo, float if_adjustment, int output[]);

/*
 * @brief			This function is mainly used to compress IQ data collected during the tes phase.
 * @param[in]		ipm		-	IQ data processed by calcTesInfo.
 * @param[in]		IQLen	-	The number of groups of IQ data collected.
 * @return			0:success; others:false.
 */
int compressTesInfo(int ipm[], int len);
#define CALIFACOR 10

/*
 * @brief			This function is mainly used for initializing the distance calculation algorithm in TES mode.
 * @param[in]		none.
 * @return			The paramter of distance calculate.
 */
paramterConstTes tesInit();

/*
 * @brief			This function is mainly used to calculate the compensation value.
 * @param[in]		I[]		-	IQ data processed by initiator.
 * @param[in]		Q[]		-	IQ data processed by reflector.
 * @param[in]		cali	-	Calibration compensation.
 * @param[in]		H2WR[]	-	The address where the results of the operation are stored.
 * @return			0:success; others:false.
 */
int calcIpmPct(const int I[], const int Q[], const int cali[], complex H2WR[]);

/*
 * @brief			This function is used to estimate the distance by means of phase difference.
 * @param[in]		H2WR[]	-	IQ data processed by calcIpmPct.
 * @param[in]		T2WR[]	-	Intermediate calculation results.
 * @param[in]		T2WRDiffMean	-	Intermediate calculation results.
 * @param[in]		para	-	The results produced by tesInit.
 * @return			The result of distance.
 */
float tesPhase(const complex H2WR[], float T2WR[], float* T2WRDiffMean, paramterConstTes para);

/*
 * @brief			This function is used to estimate the distance by means of phase difference.
 * @param[in]		H2WR[]	-	IQ data processed by calcIpmPct.
 * @param[in]		T2WR[]	-	Intermediate calculation results.
 * @param[in]		T2WRDiffMean	-	Intermediate calculation results.
 * @param[in]		para	-	The results produced by tesInit.
 * @param[in]		likeliness -	Variables for debugging.
 * @param[in]		nIterMaxEig	-	Variables for debugging.
 * @param[in]		nIterPS		-	Intermediate calculation results.
 * @param[in]		nSigCnt		-	Intermediate calculation results.
 * param[in]		EVDCap		-	Intermediate calculation results.
 * @return			The result of distance.
 */
float tesMusic(complex H2WR[], float T2WR[], float T2WRDiffMean, paramterConstTes para, float* likeliness, float* nIterMaxEig, float* nIterPS, float* nSigCnt, float* EVDCap);
#ifdef __cplusplus
}
#endif
