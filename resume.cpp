#define LDSCAR_IMPLEMENTATION
#include "ldscar.h"

LdsCar ldscar;

int main(int argc, char** argv) {
	if (argc != 2) {
		printf("usage: ./resume <portname>\n");
		return -1;
	}

	ldscar.open(argv[1]);
    ldscar.resume();
	ldscar.close();

}
