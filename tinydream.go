package tinydream

// #cgo CXXFLAGS: -I./ncnn/build/install/include -I. -I./tiny-dream -O3 -DNDEBUG -std=c++17 -fPIC -fopenmp
// #cgo LDFLAGS: -L./ncnn/build/install/lib -lgomp -lm -lstdc++ -ltinydream
// #include "tinydream.h"
// #include <stdlib.h>
import "C"
import (
	"fmt"
)

func GenerateImage(upscale, step, seed int, positive_prompt, negative_prompt, dst, asset_dir string) error {
	pp := C.CString(positive_prompt)
	np := C.CString(negative_prompt)
	ad := C.CString(asset_dir)
	destination := C.CString(dst)

	ret := C.generate_image(C.int(upscale), C.int(step), C.int(seed), pp, np, destination, ad)
	if ret != 0 {
		return fmt.Errorf("failed")
	}
	return nil
}
