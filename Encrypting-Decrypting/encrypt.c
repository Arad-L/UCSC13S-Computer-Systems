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

#define USER_MAX 1024

//The following function is used to print out the help screen when
//a user has not entered the correct arguments or has requested for
//the help screen to appear. The function takes no parameters and
//returns nothing as it exits the code afterwards no matter what.

int help() {
    printf("SYNOPSIS\n   Encrypts data using RSA encryption.\n   Encrypted data is decrypted by "
           "the decrypt program.\n\nUSAGE\n   ./encrypt [-hv] [-i infile] [-o outfile] -n "
           "pubkey\n\nOPTIONS\n   -h              Display program help and usage.\n   -v           "
           "   Display verbose program output.\n   -i infile       Input file of data to encrypt "
           "(default: stdin).\n   -o outfile      Output file for encrypted data (default: "
           "stdout).\n   -n pbfile       Public key file (default: rsa.pub).\n");
    return 0;
}

//The following function is used to check which arguments were given
//to the program, and then to parse them and, using a switch statement,
//implement each argument and it's purpose into the following program.
//The function takes its arguments as parameters, one being the amount of args
//and the other being a character array of the arguments. The function also
//creates the necessary values and uses those values to encrypt the provided file.
//The function returns zero to indicate success.

int main(int argc, char **argv) {

    int opt = 0;
    bool verbose = false;
    char *infile_n = NULL;
    char *outfile_n = NULL;
    char *pub_n = "rsa.pub";
    char username[USER_MAX];
    mpz_t m, n, e, s;
    mpz_inits(m, n, e, s, NULL);

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'i': infile_n = optarg; break;
        case 'o': outfile_n = optarg; break;
        case 'n': pub_n = optarg; break;
        case 'v': verbose = true; break;
        case 'h':
            mpz_clears(m, n, e, s, NULL);
            help();
            return 0;
        }
    }

    FILE *infile = stdin;
    if (infile_n) {
        infile = fopen(infile_n, "r");
        if (infile == NULL) {
            printf("Error: failed to open infile.\n");
            mpz_clears(m, n, e, s, NULL);
            return 1;
        }
    }

    FILE *outfile = stdout;
    if (outfile_n) {
        outfile = fopen(outfile_n, "w");
        if (outfile == NULL) {
            printf("Error: failed to open outfile.\n");
            mpz_clears(m, n, e, s, NULL);
            if (infile_n) {
                fclose(infile);
            }
            return 1;
        }
    }

    FILE *pub = fopen(pub_n, "r");
    if (pub == NULL) {
        printf("Error: failed to open public key file.\n");
        mpz_clears(m, n, e, s, NULL);
        if (infile_n) {
            fclose(infile);
        }
        if (outfile_n) {
            fclose(outfile);
        }
        return 1;
    }

    rsa_read_pub(n, e, s, username, pub);

    if (verbose) {
        printf("user = %s\n", username);
        gmp_printf("s (%d bits) = %Zd\n", mpz_sizeinbase(s, 2), s);
        gmp_printf("n (%d bits) = %Zd\n", mpz_sizeinbase(n, 2), n);
        gmp_printf("e (%d bits) = %Zd\n", mpz_sizeinbase(e, 2), e);
    }

    mpz_set_str(m, username, 62);

    if (!rsa_verify(m, s, e, n)) {
        printf("Error: failed to verify.\n");
        mpz_clears(m, n, e, s, NULL);
        if (infile_n) {
            fclose(infile);
        }
        if (outfile_n) {
            fclose(outfile);
        }
        fclose(pub);
        return 1;
    }

    rsa_encrypt_file(infile, outfile, n, e);

    if (infile_n) {
        fclose(infile);
    }
    if (outfile_n) {
        fclose(outfile);
    }
    fclose(pub);
    mpz_clears(m, n, e, s, NULL);

    return 0;
}
