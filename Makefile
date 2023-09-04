CUDA_VER:=12.1
CXX:= gcc
SRCS:= gstnvobjmeta2json.c
INCS:= $(wildcard *.h)
LIB:=libgstnvobjmeta2json.so

DEEPSTREAM_INTALL_DIR:= /opt/nvidia/deepstream/deepstream-6.3

CFLAGS := -fPIC -std=c++11 -Wunused-variable
LIBS := -shared -Wl,-no-undefined \
  -L/usr/local/cuda-$(CUDA_VER)/lib64/ -lcudart -lnppc -lnppig -lnpps \
  -L $(DEEPSTREAM_INTALL_DIR)/lib -lnvdsgst_meta \
  -Wl,-rpath,$(DEEPSTREAM_INTALL_DIR)/lib \


CFLAGS+= \
	-I /usr/local/cuda-$(CUDA_VER)/include \
	-I $(DEEPSTREAM_INTALL_DIR)/sources/includes \


OBJS:= $(SRCS:.c=.o)

PKGS:= gstreamer-1.0 gstreamer-base-1.0 gstreamer-video-1.0 json-c
CFLAGS+=$(shell pkg-config --cflags $(PKGS))
LIBS+=$(shell pkg-config --libs $(PKGS))


all: $(LIB)

%.o: %.c $(INCS) Makefile
	@echo $(CFLAGS)
	$(CXX) -c -o $@ $(CFLAGS) $<

$(LIB): $(OBJS) Makefile
	@echo $(CFLAGS)
	$(CXX) -o $@ $(OBJS) $(LIBS)

install: $(LIB)
	cp -rv $(LIB) /usr/lib/x86_64-linux-gnu/gstreamer-1.0/

clean:
	rm -rf $(OBJS) $(LIB)