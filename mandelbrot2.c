#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <png.h>

int mandelbrot(double real, double imag, int max_iter)
{
    double z_real = real;
    double z_imag = imag;
    for (int i = 0; i < max_iter; i++)
    {
        double r2 = z_real * z_real, i2 = z_imag * z_imag;
        if (r2 + i2 > 4.0)
            return i;
        z_imag = 2 * z_real * z_imag + imag;
        z_real = r2 - i2 + real;
    }
    return max_iter;
}

void write_png(const char *filename, int width, int height, int *buffer, int max_iter)
{
    FILE *fp = fopen(filename, "wb");
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    png_infop info_ptr = png_create_info_struct(png_ptr);
    png_init_io(png_ptr, fp);
    png_set_IHDR(png_ptr, info_ptr, width, height, 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
    png_write_info(png_ptr, info_ptr);

    png_bytep row = (png_bytep)malloc(3 * width * sizeof(png_byte));
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int iter = buffer[y * width + x];
            png_byte color = (iter == max_iter) ? 0 : (255 * iter / max_iter);
            row[x * 3] = color;     // Red
            row[x * 3 + 1] = color; // Green
            row[x * 3 + 2] = color; // Blue
        }
        png_write_row(png_ptr, row);
    }
    free(row);

    png_write_end(png_ptr, NULL);
    fclose(fp);
    png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);
    png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
}

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);

    int world_size, world_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    int width = 800, height = 600;
    double min_real = -2.0, max_real = 1.0, min_imag = -1.2;
    double max_imag = 1.2; // The top edge of the Mandelbrot set image
    double scale_real = (max_real - min_real) / width;
    double scale_imag = (max_imag - min_imag) / height;
    int max_iter = 1000;

    int rows_per_proc = height / world_size;
    int start_row = world_rank * rows_per_proc;
    int end_row = (world_rank == world_size - 1) ? height : (world_rank + 1) * rows_per_proc;
    int *sub_image = malloc(width * (end_row - start_row) * sizeof(int));

    for (int i = start_row; i < end_row; i++)
    {
        for (int j = 0; j < width; j++)
        {
            double real = min_real + j * scale_real;
            double imag = max_imag - i * scale_imag; // Flip the sign for correct orientation
            int color = mandelbrot(real, imag, max_iter);
            sub_image[(i - start_row) * width + j] = color;
        }
    }

    int *full_image;
    if (world_rank == 0)
    {
        full_image = malloc(width * height * sizeof(int));
    }

    MPI_Gather(sub_image, width * (end_row - start_row), MPI_INT, full_image, width * (end_row - start_row), MPI_INT, 0, MPI_COMM_WORLD);
    free(sub_image);

    if (world_rank == 0)
    {
        write_png("mandelbrot2.png", width, height, full_image, max_iter);
        free(full_image);
    }

    MPI_Finalize();
    return 0;
}
