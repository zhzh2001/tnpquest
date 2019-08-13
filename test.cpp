struct a
{
	a(int x) : x(x) {}
	int x;
};
struct b : a
{
	b(int x, int y) : a(a(x)), y(y) {}
	int y;
};
b bb[1] = {b(1, 2)};
int main()
{
#ifdef _WIN32
	throw 233;
#endif
	return 0;
}