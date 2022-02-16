/**
 *  Copyright Notice:
 *  Copyright 2021 DMTF. All rights reserved.
 *  License: BSD 3-Clause License. For full text see link: https://github.com/DMTF/libspdm/blob/main/LICENSE.md
 **/

/** @file
 * HMAC-SHA3_256/384/512 KDF Wrapper Implementation.
 *
 * RFC 5869: HMAC-based Extract-and-Expand key Derivation Function (HKDF)
 **/

#include "internal_crypt_lib.h"

/**
 * Derive SHA3_256 HMAC-based Extract-and-Expand key Derivation Function (HKDF).
 *
 * @param[in]   key              Pointer to the user-supplied key.
 * @param[in]   key_size          key size in bytes.
 * @param[in]   salt             Pointer to the salt(non-secret) value.
 * @param[in]   salt_size         salt size in bytes.
 * @param[in]   info             Pointer to the application specific info.
 * @param[in]   info_size         info size in bytes.
 * @param[out]  out              Pointer to buffer to receive hkdf value.
 * @param[in]   out_size          size of hkdf bytes to generate.
 *
 * @retval true   Hkdf generated successfully.
 * @retval false  Hkdf generation failed.
 *
 **/
bool hkdf_sha3_256_extract_and_expand(const uint8_t *key, uintn key_size,
                                      const uint8_t *salt, uintn salt_size,
                                      const uint8_t *info, uintn info_size,
                                      uint8_t *out, uintn out_size)
{
    return false;
}

/**
 * Derive SHA3_256 HMAC-based Extract key Derivation Function (HKDF).
 *
 * @param[in]   key              Pointer to the user-supplied key.
 * @param[in]   key_size          key size in bytes.
 * @param[in]   salt             Pointer to the salt(non-secret) value.
 * @param[in]   salt_size         salt size in bytes.
 * @param[out]  prk_out           Pointer to buffer to receive hkdf value.
 * @param[in]   prk_out_size       size of hkdf bytes to generate.
 *
 * @retval true   Hkdf generated successfully.
 * @retval false  Hkdf generation failed.
 *
 **/
bool hkdf_sha3_256_extract(const uint8_t *key, uintn key_size,
                           const uint8_t *salt, uintn salt_size,
                           uint8_t *prk_out, uintn prk_out_size)
{
    return false;
}

/**
 * Derive SHA3_256 HMAC-based Expand key Derivation Function (HKDF).
 *
 * @param[in]   prk              Pointer to the user-supplied key.
 * @param[in]   prk_size          key size in bytes.
 * @param[in]   info             Pointer to the application specific info.
 * @param[in]   info_size         info size in bytes.
 * @param[out]  out              Pointer to buffer to receive hkdf value.
 * @param[in]   out_size          size of hkdf bytes to generate.
 *
 * @retval true   Hkdf generated successfully.
 * @retval false  Hkdf generation failed.
 *
 **/
bool hkdf_sha3_256_expand(const uint8_t *prk, uintn prk_size,
                          const uint8_t *info, uintn info_size,
                          uint8_t *out, uintn out_size)
{
    return false;
}

/**
 * Derive SHA3_384 HMAC-based Extract-and-Expand key Derivation Function (HKDF).
 *
 * @param[in]   key              Pointer to the user-supplied key.
 * @param[in]   key_size          key size in bytes.
 * @param[in]   salt             Pointer to the salt(non-secret) value.
 * @param[in]   salt_size         salt size in bytes.
 * @param[in]   info             Pointer to the application specific info.
 * @param[in]   info_size         info size in bytes.
 * @param[out]  out              Pointer to buffer to receive hkdf value.
 * @param[in]   out_size          size of hkdf bytes to generate.
 *
 * @retval true   Hkdf generated successfully.
 * @retval false  Hkdf generation failed.
 *
 **/
bool hkdf_sha3_384_extract_and_expand(const uint8_t *key, uintn key_size,
                                      const uint8_t *salt, uintn salt_size,
                                      const uint8_t *info, uintn info_size,
                                      uint8_t *out, uintn out_size)
{
    return false;
}

