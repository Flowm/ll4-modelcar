OBJS = mqtt_entity.cc servo.cc utilities.cc controller.cc
CC = g++

all: panda rpi
    
panda:
	$(CC) -o main_panda $(OBJS) main_panda.cc -lmosquittopp -lpthread
    
rpi:
	$(CC) -o main_rpi $(OBJS) main_rpi.cc -lmosquittopp -lpthread


clean:
	rm -f main_rpi main_panda
