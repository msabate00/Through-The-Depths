#include <corecrt_math.h>

inline float dist(int ax, int ay, int bx, int by) {
	return sqrt(pow(ax - bx, 2) + (pow(ay - by, 2)));
}
