/*
How to use XXH3_128bits() one-shot hashing function
==================================================


Building
--------

cc -std=c99 -Wall -Wextra -Wundef -Wshadow -Wcast-align -Werror -I .. -L .. xxh3-128bits-one-shot.c  -l xxhash

Output
------

data[38] = {
    0x78, 0x78, 0x48, 0x61, 0x73, 0x68, 0x20, 0x2d,
    0x20, 0x45, 0x78, 0x74, 0x72, 0x65, 0x6d, 0x65,
    0x6c, 0x79, 0x20, 0x66, 0x61, 0x73, 0x74, 0x20,
    0x68, 0x61, 0x73, 0x68, 0x20, 0x61, 0x6c, 0x67,
    0x6f, 0x72, 0x69, 0x74, 0x68, 0x6d,
}
digest3_128 = 0x469ea4140c7df02847924acc77e72db2

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
void my_print_xxh128(XXH128_hash_t hashValue)
{
    XXH128_canonical_t c;
    XXH128_canonicalFromHash(&c, hashValue);

    printf("0x");
    for (size_t i = 0; i < sizeof(c.digest); ++i) {
        printf("%02x", c.digest[i]);
    }
}

// Compute XXH3 128-bit variant hash
void my_compute_xxh3_128bits_one_shot()
{
    // Setup data
    const char *data            = "xxHash - Extremely fast hash algorithm";
    size_t      dataSizeInBytes = strlen(data);

    // Display data
    printf("data[%zd] = {\n", dataSizeInBytes);
    my_print_data(data, dataSizeInBytes);
    printf("}\n");

    // Compute hash
    XXH128_hash_t digest3_128 = //
        XXH3_128bits(           // XXH3 128-bit variant one-shot hash function
            data,               // pointer to data
            dataSizeInBytes     // data length in bytes
        );

    // Display hash
    printf("digest3_128 = ");
    my_print_xxh128(digest3_128);
    printf("\n");
}

int main()
{
    my_compute_xxh3_128bits_one_shot();
    return EXIT_SUCCESS;
}
