build:
	mkdir -p build
	protoc --cpp_out=build/ car-control.proto

setup:
	sudo apt-get install autoconf automake libtool curl make g++ unzip
	git clone -b v3.2.0 https://github.com/google/protobuf.git
	cd protobuf && ./autogen.sh
	cd protobuf && ./configure
	$(MAKE) -C protobuf
	$(MAKE) -C protobuf check
	sudo $(MAKE) -C protobuf install
