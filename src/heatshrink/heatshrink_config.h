#ifndef HEATSHRINK_CONFIG_H
#define HEATSHRINK_CONFIG_H

/* Should functionality assuming dynamic allocation be used? */
#define HEATSHRINK_DYNAMIC_ALLOC 0

#if HEATSHRINK_DYNAMIC_ALLOC
    /* Optional replacement of malloc/free */
    #define HEATSHRINK_MALLOC(SZ) malloc(SZ)
    #define HEATSHRINK_FREE(PTR, SZ) free(PTR)
#else
    /* Required parameters for static configuration */
    #define HEATSHRINK_STATIC_INPUT_BUFFER_SIZE 64
    #define HEATSHRINK_STATIC_WINDOW_BITS 8
    #define HEATSHRINK_STATIC_LOOKAHEAD_BITS 4
#endif

/* Turn on logging for debugging */
#define HEATSHRINK_DEBUGGING_LOGS 0

/* Use indexing for faster compression (adds 2^(window_bits+1) bytes RAM) */
#define HEATSHRINK_USE_INDEX 1

#endif
