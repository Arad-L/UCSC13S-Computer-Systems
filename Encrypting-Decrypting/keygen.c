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

#define OPTIONS "b:in:d:s:vh"

//The following function is used to print out the help screen when
//a user has not entered the correct arguments or has requested for
//the help screen to appear. The function takes no parameters and
//returns nothing as it exits the code afterwards no matter what.

int help() {
    printf("SYNOPSIS\n   Generates an RSA public/private key pair.\n\nUSAGE\n   ./keygen [-hv] [-b "
           "bits] -n pbfile -d pvfile\n\nOPTIONS\n   -h              Display program help and "
           "usage.\n   -v              Display verbose program output.\n   -b bits         Minimum "
           "bits needed for public key n.\n   -c confidence   Miller-Rabin iterations for testing "
           "primes (default: 50).\n   -n pbfile       Public key file (default: rsa.pub).\n   -d "
           "pvfile       Private key file (default: rsa.priv).\n   -s seed         Random seed for "
           "testing.\n");
    return 0;
}

//The following function is used to check which arguments were given
//to the program, and then to parse them and, using a switch statement,
//implement each argument and it's purpose into the following program.
//The function takes its arguments as parameters, one being the amount of args
//and the other being a character array of the arguments. The function also
//creates the necessary values and uses those values to create the keys and read
//the keys that will be used to encrypt the file. The function returns zero
//to indicate success.

int main(int argc, char **argv) {

    int opt = 0;
    uint64_t bits = 256;
    uint64_t iters = 50;
    uint64_t seed = time(NULL);
    char *pubkey = "rsa.pub";
    char *privkey = "rsa.priv";
    bool verbose = false;
    mpz_t p, q, n, e, d, m, s;
    mpz_inits(p, q, n, e, d, m, s, NULL);

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'b':
            if (!isdigit(optarg)) {
                printf("Error: Bits must be entered as a number.\n");
                mpz_clears(p, q, n, e, d, m, s, NULL);
                randstate_clear();
                return 1;
            }
            bits = (unsigned long) optarg;
            break;
        case 'n': pubkey = optarg; break;
        case 'i':
            if (!isdigit(optarg)) {
                printf("Error: Iters must be entered as a number.\n");
                mpz_clears(p, q, n, e, d, m, s, NULL);
                randstate_clear();
                return 1;
            }
            iters = (unsigned long) optarg;
            break;
        case 'd': privkey = optarg; break;
        case 's':
            if (!isdigit(optarg) || !(seed >= 0) || !((unsigned int) seed <= UINT_MAX)) {
                printf("Error: Seed must be entered as a number.\n");
                mpz_clears(p, q, n, e, d, m, s, NULL);
                randstate_clear();
                return 1;
            }
            seed = (unsigned int) optarg;
            break;
        case 'v': verbose = true; break;
        case 'h':
            mpz_clears(p, q, n, e, d, m, s, NULL);
            randstate_clear();
            help();
            return 0;
        default: help(); return 1;
        }
    }

    FILE *pubfile = fopen(pubkey, "w");
    if (pubfile == NULL) {
        printf("Error: failed to open file.\n");
        mpz_clears(p, q, n, e, d, m, s, NULL);
        randstate_clear();
        return 1;
    }

    FILE *privfile = fopen(privkey, "w");
    if (privfile == NULL) {
        printf("Error: failed to open file.\n");
        mpz_clears(p, q, n, e, d, m, s, NULL);
        randstate_clear();
        fclose(pubfile);
        return 1;
    }

    int desc = fileno(privfile);
    fchmod(desc, 0600);

    randstate_init(seed);
    rsa_make_pub(p, q, n, e, bits, iters);
    rsa_make_priv(d, e, p, q);

    char *username = getenv("USER");
    mpz_set_str(m, username, 62);
    rsa_sign(s, m, d, n);

    rsa_write_pub(n, e, s, username, pubfile);
    rsa_write_priv(n, d, privfile);

    if (verbose) {
        printf("user = %s\n", username);
        gmp_printf("s (%d bits) = %Zd\n", mpz_sizeinbase(s, 2), s);
        gmp_printf("p (%d bits) = %Zd\n", mpz_sizeinbase(p, 2), p);
        gmp_printf("q (%d bits) = %Zd\n", mpz_sizeinbase(q, 2), q);
        gmp_printf("n (%d bits) = %Zd\n", mpz_sizeinbase(n, 2), n);
        gmp_printf("e (%d bits) = %Zd\n", mpz_sizeinbase(e, 2), e);
        gmp_printf("d (%d bits) = %Zd\n", mpz_sizeinbase(d, 2), d);
    }

    fclose(pubfile);
    fclose(privfile);
    randstate_clear();
    mpz_clears(p, q, n, e, d, m, s, NULL);

    return 0;
}
