/*
How to use xxHash (basic)
=========================


Building & Test
---------------

cc -std=c99 -Wall -Wextra -Wundef -Wshadow -Wcast-align -Werror -I .. -L .. xxhash-hello.c -l xxhash

./a.out


Output
------

d = 0xa1db1e138d496183

 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xxhash.h>

int main()
{
    // Setup data
    const char *const data            = "xxHash - Extremely fast hash algorithm";
    size_t const      dataSizeInBytes = strlen(data);

    // Compute hash
    XXH64_hash_t const d = XXH3_64bits(data, dataSizeInBytes);

    // Converts hash value to canonical representation
    XXH64_canonical_t c;
    XXH64_canonicalFromHash(&c, d);

    // Display the content of canonical representation
    printf("d = 0x");
    for (size_t i = 0; i < sizeof(c.digest); ++i) {
        printf("%02x", c.digest[i]);
    }
    printf("\n");

    return EXIT_SUCCESS;
}
