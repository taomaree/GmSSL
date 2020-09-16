/* ====================================================================
 * Copyright (c) 2014 - 2017 The GmSSL Project.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. All advertising materials mentioning features or use of this
 *    software must display the following acknowledgment:
 *    "This product includes software developed by the GmSSL Project.
 *    (http://gmssl.org/)"
 *
 * 4. The name "GmSSL Project" must not be used to endorse or promote
 *    products derived from this software without prior written
 *    permission. For written permission, please contact
 *    guanzhi1980@gmail.com.
 *
 * 5. Products derived from this software may not be called "GmSSL"
 *    nor may "GmSSL" appear in their names without prior written
 *    permission of the GmSSL Project.
 *
 * 6. Redistributions of any form whatsoever must retain the following
 *    acknowledgment:
 *    "This product includes software developed by the GmSSL Project
 *    (http://gmssl.org/)"
 *
 * THIS SOFTWARE IS PROVIDED BY THE GmSSL PROJECT ``AS IS'' AND ANY
 * EXPRESSED OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE GmSSL PROJECT OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 * ====================================================================
 */
/*
 * Copyright 2015-2016 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the OpenSSL license (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */

/*****************************************************************************
 *                                                                           *
 * The following definitions are PRIVATE to the state machine. They should   *
 * NOT be used outside of the state machine.                                 *
 *                                                                           *
 *****************************************************************************/

/* Max message length definitions */

/* The spec allows for a longer length than this, but we limit it */
#define HELLO_VERIFY_REQUEST_MAX_LENGTH 258
#define SERVER_HELLO_MAX_LENGTH         20000
#define SERVER_KEY_EXCH_MAX_LENGTH      102400
#define SERVER_HELLO_DONE_MAX_LENGTH    0
#define CCS_MAX_LENGTH                  1
/* Max should actually be 36 but we are generous */
#define FINISHED_MAX_LENGTH             64

/* Message processing return codes */
typedef enum {
    /* Something bad happened */
    MSG_PROCESS_ERROR,
    /* We've finished reading - swap to writing */
    MSG_PROCESS_FINISHED_READING,
    /*
     * We've completed the main processing of this message but there is some
     * post processing to be done.
     */
    MSG_PROCESS_CONTINUE_PROCESSING,
    /* We've finished this message - read the next message */
    MSG_PROCESS_CONTINUE_READING
} MSG_PROCESS_RETURN;

/* Flush the write BIO */
int statem_flush(SSL *s);

/*
 * TLS/DTLS client state machine functions
 */
int ossl_statem_client_read_transition(SSL *s, int mt);
WRITE_TRAN ossl_statem_client_write_transition(SSL *s);
WORK_STATE ossl_statem_client_pre_work(SSL *s, WORK_STATE wst);
WORK_STATE ossl_statem_client_post_work(SSL *s, WORK_STATE wst);
int ossl_statem_client_construct_message(SSL *s);
unsigned long ossl_statem_client_max_message_size(SSL *s);
MSG_PROCESS_RETURN ossl_statem_client_process_message(SSL *s, PACKET *pkt);
WORK_STATE ossl_statem_client_post_process_message(SSL *s, WORK_STATE wst);

/*
 * TLS/DTLS server state machine functions
 */
int ossl_statem_server_read_transition(SSL *s, int mt);
WRITE_TRAN ossl_statem_server_write_transition(SSL *s);
WORK_STATE ossl_statem_server_pre_work(SSL *s, WORK_STATE wst);
WORK_STATE ossl_statem_server_post_work(SSL *s, WORK_STATE wst);
int ossl_statem_server_construct_message(SSL *s);
unsigned long ossl_statem_server_max_message_size(SSL *s);
MSG_PROCESS_RETURN ossl_statem_server_process_message(SSL *s, PACKET *pkt);
WORK_STATE ossl_statem_server_post_process_message(SSL *s, WORK_STATE wst);

/* Functions for getting new message data */
__owur int tls_get_message_header(SSL *s, int *mt);
__owur int tls_get_message_body(SSL *s, unsigned long *len);
__owur int dtls_get_message(SSL *s, int *mt, unsigned long *len);

/* Message construction and processing functions */
__owur MSG_PROCESS_RETURN tls_process_change_cipher_spec(SSL *s, PACKET *pkt);
__owur MSG_PROCESS_RETURN tls_process_finished(SSL *s, PACKET *pkt);
__owur int tls_construct_change_cipher_spec(SSL *s);
__owur int dtls_construct_change_cipher_spec(SSL *s);

