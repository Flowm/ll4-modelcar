TARGET = car_project
SRC_CC = main.cc controller.cc mqtt_entity.cc utils.cc

LIBS   = base libprotobuf libmosquitto stdcxx lwip libc_lwip pthread

INC_DIR += $(call select_from_repositories,include/lwip)