/**
 * Derive SHA3_384 HMAC-based Extract key Derivation Function (HKDF).
 *
 * @param[in]   key              Pointer to the user-supplied key.
 * @param[in]   key_size          key size in bytes.
 * @param[in]   salt             Pointer to the salt(non-secret) value.
 * @param[in]   salt_size         salt size in bytes.
 * @param[out]  prk_out           Pointer to buffer to receive hkdf value.
 * @param[in]   prk_out_size       size of hkdf bytes to generate.
 *
 * @retval true   Hkdf generated successfully.
 * @retval false  Hkdf generation failed.
 *
 **/
bool hkdf_sha3_384_extract(const uint8_t *key, uintn key_size,
                           const uint8_t *salt, uintn salt_size,
                           uint8_t *prk_out, uintn prk_out_size)
{
    return false;
}

/**
 * Derive SHA3_384 HMAC-based Expand key Derivation Function (HKDF).
 *
 * @param[in]   prk              Pointer to the user-supplied key.
 * @param[in]   prk_size          key size in bytes.
 * @param[in]   info             Pointer to the application specific info.
 * @param[in]   info_size         info size in bytes.
 * @param[out]  out              Pointer to buffer to receive hkdf value.
 * @param[in]   out_size          size of hkdf bytes to generate.
 *
 * @retval true   Hkdf generated successfully.
 * @retval false  Hkdf generation failed.
 *
 **/
bool hkdf_sha3_384_expand(const uint8_t *prk, uintn prk_size,
                          const uint8_t *info, uintn info_size,
                          uint8_t *out, uintn out_size)
{
    return false;
}

/**
 * Derive SHA3_512 HMAC-based Extract-and-Expand key Derivation Function (HKDF).
 *
 * @param[in]   key              Pointer to the user-supplied key.
 * @param[in]   key_size          key size in bytes.
 * @param[in]   salt             Pointer to the salt(non-secret) value.
 * @param[in]   salt_size         salt size in bytes.
 * @param[in]   info             Pointer to the application specific info.
 * @param[in]   info_size         info size in bytes.
 * @param[out]  out              Pointer to buffer to receive hkdf value.
 * @param[in]   out_size          size of hkdf bytes to generate.
 *
 * @retval true   Hkdf generated successfully.
 * @retval false  Hkdf generation failed.
 *
 **/
bool hkdf_sha3_512_extract_and_expand(const uint8_t *key, uintn key_size,
                                      const uint8_t *salt, uintn salt_size,
                                      const uint8_t *info, uintn info_size,
                                      uint8_t *out, uintn out_size)
{
    return false;
}

/**
 * Derive SHA3_512 HMAC-based Extract key Derivation Function (HKDF).
 *
 * @param[in]   key              Pointer to the user-supplied key.
 * @param[in]   key_size          key size in bytes.
 * @param[in]   salt             Pointer to the salt(non-secret) value.
 * @param[in]   salt_size         salt size in bytes.
 * @param[out]  prk_out           Pointer to buffer to receive hkdf value.
 * @param[in]   prk_out_size       size of hkdf bytes to generate.
 *
 * @retval true   Hkdf generated successfully.
 * @retval false  Hkdf generation failed.
 *
 **/
bool hkdf_sha3_512_extract(const uint8_t *key, uintn key_size,
                           const uint8_t *salt, uintn salt_size,
                           uint8_t *prk_out, uintn prk_out_size)
{
    return false;
}

/**
 * Derive SHA3_512 HMAC-based Expand key Derivation Function (HKDF).
 *
 * @param[in]   prk              Pointer to the user-supplied key.
 * @param[in]   prk_size          key size in bytes.
 * @param[in]   info             Pointer to the application specific info.
 * @param[in]   info_size         info size in bytes.
 * @param[out]  out              Pointer to buffer to receive hkdf value.
 * @param[in]   out_size          size of hkdf bytes to generate.
 *
 * @retval true   Hkdf generated successfully.
 * @retval false  Hkdf generation failed.
 *
 **/
bool hkdf_sha3_512_expand(const uint8_t *prk, uintn prk_size,
                          const uint8_t *info, uintn info_size,
                          uint8_t *out, uintn out_size)
{
    return false;
}
