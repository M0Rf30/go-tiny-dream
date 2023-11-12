#ifdef __cplusplus
extern "C" {
#endif
int generate_image(int upscale, int  step, int seed, const char* positive_prompt, const char* negative_prompt, const char* dst, const char* assets_dir);
#ifdef __cplusplus
}
#endif