#include "context.h"


static Context context;

Context* context_create(void) {
    return &context;
}   