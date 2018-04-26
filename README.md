AES256-CBC Encrytion/Decryption
-------------------------------------------

AES encryption is located in the aes directory under the root directory.
The key is named aes256.key and the IV is named aesIV.txt
The ouput from my commands using AES is in the aes/output directory.

Each folder has a makefile. To compile the code, simply run the make command.
To delete output files and compiled files, run make clean.

Arguments:
<br>
-i input file
<br>
-o output file
<br>
-k secret key
<br>
-i IV key

To compile:
> make

To encrypt a file using AES:
 > ./enc-aes256 -k key.txt -v iv.txt -i test.txt -o enc-output.txt

To decrypt a file using AES:
 > ./dec-aes256 -k key.txt -v iv.txt -i enc-output.txt -o dec-output.txt

