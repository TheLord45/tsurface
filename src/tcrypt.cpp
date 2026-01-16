/*
 * Copyright (C) 2026 by Andreas Theofilu <andreas@theosys.at>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
 */
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/sha.h>
#include <openssl/hmac.h>

#include <string>
#include <vector>
#include <cstring>
#include <stdlib.h>
#include <iostream>

#include "tcrypt.h"
#include "base64.h"
#include "terror.h"

using std::string;
using std::vector;
using std::endl;

static unsigned long ssl_errno;

TCrypt::TCrypt()
{
    DECL_TRACER("TCrypt::TCrypt()");
}

TCrypt::~TCrypt()
{
    DECL_TRACER("TCrypt::~TCrypt()");
}

int TCrypt::encrypt(unsigned char *plaintext,
            int plaintext_len,
            unsigned char *key,
            unsigned char *iv,
            unsigned char *ciphertext)
{
    DECL_TRACER("TCrypt::encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key, unsigned char *iv, unsigned char *ciphertext)");

    EVP_CIPHER_CTX *ctx;
    int len, err;
    int ciphertext_len;

    // Create and initialise the context
    if(!(ctx = EVP_CIPHER_CTX_new()))
    {
        handleErrors(0);
        return -1;
    }

    /*
     * Initialise the encryption operation. IMPORTANT - ensure you use a key
     * and IV size appropriate for your cipher
     * In this example we are using 256 bit AES (i.e. a 256 bit key). The
     * IV size for *most* modes is the same as the block size. For AES this
     * is 128 bits
     */
    if((err = EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv)) != 1)
        handleErrors(err);

    /*
     * Provide the message to be encrypted, and obtain the encrypted output.
     * EVP_EncryptUpdate can be called multiple times if necessary
     */
    if((err = EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len)) != 1)
    {
        handleErrors(err);
        return -1;
    }

    ciphertext_len = len;
    /*
     * Finalise the encryption. Further ciphertext bytes may be written at
     * this stage.
     */
    if((err = EVP_EncryptFinal_ex(ctx, ciphertext + len, &len)) != 1)
    {
        handleErrors(err);
        return -1;
    }

    ciphertext_len += len;
    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    return ciphertext_len;
}

int  TCrypt::decrypt(unsigned char *ciphertext,
            int ciphertext_len,
            unsigned char *key,
            unsigned char *iv,
            unsigned char *plaintext)
{
    DECL_TRACER("TCrypt::decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key, unsigned char *iv, unsigned char *plaintext)");

    EVP_CIPHER_CTX *ctx;

    int len;
    int err;
    int plaintext_len;

    /* Create and initialise the context */
    if(!(ctx = EVP_CIPHER_CTX_new()))
    {
        handleErrors(0);
        return -1;
    }

    /*
     * Initialise the decryption operation. IMPORTANT - ensure you use a key
     * and IV size appropriate for your cipher
     * In this example we are using 256 bit AES (i.e. a 256 bit key). The
     * IV size for *most* modes is the same as the block size. For AES this
     * is 128 bits
     */
    if((err = EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv)) != 1)
    {
        handleErrors(err);
        return -1;
    }

    /*
     * Provide the message to be decrypted, and obtain the plaintext output.
     * EVP_DecryptUpdate can be called multiple times if necessary.
     */
    if((err = EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len)) != 1)
    {
        handleErrors(err);
        return -1;
    }

    plaintext_len = len;
    /*
     * Finalise the decryption. Further plaintext bytes may be written at
     * this stage.
     */
    if((err = EVP_DecryptFinal_ex(ctx, plaintext + len, &len)) != 1)
    {
        handleErrors(err);
        return -1;
    }

    plaintext_len += len;
    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    return plaintext_len;
}

void TCrypt::handleErrors(int err)
{
    DECL_TRACER("TCrypt::handleErrors(int err)");

    char msg[1024];

    if (err == 0)
    {
        ERR_error_string_n(ssl_errno, msg, sizeof(msg));
        MSG_ERROR("Crypt error: " << msg);
        return;
    }

    ERR_error_string_n(err, msg, sizeof(msg));
    MSG_ERROR(msg);
}

