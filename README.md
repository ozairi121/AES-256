AES256-CBC and DES-ECB Encrytion/Decryption
-------------------------------------------

AES encryption is located in the aes directory under the root directory.
The key is named aes256.key and the IV is named aesIV.txt
The ouput from my commands using AES is in the aes/output directory.

Each folder has a makefile. To compile the code, simply run the make command.
To delete output files and compiled files, run make clean.

To encrypt a file using AES:
 > ./AES256_CBC_Enc -k aes256key.txt -v aesIV.txt -i CS468-HW2.txt -o CS468-HW2.myaes256

To decrypt a file using AES:
 > ./AES256_CBC_Dec -k aes256key.txt -v aesIV.txt -i CS468-HW2.myaes256 -o CS468-HW2-myaes256.txt

