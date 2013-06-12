#include "vector.h"

int main(int argc, const char* argv[]) {
	Vector a(1024), b(1024), c(1024);
	auto d = a + b + c;
	printf("Length: %f\n", d.length());
}

