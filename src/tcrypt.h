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
#ifndef TCRYPT_H
#define TCRYPT_H

#include <QString>

class TCrypt
{
    public:
        TCrypt();
        ~TCrypt();

        QString doEncrypt(QString plainText, QString pass);
        QString doDecrypt(QString cipher, QString pass);

        QString getPassword();
        QString generatePassword(const QString& input, const QString& key, int groups=6);

    protected:
        int encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key, unsigned char *iv, unsigned char *ciphertext);
        int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key, unsigned char *iv, unsigned char *plaintext);
        void handleErrors(int err);
        void hashPassword(const QString& pass, char outputBuffer[65]);

        bool hmac_sha256(const std::string& key, const unsigned char* data, size_t len, unsigned char out[32]);
        bool prf_expand(const std::string& key, const std::string& input, std::vector<unsigned char>& out);
        QString proquint_encode(const std::vector<unsigned char>& bytes, int groups);

    private:
        const char *CONS{"bdfghjklmnprstvz"};
        const char *VOWS{"aiou"};
};

#endif // TCRYPT_H
