/*
How to use XXH32 streaming hashing function
===========================================

Building
--------

cc -std=c99 -Wall -Wextra -Wundef -Wshadow -Wcast-align -Werror -I .. -L .. xxh32-stream.c -l xxhash

./a.out ../cli/COPYING

Output
------

0xeaa66bfd  ../cli/COPYING

 */
#include <stdio.h>
#include <stdlib.h>
#include <xxhash.h>

// Print hash value via canonical struct
void my_print_xxh32(XXH32_hash_t hashValue)
{
    XXH32_canonical_t c;
    XXH32_canonicalFromHash(&c, hashValue);

    printf("0x");
    for (size_t i = 0; i < sizeof(c.digest); ++i) {
        printf("%02x", c.digest[i]);
    }
}

// Compute XXH32 hash with sreaming API
XXH32_hash_t my_compute_xxh32_streaming(FILE *fp, char *buf, size_t bufSizeInBytes)
{
    // Create state.  Abort if error.
    XXH32_state_t *const state = XXH32_createState();
    if (state == NULL) {
        fprintf(stderr, "Failed : XXH32_createState()\n");
        abort();
    }

    // Reset the state.  Abort if error.
    if (XXH32_reset(state, 0U) == XXH_ERROR) {
        fprintf(stderr, "Failed : XXH32_reset(%p)\n", state);
        abort();
    }

    // While file stream doesn't reach the end of file,
    while (!feof(fp)) {
        // Read the file stream partially
        size_t const length = fread(buf, 1, bufSizeInBytes, fp);
        // Update the state with data.  Abort if error.
        if (XXH32_update(state, buf, length) == XXH_ERROR) {
            fprintf(stderr, "Failed : XXH32_update(%p, %p, %zd)\n", state, buf, length);
            abort();
        }
    }

    // Retrieve digest hash from the state
    XXH32_hash_t const digest32 = XXH32_digest(state);

    // Free the state.
    XXH32_freeState(state);

    return digest32;
}

// Compute XXH32 hash from content of filename
XXH32_hash_t my_compute_xxh32_from_file(const char *filename)
{
    // Open file stream.  Abort if error.
    FILE *const fp = fopen(filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "Failed : fopen(%s)\n", filename);
        abort();
    }

    // Allocate file buffer.  Abort if error.
    // Note : allocation size is extremely small to demonstrate how streaming API works.
    size_t const bufSizeInBytes = 1024;
    char *const  buf            = (char *)malloc(bufSizeInBytes);
    if (buf == NULL) {
        fprintf(stderr, "Failed : malloc(%zd)\n", bufSizeInBytes);
        abort();
    }

    // Compute XXH32 hash with streaming API
    XXH32_hash_t const digest32 = my_compute_xxh32_streaming(fp, buf, bufSizeInBytes);

    // Free the file buffer
    free(buf);

    // Close the file stream
    fclose(fp);

    return digest32;
}

int main(int argc, char *argv[])
{
    // For all arguments
    for (int i = 1; i < argc; ++i) {
        const char *filename = argv[i];

        // Compute XXH32 hash with streaming API
        XXH32_hash_t const digest32 = my_compute_xxh32_from_file(filename);

        // Display hash and filename
        my_print_xxh32(digest32);
        printf("  %s\n", filename);
    }
    return EXIT_SUCCESS;
}
