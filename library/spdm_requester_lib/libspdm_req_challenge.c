/**
    Copyright Notice:
    Copyright 2021 DMTF. All rights reserved.
    License: BSD 3-Clause License. For full text see link: https://github.com/DMTF/libspdm/blob/main/LICENSE.md
**/

#include "internal/libspdm_requester_lib.h"

#pragma pack(1)

typedef struct {
    spdm_message_header_t header;
    uint8_t cert_chain_hash[LIBSPDM_MAX_HASH_SIZE];
    uint8_t nonce[SPDM_NONCE_SIZE];
    uint8_t measurement_summary_hash[LIBSPDM_MAX_HASH_SIZE];
    uint16_t opaque_length;
    uint8_t opaque_data[SPDM_MAX_OPAQUE_DATA_SIZE];
    uint8_t signature[LIBSPDM_MAX_ASYM_KEY_SIZE];
} spdm_challenge_auth_response_max_t;

#pragma pack()

#if LIBSPDM_ENABLE_CAPABILITY_CHAL_CAP

/**
  This function sends CHALLENGE
  to authenticate the device based upon the key in one slot.

  This function verifies the signature in the challenge auth.

  If basic mutual authentication is requested from the responder,
  this function also perform the basic mutual authentication.

  @param  spdm_context                  A pointer to the SPDM context.
  @param  slot_id                      The number of slot for the challenge.
  @param  measurement_hash_type          The type of the measurement hash.
  @param  measurement_hash              A pointer to a destination buffer to store the measurement hash.
  @param  slot_mask                     A pointer to a destination to store the slot mask.
  @param  requester_nonce_in            A buffer to hold the requester nonce (32 bytes) as input, if not NULL.
  @param  requester_nonce               A buffer to hold the requester nonce (32 bytes), if not NULL.
  @param  responder_nonce               A buffer to hold the responder nonce (32 bytes), if not NULL.

  @retval RETURN_SUCCESS               The challenge auth is got successfully.
  @retval RETURN_DEVICE_ERROR          A device error occurs when communicates with the device.
  @retval RETURN_SECURITY_VIOLATION    Any verification fails.
**/
return_status try_spdm_challenge(IN void *context, IN uint8_t slot_id,
                 IN uint8_t measurement_hash_type,
                 OUT void *measurement_hash,
                 OUT uint8_t *slot_mask,
                 IN void *requester_nonce_in OPTIONAL,
                 OUT void *requester_nonce OPTIONAL,
                 OUT void *responder_nonce OPTIONAL)
{
    return_status status;
    boolean result;
    spdm_challenge_request_t spdm_request;
    spdm_challenge_auth_response_max_t spdm_response;
    uintn spdm_response_size;
    uint8_t *ptr;
    void *cert_chain_hash;
    uintn hash_size;
    uintn measurement_summary_hash_size;
    void *nonce;
    void *measurement_summary_hash;
    uint16_t opaque_length;
    void *opaque;
    void *signature;
    uintn signature_size;
    spdm_context_t *spdm_context;
    uint8_t auth_attribute;

    ASSERT((slot_id < SPDM_MAX_SLOT_COUNT) || (slot_id == 0xff));

    spdm_context = context;
    spdm_reset_message_buffer_via_request_code(spdm_context, NULL,
                                    SPDM_CHALLENGE);
    if (!spdm_is_capabilities_flag_supported(
            spdm_context, TRUE, 0,
            SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_CHAL_CAP)) {
        return RETURN_UNSUPPORTED;
    }
    if (spdm_context->connection_info.connection_state <
        LIBSPDM_CONNECTION_STATE_NEGOTIATED) {
        return RETURN_UNSUPPORTED;
    }

    if ((slot_id == 0xFF) &&
        (spdm_context->local_context.peer_cert_chain_provision_size == 0)) {
        return RETURN_INVALID_PARAMETER;
    }

    spdm_context->error_state = LIBSPDM_STATUS_ERROR_DEVICE_NO_CAPABILITIES;

    spdm_request.header.spdm_version = spdm_get_connection_version (spdm_context);
    spdm_request.header.request_response_code = SPDM_CHALLENGE;
    spdm_request.header.param1 = slot_id;
    spdm_request.header.param2 = measurement_hash_type;
    if (requester_nonce_in == NULL) {
        if(!libspdm_get_random_number(SPDM_NONCE_SIZE, spdm_request.nonce)) {
            return RETURN_DEVICE_ERROR;
        }
    } else {
        copy_mem (spdm_request.nonce, requester_nonce_in, SPDM_NONCE_SIZE);
    }
    DEBUG((DEBUG_INFO, "ClientNonce - "));
    internal_dump_data(spdm_request.nonce, SPDM_NONCE_SIZE);
    DEBUG((DEBUG_INFO, "\n"));
    if (requester_nonce != NULL) {
        copy_mem (requester_nonce, spdm_request.nonce, SPDM_NONCE_SIZE);
    }

    status = spdm_send_spdm_request(spdm_context, NULL,
                    sizeof(spdm_request), &spdm_request);
    if (status == RETURN_TIMEOUT) {
        return status;
    } else if (RETURN_ERROR(status)) {
        return RETURN_DEVICE_ERROR;
    }

    spdm_response_size = sizeof(spdm_response);
    zero_mem(&spdm_response, sizeof(spdm_response));
    status = spdm_receive_spdm_response(
        spdm_context, NULL, &spdm_response_size, &spdm_response);
    if (status == RETURN_TIMEOUT) {
        return status;
    } else if (RETURN_ERROR(status)) {
        return RETURN_DEVICE_ERROR;
    }
    if (spdm_response_size < sizeof(spdm_message_header_t)) {
        return RETURN_DEVICE_ERROR;
    }
    if (spdm_response.header.spdm_version != spdm_request.header.spdm_version) {
        return RETURN_DEVICE_ERROR;
    }
    if (spdm_response.header.request_response_code == SPDM_ERROR) {
        status = spdm_handle_error_response_main(
            spdm_context, NULL,
            &spdm_response_size,
            &spdm_response, SPDM_CHALLENGE, SPDM_CHALLENGE_AUTH,
            sizeof(spdm_challenge_auth_response_max_t));
        if (RETURN_ERROR(status)) {
            return status;
        }
    } else if (spdm_response.header.request_response_code !=
           SPDM_CHALLENGE_AUTH) {
        return RETURN_DEVICE_ERROR;
    }
    if (spdm_response_size < sizeof(spdm_challenge_auth_response_t)) {
        return RETURN_DEVICE_ERROR;
    }
    if (spdm_response_size > sizeof(spdm_response)) {
        return RETURN_DEVICE_ERROR;
    }
    auth_attribute = spdm_response.header.param1;
    if (spdm_response.header.spdm_version >= SPDM_MESSAGE_VERSION_11 && slot_id == 0xFF) {
        if ((auth_attribute & SPDM_CHALLENGE_AUTH_RESPONSE_ATTRIBUTE_SLOT_ID_MASK) != 0xF) {
            return RETURN_DEVICE_ERROR;
        }
        if (spdm_response.header.param2 != 0) {
            return RETURN_DEVICE_ERROR;
        }
    } else {
        if ((spdm_response.header.spdm_version >= SPDM_MESSAGE_VERSION_11 && (auth_attribute & SPDM_CHALLENGE_AUTH_RESPONSE_ATTRIBUTE_SLOT_ID_MASK) != slot_id) ||
            (spdm_response.header.spdm_version == SPDM_MESSAGE_VERSION_10 && auth_attribute != slot_id)) {
            return RETURN_DEVICE_ERROR;
        }
        if ((spdm_response.header.param2 & (1 << slot_id)) == 0) {
            return RETURN_DEVICE_ERROR;
        }
    }
    if ((auth_attribute & SPDM_CHALLENGE_AUTH_RESPONSE_ATTRIBUTE_BASIC_MUT_AUTH_REQ) != 0) {
        if (!spdm_is_capabilities_flag_supported(
                spdm_context, TRUE,
                SPDM_GET_CAPABILITIES_REQUEST_FLAGS_MUT_AUTH_CAP,
                SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_MUT_AUTH_CAP)) {
            return RETURN_DEVICE_ERROR;
        }
    }
    hash_size = libspdm_get_hash_size(
        spdm_context->connection_info.algorithm.base_hash_algo);
    signature_size = libspdm_get_asym_signature_size(
        spdm_context->connection_info.algorithm.base_asym_algo);
    measurement_summary_hash_size = spdm_get_measurement_summary_hash_size(
        spdm_context, TRUE, measurement_hash_type);

    if (spdm_response_size <= sizeof(spdm_challenge_auth_response_t) +
                      hash_size + SPDM_NONCE_SIZE +
                      measurement_summary_hash_size +
                      sizeof(uint16_t)) {
        return RETURN_DEVICE_ERROR;
    }

    ptr = spdm_response.cert_chain_hash;

    cert_chain_hash = ptr;
    ptr += hash_size;
    DEBUG((DEBUG_INFO, "cert_chain_hash (0x%x) - ", hash_size));
    internal_dump_data(cert_chain_hash, hash_size);
    DEBUG((DEBUG_INFO, "\n"));
    result = spdm_verify_certificate_chain_hash(spdm_context,
                            cert_chain_hash, hash_size);
    if (!result) {
        spdm_context->error_state =
            LIBSPDM_STATUS_ERROR_CERTIFICATE_FAILURE;
        return RETURN_SECURITY_VIOLATION;
    }

    nonce = ptr;
    DEBUG((DEBUG_INFO, "nonce (0x%x) - ", SPDM_NONCE_SIZE));
    internal_dump_data(nonce, SPDM_NONCE_SIZE);
    DEBUG((DEBUG_INFO, "\n"));
    ptr += SPDM_NONCE_SIZE;
    if (responder_nonce != NULL) {
        copy_mem (responder_nonce, nonce, SPDM_NONCE_SIZE);
    }

    measurement_summary_hash = ptr;
    ptr += measurement_summary_hash_size;
    DEBUG((DEBUG_INFO, "measurement_summary_hash (0x%x) - ",
           measurement_summary_hash_size));
    internal_dump_data(measurement_summary_hash,
               measurement_summary_hash_size);
    DEBUG((DEBUG_INFO, "\n"));

    opaque_length = *(uint16_t *)ptr;
    if (opaque_length > SPDM_MAX_OPAQUE_DATA_SIZE) {
        return RETURN_SECURITY_VIOLATION;
    }
    ptr += sizeof(uint16_t);

    /* Cache data*/

    status = libspdm_append_message_c(spdm_context, &spdm_request,
                       sizeof(spdm_request));
    if (RETURN_ERROR(status)) {
        return RETURN_SECURITY_VIOLATION;
    }
    if (spdm_response_size <
        sizeof(spdm_challenge_auth_response_t) + hash_size +
            SPDM_NONCE_SIZE + measurement_summary_hash_size +
            sizeof(uint16_t) + opaque_length + signature_size) {
        return RETURN_DEVICE_ERROR;
    }
    spdm_response_size = sizeof(spdm_challenge_auth_response_t) +
                 hash_size + SPDM_NONCE_SIZE +
                 measurement_summary_hash_size + sizeof(uint16_t) +
                 opaque_length + signature_size;
    status = libspdm_append_message_c(spdm_context, &spdm_response,
                       spdm_response_size - signature_size);
    if (RETURN_ERROR(status)) {
        libspdm_reset_message_c(spdm_context);
        return RETURN_SECURITY_VIOLATION;
    }

    opaque = ptr;
    ptr += opaque_length;
    DEBUG((DEBUG_INFO, "opaque (0x%x):\n", opaque_length));
    internal_dump_hex(opaque, opaque_length);

    signature = ptr;
    DEBUG((DEBUG_INFO, "signature (0x%x):\n", signature_size));
    internal_dump_hex(signature, signature_size);
    result = spdm_verify_challenge_auth_signature(
        spdm_context, TRUE, signature, signature_size);
    if (!result) {
        libspdm_reset_message_c(spdm_context);
        spdm_context->error_state =
            LIBSPDM_STATUS_ERROR_CERTIFICATE_FAILURE;
        return RETURN_SECURITY_VIOLATION;
    }

    spdm_context->error_state = LIBSPDM_STATUS_SUCCESS;

    if (measurement_hash != NULL) {
        copy_mem(measurement_hash, measurement_summary_hash,
             measurement_summary_hash_size);
    }
    if (slot_mask != NULL) {
        *slot_mask = spdm_response.header.param2;
    }

    if ((auth_attribute & SPDM_CHALLENGE_AUTH_RESPONSE_ATTRIBUTE_BASIC_MUT_AUTH_REQ) != 0) {
        DEBUG((DEBUG_INFO, "BasicMutAuth :\n"));
        status = spdm_encapsulated_request(spdm_context, NULL, 0, NULL);
        DEBUG((DEBUG_INFO,
               "libspdm_challenge - spdm_encapsulated_request - %p\n",
               status));
        if (RETURN_ERROR(status)) {
            libspdm_reset_message_c(spdm_context);
            spdm_context->error_state =
                LIBSPDM_STATUS_ERROR_CERTIFICATE_FAILURE;
            return RETURN_SECURITY_VIOLATION;
        }
    }

    spdm_context->connection_info.connection_state =
        LIBSPDM_CONNECTION_STATE_AUTHENTICATED;

    return RETURN_SUCCESS;
}

