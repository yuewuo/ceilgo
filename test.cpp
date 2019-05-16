#define LDSCAR_IMPLEMENTATION
#include "ldscar.h"

LdsCar ldscar;

int main(int argc, char** argv) {
	if (argc != 2) {
		printf("usage: ./test <portname>\n");
		return -1;
	}

	ldscar.open(argv[1]);
	ldscar.setMaxSpeed(200);
	for (int i=0; i<1; ++i) {
		ldscar.move(30000, -30000, 30000);
	}
	// ldscar.stop();
	ldscar.close();

}
