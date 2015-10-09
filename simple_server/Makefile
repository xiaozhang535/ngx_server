CC = gcc 
CXX = g++ 
CFLAGS = -g -Wall 
CFLAGS += -static

OBJ_DIR = obj
DEPS_DIR = deps
RELEASE_DIR = .

$(DEPS_DIR)/%.d: %.cpp
	mkdir -p $(DEPS_DIR)
	$(CC) $(CFLAGS) $(INCLUDE) -MM $< | sed s/"^"/"$(OBJ_DIR)\/"/ > $@
	$(CC) $(CFLAGS) $(INCLUDE) -MM $< | sed s/"^"/"$(DEPS_DIR)\/"/   | sed s/"\.o"/"\.d"/  >> $@
$(OBJ_DIR)/%.o: %.cpp 
	mkdir -p $(OBJ_DIR)
	$(CXX) $(MACRO) $(CFLAGS) $(INCLUDE) -c $< -o $@

$(DEPS_DIR)/%.d: %.cc
	mkdir -p $(DEPS_DIR)
	$(CC) $(CFLAGS) $(INCLUDE) -MM $< | sed s/"^"/"$(OBJ_DIR)\/"/ > $@
	$(CC) $(CFLAGS) $(INCLUDE) -MM $< | sed s/"^"/"$(DEPS_DIR)\/"/   | sed s/"\.o"/"\.d"/  >> $@
$(OBJ_DIR)/%.o: %.cc 
	mkdir -p $(OBJ_DIR)
	$(CXX) $(MACRO) $(CFLAGS) $(INCLUDE) -c $< -o $@

$(DEPS_DIR)/%.d: %.c
	mkdir -p $(DEPS_DIR)
	$(CC) $(CFLAGS) $(INCLUDE) -MM $< | sed s/"^"/"$(OBJ_DIR)\/"/ > $@
	$(CC) $(CFLAGS) $(INCLUDE) -MM $< | sed s/"^"/"$(DEPS_DIR)\/"/   | sed s/"\.o"/"\.d"/  >> $@
$(OBJ_DIR)/%.o: %.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(MACRO) $(CFLAGS) $(INCLUDE) -c $< -o $@

TARGET = tcp_client tcp_server udp_client udp_server
all:${TARGET}

tcp_client:$(OBJ_DIR)/tcp_client.o $(OBJ_DIR)/protocol.o
	$(CXX) $(CFLAGS) $(patsubst %.o,$(OBJ_DIR)/%.o,$(^F)) $(LIB) -o $@ 
	@#cp $@ $(RELEASE_DIR)/$@
tcp_server:$(OBJ_DIR)/tcp_server.o $(OBJ_DIR)/protocol.o
	$(CXX) $(CFLAGS) $(patsubst %.o,$(OBJ_DIR)/%.o,$(^F)) $(LIB) -o $@ 
	@#cp $@ $(RELEASE_DIR)/$@
udp_client:$(OBJ_DIR)/udp_client.o
	$(CXX) $(CFLAGS) $(patsubst %.o,$(OBJ_DIR)/%.o,$(^F)) $(LIB) -o $@ 
	@#cp $@ $(RELEASE_DIR)/$@
udp_server:$(OBJ_DIR)/udp_server.o
	$(CXX) $(CFLAGS) $(patsubst %.o,$(OBJ_DIR)/%.o,$(^F)) $(LIB) -o $@ 
	@#cp $@ $(RELEASE_DIR)/$@

#http://www.cnblogs.com/hnrainll/archive/2011/04/12/2013377.html
.PHONY: all clean

#包含所有依赖关系的文件
-include $(patsubst   %.cpp,$(DEPS_DIR)/%.d,$(patsubst   %.c,$(DEPS_DIR)/%.d,$(wildcard *.cpp *.c) ))

clean:
	rm -rf $(TARGET) $(OBJS) $(OBJ_DIR)/*.o $(DEPS_DIR)/*.d
	rmdir $(DEPS_DIR)
	rmdir $(OBJ_DIR)