__owur int tls_construct_finished(SSL *s, const char *sender, int slen);
__owur WORK_STATE tls_finish_handshake(SSL *s, WORK_STATE wst);
__owur WORK_STATE dtls_wait_for_dry(SSL *s);

/* some client-only functions */
__owur int tls_construct_client_hello(SSL *s);
__owur MSG_PROCESS_RETURN tls_process_server_hello(SSL *s, PACKET *pkt);
__owur MSG_PROCESS_RETURN tls_process_certificate_request(SSL *s, PACKET *pkt);
__owur MSG_PROCESS_RETURN tls_process_new_session_ticket(SSL *s, PACKET *pkt);
__owur MSG_PROCESS_RETURN tls_process_cert_status(SSL *s, PACKET *pkt);
__owur MSG_PROCESS_RETURN tls_process_server_done(SSL *s, PACKET *pkt);
__owur int tls_construct_client_verify(SSL *s);
__owur WORK_STATE tls_prepare_client_certificate(SSL *s, WORK_STATE wst);
__owur int tls_construct_client_certificate(SSL *s);
__owur int ssl_do_client_cert_cb(SSL *s, X509 **px509, EVP_PKEY **ppkey);
__owur int tls_construct_client_key_exchange(SSL *s);
__owur int tls_client_key_exchange_post_work(SSL *s);
__owur int tls_construct_cert_status(SSL *s);
__owur MSG_PROCESS_RETURN tls_process_server_key_exchange(SSL *s, PACKET *pkt);
__owur MSG_PROCESS_RETURN tls_process_server_certificate(SSL *s, PACKET *pkt);
__owur int ssl3_check_cert_and_algorithm(SSL *s);
#ifndef OPENSSL_NO_NEXTPROTONEG
__owur int tls_construct_next_proto(SSL *s);
#endif
__owur MSG_PROCESS_RETURN dtls_process_hello_verify(SSL *s, PACKET *pkt);

/* some server-only functions */
__owur MSG_PROCESS_RETURN tls_process_client_hello(SSL *s, PACKET *pkt);
__owur WORK_STATE tls_post_process_client_hello(SSL *s, WORK_STATE wst);
__owur int tls_construct_server_hello(SSL *s);
__owur int tls_construct_hello_request(SSL *s);
__owur int dtls_construct_hello_verify_request(SSL *s);
__owur int tls_construct_server_certificate(SSL *s);
__owur int tls_construct_server_key_exchange(SSL *s);
__owur int tls_construct_certificate_request(SSL *s);
__owur int tls_construct_server_done(SSL *s);
__owur MSG_PROCESS_RETURN tls_process_client_certificate(SSL *s, PACKET *pkt);
__owur MSG_PROCESS_RETURN tls_process_client_key_exchange(SSL *s, PACKET *pkt);
__owur WORK_STATE tls_post_process_client_key_exchange(SSL *s, WORK_STATE wst);
__owur MSG_PROCESS_RETURN tls_process_cert_verify(SSL *s, PACKET *pkt);
__owur MSG_PROCESS_RETURN tls_process_cert_verify_no_s(SSL *s, PACKET *pkt);

#ifndef OPENSSL_NO_NEXTPROTONEG
__owur MSG_PROCESS_RETURN tls_process_next_proto(SSL *s, PACKET *pkt);
#endif
__owur int tls_construct_new_session_ticket(SSL *s);

#ifndef OPENSSL_NO_GMTLS_METHOD
__owur int gmtls_construct_server_certificate(SSL *s);
__owur int gmtls_construct_server_key_exchange(SSL *s);
__owur int gmtls_construct_client_certificate(SSL *s);
__owur int gmtls_construct_client_key_exchange(SSL *s);
__owur MSG_PROCESS_RETURN gmtls_process_server_certificate(SSL *s, PACKET *pkt);
__owur MSG_PROCESS_RETURN gmtls_process_server_key_exchange(SSL *s, PACKET *pkt);
__owur MSG_PROCESS_RETURN gmtls_process_client_certificate(SSL *s, PACKET *pkt);
__owur MSG_PROCESS_RETURN gmtls_process_client_key_exchange(SSL *s, PACKET *pkt);
#endif
