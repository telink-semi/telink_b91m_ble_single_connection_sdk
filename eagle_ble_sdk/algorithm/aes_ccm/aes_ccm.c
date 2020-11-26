/********************************************************************************************************
 * @file	aes_ccm.c
 *
 * @brief	for TLSR chips
 *
 * @author	BLE GROUP
 * @date	2020.06
 *
 * @par		Copyright (c) 2020, Telink Semiconductor (Shanghai) Co., Ltd.
 *			All rights reserved.
 *
 *			The information contained herein is confidential property of Telink
 *          Semiconductor (Shanghai) Co., Ltd. and is available under the terms
 *          of Commercial License Agreement between Telink Semiconductor (Shanghai)
 *          Co., Ltd. and the licensee or the terms described here-in. This heading
 *          MUST NOT be removed from this file.
 *
 *          Licensee shall not delete, modify or alter (or permit any third party to delete, modify, or  
 *          alter) any information contained herein in whole or in part except as expressly authorized  
 *          by Telink semiconductor (shanghai) Co., Ltd. Otherwise, licensee shall be solely responsible  
 *          for any claim to the extent arising out of or relating to such deletion(s), modification(s)  
 *          or alteration(s).
 *
 *          Licensees are granted free, non-transferable use of the information in this
 *          file under Mutual Non-Disclosure Agreement. NO WARRENTY of ANY KIND is provided.
 *
 *******************************************************************************************************/
#include "tl_common.h"
#include "drivers.h"
#include "stack/ble/ble_common.h"
#include "stack/ble/ble_stack.h"
#include "stack/ble/ble_format.h"

#include "aes_ccm.h"




/**
 * @brief   	this function is used to encrypt the plaintext
 * @param[in]   *key - aes key
 * @param[in]   *plaintext - plaint_text
 * @param[in]   *result - return encrypt result
 * @return  	none.
 * @Note		Input data requires strict Word alignment
 */
void aes_ll_encryption(u32* key, u32* plaintext, u8 *result)
{
	for (int i=0; i<4; i++) {
		reg_aes_key(i) = key[i];
		aes_data_buff[i] = plaintext[i];
	}

    reg_aes_ptr = (u32)aes_data_buff;

    aes_set_mode(FLD_AES_START);      //cipher mode

    while(reg_aes_mode & FLD_AES_START);

    aes_get_result(result);
}


/**
 * @brief   	this function is used to decrypt the plaintext
 * @param[in]   *key - aes key
 * @param[in]   *plaintext - plaint_text
 * @param[in]   *result - return decrypt result
 * @return  	none.
 * @Note		Input data requires strict Word alignment
 */
void aes_ll_decryption(u32* key, u32* plaintext, u8 *result)
{
	for (int i=0; i<4; i++) {
		reg_aes_key(i) = key[i];
		aes_data_buff[i] = plaintext[i];
	}

    reg_aes_ptr = (u32)aes_data_buff;

    aes_set_mode(FLD_AES_START | FLD_AES_MODE);      //decipher mode

    while(reg_aes_mode & FLD_AES_START);

    aes_get_result(result);
}


static u8 aes_initKey(u8 *key)
{
	unsigned int temp;
	for (int i = 0; i < 4; i++) {
		temp = key[16-(4*i)-4]<<24 | key[16-(4*i)-3]<<16 | key[16-(4*i)-2]<<8 | key[16-(4*i)-1];
		reg_aes_key(i) = temp;
	}

	return AES_SUCC;
}


static void aes_encGetRslt(u8* plaintext, u8 *result)
{
	u8 *ptr = plaintext + 12;

    for (int i=0; i<4; i++){
    	aes_data_buff[i] = (ptr[3]) | (ptr[2]<<8) | (ptr[1]<<16) | (ptr[0]<<24);
    	ptr -= 4;
    }

    reg_aes_ptr = (u32)aes_data_buff;

    aes_set_mode(FLD_AES_START);      //cipher mode

    while(reg_aes_mode & FLD_AES_START);

    aes_get_result(result);
}




/*********************************************************************
 * @fn      aes_ccmAuthTran
 *
 * @brief   calc the aes ccm value
 *
 * @param   micLen - mic lenth (should be 4)
 *
 * @param   iv - initial vector (should be 13 bytes nonce)
 *
 * @param   mStr - plaint text
 *
 * @param   mStrLen - plaint text length
 *
 * @param   aStr -  a string  (should be AAD the data channel PDU header’s first octet with NESN, SN and MD bits masked to 0)
 *
 * @param   aStrLen - a atring lenth (should be 1)
 *
 * @param   result - result (result)
 *
 * @return  status l2cap_sts_t
 */
