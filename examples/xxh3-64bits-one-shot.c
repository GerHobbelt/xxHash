/*
How to use XXH3_64bits() one-shot hashing function
==================================================


Building
--------

cc -std=c99 xxh3-64bits-one-shot.c -l xxhash

Output
------

data[38] = {
    0x78, 0x78, 0x48, 0x61, 0x73, 0x68, 0x20, 0x2d,
    0x20, 0x45, 0x78, 0x74, 0x72, 0x65, 0x6d, 0x65,
    0x6c, 0x79, 0x20, 0x66, 0x61, 0x73, 0x74, 0x20,
    0x68, 0x61, 0x73, 0x68, 0x20, 0x61, 0x6c, 0x67,
    0x6f, 0x72, 0x69, 0x74, 0x68, 0x6d,
}
digest3_64 = 0xa1db1e138d496183

 */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xxhash.h>

// Print data
void my_print_data(const void *data, size_t dataSizeInBytes)
{
    const uint8_t *const d = (const uint8_t *)data;
    for (size_t i = 0; i < dataSizeInBytes; ++i) {
        if (i % 8 == 0) {
            printf("    ");
        }
        printf("0x%02x, ", d[i]);
        if (i % 8 == 7 || i + 1 == dataSizeInBytes) {
            printf("\n");
        }
    }
}

// Print hash value via canonical struct
void my_print_xxh64(XXH64_hash_t hashValue)
{
    XXH64_canonical_t c;
    XXH64_canonicalFromHash(&c, hashValue);

    printf("0x");
    for (size_t i = 0; i < sizeof(c.digest); ++i) {
        printf("%02x", c.digest[i]);
    }
}

// Compute XXH3 64-bit variant hash
void my_compute_xxh3_64bits_one_shot()
{
    // Setup data
    const char *const data            = "xxHash - Extremely fast hash algorithm";
    size_t const      dataSizeInBytes = strlen(data);

    // Display data
    printf("data[%zd] = {\n", dataSizeInBytes);
    my_print_data(data, dataSizeInBytes);
    printf("}\n");

    // Compute hash
    XXH64_hash_t const digest3_64 = //
        XXH3_64bits(                // XXH3 64-bit variant hash function
            data,                   // pointer to data
            dataSizeInBytes         // data length in bytes
        );

    // Display hash
    printf("digest3_64 = ");
    my_print_xxh64(digest3_64);
    printf("\n");
}

int main()
{
    my_compute_xxh3_64bits_one_shot();
    return EXIT_SUCCESS;
}
