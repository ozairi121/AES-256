ll:AES256_CBC_Enc AES256_CBC_Dec

AES256_CBC_Enc: AES256_CBC_Enc.c
	gcc -o AES256_CBC_Enc AES256_CBC_Enc.c -lssl -lcrypto -w

AES256_CBC_Dec: AES256_CBC_Dec.c
	gcc -o AES256_CBC_Dec AES256_CBC_Dec.c -lssl -lcrypto -w

clean:
	rm -f AES256_CBC_Enc AES256_CBC_Dec CS468-HW2.myaes256 CS468-HW2-myaes256.txt ciphersize.txt
