# Module <!-- group --> `crypto`



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`namespace `[`scy::crypto`](#namespacescy_1_1crypto)    | 
`struct `[`CRYPTO_dynlock_value`](#structCRYPTO__dynlock__value)    | 
# namespace `scy::crypto` {#namespacescy_1_1crypto}



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`class `[`scy::crypto::Cipher`](#classscy_1_1crypto_1_1Cipher)    | 
`class `[`scy::crypto::Hash`](#classscy_1_1crypto_1_1Hash)    | 
`class `[`scy::crypto::X509Certificate`](#classscy_1_1crypto_1_1X509Certificate)    | This class represents a X509 Certificate.
# class `scy::crypto::Cipher` {#classscy_1_1crypto_1_1Cipher}




Provides symmetric algorithms for encryption and decryption. The algorithms that are available depend on the particular version of OpenSSL that is installed.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  Cipher(const std::string & name)` | 
`public  Cipher(const std::string & name,const `[`ByteVec`](#group__crypto_1ga54c835992b731d8c7ec3e6a3c991ec97)` & key,const `[`ByteVec`](#group__crypto_1ga54c835992b731d8c7ec3e6a3c991ec97)` & iv)` | 
`public  Cipher(const std::string & name,const std::string & passphrase,const std::string & salt,int iterationCount)` | 
`public  ~Cipher()` | Destroys the [Cipher](./doc/api-crypto.md#classscy_1_1crypto_1_1Cipher).
`public void initEncryptor()` | Initializes the [Cipher](./doc/api-crypto.md#classscy_1_1crypto_1_1Cipher) for encryption.
`public void initDecryptor()` | Initializes the [Cipher](./doc/api-crypto.md#classscy_1_1crypto_1_1Cipher) for decryption.
`public int update(const unsigned char * input,int inputLength,unsigned char * output,int outputLength)` | 
`public template<typename I,typename O>`  <br/>`inline int update(const I & input,O & output)` | Alias for [update()](#group__crypto_1ga641b7553ec5f282a77d89def7ae54f12) which accepts a range of buffer types.
`public int final(unsigned char * output,int length)` | 
`public template<typename O>`  <br/>`inline int final(O & output)` | Alias for [final()](#group__crypto_1gadfe313d58ac2e4fb224d596a2f03862f) which accepts a range of buffer types.
`public int encrypt(const unsigned char * inbuf,std::size_t inlen,unsigned char * outbuf,std::size_t outlen,`[`Encoding`](#group__crypto_1ga44fda7872beb30a68bbc8f2cac9271ae)` encoding)` | 
`public template<typename I,typename O>`  <br/>`inline int encrypt(const I & input,O & output,`[`Encoding`](#group__crypto_1ga44fda7872beb30a68bbc8f2cac9271ae)` encoding)` | Alias for [encrypt()](#group__crypto_1ga9dcc959e7adb68f9d3201162e4566493) which accepts a range of buffer types.
`public virtual std::string encryptString(const std::string & str,`[`Encoding`](#group__crypto_1ga44fda7872beb30a68bbc8f2cac9271ae)` encoding)` | Encrypts a string and encodes it using the given encoding.
`public virtual std::string decryptString(const std::string & str,`[`Encoding`](#group__crypto_1ga44fda7872beb30a68bbc8f2cac9271ae)` encoding)` | Decrypts a string that is encoded with the given encoding.
`public virtual void encryptStream(std::istream & source,std::ostream & sink,`[`Encoding`](#group__crypto_1ga44fda7872beb30a68bbc8f2cac9271ae)` encoding)` | Encrypts an input stream and encodes it using the given encoding.
`public virtual void decryptStream(std::istream & source,std::ostream & sink,`[`Encoding`](#group__crypto_1ga44fda7872beb30a68bbc8f2cac9271ae)` encoding)` | Decrypts an input stream that is encoded with the given encoding.
`public template<typename T>`  <br/>`inline void setKey(const T & key)` | Sets the key for the [Cipher](./doc/api-crypto.md#classscy_1_1crypto_1_1Cipher).
`public template<typename T>`  <br/>`inline void setIV(const T & iv)` | Sets the initialization vector (IV) for the [Cipher](./doc/api-crypto.md#classscy_1_1crypto_1_1Cipher).
`public int setPadding(int padding)` | 
`public const `[`ByteVec`](#group__crypto_1ga54c835992b731d8c7ec3e6a3c991ec97)` & getKey() const` | Returns the key for the [Cipher](./doc/api-crypto.md#classscy_1_1crypto_1_1Cipher).
`public const `[`ByteVec`](#group__crypto_1ga54c835992b731d8c7ec3e6a3c991ec97)` & getIV() const` | Returns the initialization vector (IV) for the [Cipher](./doc/api-crypto.md#classscy_1_1crypto_1_1Cipher).
`public const std::string & name() const` | Returns the name of the [Cipher](./doc/api-crypto.md#classscy_1_1crypto_1_1Cipher).
`public int blockSize() const` | Returns the block size of the [Cipher](./doc/api-crypto.md#classscy_1_1crypto_1_1Cipher).
`public int keySize() const` | Returns the key size of the [Cipher](./doc/api-crypto.md#classscy_1_1crypto_1_1Cipher).
`public int ivSize() const` | Returns the IV size of the [Cipher](./doc/api-crypto.md#classscy_1_1crypto_1_1Cipher).
`public const EVP_CIPHER * cipher()` | Returns the cipher object.
`protected bool _initialized` | 
`protected bool _encrypt` | 
`protected const EVP_CIPHER * _cipher` | 
`protected std::string _name` | 
`protected EVP_CIPHER_CTX _ctx` | 
`protected `[`ByteVec`](#group__crypto_1ga54c835992b731d8c7ec3e6a3c991ec97)` _key` | 
`protected `[`ByteVec`](#group__crypto_1ga54c835992b731d8c7ec3e6a3c991ec97)` _iv` | 
`protected  Cipher()` | 
`protected  Cipher(const `[`Cipher`](#classscy_1_1crypto_1_1Cipher)` &)` | 
`protected `[`Cipher`](#classscy_1_1crypto_1_1Cipher)` & operator=(const `[`Cipher`](#classscy_1_1crypto_1_1Cipher)` &)` | 
`protected void generateKey(const std::string & passphrase,const std::string & salt,int iterationCount)` | 
`protected void setRandomKey()` | Generates and sets key from random data.
`protected void setRandomIV()` | Generates and sets IV from random data.
`protected void initialize(bool encrypt)` | Initializes the [Cipher](./doc/api-crypto.md#classscy_1_1crypto_1_1Cipher) using the given direction.

## Members

#### `public  Cipher(const std::string & name)` {#group__crypto_1ga29c00ec7db3b341e2a40fb34324a6d00}



Creates a new [Cipher](#classscy_1_1crypto_1_1Cipher) object. Auto initializes the key and initialization vector with random bytes.

#### `public  Cipher(const std::string & name,const `[`ByteVec`](#group__crypto_1ga54c835992b731d8c7ec3e6a3c991ec97)` & key,const `[`ByteVec`](#group__crypto_1ga54c835992b731d8c7ec3e6a3c991ec97)` & iv)` {#group__crypto_1ga3084b42100789d88220012f926e60504}



Creates a new [Cipher](#classscy_1_1crypto_1_1Cipher) object, using the given cipher name, key and initialization vector.

#### `public  Cipher(const std::string & name,const std::string & passphrase,const std::string & salt,int iterationCount)` {#group__crypto_1gafa9fbfd0ac80c3e5fe6ee9d939e44907}



Creates a new [Cipher](#classscy_1_1crypto_1_1Cipher) object, using the given cipher name, passphrase, salt value and iteration count.

#### `public  ~Cipher()` {#group__crypto_1gabae774949c196156b08b3e62c997516f}

Destroys the [Cipher](#classscy_1_1crypto_1_1Cipher).



#### `public void initEncryptor()` {#group__crypto_1ga144dd6c25b7d3d0ec97da382829cf636}

Initializes the [Cipher](#classscy_1_1crypto_1_1Cipher) for encryption.



#### `public void initDecryptor()` {#group__crypto_1ga0677c7dac45328f0517b35cb32e3d48c}

Initializes the [Cipher](#classscy_1_1crypto_1_1Cipher) for decryption.



#### `public int update(const unsigned char * input,int inputLength,unsigned char * output,int outputLength)` {#group__crypto_1ga641b7553ec5f282a77d89def7ae54f12}



Encrypts data in a streaming fashion. Hand consecutive blocks of data to the update method in order to encrypt it. Returns the encrypted data chunk. When done, the output of [final()](#group__crypto_1gadfe313d58ac2e4fb224d596a2f03862f) should be additionally added to the result.

#### `public template<typename I,typename O>`  <br/>`inline int update(const I & input,O & output)` {#group__crypto_1ga7d87b66b0f3c68f2c167bc6283e5246a}

Alias for [update()](#group__crypto_1ga641b7553ec5f282a77d89def7ae54f12) which accepts a range of buffer types.



#### `public int final(unsigned char * output,int length)` {#group__crypto_1gadfe313d58ac2e4fb224d596a2f03862f}



Returns the remaining data held in the cipher object. Further calls to [update()](#group__crypto_1ga641b7553ec5f282a77d89def7ae54f12) or [final()](#group__crypto_1gadfe313d58ac2e4fb224d596a2f03862f) will return garbage.

See EVP_CipherFinal_ex for further information.

#### `public template<typename O>`  <br/>`inline int final(O & output)` {#group__crypto_1gafa479af83094e2e9209058ed8a9b0cfb}

Alias for [final()](#group__crypto_1gadfe313d58ac2e4fb224d596a2f03862f) which accepts a range of buffer types.



#### `public int encrypt(const unsigned char * inbuf,std::size_t inlen,unsigned char * outbuf,std::size_t outlen,`[`Encoding`](#group__crypto_1ga44fda7872beb30a68bbc8f2cac9271ae)` encoding)` {#group__crypto_1ga9dcc959e7adb68f9d3201162e4566493}



Encrypts a buffer and encode it using the given encoding. This method performs the encryption, and calls [final()](#group__crypto_1gadfe313d58ac2e4fb224d596a2f03862f) internally.

#### `public template<typename I,typename O>`  <br/>`inline int encrypt(const I & input,O & output,`[`Encoding`](#group__crypto_1ga44fda7872beb30a68bbc8f2cac9271ae)` encoding)` {#group__crypto_1ga0ad0d099abd1007a6bd1c33ab5b8d84d}

Alias for [encrypt()](#group__crypto_1ga9dcc959e7adb68f9d3201162e4566493) which accepts a range of buffer types.



#### `public virtual std::string encryptString(const std::string & str,`[`Encoding`](#group__crypto_1ga44fda7872beb30a68bbc8f2cac9271ae)` encoding)` {#group__crypto_1ga8c440edf0d347751a1d1b9f43e99bd43}

Encrypts a string and encodes it using the given encoding.



#### `public virtual std::string decryptString(const std::string & str,`[`Encoding`](#group__crypto_1ga44fda7872beb30a68bbc8f2cac9271ae)` encoding)` {#group__crypto_1gaa51a187bd02ae8d71aa58686f738ff9b}

Decrypts a string that is encoded with the given encoding.



#### `public virtual void encryptStream(std::istream & source,std::ostream & sink,`[`Encoding`](#group__crypto_1ga44fda7872beb30a68bbc8f2cac9271ae)` encoding)` {#group__crypto_1ga77107e1458845773e1ea7d3a81a6a3c3}

Encrypts an input stream and encodes it using the given encoding.



#### `public virtual void decryptStream(std::istream & source,std::ostream & sink,`[`Encoding`](#group__crypto_1ga44fda7872beb30a68bbc8f2cac9271ae)` encoding)` {#group__crypto_1gada68a82360e3eb8853efe26e55fee509}

Decrypts an input stream that is encoded with the given encoding.



#### `public template<typename T>`  <br/>`inline void setKey(const T & key)` {#group__crypto_1gaeec7a89c1fc12a93e00f2a119d67fc58}

Sets the key for the [Cipher](#classscy_1_1crypto_1_1Cipher).



#### `public template<typename T>`  <br/>`inline void setIV(const T & iv)` {#group__crypto_1ga0368e6675a26e93d69beea3a090b7908}

Sets the initialization vector (IV) for the [Cipher](#classscy_1_1crypto_1_1Cipher).



#### `public int setPadding(int padding)` {#group__crypto_1ga4f8d10556e26c93af353a79d4f5bf65f}



Enables or disables padding. By default encryption operations are padded using standard block padding and the padding is checked and removed when decrypting. If the pad parameter is zero then no padding is performed, the total amount of data encrypted or decrypted must then be a multiple of the block size or an error will occur.

See EVP_CIPHER_CTX_set_padding for further information.

#### `public const `[`ByteVec`](#group__crypto_1ga54c835992b731d8c7ec3e6a3c991ec97)` & getKey() const` {#group__crypto_1gad9b197b9bc698da0defede327f66aad6}

Returns the key for the [Cipher](#classscy_1_1crypto_1_1Cipher).



#### `public const `[`ByteVec`](#group__crypto_1ga54c835992b731d8c7ec3e6a3c991ec97)` & getIV() const` {#group__crypto_1gacb5497a7ce6d4a39a57147cdf0b39f7c}

Returns the initialization vector (IV) for the [Cipher](#classscy_1_1crypto_1_1Cipher).



#### `public const std::string & name() const` {#group__crypto_1ga845591809aab05eef583ce02fd139495}

Returns the name of the [Cipher](#classscy_1_1crypto_1_1Cipher).



#### `public int blockSize() const` {#group__crypto_1gaddae447539926bb35a9394ad96dec391}

Returns the block size of the [Cipher](#classscy_1_1crypto_1_1Cipher).



#### `public int keySize() const` {#group__crypto_1ga25f3530bbbd017c9fc219c538a50726e}

Returns the key size of the [Cipher](#classscy_1_1crypto_1_1Cipher).



#### `public int ivSize() const` {#group__crypto_1ga50eab14578b8d9404f38276136514ea2}

Returns the IV size of the [Cipher](#classscy_1_1crypto_1_1Cipher).



#### `public const EVP_CIPHER * cipher()` {#group__crypto_1gae0e917584002004e1ac5b31bce64f733}

Returns the cipher object.



#### `protected bool _initialized` {#group__crypto_1gaef37db5562b977395a89945b02e3b684}





#### `protected bool _encrypt` {#group__crypto_1ga234fb383e465ae6c7f322a4be0ba6a2f}





#### `protected const EVP_CIPHER * _cipher` {#group__crypto_1ga6234e02c10a2a4be511fb2caaa7180b0}





#### `protected std::string _name` {#group__crypto_1gae8bceb0c5aab20317421d37ccdf9b046}





#### `protected EVP_CIPHER_CTX _ctx` {#group__crypto_1gadac11f2be56d934d02109dfbc49ef5c8}





#### `protected `[`ByteVec`](#group__crypto_1ga54c835992b731d8c7ec3e6a3c991ec97)` _key` {#group__crypto_1ga88a997190e8cd5880718fdbe4a613109}





#### `protected `[`ByteVec`](#group__crypto_1ga54c835992b731d8c7ec3e6a3c991ec97)` _iv` {#group__crypto_1ga9fb992dd64b02eaa38cac541d5072ac3}





#### `protected  Cipher()` {#group__crypto_1gaf9ea63a13bf17d5c8b74db6e5e382ca7}





#### `protected  Cipher(const `[`Cipher`](#classscy_1_1crypto_1_1Cipher)` &)` {#group__crypto_1gad155e284a60f65add5a96be985120f7c}





#### `protected `[`Cipher`](#classscy_1_1crypto_1_1Cipher)` & operator=(const `[`Cipher`](#classscy_1_1crypto_1_1Cipher)` &)` {#group__crypto_1gac2a5a2e06c104fecff051c62948076e1}





#### `protected void generateKey(const std::string & passphrase,const std::string & salt,int iterationCount)` {#group__crypto_1ga6edba619802b9ca520bc4f8dd1b5d8b2}



Generates and sets the key and IV from a password and optional salt string.

#### `protected void setRandomKey()` {#group__crypto_1gad71d5cde5fea40cfeab6b08518ddbc95}

Generates and sets key from random data.



#### `protected void setRandomIV()` {#group__crypto_1gab3206763d10ce4674eb7e05f0390913a}

Generates and sets IV from random data.



#### `protected void initialize(bool encrypt)` {#group__crypto_1ga20620b556634ea17605847fd26d1bce3}

Initializes the [Cipher](#classscy_1_1crypto_1_1Cipher) using the given direction.



# class `scy::crypto::Hash` {#classscy_1_1crypto_1_1Hash}






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  Hash(const std::string & algorithm)` | 
`public  ~Hash()` | 
`public void update(char data)` | 
`public void update(const std::string & data)` | 
`public void update(const void * data,unsigned length)` | 
`public const `[`ByteVec`](#group__crypto_1ga54c835992b731d8c7ec3e6a3c991ec97)` & digest()` | Finish up the digest operation and return the result.
`public std::string digestStr()` | Finish up the digest operation and return the result as a string.
`public void reset()` | Resets the engine and digest state ready for the next computation.
`public const std::string & algorithm(void) const` | Returns the hash algorithm being used.
`protected EVP_MD_CTX _ctx` | 
`protected const EVP_MD * _md` | 
`protected `[`crypto::ByteVec`](#group__crypto_1ga54c835992b731d8c7ec3e6a3c991ec97)` _digest` | 
`protected std::string _algorithm` | 
`protected `[`Hash`](#classscy_1_1crypto_1_1Hash)` & operator=(`[`Hash`](#classscy_1_1crypto_1_1Hash)` const &)` | 

## Members

#### `public  Hash(const std::string & algorithm)` {#group__crypto_1gaed22bcaf84b35efc1815a03903541da4}





#### `public  ~Hash()` {#group__crypto_1gaf1bea9c52e354b7b97550e1715105ee0}





#### `public void update(char data)` {#group__crypto_1ga7389984a20183881be045ff427036ce8}





#### `public void update(const std::string & data)` {#group__crypto_1ga8b68b47d859b92608a2ac7295c3ef702}





#### `public void update(const void * data,unsigned length)` {#group__crypto_1ga1b9b8626b6259cb050a8917b8539e59b}



This function may (and normally will) be called many times for large blocks of data.

#### `public const `[`ByteVec`](#group__crypto_1ga54c835992b731d8c7ec3e6a3c991ec97)` & digest()` {#group__crypto_1gacbc12b3d6816140c2e5033fe32efafca}

Finish up the digest operation and return the result.



#### `public std::string digestStr()` {#group__crypto_1gad11ff9012ce5157206e0a05a7c3f7347}

Finish up the digest operation and return the result as a string.



#### `public void reset()` {#group__crypto_1ga58abad9c8210a2882d69dceb3883a973}

Resets the engine and digest state ready for the next computation.



#### `public const std::string & algorithm(void) const` {#group__crypto_1ga6f4e22146b390772a4d2c1d4ef6352c7}

Returns the hash algorithm being used.



#### `protected EVP_MD_CTX _ctx` {#group__crypto_1gaa41b5af95e2c28fe59bba1871e60eb8b}





#### `protected const EVP_MD * _md` {#group__crypto_1gada899a7cc6726572e494dfa2e0aa4838}





#### `protected `[`crypto::ByteVec`](#group__crypto_1ga54c835992b731d8c7ec3e6a3c991ec97)` _digest` {#group__crypto_1gaa8321b86e61d992786de9a8be63630a7}





#### `protected std::string _algorithm` {#group__crypto_1ga778a1b93ea54aff4e5b1b1b80ccbcb6f}





#### `protected `[`Hash`](#classscy_1_1crypto_1_1Hash)` & operator=(`[`Hash`](#classscy_1_1crypto_1_1Hash)` const &)` {#group__crypto_1ga3352d55f48656da5726b58e3c663aef8}





# class `scy::crypto::X509Certificate` {#classscy_1_1crypto_1_1X509Certificate}


This class represents a X509 Certificate.



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  explicit X509Certificate(const char * data,std::size_t length)` | 
`public  explicit X509Certificate(const std::string & path)` | 
`public  explicit X509Certificate(X509 * pCert)` | 
`public  X509Certificate(X509 * pCert,bool shared)` | 
`public  X509Certificate(const `[`X509Certificate`](#classscy_1_1crypto_1_1X509Certificate)` & cert)` | Creates the certificate by copying another one.
`public `[`X509Certificate`](#classscy_1_1crypto_1_1X509Certificate)` & operator=(const `[`X509Certificate`](#classscy_1_1crypto_1_1X509Certificate)` & cert)` | Assigns a certificate.
`public void swap(`[`X509Certificate`](#classscy_1_1crypto_1_1X509Certificate)` & cert)` | Exchanges the certificate with another one.
`public  ~X509Certificate()` | Destroys the [X509Certificate](./doc/api-crypto.md#classscy_1_1crypto_1_1X509Certificate).
`public const std::string & issuerName() const` | Returns the certificate issuer's distinguished name.
`public std::string issuerName(`[`NID`](#group__crypto_1gaf15888f0ebe57ab7978cb91f7bbd40d7)` nid) const` | 
`public const std::string & subjectName() const` | Returns the certificate subject's distinguished name.
`public std::string subjectName(`[`NID`](#group__crypto_1gaf15888f0ebe57ab7978cb91f7bbd40d7)` nid) const` | 
`public std::string commonName() const` | 
`public void extractNames(std::string & commonName,std::set< std::string > & domainNames) const` | 
`public `[`DateTime`](#classscy_1_1DateTime)` validFrom() const` | Returns the date and time the certificate is valid from.
`public `[`DateTime`](#classscy_1_1DateTime)` expiresOn() const` | Returns the date and time the certificate expires.
`public void save(std::ostream & stream) const` | 
`public void save(const std::string & path) const` | 
`public bool issuedBy(const `[`X509Certificate`](#classscy_1_1crypto_1_1X509Certificate)` & issuerCertificate) const` | 
`public const X509 * certificate() const` | Returns the underlying OpenSSL certificate.
`protected void load(const char * data,std::size_t length)` | 
`protected void load(const std::string & path)` | 
`protected void init()` | Extracts issuer and subject name from the certificate.

## Members

#### `public  explicit X509Certificate(const char * data,std::size_t length)` {#group__crypto_1ga625706c3ae74397104ce475816934374}



Creates the [X509Certificate](#classscy_1_1crypto_1_1X509Certificate) object by reading a certificate in PEM format from the given buffer.

#### `public  explicit X509Certificate(const std::string & path)` {#group__crypto_1ga417bdf3f76dfecef89626b6cb71a9a32}



Creates the [X509Certificate](#classscy_1_1crypto_1_1X509Certificate) object by reading a certificate in PEM format from a file.

#### `public  explicit X509Certificate(X509 * pCert)` {#group__crypto_1ga2aa7feaea1f6c99c994fcc6fb7bea0ac}



Creates the [X509Certificate](#classscy_1_1crypto_1_1X509Certificate) from an existing OpenSSL certificate. Ownership is taken of the certificate.

#### `public  X509Certificate(X509 * pCert,bool shared)` {#group__crypto_1gaf7e31eac8d3095866847b5af0db366fa}



Creates the [X509Certificate](#classscy_1_1crypto_1_1X509Certificate) from an existing OpenSSL certificate. Ownership is taken of the certificate. If shared is true, the certificate's reference count is incremented.

#### `public  X509Certificate(const `[`X509Certificate`](#classscy_1_1crypto_1_1X509Certificate)` & cert)` {#group__crypto_1ga9aaeb1e01fbc97752b90be697230a504}

Creates the certificate by copying another one.



#### `public `[`X509Certificate`](#classscy_1_1crypto_1_1X509Certificate)` & operator=(const `[`X509Certificate`](#classscy_1_1crypto_1_1X509Certificate)` & cert)` {#group__crypto_1ga1516b536eadd451c76ecafa299f63d7c}

Assigns a certificate.



#### `public void swap(`[`X509Certificate`](#classscy_1_1crypto_1_1X509Certificate)` & cert)` {#group__crypto_1gac487d92721c8e2bca13f0d13edcf942d}

Exchanges the certificate with another one.



#### `public  ~X509Certificate()` {#group__crypto_1ga9bca014de92bf3dcb7988eea0a41b92f}

Destroys the [X509Certificate](#classscy_1_1crypto_1_1X509Certificate).



#### `public const std::string & issuerName() const` {#group__crypto_1ga59be81cc1b19268c8c3f9b1fd046b20d}

Returns the certificate issuer's distinguished name.



#### `public std::string issuerName(`[`NID`](#group__crypto_1gaf15888f0ebe57ab7978cb91f7bbd40d7)` nid) const` {#group__crypto_1gade700582f523d36f9ba35b4ff42ae9da}



Extracts the information specified by the given NID (name identifier) from the certificate issuer's distinguished name.

#### `public const std::string & subjectName() const` {#group__crypto_1ga08cbb2a291c084ac15d427f2a75bed51}

Returns the certificate subject's distinguished name.



#### `public std::string subjectName(`[`NID`](#group__crypto_1gaf15888f0ebe57ab7978cb91f7bbd40d7)` nid) const` {#group__crypto_1ga448f08e983b8fc5e93bbd1de5f33d0ca}



Extracts the information specified by the given NID (name identifier) from the certificate subject's distinguished name.

#### `public std::string commonName() const` {#group__crypto_1ga5c8fd7098b3c4114b414a737ad5b10f8}



Returns the common name stored in the certificate subject's distinguished name.

#### `public void extractNames(std::string & commonName,std::set< std::string > & domainNames) const` {#group__crypto_1gacfd53defe5a0eb4329622b02f0725221}



Extracts the common name and the alias domain names from the certificate.

#### `public `[`DateTime`](#classscy_1_1DateTime)` validFrom() const` {#group__crypto_1ga79d4899027eb85cfad01e892f3595e33}

Returns the date and time the certificate is valid from.



#### `public `[`DateTime`](#classscy_1_1DateTime)` expiresOn() const` {#group__crypto_1gad2d8ba7bc9502843db12690f6ed24646}

Returns the date and time the certificate expires.



#### `public void save(std::ostream & stream) const` {#group__crypto_1gaf0aaafac5aa2869c52d19b34f8768c0b}



Writes the certificate to the given stream. The certificate is written in PEM format.

#### `public void save(const std::string & path) const` {#group__crypto_1ga0cea8f317ad0371de4a05ae1f4c4387d}



Writes the certificate to the file given by path. The certificate is written in PEM format.

#### `public bool issuedBy(const `[`X509Certificate`](#classscy_1_1crypto_1_1X509Certificate)` & issuerCertificate) const` {#group__crypto_1ga8e93fd651db9982f6178b5fae9447fcc}



Checks whether the certificate has been issued by the issuer given by issuerCertificate. This can be used to validate a certificate chain.

Verifies if the certificate has been signed with the issuer's private key, using the public key from the issuer certificate.

Returns true if verification against the issuer certificate was successful, false otherwise.

#### `public const X509 * certificate() const` {#group__crypto_1ga351a6a8c90b667597ab6ddb1233be511}

Returns the underlying OpenSSL certificate.



#### `protected void load(const char * data,std::size_t length)` {#group__crypto_1gaf492e6a184085172b88509a004f69bf5}



Loads the certificate from the given buffer. The certificate must be in PEM format.

#### `protected void load(const std::string & path)` {#group__crypto_1ga3d18050527eb8fd86b4b63924ecc7ca5}



Loads the certificate from the given file. The certificate must be in PEM format.

#### `protected void init()` {#group__crypto_1gaaf8694ec9aa62ec67324253b9456ba2e}

Extracts issuer and subject name from the certificate.



# struct `CRYPTO_dynlock_value` {#structCRYPTO__dynlock__value}






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`scy::Mutex`](./doc/api-base.md#classscy_1_1Mutex)` _mutex` | 

## Members

#### `public `[`scy::Mutex`](./doc/api-base.md#classscy_1_1Mutex)` _mutex` {#group__crypto_1ga9bfa006e68b7b9a92ef3ca9db96d9d01}