/**
  This function sends CHALLENGE
  to authenticate the device based upon the key in one slot.

  This function verifies the signature in the challenge auth.

  If basic mutual authentication is requested from the responder,
  this function also perform the basic mutual authentication.

  @param  spdm_context                  A pointer to the SPDM context.
  @param  slot_id                      The number of slot for the challenge.
  @param  measurement_hash_type          The type of the measurement hash.
  @param  measurement_hash              A pointer to a destination buffer to store the measurement hash.
  @param  slot_mask                     A pointer to a destination to store the slot mask.

  @retval RETURN_SUCCESS               The challenge auth is got successfully.
  @retval RETURN_DEVICE_ERROR          A device error occurs when communicates with the device.
  @retval RETURN_SECURITY_VIOLATION    Any verification fails.
**/
return_status libspdm_challenge(IN void *context, IN uint8_t slot_id,
                 IN uint8_t measurement_hash_type,
                 OUT void *measurement_hash,
                 OUT uint8_t *slot_mask)
{
    spdm_context_t *spdm_context;
    uintn retry;
    return_status status;

    spdm_context = context;
    spdm_context->crypto_request = TRUE;
    retry = spdm_context->retry_times;
    do {
        status = try_spdm_challenge(spdm_context, slot_id,
                        measurement_hash_type,
                        measurement_hash, slot_mask, NULL, NULL, NULL);
        if (RETURN_NO_RESPONSE != status) {
            return status;
        }
    } while (retry-- != 0);

    return status;
}

