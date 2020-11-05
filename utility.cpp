
inline int
clamp(int min, int val, int max)
{
	return val < min ? min : val > max ? max : val;
}