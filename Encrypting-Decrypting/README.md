# Assignment 6: Public Key Cryptography

## Program Description:

The purpose of this program is to implement a RSA encrypter and decrypter,
with the encrypter reading a given file and encrypting it and the decrypter
reading an encrypted file and decrypting it, with the file being the same
as it was before encryption. This will be done using keys generated using keygen,
using math functions that create prime numbers and perform various other operations.
The generated keys will be used to encrypt the provided files and then create a file
containing the encrypted data. Thereafter, the decrypt program can be used.
With full functionality, a file that is encrypted by the encryption program will be 
able to be successfully decrypted back to its original state by the decryption program.
A help page can also be printed in order to aid with usage of the program, and there 
are multiple command line options that can be used as well which will be discussed later on.

## How to Build The Program:

Run the command "make" which will use rules provided in the Makefile to build
the code using clang. You can also use "make clean" to remove trash files and
"make format" to format the code to a specific standard defined in the Makefile.

## How to Run The Program:

Type ./encrypt -(argument(s)) while in the folder containing the 'encrypt' executable, as well
as any other necessary files such as numtheory.c, numtheory.h, rsa.c, rsa.h, randstate.c,
randstate.h, rsa.pub, and rsa.priv.

Type ./decrypt -(argument(s)) while in the folder containing the 'decrypt' executable, as well
as any other necessary files such as numtheory.c, numtheory.h, rsa.c, rsa.h, randstate.c,
randstate.h, rsa.pub, and rsa.priv.

Type ./keygen -(argument(s)) while in the folder containing the 'keygen' executable, as well
as any other necessary files such as numtheory.c, numtheory.h, rsa.c, rsa.h, randstate.c,
and randstate.h.

The following command-line arguments are available for encrypt and decrypt:
 -i infile  : Specifies the input file path for the file to be (en/de)crypted. Defaults to stdin.
 -o outfile : Specifies the output file path for the (en/de)crypted file. Defaults to stdout.
 -n pbfile  : Specifies the input file path for the public key. Defaults to rsa.pub.
 -v         : When enabled, the username and all mpz's are printed.
 -h         : Prints out a help page showing these options.

The following command-line arguments are available for keygen:
 -b bits       : Minimum bits needed for public key n (default: 256).
 -i confidence : Miller-Rabin iterations for testing primes (default: 50).
 -n pbfile     : Public key file (default: rsa.pub).
 -d pvfile     : Private key file (default: rsa.priv).
 -s seed       : Random seed for testing.
 -v            : When enabled, the username and all mpz's are printed.
 -h            : Prints out a help page showing these options.