static u8 aes_ccmAuthTran(u8 micLen, u8 *key, u8 *iv, u8 *mStr, u16 mStrLen, u8 *aStr, u16 aStrLen, u8 *result)
{
	u16 msgLen;
	u8 mStrIndex = 0;
	ccm_flags_t flags;
	u16 i,j;

	if ( aStrLen + 2 > AES_BLOCK_SIZE ) {
		while(1);
	}
	//aes_enc_t *tmpPtr = (aes_enc_t*)ev_buf_allocate(sizeof(aes_enc_t));
	aes_enc_t *tmpPtr, encTmp;
	tmpPtr = &encTmp;
	memset(tmpPtr, 0, sizeof(aes_enc_t));
	if ( tmpPtr == NULL ) {
		while(1);
	}
	/* get B0 */
	flags.bf.L = 1;  /* L-1 (15-nonceLen-1)*/
	flags.bf.M = (micLen - 2)>>1;
	flags.bf.aData = (aStrLen > 0) ? 1 : 0;
	flags.bf.reserved = 0;

	tmpPtr->bf.B[0] = flags.val;
	/* copy nonce N */
	memcpy(tmpPtr->bf.B + 1, iv, 13);
	/* last byte is mStrlen */
	tmpPtr->bf.B[14] = mStrLen>>8;
	tmpPtr->bf.B[15] = mStrLen & 0xff;


	tmpPtr->newAstr[0] = aStrLen>>8;
	tmpPtr->newAstr[1] = aStrLen & 0xff;
	memcpy(tmpPtr->newAstr + 2, aStr, aStrLen);
	aStrLen += 2;

	/* X0 is zero */
	memset(tmpPtr->tmpResult, 0, AES_BLOCK_SIZE);

	msgLen = aStrLen;
	/* adjust msgLen according to aStrLen and mStrLen, should be 16x */
	if (aStrLen & 0x0f) {
		msgLen &= ~0x0F;
		msgLen += 0x10;
	}

	msgLen += mStrLen;
	if (mStrLen & 0x0f) {
		msgLen &= ~0x0F;
		msgLen += 0x10;
	}


	/* now the msgLen should be the length of AuthData, which is generated by AddAuthData (astring, padded by 0) || PlaintexeData (mString, padded by 0)*/
	for ( i=0; i<msgLen+16; i+=(1<<4) ) {
		for ( j=0; j<AES_BLOCK_SIZE; j++) {
			/* get Xi XOR Bi */
			tmpPtr->tmpResult[j] ^= tmpPtr->bf.B[j];
		}

		/* use aes to get E(key, Xi XOR Bi) */
		aes_encGetRslt(tmpPtr->tmpResult, tmpPtr->tmpResult);
		/* update B */
		if ( aStrLen >= AES_BLOCK_SIZE ) {
			memcpy(tmpPtr->bf.B, tmpPtr->newAstr + i, AES_BLOCK_SIZE);
			aStrLen -= AES_BLOCK_SIZE;
		} else if ( (aStrLen>0) && (aStrLen<AES_BLOCK_SIZE) ) {
			memcpy(tmpPtr->bf.B, tmpPtr->newAstr + i, aStrLen);
			memset(tmpPtr->bf.B + aStrLen, 0, AES_BLOCK_SIZE - aStrLen);
			aStrLen = 0;
			/* reset the mstring index */
			mStrIndex = 0;
		} else if ( mStrLen >= AES_BLOCK_SIZE ) {
			memcpy(tmpPtr->bf.B, mStr + (mStrIndex*AES_BLOCK_SIZE), AES_BLOCK_SIZE);
			mStrIndex++;
			mStrLen -= AES_BLOCK_SIZE;
		} else {
			memcpy(tmpPtr->bf.B, mStr + (mStrIndex*AES_BLOCK_SIZE), mStrLen);
			memset(tmpPtr->bf.B + mStrLen, 0, AES_BLOCK_SIZE - mStrLen);
		}
	}
	memcpy(result, tmpPtr->tmpResult, micLen);

	return 0;
}


