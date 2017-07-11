OBJS = main.cc mqtt_entity.cc servo.cc utilities.cc
CC = g++

all:
	$(CC) -o main $(OBJS) -lmosquittopp -lpthread

clean:
	rm -f main
