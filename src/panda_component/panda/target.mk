TARGET = panda
SRC_CC = main.cc mqtt_entity.cc

LIBS   = base libmosquitto stdcxx lwip libc_lwip pthread

INC_DIR += $(call select_from_repositories,include/lwip)