/*********************************************************************
 * @fn      aes_ccmBaseTran
 *
 * @brief   calc the aes ccm value
 *
 * @param   micLen - mic lenth (should be 4)
 *
 * @param   iv - initial vector (should be 13 bytes nonce)
 *
 * @param   mStr - plaint text
 *
 * @param   mStrLen - plaint text length
 *
 * @param   aStr -  a string  (should be AAD the data channel PDU header’s first octet with NESN, SN and MD bits masked to 0)
 *
 * @param   aStrLen - a atring lenth (should be 1)
 *
 * @param   result - result (result)
 *
 * @return  status l2cap_sts_t
 */

#if(DEBUG_PAIRING_ENCRYPTION)
u8 AF_key[16];    //26 80 00 00 00 00 5d 02  4b 4e 4c 54 fa 01 88 00
u8 AF_a[16];      //01 00 00 00 00 80 24 ab  dc ba e1 77 ad 92 00 00
u8 AF_result[16]; //05 eb b3 5e d4 f4 36 3a  9e 6a e5 07 f7 10 82 eb
#endif

_attribute_ram_code_
static u8 aes_ccmBaseTran(u8 micLen, u8 *key, u8 *iv, u8 *mStr, u16 mStrLen, u8 *aStr, u8 aStrLen, u8 *mic)
{
	ccm_flags_t flags;
	u16 msgLen;//fix, DLE length greater than 240Bytes, msgLen is 256
	u16 i;
	u8 j;
	u16 counter = 1;

	//aes_enc_t *tmpPtr = (aes_enc_t*)ev_buf_allocate(sizeof(aes_enc_t));
	aes_enc_t *tmpPtr, encTmp;
	tmpPtr = &encTmp;
	memset(tmpPtr, 0, sizeof(aes_enc_t));

	if ( tmpPtr == NULL ) {
	  while(1);
	}


	memset(&flags, 0, sizeof(ccm_flags_t));
	flags.bf.L = 1;
	tmpPtr->bf.A[0] = flags.val;

	/* set the iv */
	memcpy(tmpPtr->bf.A+1, iv, 13);

	tmpPtr->bf.A[14] = tmpPtr->bf.A[15] = 0;

	aes_encGetRslt(tmpPtr->bf.A, tmpPtr->tmpResult);

#if(DEBUG_PAIRING_ENCRYPTION)
		memcpy(AF_key, key, 16);
		memcpy(AF_a, tmpPtr->bf.A, 16);
		memcpy(AF_result, tmpPtr->tmpResult, 16);
#endif

	for ( i=0; i<micLen; i++ ) {
		mic[i] ^= tmpPtr->tmpResult[i];
	}

	tmpPtr->bf.A[14] = counter>>8;
	tmpPtr->bf.A[15] = counter & 0xff;


	msgLen = mStrLen;
	if (msgLen & 0x0f) {
		msgLen &= ~0x0F;
		msgLen += 0x10;
	}


	for ( i=0; i<msgLen; i+=(1<<4) ) {
		/* use aes to the E(key, Ai) */
		aes_encGetRslt(tmpPtr->bf.A, tmpPtr->tmpResult);

		for ( j=0; (j<AES_BLOCK_SIZE) && (i+j < mStrLen); j++) {
			mStr[i+j] ^= tmpPtr->tmpResult[j];
		}

		/* update Ai */
		counter++;
		tmpPtr->bf.A[14] = counter>>8;
		tmpPtr->bf.A[15] = counter & 0xff;
	}

	return 0;
}



_attribute_ram_code_
static u8 aes_ccmDecAuthTran(u8 micLen, u8 *key, u8 *iv, u8 *mStr, u16 mStrLen, u8 *aStr, u8 aStrLen, u8 *mic)
{
    u8 tmpMic[AES_BLOCK_SIZE];
    u8 i;
    aes_ccmAuthTran(micLen, key, iv, mStr, mStrLen, aStr, aStrLen, tmpMic);
    for ( i=0; i<micLen; i++ ) {
        if ( mic[i] != tmpMic[i] ) {
            return -1;
        }
    }
    return 0;
}


/**
 * @brief   	this function is used to initialize the aes_ccm initial value
 * @param[in]   ltk - encryption key, LTK
 * @param[in]   skdm -
 * @param[in]   skds -
 * @param[in]   ivm -
 * @param[in]   ivs -
 * @param[in]   pd - Reference structure ble_crypt_para_t
 * @return  	none
 */