/**
  This function sends CHALLENGE
  to authenticate the device based upon the key in one slot.

  This function verifies the signature in the challenge auth.

  If basic mutual authentication is requested from the responder,
  this function also perform the basic mutual authentication.

  @param  spdm_context                  A pointer to the SPDM context.
  @param  slot_id                      The number of slot for the challenge.
  @param  measurement_hash_type          The type of the measurement hash.
  @param  measurement_hash              A pointer to a destination buffer to store the measurement hash.
  @param  slot_mask                     A pointer to a destination to store the slot mask.
  @param  requester_nonce_in            A buffer to hold the requester nonce (32 bytes) as input, if not NULL.
  @param  requester_nonce               A buffer to hold the requester nonce (32 bytes), if not NULL.
  @param  responder_nonce               A buffer to hold the responder nonce (32 bytes), if not NULL.

  @retval RETURN_SUCCESS               The challenge auth is got successfully.
  @retval RETURN_DEVICE_ERROR          A device error occurs when communicates with the device.
  @retval RETURN_SECURITY_VIOLATION    Any verification fails.
**/
return_status libspdm_challenge_ex(IN void *context, IN uint8_t slot_id,
                 IN uint8_t measurement_hash_type,
                 OUT void *measurement_hash,
                 OUT uint8_t *slot_mask,
                 IN void *requester_nonce_in OPTIONAL,
                 OUT void *requester_nonce OPTIONAL,
                 OUT void *responder_nonce OPTIONAL)
{
    spdm_context_t *spdm_context;
    uintn retry;
    return_status status;

    spdm_context = context;
    spdm_context->crypto_request = TRUE;
    retry = spdm_context->retry_times;
    do {
        status = try_spdm_challenge(spdm_context, slot_id,
                        measurement_hash_type,
                        measurement_hash,
                        slot_mask,
                        requester_nonce_in,
                        requester_nonce, responder_nonce);
        if (RETURN_NO_RESPONSE != status) {
            return status;
        }
    } while (retry-- != 0);

    return status;
}

#endif /* LIBSPDM_ENABLE_CAPABILITY_CHAL_CAP*/