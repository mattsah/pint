#include <string>
#include <cstring>
#include <iostream>

struct pint {
	std::string
		type;
	std::size_t
		size;
	void*
		pntr;
};


pint* pint_init(std::string type, std::size_t size) {
	pint* val = new pint;

	val->type = type;
	val->pntr = malloc(size);

	return val;
}


bool pint_copy(pint* src, pint* dst) {
	if (src->size != dst->size) {
		dst->pntr = realloc(dst->pntr, src->size);

		if (dst->pntr == NULL) {
			dst->size = 0;

			std::cerr << "Could not reallocate memory\n";

			return false;

		} else {
			dst->size = src->size;

		}
	}

	memcpy(src->pntr, dst->pntr, dst->size);

	return true;
}


int main() {
	pint* x  = new pint;
	pint* y  = new pint;
	int8_t xval = 15;
	int yval = 12;
	void* xpntr = &xval;
	void* ypntr = &yval;

	x->type = "main.int";
	x->size = sizeof(int8_t);
	x->pntr = malloc(x->size);

	y->type = "main.int";
	y->size = sizeof(yval);
	y->pntr = malloc(y->size);

	memcpy(x->pntr, xpntr, x->size);
	memcpy(y->pntr, ypntr, y->size);

	std::cout << "Size of x is first: " << x->size << " at: " << x->pntr << std::endl;

	pint_copy(y, x);

	std::cout << "Size of x is later: " << x->size << " at: " << x->pntr << std::endl;

	return 0;
}