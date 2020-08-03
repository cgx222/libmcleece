#pragma once

#include "base64/base.hpp"
#include "mceliece348864/crypto_kem.h"
#include <string>
#include <vector>

namespace mcleece {


// this class currently serves two distinct purposes right now, which is interesting
// maybe a protected subclass with the "more public" methods? (and hiding the data accessors?)
class session_key
{
public:
	session_key()
	    : _key(crypto_kem_BYTES)
	    , _encryptedKey(crypto_kem_CIPHERTEXTBYTES)
	    , _needsDecrypt(false)
	{
	}

	session_key(const std::string& encoded)
	    : session_key()
	{
		init_decode(encoded);
		_needsDecrypt = true;
	}

	bool init_decode(const std::string& encoded)
	{
		std::string out = base64::decode(encoded);
		if (out.size() != _encryptedKey.size())
			return false;

		_encryptedKey.assign(out.begin(), out.end());
		return true;
	}

	bool needs_decrypt() const
	{
		return _needsDecrypt;
	}

	unsigned char* key_data()
	{
		return _key.data();
	}

	unsigned char* encrypted_key_data()
	{
		return _encryptedKey.data();
	}

	const std::vector<unsigned char>& key() const
	{
		return _key;
	}

	std::string encrypted_key() const
	{
		std::string temp(_encryptedKey.begin(), _encryptedKey.end());
		return base64::encode(temp);
	}

protected:
	bool _needsDecrypt;
	std::vector<unsigned char> _key;
	std::vector<unsigned char> _encryptedKey;
};

}