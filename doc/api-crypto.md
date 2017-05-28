# Module <!-- group --> `crypto`



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`namespace `[`scy::crypto`](#namespacescy_1_1crypto)    | 
`struct `[`CRYPTO_dynlock_value`](#structCRYPTO__dynlock__value)    | 
# namespace `scy::crypto` 



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`class `[`scy::crypto::Cipher`](#classscy_1_1crypto_1_1Cipher)    | 
`class `[`scy::crypto::Hash`](#classscy_1_1crypto_1_1Hash)    | 
`class `[`scy::crypto::X509Certificate`](#classscy_1_1crypto_1_1X509Certificate)    | This class represents a X509 Certificate.
# class `scy::crypto::Cipher` 




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
`public ssize_t update(const unsigned char * input,size_t inputLength,unsigned char * output,size_t outputLength)` | 
`public template<typename I,typename O>`  <br/>`inline ssize_t update(const I & input,O & output)` | Alias for [update()](#group__crypto_1ga3041433d718ba3d7d7b47b4b40afac42) which accepts a range of buffer types.
`public ssize_t final(unsigned char * output,size_t length)` | 
`public template<typename O>`  <br/>`inline ssize_t final(O & output)` | Alias for [final()](#group__crypto_1ga67e162bd2a71957d98cb1e0e2675e4ea) which accepts a range of buffer types.
`public ssize_t encrypt(const unsigned char * inbuf,size_t inlen,unsigned char * outbuf,size_t outlen,`[`Encoding`](#group__crypto_1ga44fda7872beb30a68bbc8f2cac9271ae)` encoding)` | 
`public template<typename I,typename O>`  <br/>`inline ssize_t encrypt(const I & input,O & output,`[`Encoding`](#group__crypto_1ga44fda7872beb30a68bbc8f2cac9271ae)` encoding)` | Alias for [encrypt()](#group__crypto_1ga7cd265c66a382e6b9b396db93875a54c) which accepts a range of buffer types.
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
`protected EVP_CIPHER_CTX * _ctx` | 
`protected std::string _name` | 
`protected `[`ByteVec`](#group__crypto_1ga54c835992b731d8c7ec3e6a3c991ec97)` _key` | 
`protected `[`ByteVec`](#group__crypto_1ga54c835992b731d8c7ec3e6a3c991ec97)` _iv` | 
`protected inline  Cipher()` | 
`protected  Cipher(const `[`Cipher`](#classscy_1_1crypto_1_1Cipher)` &) = delete` | 
`protected `[`Cipher`](#classscy_1_1crypto_1_1Cipher)` & operator=(const `[`Cipher`](#classscy_1_1crypto_1_1Cipher)` &) = delete` | 
`protected void generateKey(const std::string & passphrase,const std::string & salt,int iterationCount)` | Generates and sets the key and IV from a password and optional salt string.
`protected void setRandomKey()` | Generates and sets key from random data.
`protected void setRandomIV()` | Generates and sets IV from random data.
`protected void init(bool encrypt)` | Initializes the [Cipher](./doc/api-crypto.md#classscy_1_1crypto_1_1Cipher) using the given direction.

## Members

#### `public  Cipher(const std::string & name)` 



Creates a new [Cipher](#classscy_1_1crypto_1_1Cipher) object. Auto initializes the key and initialization vector with random bytes.

#### `public  Cipher(const std::string & name,const `[`ByteVec`](#group__crypto_1ga54c835992b731d8c7ec3e6a3c991ec97)` & key,const `[`ByteVec`](#group__crypto_1ga54c835992b731d8c7ec3e6a3c991ec97)` & iv)` 



Creates a new [Cipher](#classscy_1_1crypto_1_1Cipher) object, using the given cipher name, key and initialization vector.

#### `public  Cipher(const std::string & name,const std::string & passphrase,const std::string & salt,int iterationCount)` 



Creates a new [Cipher](#classscy_1_1crypto_1_1Cipher) object, using the given cipher name, passphrase, salt value and iteration count.

#### `public  ~Cipher()` 

Destroys the [Cipher](#classscy_1_1crypto_1_1Cipher).



#### `public void initEncryptor()` 

Initializes the [Cipher](#classscy_1_1crypto_1_1Cipher) for encryption.



#### `public void initDecryptor()` 

Initializes the [Cipher](#classscy_1_1crypto_1_1Cipher) for decryption.



#### `public ssize_t update(const unsigned char * input,size_t inputLength,unsigned char * output,size_t outputLength)` 



Encrypts data in a streaming fashion. Hand consecutive blocks of data to the update method in order to encrypt it. Returns the encrypted data chunk. When done, the output of [final()](#group__crypto_1ga67e162bd2a71957d98cb1e0e2675e4ea) should be additionally added to the result.

#### `public template<typename I,typename O>`  <br/>`inline ssize_t update(const I & input,O & output)` 

Alias for [update()](#group__crypto_1ga3041433d718ba3d7d7b47b4b40afac42) which accepts a range of buffer types.



#### `public ssize_t final(unsigned char * output,size_t length)` 



Returns the remaining data held in the cipher object. Further calls to [update()](#group__crypto_1ga3041433d718ba3d7d7b47b4b40afac42) or [final()](#group__crypto_1ga67e162bd2a71957d98cb1e0e2675e4ea) will return garbage.

See EVP_CipherFinal_ex for further information.

#### `public template<typename O>`  <br/>`inline ssize_t final(O & output)` 

Alias for [final()](#group__crypto_1ga67e162bd2a71957d98cb1e0e2675e4ea) which accepts a range of buffer types.



#### `public ssize_t encrypt(const unsigned char * inbuf,size_t inlen,unsigned char * outbuf,size_t outlen,`[`Encoding`](#group__crypto_1ga44fda7872beb30a68bbc8f2cac9271ae)` encoding)` 



Encrypts a buffer and encode it using the given encoding. This method performs the encryption, and calls [final()](#group__crypto_1ga67e162bd2a71957d98cb1e0e2675e4ea) internally.

#### `public template<typename I,typename O>`  <br/>`inline ssize_t encrypt(const I & input,O & output,`[`Encoding`](#group__crypto_1ga44fda7872beb30a68bbc8f2cac9271ae)` encoding)` 

Alias for [encrypt()](#group__crypto_1ga7cd265c66a382e6b9b396db93875a54c) which accepts a range of buffer types.



#### `public virtual std::string encryptString(const std::string & str,`[`Encoding`](#group__crypto_1ga44fda7872beb30a68bbc8f2cac9271ae)` encoding)` 

Encrypts a string and encodes it using the given encoding.



#### `public virtual std::string decryptString(const std::string & str,`[`Encoding`](#group__crypto_1ga44fda7872beb30a68bbc8f2cac9271ae)` encoding)` 

Decrypts a string that is encoded with the given encoding.



#### `public virtual void encryptStream(std::istream & source,std::ostream & sink,`[`Encoding`](#group__crypto_1ga44fda7872beb30a68bbc8f2cac9271ae)` encoding)` 

Encrypts an input stream and encodes it using the given encoding.



#### `public virtual void decryptStream(std::istream & source,std::ostream & sink,`[`Encoding`](#group__crypto_1ga44fda7872beb30a68bbc8f2cac9271ae)` encoding)` 

Decrypts an input stream that is encoded with the given encoding.



#### `public template<typename T>`  <br/>`inline void setKey(const T & key)` 

Sets the key for the [Cipher](#classscy_1_1crypto_1_1Cipher).



#### `public template<typename T>`  <br/>`inline void setIV(const T & iv)` 

Sets the initialization vector (IV) for the [Cipher](#classscy_1_1crypto_1_1Cipher).



#### `public int setPadding(int padding)` 



Enables or disables padding. By default encryption operations are padded using standard block padding and the padding is checked and removed when decrypting. If the pad parameter is zero then no padding is performed, the total amount of data encrypted or decrypted must then be a multiple of the block size or an error will occur.

See EVP_CIPHER_CTX_set_padding for further information.

#### `public const `[`ByteVec`](#group__crypto_1ga54c835992b731d8c7ec3e6a3c991ec97)` & getKey() const` 

Returns the key for the [Cipher](#classscy_1_1crypto_1_1Cipher).



#### `public const `[`ByteVec`](#group__crypto_1ga54c835992b731d8c7ec3e6a3c991ec97)` & getIV() const` 

Returns the initialization vector (IV) for the [Cipher](#classscy_1_1crypto_1_1Cipher).



#### `public const std::string & name() const` 

Returns the name of the [Cipher](#classscy_1_1crypto_1_1Cipher).



#### `public int blockSize() const` 

Returns the block size of the [Cipher](#classscy_1_1crypto_1_1Cipher).



#### `public int keySize() const` 

Returns the key size of the [Cipher](#classscy_1_1crypto_1_1Cipher).



#### `public int ivSize() const` 

Returns the IV size of the [Cipher](#classscy_1_1crypto_1_1Cipher).



#### `public const EVP_CIPHER * cipher()` 

Returns the cipher object.



#### `protected bool _initialized` 





#### `protected bool _encrypt` 





#### `protected const EVP_CIPHER * _cipher` 





#### `protected EVP_CIPHER_CTX * _ctx` 





#### `protected std::string _name` 





#### `protected `[`ByteVec`](#group__crypto_1ga54c835992b731d8c7ec3e6a3c991ec97)` _key` 





#### `protected `[`ByteVec`](#group__crypto_1ga54c835992b731d8c7ec3e6a3c991ec97)` _iv` 





#### `protected inline  Cipher()` 





#### `protected  Cipher(const `[`Cipher`](#classscy_1_1crypto_1_1Cipher)` &) = delete` 





#### `protected `[`Cipher`](#classscy_1_1crypto_1_1Cipher)` & operator=(const `[`Cipher`](#classscy_1_1crypto_1_1Cipher)` &) = delete` 





#### `protected void generateKey(const std::string & passphrase,const std::string & salt,int iterationCount)` 

Generates and sets the key and IV from a password and optional salt string.



#### `protected void setRandomKey()` 

Generates and sets key from random data.



#### `protected void setRandomIV()` 

Generates and sets IV from random data.



#### `protected void init(bool encrypt)` 

Initializes the [Cipher](#classscy_1_1crypto_1_1Cipher) using the given direction.



# class `scy::crypto::Hash` 






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  Hash(const std::string & algorithm)` | 
`public  ~Hash()` | 
`public void update(char data)` | 
`public void update(const std::string & data)` | 
`public void update(const void * data,size_t length)` | 
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

#### `public  Hash(const std::string & algorithm)` 





#### `public  ~Hash()` 





#### `public void update(char data)` 





#### `public void update(const std::string & data)` 





#### `public void update(const void * data,size_t length)` 



This function may (and normally will) be called many times for large blocks of data.

#### `public const `[`ByteVec`](#group__crypto_1ga54c835992b731d8c7ec3e6a3c991ec97)` & digest()` 

Finish up the digest operation and return the result.



#### `public std::string digestStr()` 

Finish up the digest operation and return the result as a string.



#### `public void reset()` 

Resets the engine and digest state ready for the next computation.



#### `public const std::string & algorithm(void) const` 

Returns the hash algorithm being used.



#### `protected EVP_MD_CTX _ctx` 





#### `protected const EVP_MD * _md` 





#### `protected `[`crypto::ByteVec`](#group__crypto_1ga54c835992b731d8c7ec3e6a3c991ec97)` _digest` 





#### `protected std::string _algorithm` 





#### `protected `[`Hash`](#classscy_1_1crypto_1_1Hash)` & operator=(`[`Hash`](#classscy_1_1crypto_1_1Hash)` const &)` 





# class `scy::crypto::X509Certificate` 


This class represents a X509 Certificate.



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  explicit X509Certificate(const char * data,size_t length)` | 
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
`protected void load(const char * data,size_t length)` | 
`protected void load(const std::string & path)` | 
`protected void init()` | Extracts issuer and subject name from the certificate.

## Members

#### `public  explicit X509Certificate(const char * data,size_t length)` 



Creates the [X509Certificate](#classscy_1_1crypto_1_1X509Certificate) object by reading a certificate in PEM format from the given buffer.

#### `public  explicit X509Certificate(const std::string & path)` 



Creates the [X509Certificate](#classscy_1_1crypto_1_1X509Certificate) object by reading a certificate in PEM format from a file.

#### `public  explicit X509Certificate(X509 * pCert)` 



Creates the [X509Certificate](#classscy_1_1crypto_1_1X509Certificate) from an existing OpenSSL certificate. Ownership is taken of the certificate.

#### `public  X509Certificate(X509 * pCert,bool shared)` 



Creates the [X509Certificate](#classscy_1_1crypto_1_1X509Certificate) from an existing OpenSSL certificate. Ownership is taken of the certificate. If shared is true, the certificate's reference count is incremented.

#### `public  X509Certificate(const `[`X509Certificate`](#classscy_1_1crypto_1_1X509Certificate)` & cert)` 

Creates the certificate by copying another one.



#### `public `[`X509Certificate`](#classscy_1_1crypto_1_1X509Certificate)` & operator=(const `[`X509Certificate`](#classscy_1_1crypto_1_1X509Certificate)` & cert)` 

Assigns a certificate.



#### `public void swap(`[`X509Certificate`](#classscy_1_1crypto_1_1X509Certificate)` & cert)` 

Exchanges the certificate with another one.



#### `public  ~X509Certificate()` 

Destroys the [X509Certificate](#classscy_1_1crypto_1_1X509Certificate).



#### `public const std::string & issuerName() const` 

Returns the certificate issuer's distinguished name.



#### `public std::string issuerName(`[`NID`](#group__crypto_1gaf15888f0ebe57ab7978cb91f7bbd40d7)` nid) const` 



Extracts the information specified by the given NID (name identifier) from the certificate issuer's distinguished name.

#### `public const std::string & subjectName() const` 

Returns the certificate subject's distinguished name.



#### `public std::string subjectName(`[`NID`](#group__crypto_1gaf15888f0ebe57ab7978cb91f7bbd40d7)` nid) const` 



Extracts the information specified by the given NID (name identifier) from the certificate subject's distinguished name.

#### `public std::string commonName() const` 



Returns the common name stored in the certificate subject's distinguished name.

#### `public void extractNames(std::string & commonName,std::set< std::string > & domainNames) const` 



Extracts the common name and the alias domain names from the certificate.

#### `public `[`DateTime`](#classscy_1_1DateTime)` validFrom() const` 

Returns the date and time the certificate is valid from.



#### `public `[`DateTime`](#classscy_1_1DateTime)` expiresOn() const` 

Returns the date and time the certificate expires.



#### `public void save(std::ostream & stream) const` 



Writes the certificate to the given stream. The certificate is written in PEM format.

#### `public void save(const std::string & path) const` 



Writes the certificate to the file given by path. The certificate is written in PEM format.

#### `public bool issuedBy(const `[`X509Certificate`](#classscy_1_1crypto_1_1X509Certificate)` & issuerCertificate) const` 



Checks whether the certificate has been issued by the issuer given by issuerCertificate. This can be used to validate a certificate chain.

Verifies if the certificate has been signed with the issuer's private key, using the public key from the issuer certificate.

Returns true if verification against the issuer certificate was successful, false otherwise.

#### `public const X509 * certificate() const` 

Returns the underlying OpenSSL certificate.



#### `protected void load(const char * data,size_t length)` 



Loads the certificate from the given buffer. The certificate must be in PEM format.

#### `protected void load(const std::string & path)` 



Loads the certificate from the given file. The certificate must be in PEM format.

#### `protected void init()` 

Extracts issuer and subject name from the certificate.



# struct `CRYPTO_dynlock_value` 






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public std::mutex _mutex` | 

## Members

#### `public std::mutex _mutex` 





