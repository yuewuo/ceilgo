#define LDSCAR_IMPLEMENTATION
#include "ldscar.h"

LdsCar ldscar;

int main(int argc, char** argv) {
	if (argc != 2) {
		printf("usage: ./test <portname>\n");
		return -1;
	}

	ldscar.open(argv[1]);
	ldscar.move(30000, 0, 0);
	ldscar.close();

}
