package tinydream

// #cgo CXXFLAGS: -I./ncnn/build/install/include -I./tiny-dream -I/usr/include
// #cgo LDFLAGS: -L./ncnn/build/install/lib -ltinydream -lm -lstdc++
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
