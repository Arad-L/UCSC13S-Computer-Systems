#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include <inttypes.h>
#include <gmp.h>
#include <time.h>
#include <ctype.h>
#include <sys/stat.h>
#include "rsa.h"
#include "numtheory.h"
#include "randstate.h"

#define OPTIONS "i:o:n:vh"

//The following function is used to print out the help screen when
//a user has not entered the correct arguments or has requested for
//the help screen to appear. The function takes no parameters and
//returns nothing as it exits the code afterwards no matter what.

int help() {
    printf("SYNOPSIS\n   Decrypts data using RSA decryption.\nEncrypted data is encrypted by the "
           "encrypt program.\n\nUSAGE\n   ./decrypt [-hv] [-i infile] [-o outfile] -n "
           "privkey\n\nOPTIONS\n   -h              Display program help and usage.\n   -v          "
           "    Display verbose program output.\n   -i infile       Input file of data to decrypt "
           "(default: stdin).\n   -o outfile       Output file for decrypted data (default: "
           "stdout).\n   -n pvfile       Private key file (default: rsa.priv).\n");
    return 0;
}

//The following function is used to check which arguments were given
//to the program, and then to parse them and, using a switch statement,
//implement each argument and it's purpose into the following program.
//The function takes its arguments as parameters, one being the amount of args
//and the other being a character array of the arguments. The function also
//creates the necessary values and uses those values to decrypt the provided file.
//The function returns zero to indicate success.

int main(int argc, char **argv) {

    int opt = 0;
    bool verbose = false;
    char *infile_n = NULL;
    char *outfile_n = NULL;
    char *priv_n = "rsa.priv";
    mpz_t n, d;
    mpz_inits(n, d, NULL);

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'i': infile_n = optarg; break;
        case 'o': outfile_n = optarg; break;
        case 'n': priv_n = optarg; break;
        case 'v': verbose = true; break;
        case 'h':
            mpz_clears(n, d, NULL);
            help();
            return 0;
        default: help(); return 1;
        }
    }

    FILE *infile = stdin;
    if (infile_n) {
        infile = fopen(infile_n, "r");
        if (infile == NULL) {
            printf("Error: failed to open file.\n");
            mpz_clears(n, d, NULL);
            return 1;
        }
    }

    FILE *outfile = stdout;
    if (outfile_n) {
        outfile = fopen(outfile_n, "w");
        if (outfile == NULL) {
            printf("Error: failed to open file.\n");
            mpz_clears(n, d, NULL);
            if (infile_n) {
                fclose(outfile);
            }
            return 1;
        }
    }

    FILE *priv = fopen(priv_n, "r");
    if (priv == NULL) {
        printf("Error: failed to open file.\n");
        mpz_clears(n, d, NULL);
        if (infile_n) {
            fclose(infile);
        }
        if (outfile_n) {
            fclose(outfile);
        }
        return 1;
    }

    rsa_read_priv(n, d, priv);

    if (verbose) {
        gmp_printf("n (%d bits) = %Zd\n", mpz_sizeinbase(n, 2), n);
        gmp_printf("d (%d bits) = %Zd\n", mpz_sizeinbase(d, 2), d);
    }

    rsa_decrypt_file(infile, outfile, n, d);

    if (infile_n) {
        fclose(infile);
    }
    if (outfile_n) {
        fclose(outfile);
    }
    fclose(priv);
    mpz_clears(n, d, NULL);

    return 0;
}
