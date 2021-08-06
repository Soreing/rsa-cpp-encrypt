#include <rsa-crypt/RSAcipher.h>
#include <rsa-crypt/ANS1.h>
#include <ctime>


void main()
{
	srand(time(NULL));

	RSACipher rsa;
	
	/*rsa.importPubKey(
		"-----BEGIN RSA PUBLIC KEY-----\n"
		"MIIBCgKCAQEAqDF6gygTSc4LnLGwCBDvF5hAjKjKP1XVl4jmd4mRzH93CElC"
		"VPd4kXWR91n/aTJME5XmCak1pY17TtxiaWQQDjqhVOLL+pHR7SSI9vNK8uRi"
		"hRhdDmfaD/2z0XTpXBGEMQhpZMS6x4pZV8oBGmGlzJKEvVMhuMB0iKB75mhC"
		"XiiLT/omLzJ3uASIznLzhltkFiriBp38Zik3JuyokYusdMD7Giui4DkCVBwL"
		"rJER+o/QXgDmTYaif/hdMsQhXMRIzHD1wOHy9K5CnrI2stHSSS8Qil91jdHJ"
		"fOEfa4TsR9pna3EIuygOlso0QgghdxALCJhECwpVHqnRnWxR7dcERQIDAQAB"
		"\n-----END RSA PUBLIC KEY-----"
	);*/

	rsa.importPrvKey(
		"-----BEGIN RSA PRIVATE KEY-----\n"
		"MIIEpQIBAAKCAQEAqDF6gygTSc4LnLGwCBDvF5hAjKjKP1XVl4jmd4mRzH93"
		"CElCVPd4kXWR91n/aTJME5XmCak1pY17TtxiaWQQDjqhVOLL+pHR7SSI9vNK"
		"8uRihRhdDmfaD/2z0XTpXBGEMQhpZMS6x4pZV8oBGmGlzJKEvVMhuMB0iKB7"
		"5mhCXiiLT/omLzJ3uASIznLzhltkFiriBp38Zik3JuyokYusdMD7Giui4DkC"
		"VBwLrJER+o/QXgDmTYaif/hdMsQhXMRIzHD1wOHy9K5CnrI2stHSSS8Qil91"
		"jdHJfOEfa4TsR9pna3EIuygOlso0QgghdxALCJhECwpVHqnRnWxR7dcERQID"
		"AQABAoIBAFhm2SvCjpDGOZg8VHeI3wIZxiOoO9UaOk+RZwEBT56aiGfUiYa7"
		"I6Xgk73ckcAq7Y2Fxgjh5Q6E1IDYmOkn7MBt1IoXQnsK1bm88DhMG+KaY6Xs"
		"7kw6DYVGJZM7zcPUeRSEhVCvjjR+5QKdbAL/u/NQN3CDF0BR924wW674sZlm"
		"/MSUzKUShqSAWgV5Gby2GLQ5fke6jhHI1KEZWZYcC+QMFLBMG+A5ZQdrHSVp"
		"EYSwsPEr01AID+OxIsktH/SS+U1p4NOQLUj6ssWSm3qur5QjDC/7Wi7CTmtT"
		"cXa7x/Q1dUeJ5fqzhvjV2fBH+fPK+xWczKQ2tmKNIy/M+RMd+OkCgYEA9aMJ"
		"1/0ANRqLPdhKi9zZaTaYCS7gJ5DZAi+cV4mGS5hfsNgvOZUDKqKCxTNHYTPm"
		"hDqxcy3rWnagJq0UCap7WfbED88eCJ5hbUrwclLhTjdKHq1crwkb1bxdaAPj"
		"xRgJU1eHb8JPXPeCDzG+wlWDliK91qF8eSsv8E2m2zDQf1MCgYEAr0oF5Lzx"
		"Df1O+LigxzkjZ9pH1x/TMTWjonQj9S1B9qDV76FNSMEdoojbGmI7gObANJCd"
		"BUBAkWQzG/kbNTffzUi6Rh4cjpqOFmJbmZ3kCw6mO1evgcmDCOQhGpM3RdYL"
		"ICPLDlFR2fmPBlL/BVrqicrB3tSuw2bHSwBeFEtiMwcCgYEAy2ZiNFPv5AlC"
		"28dKk14hOZpXcuM6AglTaRf7ohDi/54BPvMEx+24XYmMN7cqmDbd3wiq//E6"
		"PgKUwvH28boVwn0wBNdAy2o6ysGPMVun7Zii65wqb1ZfkjEUN5lg9kfdEQId"
		"yFFiXl0leWd7ynf9KklCYYP/CmbUn2vtivt06rkCgYEAj1D3VpiAgk6cVdEm"
		"axy1bQDUAfhcRPy1KGrKx3fUWeAQapc4SAbUwwIgGgc83VxMw+2E5DkvUXBa"
		"NUHL1Aw16HLREekmshV9nKGYgRXN0JiOozBDJ9tpTkivJDo/U9L4T7PUHoWw"
		"ZYacQLB1H5zA7B4hup2rscmDojfPs8oapNsCgYEA7EY7Lb/Pkq8017sNmiy1"
		"Q3Vrbg3vfY6MrZzbxkdARzrticUVEJ7vDEsDFtbx0axmplhzivvhoOFzAN7w"
		"gduQYuCcPxixbjWl9zCmyIGtyaYOrwTQW1p4zDidyVInZ5DsWZnYWdjflJCa"
		"UUE9Ou6MwYIZpyQ6sqISdMbKLovrxqU="
		"\n-----END RSA PRIVATE KEY-----"
	);

	char bf1[2048];
	//char bf2[2048];
	rsa.exportPrvKey(bf1, 2048);
	//rsa.exportPrvKey(bf2, 2048);
	std::cout<< bf1 << "\n";
	//std::cout<< bf2 << "\n";
	
	BigInt<256> message("123456");
	BigInt<256> ciphertext = rsa.encrypt(message);
	BigInt<256> plaintext  = rsa.decrypt(ciphertext);

	std::cout<< message.toString(64) <<"\n";
	//std::cout<< ciphertext.toString(64) <<"\n";
	std::cout<< plaintext.toString(64) <<"\n";

}