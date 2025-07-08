#ifndef PNGLOADER_H
#define PNGLOADER_H

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#if defined(_WIN32_)
#  define EXPORT __declspec(dllexport)
#else
#  define EXPORT
#endif // _WIN32_

enum png_error_tag
{
  PNG_ERROR_OK = 0,
  PNG_ERROR_NO_SUCH_FILE = 1,
  PNG_ERROR_NOT_PNG = 2,
  PNG_ERROR_WRONG_CHUNK_ORDER = 3,
  PNG_ERROR_MALLOC = 4,
  PNG_ERROR_EMPTY_IMAGE = 5,
  PNG_ERROR_NOT_SUPPORTED = 6,
  PNG_ERROR_UNCOMPRESS = 7
};

typedef enum png_error_tag png_error_t;

#pragma pack(push, 1)
struct png_header_tag
{
  uint32_t width;
  uint32_t height;
  uint8_t depth;
  uint8_t color_type;
  uint8_t compression;
  uint8_t filter;
  uint8_t interlace;
};

typedef struct png_header_tag png_header_t;
#pragma pack(pop)

/**
 * \brief LoadPNGFromArray
 * \param [in] data -- Array with image data
 * \param [out] header   -- PNG header with image info
 * \param [out] data     -- PNG image data
 * \param [in] flip      -- Set to flip image over x
 * \return Return result of PNG loading
 *
 * Load PNG file and store PNG header info and image bites into user defined buffer.
 * Image can be flipped by x axis.
 */
png_error_t EXPORT LoadPNGFromArray(const uint8_t* raw_data, size_t length, png_header_t* header, uint8_t** data, bool flip);

/**
 * \brief LoadPNGFromFile
 * \param [in] file_name -- Path and name of the PNG file
 * \param [out] header   -- PNG header with image info
 * \param [out] data     -- PNG image data
 * \param [in] flip      -- Set to flip image over x
 * \return Return result of PNG loading
 *
 * Load PNG file and store PNG header info and image bites into user defined buffer.
 * Image can be flipped by x axis.
 */
png_error_t EXPORT LoadPNGFromFile(const char* file_name, png_header_t* header, uint8_t** data, bool flip);

/**
 * \brief FreePNG
 * \param [in] data -- Pointer to the data, received by LoadPNG.
 *
 * This function is only way to free data array, received from PNG file by LoadPNG function.
 */
void EXPORT FreePNG(uint8_t** data);

/**
 * \brief PNGLoaderVersion
 * \param [out] str -- Buffer for version string
 * \param [in] n    -- Length of the buffer
 *
 * Copy version string into user buffer.
 */
void EXPORT PNGLoaderVersion(char* str, size_t n);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // PNGLOADER_H

