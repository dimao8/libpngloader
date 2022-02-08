#include <pngloader.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

int main(int argc, char ** argv)
{
  uint8_t * data;
  png_header_t hdr;
  int n;
  png_error_t result;
  int r;

  if (argc == 1)
    return -1;

  for (n = 1; n < argc; n++)
    {
      if (strncmp(argv[n], "--except=", 9) == 0)
        {
          sscanf(argv[n], "--except=%i", &r);
        }
      else
        {
					printf("%s\n", argv[n]);
          result = LoadPNGFromFile(argv[n], &hdr, &data, false);
        }
    }

  switch (result)
    {

    case PNG_ERROR_OK:
      printf("No error\n");
      break;

    case PNG_ERROR_NO_SUCH_FILE:
      printf("No such file\n");
      break;

    case PNG_ERROR_NOT_SUPPORTED:
      printf("Not supported\n");
      break;

    case PNG_ERROR_EMPTY_IMAGE:
      printf("Empty image\n");
      break;

    case PNG_ERROR_MALLOC:
      printf("Malloc/Realloc\n");
      break;

    case PNG_ERROR_NOT_PNG:
      printf("Not a PNG\n");
      break;

    case PNG_ERROR_UNCOMPRESS:
      printf("Zlib problem\n");
      break;

    case PNG_ERROR_WRONG_CHUNK_ORDER:
      printf("Wrong chunk order\n");
      break;

    default:
      printf("Unknown error value\n");
      break;

    }

  return (r == result) ? -1 : 0;
}
