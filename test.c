#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "streamingatof.h"

static inline double myintpow10(int exponent)
{
        double a = 1.0;
        double b = 10.0;
        if (exponent < 0)
        {
		//abort();
                return 1.0 / myintpow10(-exponent);
        }
        // invariant: a * b^exponent stays the same
        while (exponent > 0)
        {
                if ((exponent % 2) == 0)
                {
                        exponent /= 2;
                        b *= b;
                }
                else
                {
                        exponent -= 1;
                        a *= b;
                }
        }
        return a;
}

static double my_atof(const char *data)
{
	struct streaming_atof_ctx sctx;
	struct streaming_atof_ctx *ctx = &sctx;
	int b;
	streaming_atof_init(ctx);

	b = (rand()>>6) % 2;

	size_t len = strlen(data);
	if (b)
	{
		for (size_t i = 0; i < len; i++)
		{
			if (streaming_atof_feed(ctx, &data[i], 1) != 1)
			{
				abort();
			}
		}
	}
	else
	{
		ssize_t slen = (ssize_t)len;
		if (streaming_atof_feed(ctx, data, len) != slen)
		{
			abort();
		}
	}

	return streaming_atof_end(ctx);
}

int main(int argc, char **argv)
{
	char buf[128];
	double d;
	int exponent;

	/*
	d = 54774.939167751669;
	snprintf(buf, sizeof(buf), "%.17g", d);
	printf("%s\n", buf);
	printf("%.17g\n", my_atof(buf));
	exit(1);
	*/

	if (my_atof("0.00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000005e-2") != 0.00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000005e-2)
	{
		printf("err1\n");
		printf("%g\n", my_atof("0.00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000005e-2"));
		printf("%g\n", 0.00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000005e-2);
		abort();
	}
	if (my_atof("500000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000.1") != 500000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000.1)
	{
		printf("err2\n");
		printf("%g\n", my_atof("500000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000.1"));
		printf("%g\n", 500000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000.1);
		abort();
	}
	if (my_atof("500000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000") != 500000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000.0)
	{
		printf("err3\n");
		printf("%g\n", my_atof("500000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"));
		printf("%g\n", 500000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000.0);
		abort();
	}


	srand48(1);
	size_t iter;
	for (iter = 0; iter < 100*1000*1000; iter++)
	{
		d = drand48();
		exponent = rand() % 21;
		exponent -= 10;
		d *= myintpow10(exponent);
		if (drand48() < 0.5)
		{
			d = -d;
		}
		snprintf(buf, sizeof(buf), "%.17g", d);
		if (iter % 100000 == 0)
		{
			printf("%zu\n", iter);
		}
		//printf("%s\n", buf);
		if (atof(buf) != d)
		{
			printf("mismatch1: %.17g %.17g\n", atof(buf), d);
			return 1;
		}
		double myf = my_atof(buf);
		if (myf != d)
		{
			printf("mismatch2: %.17g %.17g\n", myf, d);
			return 1;
		}
		else
		{
			//printf("correct: %.17g %.17g\n", myf, d);
		}
	}
	return 0;
}
