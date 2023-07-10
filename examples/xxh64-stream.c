/*
How to use XXH64 streaming hashing function
===========================================


Building & Test
---------------

cc -std=c99 xxh64-stream.c -l xxhash

./a.out ../cli/COPYING


Output
------

0xf595b975eca01d18  ../cli/COPYING

 */
#include <stdio.h>
#include <stdlib.h>
#include <xxhash.h>

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

// Compute XXH64 hash with sreaming API
XXH64_hash_t my_compute_xxh64_streaming_with_filestream_and_buffer(FILE *fp, char *buf, size_t bufSizeInBytes)
{
    // Create state.  Abort if error.
    XXH64_state_t *const state = XXH64_createState();
    if (state == NULL) {
        fprintf(stderr, "Failed : XXH64_createState()\n");
        abort();
    }

    // Reset the state.  Abort if error.
    if (XXH64_reset(state, 0ULL) == XXH_ERROR) {
        fprintf(stderr, "Failed : XXH64_reset(%p)\n", state);
        abort();
    }

    // While file stream doesn't reach the end of file,
    while (!feof(fp)) {
        // Read the file stream partially
        size_t const length = fread(buf, 1, bufSizeInBytes, fp);
        // Update the state with data.  Abort if error.
        if (XXH64_update(state, buf, length) == XXH_ERROR) {
            fprintf(stderr, "Failed : XXH64_update(%p, %p, %zd)\n", state, buf, length);
            abort();
        }
    }

    // Retrieve digest hash from the state
    XXH64_hash_t const digest64 = XXH64_digest(state);

    // Free the state.
    XXH64_freeState(state);

    return digest64;
}

// Compute XXH64 hash from content of filename
XXH64_hash_t my_compute_xxh64_streaming(const char *filename)
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

    // Compute XXH64 hash with streaming API
    XXH64_hash_t const digest64 = my_compute_xxh64_streaming_with_filestream_and_buffer(fp, buf, bufSizeInBytes);

    // Free the file buffer
    free(buf);

    // Close the file stream
    fclose(fp);

    return digest64;
}

int main(int argc, char *argv[])
{
    // For all arguments
    for (int i = 1; i < argc; ++i) {
        const char *const filename = argv[i];

        // Compute XXH64 hash with streaming API
        XXH64_hash_t const digest64 = my_compute_xxh64_streaming(filename);

        // Display hash and filename
        my_print_xxh64(digest64);
        printf("  %s\n", filename);
    }
    return EXIT_SUCCESS;
}
