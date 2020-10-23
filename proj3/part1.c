#include <emmintrin.h>
#include <string.h>
#include <stdio.h>
#define KERNX 3 //this is the x-size of the kernel. It will always be odd.
#define KERNY 3 //this is the y-size of the kernel. It will always be odd.
int conv2D(float* in, float* out, int data_size_X, int data_size_Y,
           float* kernel)
{
    // the x coordinate of the kernel's center
    int kern_cent_X = (KERNX - 1)/2;
    // the y coordinate of the kernel's center
    int kern_cent_Y = (KERNY - 1)/2;
    
    int kernel_size = KERNX*KERNY;
    
    // flipping the kernel : reverse the order of the array
    // the flipped kernel is simply in order of 0, 1, 2, 3, 4, 5, 6, 7, 8 when going through the traversal
    
    float flipped_kernel[kernel_size];
    for (int i = 0; i<kernel_size; i++) {
        flipped_kernel[i] = kernel[kernel_size-1-i];
    }
    
    int canvas_x = data_size_X + 2;
    int canvas_y = data_size_Y + 2;
    
    // pading the the matrix and assigning it with the name canvas
    
    int canvas_size = canvas_x * canvas_y;
    
    // filling the canvas memery with floating point value 0
    float *canvas = (float*)calloc(canvas_size, sizeof(float));
    
    for (int row = 1; row <= data_size_Y; row++) {
        for (int col = 1; col <= data_size_X; col++){
            canvas[col+row*(canvas_x)] = in[(col-1) + (row-1)*data_size_X];
        }
    }
    
    /***
     ////////////////////////////////////////////////////////////////////////////////
     //////// sample y-x-i-j traverse (with original unflipped kernel) //////////////
     ////////////////////////////////////////////////////////////////////////////////
     *  x = 1, y = 0, i = -1, j = -1              out[1] += kernel[8] * in[1]
     *  x = 1, y = 0, i = -1, j = 0               out[1] += kernel[5] * in[2]
     *  x = 1, y = 0, i = -1, j = 1               out[1] += kernel[2] * in[3]
     *  x = 1, y = 0, i = 0,  j = -1              out[1] += kernel[7] * in[13]
     *  x = 1, y = 0, i = 0,  j = 0               out[1] += kernel[4] * in[14]
     *  x = 1, y = 0, i = 0,  j = 1               out[1] += kernel[1] * in[15]
     *  x = 1, y = 0, i = 1,  j = -1              out[1] += kernel[6] * in[25]
     *  x = 1, y = 0, i = 1,  j = 0               out[1] += kernel[3] * in[26]
     *  x = 1, y = 0, i = 1,  j = 1               out[1] += kernel[0] * in[27]
     
     *  x = 2, y = 0, i = -1, j = -1              out[2] += kernel[8] * in[2]
     *  x = 2, y = 0, i = -1, j = 0               out[2] += kernel[5] * in[3]
     *  x = 2, y = 0, i = -1, j = 1               out[2] += kernel[2] * in[4]
     *  x = 2, y = 0, i = 0,  j = -1              out[2] += kernel[7] * in[14]
     *  x = 2, y = 0, i = 0,  j = 0               out[2] += kernel[4] * in[15]
     *  x = 2, y = 0, i = 0,  j = 1               out[2] += kernel[1] * in[16]
     *  x = 2, y = 0, i = 1,  j = -1              out[2] += kernel[6] * in[26]
     *  x = 2, y = 0, i = 1,  j = 0               out[2] += kernel[3] * in[27]
     *  x = 2, y = 0, i = 1,  j = 1               out[2] += kernel[0] * in[28]
     
     
     * //////////////////////////////////////////////// to be vectorized ///////////////////////////////////////////////////////////////////////////
     *  for(int y = 0; y < data_size_Y; y++){ // the y coordinate of theoutput location we're focusing on
     *       for(int x = 0; x < data_size_X; x+=4){ // the x coordinate of the output location we're focusing on
     *           for(int i = -kern_cent_X; i <= kern_cent_X; i++){ // kernel unflipped x coordinate
     *               for(int j = -kern_cent_Y; j <= kern_cent_Y; j++){ // kernel unflipped y coordinate
     *                  out[x+y*data_size_X] += kernel[(kern_cent_X-i)+(kern_cent_Y-j)*KERNX] * canvas[(x+j+kern_cent_X) + (y+i+kern_cent_Y)*(data_size_X+2)];
     *                  out[x+1+y*data_size_X] += kernel[(kern_cent_X-i)+(kern_cent_Y-j)*KERNX] * canvas[(x+1+j+kern_cent_X) + (y+i+kern_cent_Y)*(data_size_X+2)];
     *                  out[x+2+y*data_size_X] += kernel[(kern_cent_X-i)+(kern_cent_Y-j)*KERNX] * canvas[(x+2+j+kern_cent_X) + (y+i+kern_cent_Y)*(data_size_X+2)];
     *                  out[x+3+y*data_size_X] += kernel[(kern_cent_X-i)+(kern_cent_Y-j)*KERNX] * canvas[(x+3+j+kern_cent_X) + (y+i+kern_cent_Y)*(data_size_X+2)];
     *               }
     *           }
     *      }
     *  }
     *
     *
     *      TODO:
     *          vectorized --> out[x+y*data_size_X]
     *          vectorized --> kernel[(kern_cent_X-i)+(kern_cent_Y-j)*KERNX]
     *          vectorized --> canvas[(x+j+kern_cent_X) + (y+i+kern_cent_Y)*(data_size_X+2)] ***/
    
    
    
    // main convolution loop
    int x;
    int y;
    
    for(y = 0; y < data_size_Y; y++){
        for(x = 0; x + 15 < data_size_X; x+=16){
            
            float * out_matrix;
            __m128 out_vector;
            
            
            out_matrix = out+x+y*data_size_X;
            out_vector = _mm_setzero_ps(); // initializing 4 element of 0's for output vector
            
            // since kernel values are in order and close to each other,
            // it is faster to traverse through many kernel values at the same time
            for (int i = 0; i < kernel_size; i++) {
                __m128 kernel_vector = _mm_load1_ps(flipped_kernel+i); // loading the first 4 elements from the flipped kernel
                __m128 canvas_vector = _mm_loadu_ps(canvas + (x+ i%KERNX) + (y+i/KERNY)*(canvas_x)); // j is replaced with kernel size
                out_vector = _mm_add_ps(out_vector, _mm_mul_ps(kernel_vector, canvas_vector));
            }
            _mm_storeu_ps(out_matrix, out_vector);
            
            
            // unrolling the same thing like the above block with ----- x+4
            out_matrix = out+(x+4)+y*data_size_X;
            out_vector = _mm_setzero_ps();
            for (int i = 0; i < kernel_size; i++) {
                __m128 kernel_vector = _mm_load1_ps(flipped_kernel+i);
                __m128 canvas_vector = _mm_loadu_ps(canvas + (x+4 + i%KERNX) + (y + i/KERNY)*(canvas_x));
                out_vector = _mm_add_ps(out_vector, _mm_mul_ps(kernel_vector, canvas_vector));
            }
            _mm_storeu_ps(out_matrix, out_vector);
            
            
            // unrolling the same thing like the above block with ----- x+8
            out_matrix = out+(x+8)+y*data_size_X;
            out_vector = _mm_setzero_ps();
            for (int i = 0; i < kernel_size; i++) {
                __m128 kernel_vector = _mm_load1_ps(flipped_kernel+i);
                __m128 canvas_vector = _mm_loadu_ps(canvas + (x+8 + i%KERNX) + (y + i/KERNY)*(canvas_x));
                out_vector = _mm_add_ps(out_vector, _mm_mul_ps(kernel_vector, canvas_vector));
            }
            _mm_storeu_ps(out_matrix, out_vector);
            
            
            // unrolling the same thing like the above block with ---- x+12
            out_matrix = out+(x+12)+y*data_size_X;
            out_vector = _mm_setzero_ps();
            for (int i = 0; i < kernel_size; i++) {
                __m128 kernel_vector = _mm_load1_ps(flipped_kernel+i);
                __m128 canvas_vector = _mm_loadu_ps(canvas + (x+12 + i%KERNX) + (y + i/KERNY)*(canvas_x));
                out_vector = _mm_add_ps(out_vector, _mm_mul_ps(kernel_vector, canvas_vector));
            }
            _mm_storeu_ps(out_matrix, out_vector);
        }
        
        // tail handling for x variable
        for (; x<data_size_X; x ++) {
            for (int i = 0; i < kernel_size; i++) {
                out[x+y*data_size_X] += flipped_kernel[i] * canvas[x + i%KERNX + (y + i/KERNY)*(canvas_x)];
            }
        }
    }
    
    free(canvas);
    return 1;
    
}
