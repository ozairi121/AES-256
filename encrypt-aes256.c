/*
 *  AES_256_CBC Encryption
 *	
 *	Author : Omar Zairi
 *
 *	GMU CS-468
 * 
 *	Gets command line arguments
 *	 -k Key File
 *	 -v IV File
 *	 -i Input text file
 *	 -o Ouput binary File
 *
 *	Reads the input file, which should be plain text
 *	Encrypts the plaintext using a key and IV file
 *	Writes the encrypted ciphertext to the output file
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>

/*
 * Returns a char array filled with the file contents
 */
char *stringFromFile( char filename[] , char type[] )
{
	unsigned char *str;

	/* Open file */
	FILE *fp;
	 
	fp = fopen(filename, type);
	 
	/* Check if file is readable */
	if ( !fp )
	{
	printf("ERROR. Couldn't read file.\n");
	exit(1);
	}

	// Get the end position of the file
	fseek (fp , 0 , SEEK_END);
	int length = ftell (fp);
	fseek (fp , 0 , SEEK_SET);

	// Allocate memory for the string
	str = malloc(length + 1);

	int c , n = 0;

	// Read in each char from file
	while ( (c = fgetc(fp)) != EOF )
	{
	  str[n++] = (unsigned char)c;
	}
	str[n] = '\0';

	fclose(fp);

	return str;
}


/* Encrypts a given string of plaintext
 * Returns a ciphertext string
 */
unsigned char *aes256_encrypt(EVP_CIPHER_CTX *en, unsigned char *plaintext, int *plain_len)
{
	/* Allocate memory from size of plaintext */
	int cipher_len = *plain_len + 8;
	
	int tmp;
	
	unsigned char *ciphertext = malloc(cipher_len);
	
	/* Encrypt the plaintext to ciphertext */
	EVP_EncryptUpdate(en, ciphertext, &cipher_len, plaintext, *plain_len);
	
	EVP_EncryptFinal_ex(en, ciphertext+cipher_len, &tmp);
	
	*plain_len = cipher_len + tmp;
	
	return ciphertext;
}







int main(int argc, char *argv[])
{
  
	char *keyFile = NULL;
	char *ivFile = NULL;
	char *inputFile = NULL;
	char *outputFile = NULL;

	char *key = NULL;
	char *iv = NULL;
	char *plain;
	unsigned char *ciphertext;
	
	int plain_len;
	int tmp;

	
	/* Check arguments */

	int i;
	
  	for (i = 1; i < argc; i++)
	{	
		//get key
		if (strcmp( "-k" , argv[i] ) == 0)
		{	
			// try to get argument after option
			if (argv[i + 1] != NULL)
			{
				// Get key file name
				keyFile = argv[i + 1];
				
				key = stringFromFile(keyFile , "r");
				
				// Check if key is 32 bytes long
				if ( strlen(key)-1 < 32 )
				{
					printf("ERROR. Key is not long enough.\n");
					exit(1);
				}
			}
			else
			{
				printf("No key file specified\n");
				exit(1);
			}
		}
		
		//get iv file
		if (strcmp( "-v" , argv[i] ) == 0)
		{	
			// try to get argument after option
			if (argv[i + 1] != NULL)
			{	
				// Get IV file name
				ivFile = argv[i + 1];
				
				// get IV from file
				iv = stringFromFile(ivFile , "r");
				
				// Check if iv is 16 bytes long
				if ( strlen(iv)-1 < 16 )
				{
					printf("ERROR. IV is not 16 bytes long.\n");
					exit(1);
				}
			}
			else
			{
				printf("No key file specified\n");
				exit(1);
			}
		}

		// Get the input file
		else if (strcmp( "-i" , argv[i] ) == 0)
		{
			if (argv[i + 1] != NULL)
			{
				// Get input file name
				inputFile = argv[i + 1];
				
				// get plaintext from input file
				plain = stringFromFile(inputFile , "r");
				
				printf("Input file: %s\n", inputFile);
			}
			else
			{
				printf("No input file specified\n");
				exit(1);
			}
		}

		// Get output file
		else if (strcmp( "-o" , argv[i] ) == 0)
		{
			if (argv[i + 1] != NULL)
			{
				// Get output file name
				outputFile = argv[i + 1];

				printf("Output file: %s\n", outputFile);
			}
			else
			{
				printf("No output file specified\n");
				exit(1);
			}
		}
	}
	
	
	/* Check that input and output files have been set */
	if( !inputFile || !outputFile)
	{
		printf("No input or output file specified.\n");
		exit(1);
	}
	
	
	/* Encyption Process */
	
	// if iv is set, encrypt with iv and key and write to file
	if ( iv && key )
	{
		/* Get the length of plaintext */
		int input_len = strlen(plain)+1;
		
		/* Initialize the EVP key */
		EVP_CIPHER_CTX en;
		
		EVP_CIPHER_CTX_init(&en);
		
		EVP_EncryptInit_ex(&en, EVP_aes_256_cbc(), NULL, key, iv);
		
		/* Encrypt */
		ciphertext = aes256_encrypt(&en, (unsigned char *)plain, &input_len);
		
		/* Write ciphertext to file */
		FILE *inf = fopen(outputFile,"wb");
		
		int fl = input_len;
		
		int i = 0;
		
		while(i < fl)
		
			fputc(ciphertext[i++],inf);
		
		fclose(inf);
		
		/* Write cipher size to file */
		inf = fopen("ciphersize.txt","w");
		
		fprintf(inf, "%d", input_len);
		
		fclose(inf);
	
		/* Free the EVP key */
		EVP_CIPHER_CTX_cleanup(&en);

		/* Free variables */
		free(key);
		free(iv);
		free(plain);
		free(ciphertext);
		
	}
	
	/* ERRORs */
	else if ( !iv && !key)
	{
		printf("No key and IV file specified.\n");
		exit(1);
	}
	
	else if( !iv )
	{
		printf("No IV file specified.\n");
		exit(1);
	}
	
	else if( !key )
	{
		printf("No key file specified.\n");
		exit(1);
	}
	

	return 0;
}
