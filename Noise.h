#pragma once
#include "Headers.h"

class Noise
{
public:
	struct noise_layer_info
	{
		double intensity;
		int size;
		noise_layer_info(double i, int s)
		{
			intensity = i;
			size = s;
		}
	};

	double random()
	{
		return (rand() % 10000) / 10000.0;
	}

	int length(int * a)
	{
		return (sizeof(a) / sizeof(*a));
	}

	double lerp(float a0, float a1, double t)
	{
		return a0 + t * (a1 - a0);
	}

	double fast_sigmoid(double a)
	{
		return 1.5 * ((a - 0.5) / (1 + fabs(a - 0.5))) + 0.5;

	}

	double sigmoid_2(double a)
	{
		return (a / (1 + fabs(a))) + 0.5;
	}

	int grad_hash(int x, int y, int seed)
	{
		int q = x & y;
		int xb = x & seed;
		int yb = y | seed;
		q = q << 2;
		x *= seed * 0XF6CC29;
		y *= seed * 0xABC78;
		return y + 0.5 * ((x + y) * (x + y + 1)) + seed;
	}

	double hash_grad_dot(int ix, int iy, double x, double y, int seed_offset)
	{
		//to get the gradient values. Do a hash I guess
		int seed = grad_hash(ix, iy, seed_offset);
		srand(seed);
		double gx = random() - 0.5;
		double gy = random() - 0.5;
		double n = sqrt(gx * gx + gy * gy);
		gx /= n;
		gy /= n;
		double dx = x - ix;
		double dy = y - iy;
		return gx * dx + gy * dy;
	}

	double perlin(int x_size, int y_size, double x_, double y_, int seed)
	{
		double x = ((double)x_ / x_size) * x_size;
		double y = ((double)y_ / y_size) * y_size;

		int x0 = int(x);
		int x1 = x0 + 1;
		int y0 = int(y);
		int y1 = y0 + 1;

		double dx = x - x0;
		double sx = (3 - 2 * dx) * dx * dx;

		double dy = y - y0;
		double sy = (3 - 2 * dy) * dy * dy;

		double n0, n1, ix0, ix1;
		n0 = hash_grad_dot(x0, y0, x, y, seed);
		n1 = hash_grad_dot(x1, y0, x, y, seed);
		ix0 = lerp(n0, n1, sx);
		n0 = hash_grad_dot(x0, y1, x, y, seed);
		n1 = hash_grad_dot(x1, y1, x, y, seed);
		ix1 = lerp(n0, n1, sx);
		double val = lerp(ix0, ix1, sy);
		val = sigmoid_2(val);
		return val;
	}

	// ============================== 3D ===================================








};
