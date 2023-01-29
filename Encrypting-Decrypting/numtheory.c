#include <gmp.h>
#include "numtheory.h"
#include "randstate.h"
#include <stdlib.h>

void pow_mod(mpz_t out, mpz_t base, mpz_t exponent, mpz_t modulus) {
    mpz_t v, p, d, temp_exp, temp_base;
    mpz_inits(v, p, d, temp_exp, temp_base, NULL);
    mpz_set_ui(v, 1);
    mpz_set(p, base);
    mpz_set(d, exponent);
    while (mpz_cmp_ui(d, 0) == 1) {
        mpz_mod_ui(temp_exp, d, 2);
        if (mpz_cmp_ui(temp_exp, 0)) {
            mpz_mul(temp_base, p, v);
            mpz_mod(v, temp_base, modulus);
        }
        mpz_mul(temp_base, p, p);
        mpz_mod(p, temp_base, modulus);
        mpz_fdiv_q_ui(d, d, 2);
    }
    mpz_set(out, v);
    mpz_clears(v, p, d, temp_exp, temp_base, NULL);
    return;
}

//This function was inspired by information attained at Eric's section

bool is_prime(mpz_t n, uint64_t iters) {
    if (!mpz_cmp_ui(n, 0) || !mpz_cmp_ui(n, 1) || !mpz_cmp_ui(n, 4)) {
        return false;
    } else if (!mpz_cmp_ui(n, 2) || !mpz_cmp_ui(n, 3)) {
        return true;
    }
    mpz_t a, j, y, r, two, up, one_less_n;
    mpz_inits(a, j, y, r, two, up, one_less_n, NULL);
    mpz_sub_ui(one_less_n, n, 1);
    mpz_set_ui(two, 2);
    mp_bitcnt_t s = 2;
    while (mpz_divisible_2exp_p(one_less_n, s)) {
        ++s;
    }
    --s;
    mpz_tdiv_q_2exp(r, one_less_n, s);
    for (uint64_t i = 0; i < iters; ++i) {
        mpz_sub_ui(up, n, 3);
        mpz_urandomm(a, state, up);
        mpz_add_ui(a, a, 2);
        pow_mod(y, a, r, n);
        if (mpz_cmp_ui(y, 1) != 0 && mpz_cmp(y, one_less_n) != 0) {
            mpz_set_ui(j, 1);
            while (mpz_cmp_ui(j, s) != 1 && mpz_cmp(y, one_less_n) != 0) {
                pow_mod(y, y, two, n);
                if (mpz_cmp_ui(y, 1) == 0) {
                    mpz_clears(a, j, y, r, two, up, one_less_n, NULL);
                    return false;
                }
                mpz_add_ui(j, j, 1);
            }
            if (mpz_cmp(y, one_less_n) != 0) {
                mpz_clears(a, j, y, r, two, up, one_less_n, NULL);
                return false;
            }
        }
    }
    mpz_clears(a, j, y, r, two, up, one_less_n, NULL);
    return true;
}

void make_prime(mpz_t p, uint64_t bits, uint64_t iters) {
    mpz_t new_bits;
    mpz_inits(new_bits, NULL);
    mpz_set_ui(new_bits, 1);
    mpz_mul_2exp(new_bits, new_bits, bits);
    do {
        mpz_urandomb(p, state, bits);
        mpz_add(p, p, new_bits);
    } while (!is_prime(p, iters));
    mpz_clears(new_bits, NULL);
    return;
}

void gcd(mpz_t d, mpz_t a, mpz_t b) {
    mpz_t t, temp_a, temp_b, temp_mod;
    mpz_inits(t, temp_a, temp_b, temp_mod, NULL);
    mpz_set(temp_a, a);
    mpz_set(temp_b, b);
    while (mpz_cmp_ui(temp_b, 0)) {
        mpz_set(t, temp_b);
        mpz_mod(temp_mod, temp_a, temp_b);
        mpz_set(temp_b, temp_mod);
        mpz_set(temp_a, t);
    }
    mpz_set(d, temp_a);
    mpz_clears(t, temp_a, temp_b, temp_mod, NULL);
    return;
}

void mod_inverse(mpz_t i, mpz_t a, mpz_t n) {
    mpz_t q, r, rp, t, tp, mul, sub;
    mpz_inits(q, r, rp, t, tp, mul, sub, NULL);
    mpz_set(r, n);
    mpz_set(rp, a);
    mpz_set_ui(t, 0);
    mpz_set_ui(tp, 1);
    while (mpz_cmp_ui(rp, 0)) {
        mpz_div(q, r, rp);
        mpz_mul(mul, q, rp);
        mpz_sub(sub, r, mul);
        mpz_set(r, rp);
        mpz_set(rp, sub);
        mpz_mul(mul, q, tp);
        mpz_sub(sub, t, mul);
        mpz_set(t, tp);
        mpz_set(tp, sub);
    }
    if (mpz_cmp_ui(r, 1) == 1) {
        mpz_set_ui(i, 0);
        mpz_clears(q, r, rp, t, tp, mul, sub, NULL);
        return;
    }
    if (mpz_cmp_ui(t, 0) == -1) {
        mpz_add(t, t, n);
    }
    mpz_set(i, t);
    mpz_clears(q, r, rp, t, tp, mul, sub, NULL);
    return;
}
