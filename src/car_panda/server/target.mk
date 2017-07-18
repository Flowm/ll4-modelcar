TARGET = hello_server
SRC_CC = main.cc controller.cc mqtt_entity.cc utilities.cc

LIBS   = base libprotobuf libmosquitto stdcxx lwip pthread

INC_DIR += $(call select_from_repositories,include/lwip) 
