/*
 * A library to control ldscar
 * car must be in SPEED mode (there's a switch on the car, switch it to 'V' rather than 'P')
 */

#include <mutex>
#include <vector>
#include <assert.h>
#include <string.h>
#include "serial/serial.h"
using namespace std;

struct LdsCar {
	LdsCar();
	serial::Serial *com;
	string port;
	bool verbose;
	mutex lock;
	int open(const char* portname);
	int close();
	unsigned char ctrl_buf[10];
	int __send_ctrl_8byte(const unsigned char* data);  // will add 0xff and 0xfe at head
// immediate motion control
	void stop();
	void move(int A, int B, int C);
	void setMaxSpeed(unsigned int speed);  // default is 100
};

#ifdef LDSCAR_IMPLEMENTATION
#undef LDSCAR_IMPLEMENTATION

LdsCar::LdsCar(): com(NULL) {
	verbose = false;
}

int LdsCar::open(const char* _port) {
	assert(com == NULL && "device has been opened");
	// open com port
	lock.lock();
	port = _port;
	if (verbose) printf("opening device \"%s\"\n", port.c_str());
	com = new serial::Serial(port.c_str(), 115200, serial::Timeout::simpleTimeout(1000));
	assert(com->isOpen() && "port is not opened");
	lock.unlock();
	return 0;
}

int LdsCar::close() {
	lock.lock();
	if (com) {
		com->close();
		delete com;
		com = NULL;
	}
	lock.unlock();
	return 0;
}

int LdsCar::__send_ctrl_8byte(const unsigned char* data) {
	assert(com && "device not opened");
	ctrl_buf[0] = 0xff;
	ctrl_buf[1] = 0xfe;
	memcpy(ctrl_buf+2, data, 8);  // copy 8 byte control
	lock.lock();
	assert(com->write(ctrl_buf, 10) == 10 && "write failed");
	com->flush();
	lock.unlock();
	return 0;
}

#define a2u8(x) ((unsigned char)((x)&0xFF))

void LdsCar::stop() {
	const unsigned char msg[8] = {0x77, 0, 0, 0, 0, 0, 0, 0};
	__send_ctrl_8byte(msg);
}

void LdsCar::move(int A, int B, int C) {
	const unsigned char msg[8] = {0x66, a2u8(A>>8), a2u8(A), a2u8(B>>8), a2u8(B), a2u8(C>>8), a2u8(C), 0};
	__send_ctrl_8byte(msg);
}

void LdsCar::setMaxSpeed(unsigned int speed) {
	assert(speed <= 1000 && "speed too large, may slipper");
	const unsigned char msg[8] = {0x88, a2u8(speed>>8), a2u8(speed), 0, 0, 0, 0, 0};
	__send_ctrl_8byte(msg);
}

#endif
