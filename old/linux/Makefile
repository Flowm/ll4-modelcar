OBJS = mqtt_entity.cc servo.cc utils.cc controller.cc
CC = g++

all: panda rpi

panda:
	$(CC) -o main_panda $(OBJS) main_panda_linux.cc -lmosquittopp -lpthread -std=c++11

rpi:
	$(CC) -o main_rpi $(OBJS) main_rpi_linux.cc -lmosquittopp -lpthread -std=c++11

clean:
	rm -f main_rpi main_panda
