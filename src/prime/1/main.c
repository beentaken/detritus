#include <stdlib.h>
#include <stdio.h>
#include <png.h>

#ifdef DEBUG
  #include <sys/types.h>
  #include <sys/stat.h>
  #include <fcntl.h>
  #include <unistd.h>
#endif

#define BYTE 8
#define USAGE printf("Usage:\tprime\tn\n\t\tWhere n is a exponent, such that MAX = 2^n && n > 2\n")

#define ELEMENT(x) (x) / BYTE
#define BIT(x) (1 << ((x) % BYTE))

#ifdef USEPNG
int WritePNG(char *filename, char *packed_data, unsigned int pow) {
  int bitdepth = 8;

  unsigned int i;
  unsigned int max = 1;

  unsigned int width = 1;
  unsigned int height = width;

  FILE *fp;

  png_structp png_ptr;

  png_infop info_ptr;

  png_bytep *row_pointers;

  char *data;

  i = pow;

  while(i) {
    max *= 2;
    i--;
  }

  i = pow / 2;

  while(i) {
    width *= 2;
    i--;
  }

  height = max / width;

  data = calloc(max, sizeof(char));

  if (!data) {
    perror("WritePNG:data:");
    return(1);
  }

  row_pointers = calloc(height, sizeof(png_bytep));

  if (!row_pointers) {
    perror("WritePNG:row_pointers:");
    free(data);
    return(1);
  }

  fp = fopen(filename, "wb");
  if (fp == NULL)
    return(1);

  png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

  if (png_ptr == NULL) {
    fclose(fp);
    return(1);
  }

  info_ptr = png_create_info_struct(png_ptr);
  if (info_ptr == NULL) {
    fclose(fp);
    png_destroy_write_struct(&png_ptr,  png_infopp_NULL);
    free(row_pointers);
    return(1);
  }

  if (setjmp(png_jmpbuf(png_ptr))) {
    fclose(fp);
    png_destroy_write_struct(&png_ptr, &info_ptr);
    free(row_pointers);
    return(1);
  }

  png_init_io(png_ptr, fp);

  png_set_IHDR(png_ptr, info_ptr, width, height, bitdepth, PNG_COLOR_TYPE_GRAY, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

  png_write_info(png_ptr, info_ptr);

  for (i = 0; i < max; i++) {
    if (!(packed_data[ELEMENT(i)] & BIT(i))) {
      data[i] = 0 - 1;
    }
  }

  for (i = 0; i < height; i++)
    row_pointers[i] = (png_bytep)(data + i * width);

  png_write_image(png_ptr, row_pointers);

  png_write_end(png_ptr, info_ptr);

  png_destroy_write_struct(&png_ptr, &info_ptr);

  fclose(fp);

  free(row_pointers);
  free(data);

  return(0);
}
#endif

int main(int argc, char **argv) {
  char *foo = NULL;

  unsigned int i, j;
  unsigned int bytes;

  unsigned int MAX = 1;

  unsigned int count = 0;

  unsigned int pow = 0;

  if (argc < 2) {
    USAGE;
    exit(0);
  }

  pow = i = atoi(argv[1]);

  if (i < 3) {
    USAGE;
    exit(0);
  }

  /* Generate the MAX number */
  while (i) {
    MAX = MAX * 2;
    --i;
  }

  MAX--;

  bytes = ELEMENT(MAX);

  printf("Attempting to allocate %d bytes.\n", bytes);

  foo = calloc(bytes, sizeof(char));

  if (foo == NULL) {
    perror("main");
    exit(1);
  }

  for (i = 2; i < MAX; i++) {
    /* if the current number is a prime */
    if (!(foo[ELEMENT(i)] & BIT(i))) {
      /* Only use this if you are keen on seeing the primes */
      #ifdef DEBUG
        printf("%d is a prime.\n", i);
      #endif

      count++;

      /* Mark all multiples of i as composite */
      for (j = 2 * i ; j < MAX; j += i) {
        foo[ELEMENT(j)] = foo[ELEMENT(j)] | BIT(j);
      }
    }
  }
          
  printf("%d out of %d were primes.\n", count, MAX);

#ifdef USEPNG
  WritePNG("/tmp/prime.png", foo, pow);
#endif

  #ifdef DEBUG
    j = creat("/tmp/prime.dat", S_IRUSR | S_IWUSR);

    if (j == -1) {
      perror("main:creat:");
    } else {
      if (write(j, foo, bytes) != bytes) {
        perror("main:write:");
      } else {
        if(close(j)) {
          perror("main:close:");
        }
      }
    }
  #endif

  return(0);
}
