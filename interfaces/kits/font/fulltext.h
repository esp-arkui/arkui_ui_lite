// ---- cjf ----
#define ENABLE_FULLTEXT 0
#if defined(ENABLE_FULLTEXT) && ENABLE_FULLTEXT
    #define SR_1 1
    #if defined(SR_1) && SR_1
    #define ENABLE_SPANNABLE_STRING 1
    #endif
#endif
// ---- cjf end ----