void aes_ll_ccm_encryption_init (u8 *ltk, u8 *skdm, u8 *skds, u8 *ivm, u8 *ivs, ble_crypt_para_t *pd)
{
	u8 sk[16];
	memcpy (sk, skdm, 8);
	memcpy (sk+8, skds, 8);

	aes_ll_encryption ((u32 *)ltk, (u32 *)sk, sk);

	aes_initKey(sk);


	memcpy (pd->sk, sk, 16);
	memcpy (pd->nonce.iv, ivm, 4);
	memcpy (pd->nonce.iv + 4, ivs, 4);
	pd->enc_pno = 0;
	pd->dec_pno = 0;

}


/**
 * @brief   	this function is used to encrypt the aes_ccm value
 * @param[in]   pkt - plaint_text
 * @param[in]   master - ll_ccm_enc: Master role must use 1, Slave role must use 0;
                         ll_ccm_dec: Master role must use 0, Slave role must use 1;
 * @param[in]   pd - Reference structure ble_crypt_para_t
 * @return  	none
 */
void aes_ll_ccm_encryption(u8 *pkt, int master, ble_crypt_para_t *pd)
{
	u8 mic[4];
	u8 llid = pkt[0] & 3;
	u8 len = pkt[1];
	pkt[1] += 4;
	pkt += 2;
	pd->nonce.pkt = pd->enc_pno++;
	pd->nonce.dir = master ? 0x80 : 0;
	aes_initKey(pd->sk);
	aes_ccmAuthTran(4, 0, (u8*)&pd->nonce, pkt, len, &llid, 1, mic);
	aes_ccmBaseTran(4, 0, (u8*)&pd->nonce, pkt, len, &llid, 1, mic);
    memcpy (pkt + len, mic, 4);
}



#if(DEBUG_PAIRING_ENCRYPTION)

u8 AA_pkt1[64];
/*
00 00 00 00 24 35 b3 22  3e 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00  00 00 00 00 00 00 c0 00
 */

int aes_ll_ccm_decryption(u8 *pkt, int master, ble_crypt_para_t *pd)
{
	//found by yafei/qinghua.
	//situation: click phone BLUE icon to open/close blue frequently.it is possible that peer not encryption while we encryption.
	if(pkt[1] < 5){
		return 1; ///error, and response mic_fail to peer device
	}

	pkt[1] -= 4;
	u8 llid = pkt[0] & 3;
	u8 len = pkt[1];
	pkt += 2;
	pd->nonce.pkt = pd->dec_pno++;
	pd->nonce.dir = master ? 0x80 : 0;
	aes_initKey(pd->sk);
	aes_ccmBaseTran(4, 0, (u8*)&pd->nonce, pkt, len, &llid, 1, pkt + len);

#if(DEBUG_PAIRING_ENCRYPTION)
	memcpy(AA_pkt1 + 4, pkt, 60);
#endif



    u8 r = aes_ccmDecAuthTran(4, 0, (u8*)&pd->nonce, pkt, len, &llid, 1, pkt + len);


    return r;
}



#else
/**
 * @brief   	this function is used to decrypt the aes_ccm value
 * @param[in]   pkt - plaint_text
 * @param[in]   master - ll_ccm_enc: Master role must use 1, Slave role must use 0;
                         ll_ccm_dec: Master role must use 0, Slave role must use 1;
 * @param[in]   pd - Reference structure ble_crypt_para_t
 * @return  	0: decryption succeeded; 1: decryption failed
 */
_attribute_ram_code_
int aes_ll_ccm_decryption(u8 *pkt, int master, ble_crypt_para_t *pd)
{
	//found by yafei/qinghua.
	//situation: click phone BLUE icon to open/close blue frequently.it is possible that peer not encryption while we encryption.
	if(pkt[1] < 5){
		return 1; ///error, and response mic_fail to peer device
	}

	pkt[1] -= 4;
	u8 llid = pkt[0] & 3;
	u8 len = pkt[1];
	pkt += 2;
	pd->nonce.pkt = pd->dec_pno++;
	pd->nonce.dir = master ? 0x80 : 0;
	aes_initKey(pd->sk);
	aes_ccmBaseTran(4, 0, (u8*)&pd->nonce, pkt, len, &llid, 1, pkt + len);
    u8 r = aes_ccmDecAuthTran(4, 0, (u8*)&pd->nonce, pkt, len, &llid, 1, pkt + len);

    return r;
}

