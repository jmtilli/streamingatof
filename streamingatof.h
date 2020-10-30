#ifndef _STREAMINGATOF_H_
#define _STREAMINGATOF_H_ 1

#include <stdint.h>

enum streaming_atof_mode {
	STREAMING_ATOF_MODE_PERIOD_OR_EXPONENT_CHAR,
        STREAMING_ATOF_MODE_MANTISSA_SIGN,
        STREAMING_ATOF_MODE_MANTISSA_FIRST,
        STREAMING_ATOF_MODE_MANTISSA,
        STREAMING_ATOF_MODE_MANTISSA_FRAC_FIRST,
        STREAMING_ATOF_MODE_MANTISSA_FRAC,
        STREAMING_ATOF_MODE_EXPONENT_CHAR,
        STREAMING_ATOF_MODE_EXPONENT_SIGN,
        STREAMING_ATOF_MODE_EXPONENT_FIRST,
        STREAMING_ATOF_MODE_EXPONENT,
	STREAMING_ATOF_MODE_DONE,
};

struct streaming_atof_ctx {
	char buf[64];
	int bufsiz;
	enum streaming_atof_mode mode;
	int exponent_offset_set;
	int64_t exponent_offset;
	int64_t skip_offset;
	int64_t exponent;
	int expnegative;
};

static inline void streaming_atof_init(struct streaming_atof_ctx *ctx)
{
	ctx->bufsiz = 0;
	ctx->mode = STREAMING_ATOF_MODE_MANTISSA_SIGN;
	ctx->exponent_offset = 0;
	ctx->exponent_offset_set = 0;
	ctx->skip_offset = 0;
	ctx->exponent = 0;
	ctx->expnegative = 0;
}

double streaming_atof_end(struct streaming_atof_ctx *ctx);

ssize_t streaming_atof_feed(struct streaming_atof_ctx *ctx, const char *data, size_t len);

#endif
