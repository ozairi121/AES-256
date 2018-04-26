all: dec-aes256 dec-aes256
enc-aes256: encrypt-aes256.c
	gcc -o enc-aes256 encrypt-aes256.c -lssl -lcrypto -w

AES256_CBC_Dec: decrypt-aes256.c
	gcc -o dec-aes256 decrypt-aes256.c -lssl -lcrypto -w

clean:
	rm -f dec-aes256 dec-aes256 enc-output.txt dec-output.txt ciphersize.txt
