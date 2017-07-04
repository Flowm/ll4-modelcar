TARGET = hello_server
SRC_CC = main.cc sender.cc
LIBS   = base libprotobuf libmosquitto stdcxx lwip

INC_DIR += $(call select_from_repositories,include/lwip) 