#endif




#if (LL_FEATURE_ENABLE_ISO)

void aes_ll_ccm_encryption_v2(leCryptCtrl_t *pd)
{
	u8  enEncFlg = pd->cryptBitsInfo.enEncFlg;
	u8* pPduLen  = &pd->pllPhysChnPdu->llPduHdr.pduHdr.rf_len;
	u8  rawPduLen = *pPduLen;

	if(rawPduLen == 0 || !enEncFlg){ //rf_len must > 0 && encryption enable
		return;
	}

	*pPduLen += 4; //plus 4 byte mic length

	/*
	 * IgnoreBITS_ACL = 0x1C;     // NESN, SN and MD
	 * IgnoreBITS_CIS = 0x5C;     // NESN, SN, NPI and CIE
	 * IgnoreBITS_BIS = 0x3C;     // CSSN and CSTF
	 */
	u8 AAD;
	u8 mic[4];
	u8 nonceType = pd->cryptBitsInfo.noneType;
	u8 pduType = pd->pllPhysChnPdu->llPduHdr.pduHdr.type;
	u8* pPduPkt = pd->pllPhysChnPdu->llPayload;
	ble_cyrpt_nonce_t*	pCcmNonce = &pd->ccmNonce;

	u8 pktcntr[5] = {0}; //39-bit packet counter + 1bit dir
	memcpy(pktcntr, (u8*)&pd->txPayloadCnt, 5);
	pCcmNonce->pkt = pd->txPayloadCnt & 0xffffffff;
	pCcmNonce->dir = (pktcntr[4] & 0x7F) | (pd->cryptBitsInfo.role ? BIT(7) : 0); //dir:bit7, pktcntr's 32~38bits:bit0~bit6

	if(nonceType == CRYPT_NONCE_TYPE_ACL){
		AAD = pduType & (~0x1c);
		pd->txPayloadCnt++;//For ACL connection, txPayloadCnt's value need ++ here
		pd->txPayloadCnt &= 0x7FFFFFFFFF;
	}
	else if(nonceType == CRYPT_NONCE_TYPE_CIS){
		AAD = pduType & (~0x5c);
		//Notice:Before input parameter, the txPayloadCnt's value use cisPayloadNum
	}
	else{ //BIS
		AAD = pduType & (~0x3c);
		//Notice:Before input parameter, the txPayloadCnt's value use bisPayloadNum
	}

	aes_initKey(pd->sk);
	aes_ccmAuthTran(4, NULL, (u8*)pCcmNonce, pPduPkt, rawPduLen, &AAD, 1, mic);
	aes_ccmBaseTran(4, NULL, (u8*)pCcmNonce, pPduPkt, rawPduLen, &AAD, 1, mic);
    smemcpy ((pPduPkt + rawPduLen), mic, 4);
}

