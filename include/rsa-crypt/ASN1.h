#ifndef ASN1_H
#define ASN1_H

// Writes a byte stream to a buffer in base64 format
int writeBase64(const char* buffer, size_t bytes, char* string, size_t size);

// Reads a base64 format value into a buffer in bytes
int readBase64(char* buffer, size_t size, const char* value, size_t digits);

// Gets an integer from an ANS1 encoded stream
int getInt(const char* buffer, size_t size, char* &intPtr, size_t &bytes);

// Writes an integer in ANS1 format to a byte array
int putInt(const char* val, size_t bytes, char* buffer, size_t size, bool bigEndian);

#endif