void TCrypt::hashPassword(const QString& string, char outputBuffer[65])
{
    DECL_TRACER("TCrypt::hashPassword(const char *string, char outputBuffer[65])");

#if OPENSSL_VERSION_NUMBER < 0x10100000L
#  define EVP_MD_CTX_new   EVP_MD_CTX_create
#  define EVP_MD_CTX_free  EVP_MD_CTX_destroy
#endif

    EVP_MD_CTX *ctx = EVP_MD_CTX_new();
    unsigned int hashSize = 65;
    unsigned char *tempOutBuff = new unsigned char[hashSize];

    EVP_DigestInit(ctx, EVP_sha256());
    EVP_DigestUpdate(ctx, string.data(), string.length());
    EVP_DigestFinal(ctx, tempOutBuff, &hashSize);

    for(int i = 0 ; i < hashSize ; i++)
        sprintf(outputBuffer + (i * 2), "%02x", tempOutBuff[i]);

    delete[] tempOutBuff;
    outputBuffer[64] = 0;
}

QString TCrypt::doEncrypt(QString plainText, QString pass)
{
    DECL_TRACER("TCrypt::doEncrypt(QString plainText, QString pass)");

    if (plainText.isEmpty() || pass.isEmpty())
        return QString();

    // Converting string to unsigned character (bytes) array
    unsigned char *plainTextBytes = new unsigned char[plainText.length()+1];
    std::memset(plainTextBytes, 0, plainText.length()+1);
    std::memcpy(plainTextBytes, plainText.toStdString().c_str(), plainText.length());

    // Allocate memory for hash bytes (chars) array with 65 elements
    char *hash = (char *) malloc(sizeof(char) * 65);

    // Allocate memory for cipher bytes (chars) array with 1024 elements
    unsigned char *cipher = new unsigned char[1024];

    // Here we hash the plain text password
    hashPassword((const char *)pass.toStdString().c_str(), hash);

    // We convert the char array (hash variable) into an std::string
    string hashStr(hash);
    // The hash contains the encrytion key and the initialization vector -> (iv)
    // so we divide it between the key and the iv using std::string method substr()
    string keyStr = hashStr.substr(0, 16);
    string iv = hashStr.substr(16, 16);
    // here we initiate the encryption process
    int cipherSize = encrypt(
        plainTextBytes,
        strlen((char *) plainTextBytes),
        (unsigned char *) keyStr.c_str(),
        (unsigned char *) iv.c_str(),
        cipher
        );

    // We encode the cipher into a base64 encoded std::string
    // string base64Cipher = base64_encode(
    //     cipher,
    //     cipherSize);

    unsigned char base64Cipher[1024];
    EVP_EncodeBlock(base64Cipher, cipher, cipherSize);

    // We encode the iv to the base64
    string base64Iv = Base64::encode((const BYTE *)(iv.c_str()), iv.length());

    // Append the base64 encode iv to the end of the base64 encoded cipher with a `:` in between
    // string cipherStr((char *) base64Cipher.c_str());
    string cipherStr((char *) base64Cipher);

    delete[] plainTextBytes;
    delete[] cipher;
    return QString::fromStdString(cipherStr);
}

QString TCrypt::doDecrypt(QString cipher, QString pass)
{
    DECL_TRACER("TCrypt::doDecrypt(QString cipher, QString pass)");

    if (cipher.isEmpty() || pass.isEmpty())
        return QString();

    std::vector<BYTE> cipherDecoded = Base64::decode(cipher.toStdString());
    unsigned char * cipherBytes = (unsigned char *) cipherDecoded.data();

//    unsigned char *cipherBytes = (unsigned char *) malloc(sizeof(char) * 1024);
//    EVP_DecodeBlock(cipherBytes, (unsigned char *) cipher.toStdString().c_str(), cipher.length());

    // Converting string to unsigned character (bytes) array
    unsigned char *plainTextBytes = new unsigned char[1024];

    // Allocate memory for hash bytes (chars) array with 65 elements
    char *hash = new char[65];

    // Here we hash the plain text password
    hashPassword((const char *)pass.toStdString().c_str(), hash);

    // We convert the char array (hash variable) into an std::string
    string hashStr(hash);

    // The hash contains the encrytion key and the initialization vector -> (iv)
    // so we divide it between the key and the iv using std::string method substr()
    string keyStr = hashStr.substr(0, 16);
    string iv = hashStr.substr(16, 16);

    int len = decrypt(
                cipherBytes,
                cipherDecoded.size(),
                (unsigned char *) keyStr.c_str(),
                (unsigned char *) iv.c_str(),
                plainTextBytes);

    if (len <= 0)
        return QString();

    string plainText((char *) plainTextBytes, len);
    delete[] plainTextBytes;
    delete[] hash;
    MSG_DEBUG("Plain password: " << plainText);
    return QString::fromStdString(plainText);
}

