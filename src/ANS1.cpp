#include <rsa-crypt/ANS1.h>

static const char b64[64] =
{	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
	'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
	'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
	'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
	'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
	'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
	'w', 'x', 'y', 'z', '0', '1', '2', '3',
	'4', '5', '6', '7', '8', '9', '+', '/' };

static const short v64[256] =
{	-1,  1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1, -1, 63,
	52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -1, -1, -1,
	-1,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
	15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1,
	-1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
	41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1,
	-1,  1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1,  1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1,  1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1,  1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1,  1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1,  1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1,  1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1,  1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
};

// Writes a byte stream to a buffer in base64 format
char* writeBase64(const char* buffer, size_t bytes, char* string, size_t size)
{
	int bitCount = 0;
	int bitBox   = 0;
	int shift    = 0;
	size_t r     = 0;

	for (size_t i = 0; i < bytes; i++)
	{	
		bitBox <<= 8;
		bitBox |= buffer[i];
		shift += 2;

		string[r++] = b64[(bitBox >> shift) & 0x3F];

		if (shift == 6)
		{	string[r++] = b64[bitBox & 0x3F];
			shift = 0;
		}
	}

	if (shift == 2)
	{	string[r] = b64[(bitBox << 4) & 0x3F];
		string[r+1] = string[r+2]= '=';
	}
	if (shift == 4)
	{	string[r] = b64[(bitBox << 2) & 0x3F];
		string[r+1] = '=';
	}

	string[(bytes+2)/3*4] = 0;
	return string;
}

// Reads a base64 format value into a buffer in bytes
char* readBase64(char* buffer, size_t size, const char* value, size_t digits)
{
	if (size < (digits * 4 / 3))
		return buffer;

	for (size_t i = 0, r = 0; i < digits; i += 4, r += 3)
	{	buffer[r + 0] = (v64[value[i + 0]] << 2) | (v64[value[i + 1]] >> 4);
		buffer[r + 1] = (v64[value[i + 1]] << 4) | (v64[value[i + 2]] >> 2);
		buffer[r + 2] = (v64[value[i + 2]] << 6) | (v64[value[i + 3]] >> 0);
	}

	return buffer;
}

// Gets an integer from an ANS1 encoded stream
int getInt(const char* buffer, size_t size, char* &intPtr, size_t &bytes)
{
    bytes = 0;
    intPtr = (char*)buffer;
    
    size_t intIndex = 0;
    size_t dataBytes = 0;
    char sizeBytes = 0;
    
    if(buffer[0] == 2)
    {   char sizeBytes = buffer[1];

        if(sizeBytes < 0)
        {   sizeBytes &= 0x7F;
            for(intIndex=2; sizeBytes>0; intIndex++, sizeBytes--)
            {   dataBytes <<= 8;
                dataBytes |= (unsigned char)buffer[intIndex];
            }
        }
        else
        {   dataBytes = sizeBytes;
            intIndex = 2;
        }

        intPtr = (char*)buffer+intIndex;
        bytes = dataBytes;
    }

    return bytes + intIndex;
}

// Writes an integer in ANS1 format to a byte array
int putInt(const char* val, size_t bytes, char* buffer, size_t size, bool bigEndian)
{
    buffer[0] = 2;
    int intIdx = 0;

    if(bytes < 128)
    {   buffer[1] = (char)bytes;
        intIdx = 2;
    }
    else
    {   size_t bytesCpy = bytes;
        int sizeCount = 0;

        while(bytesCpy > 0)
        {
            *(long long*)(buffer+2) <<= 8;
            buffer[2] = bytesCpy & 0xFF;
            bytesCpy >>= 8;
            sizeCount++;
        }

        buffer[1] = sizeCount | 0x80;
        intIdx = 2 + sizeCount;
    }

    if(!bigEndian)
    {   for(int d=intIdx, s=bytes-1; s>=0; s--, d++)
        {   buffer[d]= val[s];
        }
    }
    else
    {   for(int i=0; i<(int)bytes; i++)
        {   buffer[i+intIdx] = val[i];
        }
    }

    return bytes + intIdx;
    
}