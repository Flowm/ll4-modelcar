setup:
	PROJECT=car_panda GENODE_TARGET=focnados_panda $(MAKE) -C ../../../ build_dir
	PROJECT=car_rpi GENODE_TARGET=focnados_rpi $(MAKE) -C ../../../ build_dir

panda:
	PROJECT=car_panda GENODE_TARGET=focnados_panda $(MAKE) -C ../../../ run

rpi:
	PROJECT=car_rpi GENODE_TARGET=focnados_rpi $(MAKE) -C ../../../ run

clean:
	PROJECT=car_panda GENODE_TARGET=focnados_panda $(MAKE) -C ../../../ clean
	PROJECT=car_rpi GENODE_TARGET=focnados_rpi $(MAKE) -C ../../../ clean
