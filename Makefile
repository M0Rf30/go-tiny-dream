INCLUDE_PATH := $(abspath ./)
LIBRARY_PATH := $(abspath ./)

BUILD_TYPE?=
# keep standard at C11 and C++11
CFLAGS   = -I./ncnn -I./ncnn/src -I./ncnn/build/src/ -I. -I./tiny-dream -O3 -DNDEBUG -std=c11 -fPIC
CXXFLAGS = -I./ncnn -I./ncnn/src -I./ncnn/build/src/ -I. -I./tiny-dream -O3 -DNDEBUG -std=c++17 -fPIC
LDFLAGS  =
TINY_CXXFLAGS = -I./ncnn -I./ncnn/src -I./ncnn/build/src/ -I. -I./tiny-dream -funsafe-math-optimizations -Ofast -flto=auto -funroll-all-loops -pipe -march=native -std=c++17 -Wextra -lstdc++ -pthread -flto -fPIC

ifneq ($(BUILD_TYPE),hipblas)
	TINY_CXXFLAGS += -fopt-info-vec-optimized
endif

# warnings
CFLAGS   += -Wall -Wextra -Wpedantic -Wcast-qual -Wdouble-promotion -Wshadow -Wstrict-prototypes -Wpointer-arith -Wno-unused-function
CXXFLAGS += -Wall -Wextra -Wpedantic -Wcast-qual -Wno-unused-function
#
# Print build information
#

$(info I go-tiny-dream build info: )

ncnn/build/src/libncnn.a:
	cd ncnn && mkdir -p build && cd build && cmake -DCMAKE_BUILD_TYPE=Release -DNCNN_VULKAN=OFF -DNCNN_BUILD_EXAMPLES=ON .. && make
	cd ncnn && cp -rf src/* ./

tinydream.o: ncnn/build/src/libncnn.a
	$(CXX) $(TINY_CXXFLAGS) tinydream.cpp -o tinydream.o -c $(LDFLAGS)

unpack: ncnn/build/src/libncnn.a
	mkdir -p unpack && cd unpack && ar x ../ncnn/build/src/libncnn.a

libtinydream.a: tinydream.o unpack $(EXTRA_TARGETS)
	ar src libtinydream.a tinydream.o $(shell ls unpack/* | xargs echo)

example/main: libtinydream.a
	@C_INCLUDE_PATH=${INCLUDE_PATH} LIBRARY_PATH=${LIBRARY_PATH} go build -x -o example/main ./example

clean:
	rm -rf *.o
	rm -rf *.a
	rm -rf unpack
