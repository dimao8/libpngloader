#include <pngloader.h>

#include <zlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif // HAVE_CONFIG_H

#define PNG_MAGIC1 0x89504E47
#define PNG_MAGIC2 0x0D0A1A0A

#define PNG_CHUNK_IHDR 0x49484452
#define PNG_CHUNK_IDAT 0x49444154
#define PNG_CHUNK_IEND 0x49454E44
#define PNG_CHUNK_PLTE 0x504C5445

#define PNG_COLOR_TYPE_GRAYSCALE        0
#define PNG_COLOR_TYPE_RGB              2
#define PNG_COLOR_TYPE_PALETTE          3
#define PNG_COLOR_TYPE_GRAYSCALE_ALPHA  4
#define PNG_COLOR_TYPE_RGB_ALPHA        6

#define PNG_FILTER_NONE     0
#define PNG_FILTER_SUB      1
#define PNG_FILTER_UP       2
#define PNG_FILTER_AVERAGE  3
#define PNG_FILTER_PAETH    4

/* ********************************* ntohl ********************************* */

uint32_t ntohl(uint32_t u32)
{
#if defined(WORDS_BIGENDIAN)
  return u32;
#else
  return ((u32 >> 24) & 0xFF)
         | ((u32 >> 8) & 0xFF00)
         | ((u32 << 8) & 0xFF0000)
         | ((u32 << 24) & 0xFF000000);
#endif // defined
}

/* ********************************* ntohs ********************************* */

uint32_t ntohs(uint16_t u16)
{
#if defined(WORDS_BIGENDIAN)
  return u16;
#else
  return ((u16 >> 8) & 0xFF)
         | (u16 & 0xFF00);
#endif // defined
}

/* ********************************* f_copy ******************************** */

void f_copy(uint8_t* dst, const uint8_t* src, size_t bpl)
{
  memcpy(dst, src, bpl);
}

/* ********************************* f_sub ********************************* */

void f_sub(uint8_t* dst, const uint8_t* src, size_t bpl, size_t stride)
{
  size_t n;
  for (n = 0; n < stride; n++)
    dst[n] = src[n];
  for (n = stride; n < bpl; n++)
    dst[n] = (src[n] + dst[n - stride]) & 0xFF;
}

/* ********************************** f_up ********************************* */

void f_up(uint8_t* dst, const uint8_t* src, const uint8_t* prior, size_t bpl)
{
  size_t n;
  for (n = 0; n < bpl; n++)
    dst[n] = (src[n] + prior[n]) & 0xFF;
}

/* ******************************* f_average ******************************* */

void f_average(uint8_t* dst, const uint8_t* src, const uint8_t* prior, size_t bpl, size_t stride)
{
  size_t n;
  for (n = 0; n < stride; n++)
    dst[n] = (src[n] + prior[n]/2) & 0xFF;
  for (n = stride; n < bpl; n++)
    dst[n] = (src[n] + (dst[n - stride] + prior[n])/2) & 0xFF;
}

/* **************************** paeth_predictor **************************** */

uint8_t paeth_predictor(uint8_t a, uint8_t b, uint8_t c)
{
  int p = a + b - c;
  int pa = abs(p - a);
  int pb = abs(p - b);
  int pc = abs(p - c);

  if ((pa <= pb) && (pa <= pc))
    return a;
  else if (pb <= pc)
    return b;
  else
    return c;
}

/* ******************************** f_paeth ******************************** */

void f_paeth(uint8_t* dst, const uint8_t* src, const uint8_t* prior, size_t bpl, size_t stride)
{
  size_t n;
  for (n = 0; n < stride; n++)
    dst[n] = (src[n] + paeth_predictor(0, prior[n], 0)) & 0xFF;
  for (n = stride; n < bpl; n++)
    dst[n] = (src[n] + paeth_predictor(dst[n - stride], prior[n], prior[n - stride])) & 0xFF;
}

/* **************************** LoadPNGFromArray *************************** */

