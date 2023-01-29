#include "rsa.h"
#include "randstate.h"
#include "numtheory.h"
#include <stdlib.h>
#include <gmp.h>

//This function is moderately inspired by information attained at Eric's section

void rsa_make_pub(mpz_t p, mpz_t q, mpz_t n, mpz_t e, uint64_t nbits, uint64_t iters) {
    uint64_t pbits = (random() % ((3 * nbits) / 4)) + (nbits / 4);
    uint64_t qbits = nbits - pbits;
    make_prime(p, pbits, iters);
    make_prime(q, qbits, iters);
    mpz_mul(n, p, q);
    mpz_t ntotient, denom, pt, qt;
    mpz_inits(ntotient, denom, pt, qt, NULL);
    mpz_sub_ui(pt, p, 1);
    mpz_sub_ui(qt, q, 1);
    mpz_mul(ntotient, pt, qt);
    do {
        mpz_urandomb(e, state, nbits);
        gcd(denom, e, ntotient);
    } while (mpz_cmp_ui(denom, 1));
    mpz_clears(ntotient, denom, pt, qt, NULL);
    return;
}

void rsa_write_pub(mpz_t n, mpz_t e, mpz_t s, char username[], FILE *pbfile) {
    gmp_fprintf(pbfile, "%Zx\n", n);
    gmp_fprintf(pbfile, "%Zx\n", e);
    gmp_fprintf(pbfile, "%Zx\n", s);
    fprintf(pbfile, "%s\n", username);
    return;
}

void rsa_read_pub(mpz_t n, mpz_t e, mpz_t s, char username[], FILE *pbfile) {
    gmp_fscanf(pbfile, "%Zx\n", n);
    gmp_fscanf(pbfile, "%Zx\n", e);
    gmp_fscanf(pbfile, "%Zx\n", s);
    fscanf(pbfile, "%s\n", username);
    return;
}

void rsa_make_priv(mpz_t d, mpz_t e, mpz_t p, mpz_t q) {
    mpz_t ntotient, pt, qt;
    mpz_inits(ntotient, pt, qt, NULL);
    mpz_sub_ui(pt, p, 1);
    mpz_sub_ui(qt, q, 1);
    mpz_mul(ntotient, pt, qt);
    mod_inverse(d, e, ntotient);
    mpz_clears(ntotient, pt, qt, NULL);
    return;
}

void rsa_write_priv(mpz_t n, mpz_t d, FILE *pvfile) {
    gmp_fprintf(pvfile, "%Zx\n", n);
    gmp_fprintf(pvfile, "%Zx\n", d);
    return;
}

void rsa_read_priv(mpz_t n, mpz_t d, FILE *pvfile) {
    gmp_fscanf(pvfile, "%Zx\n", n);
    gmp_fscanf(pvfile, "%Zx\n", d);
    return;
}

void rsa_encrypt(mpz_t c, mpz_t m, mpz_t e, mpz_t n) {
    pow_mod(c, m, e, n);
    return;
}

//This function slightly inspired by information attained at Eric's section.

void rsa_encrypt_file(FILE *infile, FILE *outfile, mpz_t n, mpz_t e) {
    mpz_t m, c;
    mpz_inits(m, c, NULL);
    size_t size = (mpz_sizeinbase(n, 2) - 1) / 8;
    uint8_t *block = calloc(size, sizeof(uint8_t));
    block[0] = 0xFF;
    uint8_t chunk;
    while (!feof(infile)) {
        chunk = fread(block + 1, sizeof(uint8_t), size - 1, infile);
        mpz_import(m, chunk + 1, 1, sizeof(uint8_t), 1, 0, block);
        rsa_encrypt(c, m, e, n);
        gmp_fprintf(outfile, "%Zx\n", c);
    }
    mpz_clears(m, c, NULL);
    free(block);
    return;
}

void rsa_decrypt(mpz_t m, mpz_t c, mpz_t d, mpz_t n) {
    pow_mod(m, c, d, n);
    return;
}

//This function slightly inspired by information attained at Eric's section.

void rsa_decrypt_file(FILE *infile, FILE *outfile, mpz_t n, mpz_t d) {
    mpz_t m, c;
    mpz_inits(m, c, NULL);
    size_t size = (mpz_sizeinbase(n, 2) - 1) / 8;
    uint8_t *block = calloc(size, sizeof(uint8_t));
    size_t chunk;
    while (!feof(infile)) {
        if (!gmp_fscanf(infile, "%Zx\n", c)) {
            printf("Failed to read\n");
            exit(1);
        }
        rsa_decrypt(m, c, d, n);
        mpz_export(block, &chunk, 1, sizeof(uint8_t), 1, 0, m);
        fwrite(block + 1, sizeof(uint8_t), chunk - 1, outfile);
    }
    mpz_clears(m, c, NULL);
    free(block);
}

void rsa_sign(mpz_t s, mpz_t m, mpz_t d, mpz_t n) {
    pow_mod(s, m, d, n);
    return;
}

bool rsa_verify(mpz_t m, mpz_t s, mpz_t e, mpz_t n) {
    mpz_t t;
    mpz_inits(t, NULL);
    pow_mod(t, s, e, n);
    if (!mpz_cmp(m, t)) {
        mpz_clears(t, NULL);
        return true;
    }
    mpz_clears(t, NULL);
    return false;
}
