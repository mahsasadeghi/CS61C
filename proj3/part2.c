#include <emmintrin.h>
#include <string.h>
#include <stdio.h>
#include <omp.h>
#define KERNX 3     //this is the x-size of the kernel. It will always be odd.
#define KERNY 3     //this is the y-size of the kernel. It will always be odd.
int conv2D(float* in, float* out, int data_size_X, int data_size_Y,
           float* kernel)
{
    
    // the x coordinate of the kernel's center
    int kern_cent_X = (KERNX - 1)/2;
    // the y coordinate of the kernel's center
    int kern_cent_Y = (KERNY - 1)/2;
    
    // Loading and fliping kernel's elements to 9 intrinsics as vector
    __m128 kElem0_vector = _mm_load1_ps(kernel+8);          // Loading four copies of the last element of the kernel to kElem0_vecotr instrinsic
    __m128 kElem1_vector = _mm_load1_ps(kernel+7);
    __m128 kElem2_vector = _mm_load1_ps(kernel+6);
    __m128 kElem3_vector = _mm_load1_ps(kernel+5);
    __m128 kElem4_vector = _mm_load1_ps(kernel+4);
    __m128 kElem5_vector = _mm_load1_ps(kernel+3);
    __m128 kElem6_vector = _mm_load1_ps(kernel+2);
    __m128 kElem7_vector = _mm_load1_ps(kernel+1);
    __m128 kElem8_vector = _mm_load1_ps(kernel+0);
    
    
    
    // main convolution loop
    int x;
    int y;
    
#pragma omp parallel for firstprivate(data_size_X, data_size_Y) private(x)
    // Handling all rows besides the 0th and data_size_Yth
    for(y = 1; y < data_size_Y - 1; y++){
        float* out_matrix;
        __m128 out_vector;
        
        //Handeling the first element of each row                                   // Can we vectorize this???
        out[y*data_size_X] += kernel[7] * in[(y-1) * data_size_X];
        out[y*data_size_X] += kernel[6] * in[1 + (y-1) * data_size_X];
        out[y*data_size_X] += kernel[4] * in[y * data_size_X];
        out[y*data_size_X] += kernel[3] * in[1 + y * data_size_X];
        out[y*data_size_X] += kernel[1] * in[(y+1) * data_size_X];
        out[y*data_size_X] += kernel[0] * in[1 + (y+1) * data_size_X];
        
        // Handeling first element to (data_size_X - 1)th element of each row
        for(x = 1; x + 3 < data_size_X - 1; x += 4){
            out_matrix = out+x+y*data_size_X;
            out_vector = _mm_setzero_ps();
            
            // Saay konam in ro joda benevisam --> in_vector bejaye in
            out_vector = _mm_add_ps(out_vector, _mm_mul_ps(kElem0_vector, _mm_loadu_ps(in + x-1 + (y-1) * data_size_X)));
            out_vector = _mm_add_ps(out_vector, _mm_mul_ps(kElem1_vector, _mm_loadu_ps(in + x + (y-1) * data_size_X)));
            out_vector = _mm_add_ps(out_vector, _mm_mul_ps(kElem2_vector, _mm_loadu_ps(in + x+1 + (y-1) * data_size_X)));
            out_vector = _mm_add_ps(out_vector, _mm_mul_ps(kElem3_vector, _mm_loadu_ps(in + x-1 + y * data_size_X)));
            out_vector = _mm_add_ps(out_vector, _mm_mul_ps(kElem4_vector, _mm_loadu_ps(in + x + y * data_size_X)));
            out_vector = _mm_add_ps(out_vector, _mm_mul_ps(kElem5_vector, _mm_loadu_ps(in + x+1 + y * data_size_X)));
            out_vector = _mm_add_ps(out_vector, _mm_mul_ps(kElem6_vector, _mm_loadu_ps(in + x-1 + (y+1) * data_size_X)));
            out_vector = _mm_add_ps(out_vector, _mm_mul_ps(kElem7_vector, _mm_loadu_ps(in + x + (y+1) * data_size_X)));
            out_vector = _mm_add_ps(out_vector, _mm_mul_ps(kElem8_vector, _mm_loadu_ps(in + x+1 + (y+1) * data_size_X)));
            
            _mm_storeu_ps(out_matrix, out_vector);
            
        }
        
        // Handling cases that data_size-1 is not divisible by four [tail handling for inner elements]
        for(;x < data_size_X - 1; x++){
            out[x+y*data_size_X] += kernel[8] * in[x-1 + (y-1) * data_size_X];
            out[x+y*data_size_X] += kernel[7] * in[x + (y-1) * data_size_X];
            out[x+y*data_size_X] += kernel[6] * in[x+1 + (y-1) * data_size_X];
            out[x+y*data_size_X] += kernel[5] * in[x-1 + y * data_size_X];
            out[x+y*data_size_X] += kernel[4] * in[x + y * data_size_X];
            out[x+y*data_size_X] += kernel[3] * in[x+1 + y * data_size_X];
            out[x+y*data_size_X] += kernel[2] * in[x-1 + (y+1) * data_size_X];
            out[x+y*data_size_X] += kernel[1] * in[x + (y+1) * data_size_X];
            out[x+y*data_size_X] += kernel[0] * in[x+1 + (y+1) * data_size_X];
        }
        
        // Handling last element of each row
        out[(y+1)*data_size_X-1] += kernel[8] * in[x-1 + (y-1) * data_size_X];
        out[(y+1)*data_size_X-1] += kernel[7] * in[x + (y-1) * data_size_X];
        out[(y+1)*data_size_X-1] += kernel[5] * in[x-1 + y * data_size_X];
        out[(y+1)*data_size_X-1] += kernel[4] * in[x + y * data_size_X];
        out[(y+1)*data_size_X-1] += kernel[2] * in[x-1 + (y+1) * data_size_X];
        out[(y+1)*data_size_X-1] += kernel[1] * in[x + (y+1) * data_size_X];
        
    }
    
    // Handeling 0th row except 0th and last element
    for(x = 1; x + 3 < data_size_X - 1; x += 4){
        
        // **
        float* out_matrix = out+x;
        __m128 out_vector = _mm_setzero_ps();
        
        out_vector = _mm_add_ps(out_vector, _mm_mul_ps(kElem3_vector, _mm_loadu_ps(in + x-1)));
        out_vector = _mm_add_ps(out_vector, _mm_mul_ps(kElem4_vector, _mm_loadu_ps(in + x )));
        out_vector = _mm_add_ps(out_vector, _mm_mul_ps(kElem5_vector, _mm_loadu_ps(in + x+1)));
        out_vector = _mm_add_ps(out_vector, _mm_mul_ps(kElem6_vector, _mm_loadu_ps(in + x-1 + data_size_X)));
        out_vector = _mm_add_ps(out_vector, _mm_mul_ps(kElem7_vector, _mm_loadu_ps(in + x + data_size_X)));
        out_vector = _mm_add_ps(out_vector, _mm_mul_ps(kElem8_vector, _mm_loadu_ps(in + x+1 + data_size_X)));
        _mm_storeu_ps(out_matrix, out_vector);
        
    }
    
    // Tail handling for the first row
    for (; x<data_size_X-1; x ++) {
        out[x] += kernel[5] * in[x-1];
        out[x] += kernel[4] * in[x];
        out[x] += kernel[3] * in[x+1];
        out[x] += kernel[2] * in[x-1 + data_size_X];
        out[x] += kernel[1] * in[x + data_size_X];
        out[x] += kernel[0] * in[x+1 + data_size_X];
    }
    
    // Handeling last row except 0th and last element
    for(x = 1; x + 3 < data_size_X - 1; x += 4){
        float* out_matrix = out+x+((data_size_Y-1) * data_size_X);
        __m128 out_vector = _mm_setzero_ps();
        
        out_vector = _mm_add_ps(out_vector, _mm_mul_ps(kElem0_vector, _mm_loadu_ps(in + x-1 + (data_size_Y-2) * data_size_X)));
        out_vector = _mm_add_ps(out_vector, _mm_mul_ps(kElem1_vector, _mm_loadu_ps(in + x + (data_size_Y-2) * data_size_X)));
        out_vector = _mm_add_ps(out_vector, _mm_mul_ps(kElem2_vector, _mm_loadu_ps(in + x+1 + (data_size_Y-2)*data_size_X)));
        out_vector = _mm_add_ps(out_vector, _mm_mul_ps(kElem3_vector, _mm_loadu_ps(in + x-1 + (data_size_Y-1)*data_size_X)));
        out_vector = _mm_add_ps(out_vector, _mm_mul_ps(kElem4_vector, _mm_loadu_ps(in + x + (data_size_Y-1)*data_size_X)));
        out_vector = _mm_add_ps(out_vector, _mm_mul_ps(kElem5_vector, _mm_loadu_ps(in + x+1 + (data_size_Y-1)*data_size_X)));
        _mm_storeu_ps(out_matrix, out_vector);
    }
    
    // Tail handling for the last row
    for (; x<data_size_X-1; x ++) {
        out[x+(data_size_Y-1)*data_size_X] += kernel[8] * in[x-1 + (data_size_Y-2)*data_size_X];
        out[x+(data_size_Y-1)*data_size_X] += kernel[7] * in[x + (data_size_Y-2)*data_size_X];
        out[x+(data_size_Y-1)*data_size_X] += kernel[6] * in[x+1 + (data_size_Y-2)*data_size_X];
        out[x+(data_size_Y-1)*data_size_X] += kernel[5] * in[x-1 + (data_size_Y-1)*data_size_X];
        out[x+(data_size_Y-1)*data_size_X] += kernel[4] * in[x + (data_size_Y-1)*data_size_X];
        out[x+(data_size_Y-1)*data_size_X] += kernel[3] * in[x+1 + (data_size_Y-1)*data_size_X];
    }
    
    
    
    // Special cases:
    // Handeling case [0][0]
    out[0] += kernel[4] * in[0];
    out[0] += kernel[3] * in[1];
    out[0] += kernel[1] * in[data_size_X];
    out[0] += kernel[0] * in[1 + data_size_X];
    
    // Handeling case [0][data_size_X-1]
    out[data_size_X-1] += kernel[5] * in[data_size_X-2];
    out[data_size_X-1] += kernel[4] * in[data_size_X-1];
    out[data_size_X-1] += kernel[2] * in[2 * data_size_X-2];
    out[data_size_X-1] += kernel[1] * in[2 * data_size_X-1];
    
    // Handeling case [data_size_Y-1][0]
    out[(data_size_Y-1)*data_size_X] += kernel[7] * in[(data_size_Y-2) * data_size_X];
    out[(data_size_Y-1)*data_size_X] += kernel[6] * in[1 + (data_size_Y-2) * data_size_X];
    out[(data_size_Y-1)*data_size_X] += kernel[4] * in[(data_size_Y-1) * data_size_X];
    out[(data_size_Y-1)*data_size_X] += kernel[3] * in[1 + (data_size_Y-1) * data_size_X];
    
    // Handeling case [data_size_Y-1][data_size_X-1]
    out[data_size_Y*data_size_X-1] += kernel[8] * in[data_size_X-2 + (data_size_Y-2)*data_size_X];
    out[data_size_Y*data_size_X-1] += kernel[7] * in[data_size_X-1 + (data_size_Y-2)*data_size_X];
    out[data_size_Y*data_size_X-1] += kernel[5] * in[data_size_X-2 + (data_size_Y-1)*data_size_X];
    out[data_size_Y*data_size_X-1] += kernel[4] * in[data_size_X-1 + (data_size_Y-1)*data_size_X];
    
    
    return 1;
    
}