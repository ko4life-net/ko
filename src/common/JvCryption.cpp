#include "StdAfx.h"
#include "JvCryption.h"

CJvCryption::CJvCryption() {
    m_public_key = 0xAFFA12875C643D9B;
    m_private_key = 0x1234567890123456;
}

CJvCryption::~CJvCryption() = default;

void CJvCryption::SetPublicKey(T_KEY pk) {
    m_public_key = pk;
    Init();
}

void CJvCryption::SetPrivateKey(T_KEY pk) {
    m_private_key = pk;
    Init();
}

void CJvCryption::Init() {
    m_tkey = m_public_key ^ m_private_key;
}

void CJvCryption::JvEncryptionFast(int len, T_OCTET * datain, T_OCTET * dataout) {
    if (len <= 0) {
        return;
    }

    int32_t rkey = 0x86D;
    uint8_t lkey = (len * 0x9D) & 0xFF;

    uint8_t * ptkey = (uint8_t *)&m_tkey;
    for (size_t i = 0; i < len; i++) {
        dataout[i] = ((datain[i] ^ ((rkey >> 8) & 0xFF)) ^ ptkey[i % 8]) ^ lkey;
        rkey *= 0x87B;
    }
}

void CJvCryption::JvDecryptionFast(int len, T_OCTET * datain, T_OCTET * dataout) {
    JvEncryptionFast(len, datain, dataout);
}