png_error_t EXPORT LoadPNGFormArray(const uint8_t* raw_data, size_t length, png_header_t* header, uint8_t** data, bool flip)
{
  bool has_ihdr = false;
  bool has_iend = false;
  uint32_t chunk_name;
  uint32_t chunk_length;
  uint32_t chunk_crc;
  uint32_t magic[2];
  uint8_t * data_buffer = 0;
  uint8_t * zero_line;
  uint32_t data_buffer_size = 0;
  uint8_t * ptr;
  size_t bpp, bpl, n, bpc;
  uLongf destlen;
  size_t offset = 0;

  if (length < 8)
    return PNG_ERROR_NOT_PNG;
  memcpy(magic, raw_data + offset, sizeof(uint32_t)*2);
  offset += sizeof(uint32_t)*2;

  magic[0] = ntohl(magic[0]);
  magic[1] = ntohl(magic[1]);

  if (magic[0] != PNG_MAGIC1 || magic[1] != PNG_MAGIC2)
    return PNG_ERROR_NOT_PNG;

  while (offset < length)
    {
      // TODO : Where is the promised vertical flip, ha?
      if (offset + sizeof(uint32_t) >= length)
        return PNG_ERROR_NOT_PNG;
      memcpy(&chunk_length, raw_data + offset, sizeof(uint32_t));
      offset += sizeof(uint32_t);
      chunk_length = ntohl(chunk_length);
      if (offset + sizeof(uint32_t) >= length)
        return PNG_ERROR_NOT_PNG;
      memcpy(&chunk_name, raw_data + offset, sizeof(uint32_t));
      offset += sizeof(uint32_t);
      chunk_name = ntohl(chunk_name);

      switch (chunk_name)
        {

        case PNG_CHUNK_IHDR:
          if (offset + sizeof(png_header_t) >= length)
            return PNG_ERROR_NOT_PNG;
          memcpy(header, raw_data + offset, sizeof(png_header_t));
          offset += sizeof(png_header_t);
          header->width = ntohl(header->width);
          header->height = ntohl(header->height);
          has_ihdr = true;
          break;

        case PNG_CHUNK_IDAT:
          if (!has_ihdr)
            return PNG_ERROR_WRONG_CHUNK_ORDER;

          if (data_buffer == 0)
            {
              data_buffer = (uint8_t *)malloc(chunk_length);
              if (data_buffer == 0)
                return PNG_ERROR_MALLOC;
            }
          else
            {
              ptr = (uint8_t *)realloc((void *)data_buffer, data_buffer_size + chunk_length);
              if (ptr == 0)
                return PNG_ERROR_MALLOC;
              else
                data_buffer = ptr;
            }

          if (offset + chunk_length >= length)
            return PNG_ERROR_NOT_PNG;
          memcpy(data_buffer + data_buffer_size, raw_data + offset, chunk_length);
          offset += chunk_length;
          data_buffer_size += chunk_length;
          break;

        case PNG_CHUNK_IEND:
          has_iend = true;
          break;

        default:
          offset += chunk_length;

        }

      if (offset + 4 > length)
        return PNG_ERROR_NOT_PNG;
      offset += 4;

      if (has_iend)
        break;
    }

  if (data_buffer == 0)
    return PNG_ERROR_EMPTY_IMAGE;

  if (header->color_type == PNG_COLOR_TYPE_PALETTE)
    {
      free(data_buffer);
      return PNG_ERROR_NOT_SUPPORTED;
    }

  bpc = (header->depth + 7)/8;

  switch (header->color_type)
    {

    case PNG_COLOR_TYPE_GRAYSCALE:
      bpp = bpc;
      break;

    case PNG_COLOR_TYPE_GRAYSCALE_ALPHA:
      bpp = 2*bpc;
      break;

    case PNG_COLOR_TYPE_RGB:
      bpp = 3*bpc;
      break;

    case PNG_COLOR_TYPE_RGB_ALPHA:
      bpp = 4*bpc;
      break;

    default:
      free(data_buffer);
      return PNG_ERROR_NOT_SUPPORTED;

    }

  bpl = header->width*bpp;
  destlen = (bpl + 1)*header->height;
  ptr = (uint8_t *)malloc(destlen);
  if (ptr == 0)
    {
      free(data_buffer);
      return PNG_ERROR_MALLOC;
    }

  if (uncompress((Bytef*)ptr, &destlen, (Bytef *)data_buffer, (uLong)data_buffer_size) != Z_OK)
    {
      free(data_buffer);
      free(ptr);
      return PNG_ERROR_UNCOMPRESS;
    }

  free(data_buffer);
  *data = (uint8_t *)malloc(bpl*header->height);
  if (*data == 0)
    {
      free(ptr);
      return PNG_ERROR_MALLOC;
    }

  // Defiltering
  zero_line = (uint8_t *)malloc(bpl);
  memset(zero_line, 0, bpl);
  for (n = 0; n < header->height; n++)
    {

      switch (ptr[(bpl + 1)*n])
        {

        case PNG_FILTER_NONE:
          f_copy((*data) + bpl*n, ptr + (bpl + 1)*n + 1, bpl);
          break;

        case PNG_FILTER_SUB:
          f_sub((*data) + bpl*n, ptr + (bpl + 1)*n + 1, bpl, bpp);
          break;

        case PNG_FILTER_UP:
          if (n == 0)
            f_up((*data) + bpl*n, ptr + (bpl + 1)*n + 1, zero_line, bpl);
          else
            f_up((*data) + bpl*n, ptr + (bpl + 1)*n + 1, (*data) + bpl*(n - 1), bpl);
          break;

        case PNG_FILTER_AVERAGE:
          if (n == 0)
            f_average((*data) + bpl*n, ptr + (bpl + 1)*n + 1, zero_line, bpl, bpp);
          else
            f_average((*data) + bpl*n, ptr + (bpl + 1)*n + 1, (*data) + bpl*(n - 1), bpl, bpp);
          break;

        case PNG_FILTER_PAETH:
          if (n == 0)
            f_paeth((*data) + bpl*n, ptr + (bpl + 1)*n + 1, zero_line, bpl, bpp);
          else
            f_paeth((*data) + bpl*n, ptr + (bpl + 1)*n + 1, (*data) + bpl*(n - 1), bpl, bpp);
          break;

        }

    }
  free(zero_line);

  // Flip
  if (flip)
    {
      zero_line = (uint8_t*)malloc(bpl);

      for (n = 0; n < header->height/2; n++)
        {
          memcpy(zero_line, *data + n*bpl, bpl);
          memcpy(*data + n*bpl, *data + (header->height - n - 1)*bpl, bpl);
          memcpy(*data + (header->height - n - 1)*bpl, zero_line, bpl);
        }

      free(zero_line);
    }

  // 16 bits per channel
  if (bpc > 1)
    {
      for (n = 0; n < header->height*header->width*bpp/bpc; n++)
        {
          (*data)[n] = (ntohs(((uint16_t*)(*data))[n]) & 0xFF00) >> 8;
        }
      data_buffer = (uint8_t*)realloc(*data, bpl*header->height/bpc);
      if (data_buffer != NULL)
        *data = data_buffer;
      else
        {
          free(*data);
          return PNG_ERROR_MALLOC;
        }
    }
  

  return PNG_ERROR_OK;
}

/**************  LoadPNGFromFile  **************/

png_error_t EXPORT LoadPNGFromFile(const char* file_name, png_header_t* header, uint8_t** data, bool flip)
{
  png_error_t result;

  uint8_t * raw_data;
  size_t length;

  FILE * file = fopen(file_name, "rb");
  if (file == 0)
    return PNG_ERROR_NO_SUCH_FILE;

  fseek(file, 0, SEEK_END);
  length = ftell(file);
  fseek(file, 0, SEEK_SET);
  raw_data = (uint8_t*)malloc(length);
  if (raw_data == 0)
    {
      fclose(file);
      return PNG_ERROR_MALLOC;
    }
  fread(raw_data, length, 1, file);
  fclose(file);

  result = LoadPNGFormArray(raw_data, length, header, data, flip);
  free(raw_data);

  return result;
}

/******************  FreePNG  ******************/

void EXPORT FreePNG(uint8_t** data)
{
	if (*data != 0)
		free(*data);
	*data = 0;
}

/**************  PNGLoaderVersion  *************/

void EXPORT PNGLoaderVersion(char* str, size_t n)
{
  strncpy(str, VERSION, n);
}
