### PNG Loader ###

PNGLoader library is used to load PNG images from file into buffer with PNG file header. It is lite and
OpenGL friendly rather than libPNG.

Here the main features of the PNGLoader:

- Small size of memory usage (about 35kB),
- Simple functioning. Except some functions like saving image to the PNG file (rather than libPNG),
- Possible to load PNG image from last row (useful for texture loading),
- Simple programming interface (only one function to call)
- Open permissive license, that accept usage in the commercial projects (LGPLv2).

Common restrictions:

- Current version (0.2.0) works only with 8 bits per channel,
- Can not load palette PNG images,
- Skip auxiliary chunks
- Depend on side library (zlib)

#### Usage of the PNGLoader library ####

Include `pngloader.h` into source code to use PNGLoader library functions.

```C
#include <pngloader.h>
```

Add to linker flags `-lpngloader` for static linking or `-lpngloader.dll` for dynamic linking.
If dynamic linked version is used, `-lz` must be added to the linker flags.

#### Functions and types ####

`png_error_t` describes PNGLoader error type.

| Name | Description |
| --- | ---|
| PNG_NO_ERROR | This value always 0 and depicts success |
| PNG_ERROR_NO_SUCH_FILE | This error code depicts file access error. This value can return on file missing, file blocking or some other input-output troubles |
| PNG_ERROR_NOT_PNG | This error code has been returned when module desided loded file not meet PNG specification |
| PNG_ERROR_WRONG_CHUNK_ORDER | Rare error, that returned when IDAT chunk has been met before IHDR chunk. This situation occures when PNG file structure was misformed and size of image can not be defined |
| PNG_ERROR_MALLOC | This error occures when dynamic memory can not be allocated. Usually this error occures when PNG header has wrong image size or other chunk has bad length field |
| PNG_ERROR_EMPTY_IMAGE | This error indicates that IDAT chunk missing or empty (this is acceptable by PNG specification but does not make sense) |
| PNG_ERROR_NOT_SUPPORTED | This error occures when PNG file format is not supported by current version of PNGLoader. |
| PNG_ERROR_UNCOMPRESS | This error returns when deflate stream can not be decompressed after load from PNG file. This error frequently occures when PNG file is corrupted |

`png_header_t` describes PNG file header.

| Name | Type | Description |
| --- | --- | --- |
| width | uint32_t | Width of the image (px) |
| height | uint32_t | Height of the image (px) |
| depth | uint8_t | Bit depth of the channel |
| color_type | uint8_t | Image type from PNG specification |
| compression | uint8_t | Compression type from PNG specification |
| filter | uint8_t | Filter type from PNG specification |
| interlace | uint8_t | Using of interlace from PNG specification |

```C
png_error_t LoadPNGFromFile(const char* file_name, png_header_t* header, uint8_t** data, bool flip);
```

`LoadPNGFromFile` function load PNG from `file_name` file into memory pointed by `data`.
`header` variable holds content of IHDR chunk with PNG data. Image is loaded from bottom to top
if `flip` is set to `true`. `LoadPNGFromFile` return error code with `png_error_t` type.

`data` variable is a pointer to the pointer. Memory is allocated automatically on success. Use
`FreePNG` to safely release memory.

The `header` must point to the `png_header_t` allocated variable. It's content will be overwritten
on function `LoadPNGFromFile` call. An undefined content will be in `header` on error.

```C
png_error_t LoadPNGFromArray(const uint8_t* raw_data, size_t length, png_header_t* header, uint8_t** data, bool flip);
```

This function is the same as `LoadPNGFromFile` except the source of the PNG. The source for this
function is plain array of bytes. `raw_data` is a pointer to PNG data in memory. `length` represent
size of the memory image of the PNG.

```C
void FreePNG(uint8_t** data);
```

`FreePNG` function safely free memory after `LoadPNG` was completed. This function must be called
with `data` argument after successful complete of `LoadPNG` function if data not needed anymore.

```C
void PNGLoaderVersion(char* str, size_t n);
```

`PNGLoaderVersion` function extract module version in "N.N.N.N" form, where N is a number of version
component. Version string will be copied into `str` with null symbol at the end. To prevent buffer
overflow `n` argument must contain length of the buffer. Most of version strings does not exceed
11 chars length in the buffer.
