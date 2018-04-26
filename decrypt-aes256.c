/*
 *  AES_256_CBC Decryption
 *	
 *	Author : Omar Zairi
 *
 *	GMU CS-468
 * 
 *	Gets command line arguments
 *	 -k Key File
 *	 -v IV File
 *	 -i Input binary file
 *	 -o Ouput text File
 *
 *	Reads the input file, which should be ciphered text
 *	Decrypts the plaintext using a key and IV file
 *	Writes the decrypted ciphertext to the output file
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>

/*
 * Returns a char array filled with the file contents
 */
unsigned char *stringFromFile( char filename[] , char type[] )
{
	unsigned char *str;

	FILE *fp;
	 
	fp = fopen(filename, type);
	 
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


/*
 *	Decrypts a string containing ciphertext
 */
char *aes256_decrypt(EVP_CIPHER_CTX *de, unsigned char *ciphertext, int *cipher_len)
{
	/* Allocate memory for plaintext from size of ciphertext */
	int plain_len = *cipher_len;
	
	int tmp;
	
	unsigned char *plaintext = malloc(plain_len);
	
	/* Decrypt the ciphertext */
	EVP_DecryptUpdate(de, plaintext, &plain_len, ciphertext, *cipher_len);
	
	EVP_DecryptFinal_ex(de, plaintext + plain_len, &tmp);
	
	*cipher_len = plain_len + tmp;
	
	return (char *)plaintext;
}




int main(int argc, char *argv[])
{
  
	char *keyFile = NULL;
	char *ivFile = NULL;
	char *inputFile = NULL;
	char *outputFile = NULL;

	char *key = NULL;
	char *iv = NULL;
	char *plaintext;
	unsigned char *cipher;

	int cipher_len;
	int tmp;

	
	/* Check arguments */

	int i;
	
  	for (i = 1; i < argc; i++)
	{	
		// Get key
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
		
		// Get IV file
		if (strcmp( "-v" , argv[i] ) == 0)
		{	
			// Try to get argument after option
			if (argv[i + 1] != NULL)
			{	
				// Get IV file name
				ivFile = argv[i + 1];
				
				// Get IV from file
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
				// Get input file
				inputFile = argv[i + 1];
				
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

	
	/* Decryption Process */
	if ( iv && key )
	{
		/* Get cipher text from file*/    
		cipher = stringFromFile(inputFile,"rb");
		
		/* get ciphertext length and put in length */
		FILE *outf;
	
		outf = fopen("ciphersize.txt","r");
	
		fscanf(outf,"%d",&cipher_len); 

		fclose(outf);
		
		/* Initialize the EVP key */
		EVP_CIPHER_CTX ctx;

		EVP_CIPHER_CTX_init(&ctx);

		/* Initialize key and iv */
		EVP_DecryptInit_ex(&ctx, EVP_aes_256_cbc(), NULL, key, iv);
		
		/* Decrypt the ciphertext into plaintext */
		plaintext = aes256_decrypt(&ctx, cipher, &cipher_len);
		
		/* Write plaintext to file */
		FILE *wf = fopen(outputFile , "w");
		
		for(int x = 0; plaintext[x] ; x++)
			fputc(plaintext[x],wf);
		
		fclose(wf);
		
		/* Free the evp key */
		EVP_CIPHER_CTX_cleanup(&ctx);
		
		
		free(key);
		free(iv);
		free(plaintext);
		free(cipher);
		  
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