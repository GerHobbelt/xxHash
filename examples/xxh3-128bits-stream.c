/*
How to use XXH3 128bits variant streaming hashing function
==========================================================


Building
--------

cc -std=c99 -Wall -Wextra -Wundef -Wshadow -Wcast-align -Werror -I .. -L .. xxh3-128bits-stream.c -l xxhash

./a.out ../cli/COPYING

Output
------

0x445635c86205ac5626ffd8d23b61ee2f  ../cli/COPYING

 */
#include <stdio.h>
#include <stdlib.h>
#include <xxhash.h>

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

// Compute XXH3 128bits variant hash with streaming API
XXH128_hash_t my_compute_xxh3_128bits_streaming(FILE *fp, char *buf, size_t bufSizeInBytes)
{
    // Create state.  Abort if error.
    XXH3_state_t *const state = XXH3_createState();
    if (state == NULL) {
        fprintf(stderr, "Failed : XXH3_createState()\n");
        abort();
    }

    // Reset the state.  Abort if error.
    if (XXH3_128bits_reset(state) == XXH_ERROR) {
        fprintf(stderr, "Failed : XXH3_128bits_reset(%p)\n", state);
        abort();
    }

    // While file stream doesn't reach the end of file,
    while (!feof(fp)) {
        // Read the file stream partially
        size_t const length = fread(buf, 1, bufSizeInBytes, fp);
        // Update the state with data.  Abort if error.
        if (XXH3_128bits_update(state, buf, length) == XXH_ERROR) {
            fprintf(stderr, "Failed : XXH3_128bits_update(%p, %p, %zd)\n", state, buf, length);
            abort();
        }
    }

    // Retrieve digest hash from the state
    XXH128_hash_t const digest3_128 = XXH3_128bits_digest(state);

    // Free the state.
    XXH3_freeState(state);

    return digest3_128;
}

// Compute XXH3 128bits variant hash from content of filename
XXH128_hash_t my_compute_xxh3_128bits_from_file(const char *filename)
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

    // Compute XXH128 hash with streaming API
    XXH128_hash_t const digest3_128 = my_compute_xxh3_128bits_streaming(fp, buf, bufSizeInBytes);

    // Free the file buffer
    free(buf);

    // Close the file stream
    fclose(fp);

    return digest3_128;
}

int main(int argc, char *argv[])
{
    // For all arguments
    for (int i = 1; i < argc; ++i) {
        const char *const filename = argv[i];

        // Compute XXH3 128bits variant hash with streaming API
        XXH128_hash_t const digest3_128 = my_compute_xxh3_128bits_from_file(filename);

        // Display hash and filename
        my_print_xxh128(digest3_128);
        printf("  %s\n", filename);
    }
    return EXIT_SUCCESS;
}