int aes_ll_ccm_decryption_v2(leCryptCtrl_t *pd)
{
	u8  enEncFlg = pd->cryptBitsInfo.enEncFlg;
	u8* pPduLen  = &pd->pllPhysChnPdu->llPduHdr.pduHdr.rf_len;
	u8 r = 1;//dft:FAIL. 1: FAIL, 0: SUCC

	if(*pPduLen <= 4 || !enEncFlg){ //rf_len must > 4(mic's length 4, not empty pkt) && encryption enable
		return r;
	}

	*pPduLen -= 4; //minus 4 byte mic length
	u8 rawPduLen = *pPduLen;

	u8 AAD;
	u32 chkMIC, rawMIC;
	u8 nonceType = pd->cryptBitsInfo.noneType;
	u8 pduType = pd->pllPhysChnPdu->llPduHdr.pduHdr.type;
	u8* pPduPkt = pd->pllPhysChnPdu->llPayload;
	ble_cyrpt_nonce_t*	pCcmNonce = &pd->ccmNonce;

	u8 pktcntr[5] = {0}; //39-bit packet counter + 1bit dir
	memcpy(pktcntr, (u8*)&pd->rxPayloadCnt, 5);
	pCcmNonce->pkt = pd->rxPayloadCnt & 0xffffffff;
	pCcmNonce->dir = (pktcntr[4] & 0x7F) | (pd->cryptBitsInfo.role ? BIT(7) : 0); //dir:bit7, pktcntr's 32~38bits:bit0~bit6

	/*
	 * IgnoreBITS_ACL = 0x1C;     // NESN, SN and MD
	 * IgnoreBITS_CIS = 0x5C;     // NESN, SN, NPI and CIE
	 * IgnoreBITS_BIS = 0x3C;     // CSSN and CSTF
	 */
	if(nonceType == CRYPT_NONCE_TYPE_ACL){
		AAD = pduType & (~0x1c);
		pd->rxPayloadCnt++;//For ACL connection, rxPayloadCnt's value need ++ here
		pd->rxPayloadCnt &= 0x7FFFFFFFFF;
	}
	else if(nonceType == CRYPT_NONCE_TYPE_CIS){
		AAD = pduType & (~0x5c);
		//Notice:Before input parameter, the rxPayloadCnt's value use cisPayloadNum
	}
	else{ //BIS
		AAD = pduType & (~0x3c);
		//Notice:Before input parameter, the rxPayloadCnt's value use cisPayloadNum
	}

	aes_initKey(pd->sk);
	aes_ccmBaseTran(4, NULL, (u8*)pCcmNonce, pPduPkt, rawPduLen, &AAD, 1, pPduPkt + rawPduLen);
    aes_ccmAuthTran(4, NULL, (u8*)pCcmNonce, pPduPkt, rawPduLen, &AAD, 1, (u8*)&chkMIC);
    smemcpy((u8*)&rawMIC, pPduPkt + rawPduLen, 4);

    if(rawMIC == chkMIC){
    	r = 0; //MIC check successfully
    }

    return r;
}

	#if (0)//Test code
		const u8 sk[16] = { 0xaf,0xf7,0x3c,0xb6,0x2f,0xb7,0x06,0xe7,0xd8,0x06,0xfd,0x54,0x2e,0x13,0xb7,0xd6 };
		const u8 iv[8]  = { 0x24,0xab,0xdc,0xba,0xe1,0x77,0xad,0x92 };
																									// mic value: 4byte
		u8 ARaw[] = { 0x02, 0x0B, 0x07, 0x00, 0x04, 0x00, 0x10, 0x0E, 0x00, 0xFF, 0xFF, 0x00, 0x28, 0x00, 0x00, 0x00, 0x00 };
		u8 BRaw[] = { 0x02, 0x0B, 0x07, 0x00, 0x04, 0x00, 0x10, 0x0E, 0x00, 0xFF, 0xFF, 0x00, 0x28, 0x00, 0x00, 0x00, 0x00 };

		ble_crypt_para_t Amy_blc_cyrpt_para;
		leCryptCtrl_t    Bmy_blc_cyrpt_para;

		Amy_blc_cyrpt_para.dec_pno = 65535;
		Amy_blc_cyrpt_para.enc_pno = 65535;
		memcpy(Amy_blc_cyrpt_para.sk, sk, 16);
		memcpy(Amy_blc_cyrpt_para.nonce.iv, iv, 8);
		Amy_blc_cyrpt_para.enable = 1;
		/*
		 * ll_ccm_enc: Master role must use 1, Slave role must use 0;
		 * ll_ccm_dec: Master role must use 0, Slave role must use 1;
		 */
		aes_ll_ccm_encryption(ARaw, 0, &Amy_blc_cyrpt_para);
		aes_ll_ccm_decryption(ARaw, 0, &Amy_blc_cyrpt_para);

		Bmy_blc_cyrpt_para.txPayloadCnt = 65535;
		Bmy_blc_cyrpt_para.rxPayloadCnt = 65535;
		memcpy(Bmy_blc_cyrpt_para.sk, sk, 16);
		memcpy(Bmy_blc_cyrpt_para.ccmNonce.iv, iv, 8);
		Bmy_blc_cyrpt_para.cryptBitsInfo.enEncFlg = 1;
		Bmy_blc_cyrpt_para.cryptBitsInfo.role = 0;
		Bmy_blc_cyrpt_para.cryptBitsInfo.noneType = CRYPT_NONCE_TYPE_ACL;
		Bmy_blc_cyrpt_para.pllPhysChnPdu = (llPhysChnPdu_t*)BRaw;
		aes_ll_ccm_encryption_v2(&Bmy_blc_cyrpt_para);
		aes_ll_ccm_decryption_v2(&Bmy_blc_cyrpt_para);

		if(memcmp(ARaw, BRaw, sizoeof(BRaw))){
			printf("TEST ERR\n");
		}
	#endif
#endif