QString TCrypt::generatePassword(const QString& input, const QString& key, int groups)
{
    DECL_TRACER("TCrypt::generatePassword(const QString& input, const QString& key, int groups)");

    std::vector<unsigned char> bytes(static_cast<size_t>(groups) * 2);

    if (!prf_expand(key.toStdString(), input.toStdString(), bytes))
    {
        MSG_ERROR("PRF expansion failed");
        return QString();
    }

    QString pass = proquint_encode(bytes, groups);
    MSG_DEBUG("Encryption word: " << pass.toStdString());
    return pass;
}

QString TCrypt::getPassword()
{
    DECL_TRACER("TCrypt::getPassword()");

    return generatePassword("Andreas", "Theofilu", 6);
}

bool TCrypt::hmac_sha256(const string& key, const unsigned char* data, size_t len, unsigned char out[32])
{
    DECL_TRACER("TCrypt::hmac_sha256(const string& key, const unsigned char* data, size_t len, unsigned char out[32])");

    unsigned int outlen = 0;
    unsigned char* p = HMAC(EVP_sha256(),
                            key.data(), static_cast<int>(key.size()),
                            data, len,
                            out, &outlen);
    return p != nullptr && outlen == 32;
}

// Expand to out_len bytes using HMAC(key, input || counter)
bool TCrypt::prf_expand(const string& key, const string& input, vector<unsigned char>& out)
{
    DECL_TRACER("TCrypt::prf_expand(const string& key, const string& input, vector<unsigned char>& out)");

    size_t need = out.size();
    size_t produced = 0;
    unsigned int counter = 0;

    while (produced < need)
    {
        // buffer = input || 0x00000000..counter (big-endian)
        std::string buf;
        buf.reserve(input.size() + 4);
        buf.append(input);
        unsigned char ctr[4] = {
            static_cast<unsigned char>((counter >> 24) & 0xFF),
            static_cast<unsigned char>((counter >> 16) & 0xFF),
            static_cast<unsigned char>((counter >> 8) & 0xFF),
            static_cast<unsigned char>(counter & 0xFF)
        };

        buf.append(reinterpret_cast<const char*>(ctr), 4);
        unsigned char block[32];

        if (!hmac_sha256(key, reinterpret_cast<const unsigned char*>(buf.data()), buf.size(), block))
            return false;

        size_t take = std::min(need - produced, sizeof(block));
        std::memcpy(out.data() + produced, block, take);
        produced += take;
        counter++;
    }

    return true;
}

QString TCrypt::proquint_encode(const vector<unsigned char>& bytes, int groups)
{
    DECL_TRACER("proquint_encode(const vector<unsigned char>& bytes, int groups)");

    QString out;
    out.reserve(groups * (5 + 1));

    for (int i = 0; i < groups; ++i)
    {
        uint16_t x = (static_cast<uint16_t>(bytes[2*i]) << 8) | bytes[2*i + 1];
        char block[6];
        block[0] = CONS[(x >> 12) & 0xF];
        block[1] = VOWS[(x >> 10) & 0x3];
        block[2] = CONS[(x >> 6)  & 0xF];
        block[3] = VOWS[(x >> 4)  & 0x3];
        block[4] = CONS[x & 0xF];
        block[5] = '\0';

        if (i)
            out.push_back('-');

        out.append(block);
    }

    return out;
}
