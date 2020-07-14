#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include <xmmintrin.h>
#include <immintrin.h>
#define t 0.01f
#define Pi 3.1415926535f
#define U(a, b, c) U[((c) * Ny + a)*Nx + b ]
#define P(a, b) P[(a)*Nx + b]
#define CHz 2.3e+9

int Nx, Ny, Nt;
float* U;
float* P;
const float Xa = 0.0f;
const float Xb = 4.0f;
const float Ya = 0.0f;
const float Yb = 4.0f;


float Hx2;
float Hy2;

const float Sx = 1.0f;
float Sy;
const float tmp = 2 * Pi * 1.0f;
const float y2 = 4.0f * 4.0f;


typedef unsigned long long ull;

 ull rdtsc() {
	unsigned int lo, hi;
	asm volatile ( "rdtsc\n" : "=a" (lo), "=d" (hi) );
	return ((ull)hi << 32) | lo;
}

float f(int n){
    
         return expf(-(tmp*tmp*(n*t-1.5)*(n*t-1.5))/y2)*sinf(tmp*(n*t-1.5));
}
  
        


float p(int i, int j) {
   return (j < Nx/2)? 0.1f * 0.1f: 0.2f * 0.2f ;
}


int main(int argc, char* argv[]){
    int prevlayer = 0;
    int currentlayer = 1;

    
   
    if (argc < 3)
    {
        printf("Not enought argumets");
        exit(1);
    }

    Nx = atoi(argv[1]);
    Ny = atoi(argv[2]);
    Nt = atoi(argv[3]);

    Hx2 = Xb*Xb/((Nx-1)*(Nx-1));
    Hy2 = Yb*Yb/((Ny-1)*(Ny-1));
    float opthx = 0.5/(Hx2);
    __m256 vopthx = _mm256_set1_ps(opthx);
    float opthy = 0.5/(Hy2);
    __m256 vopthy = _mm256_set1_ps(opthy);
    Sy = Ny/2;
    float tau2=t*t;
    __m256 vtau2 = _mm256_set1_ps(tau2);
    float two =2.0f;
    __m256 vtwo = _mm256_set1_ps(two);

    // U = (float*)malloc(sizeof(float)*Nx*Ny*2);
    // P = (float*)malloc(sizeof(float)*Nx*Ny);
    if(posix_memalign((void**)&U, 32, 2*Nx*Ny*sizeof(float)) != 0){
        exit(1);
    }
    if(posix_memalign((void**)&P, 32, Nx*Ny*sizeof(float)) != 0){
        exit(1);
    }

   for(int i = 0; i < Ny; i++){
        for(int j = 0; j < Nx; j++){
                P(i,j) = p(i,j);
            }
        
    } 

    
    for(int n = 0; n < 2; n++){
        for(int i = 0; i < Ny; i++){
            for(int j = 0; j < Nx; j++){
                U(i,j,n) = 0.0f; 
            }
        }   
    }
    __m256 *VU = (__m256*)U;
    __m256 *VP = (__m256*)P;

    ull begin = rdtsc();
    for(int n = 1; n <= Nt; n++){
        for(int i = 1; i < Ny-1; i++){
            for(int j = 1; j < Nx/8-1; j++){
                int ijc = ((currentlayer)* Ny + i)*Nx/8 + j;
                int ijc2 = ((prevlayer) * Ny + i)*Nx/8 + j;
                int ij = (i)*Nx/8 + j;
                __m256 vl = _mm256_loadu_ps(&(U(i,j*8-1,currentlayer)));
                __m256 vr = _mm256_loadu_ps(&(U(i,j*8+1,currentlayer)));
                __m256 vb = VU[ijc];
                // printf("%lf %lf %lf %lf %lf %lf %lf %lf\n", VU[ijc][0],VU[ijc][1],VU[ijc][2],VU[ijc][3],VU[ijc][4],VU[ijc][5],VU[ijc][6],VU[ijc][7]);
                __m256 pb = VP[ij];
                __m256 pu = _mm256_loadu_ps(&(P(i-1,j*8-1)));
                __m256 pd = _mm256_loadu_ps(&(P(i,j*8-1)));

                // VU[ijc2] = _mm256_add_ps(_mm256_sub_ps(_mm256_add_ps(vb,vb) , VU[ijc2]) , _mm256_mul_ps(vtau2, _mm256_add_ps( _mm256_mul_ps(_mm256_add_ps(_mm256_mul_ps(_mm256_sub_ps(vr,vb) ,_mm256_add_ps(VP[ij-Nx/8],pb)) , _mm256_mul_ps(_mm256_sub_ps(vl, vb) ,_mm256_add_ps(pu, pd))), vopthx)
                //  , _mm256_mul_ps(_mm256_add_ps(_mm256_mul_ps(_mm256_sub_ps(VU[ijc+Nx/8],vb) ,_mm256_add_ps(pd,pb)) , _mm256_mul_ps(_mm256_sub_ps(VU[ijc-Nx/8],vb) ,_mm256_add_ps(pu,VP[ij-Nx/8]))),vopthy))));

                 VU[ijc2] = _mm256_fmadd_ps(vtau2, _mm256_fmadd_ps(_mm256_add_ps(_mm256_mul_ps(_mm256_sub_ps(vr,vb) ,_mm256_add_ps(VP[ij-Nx/8],pb)) , _mm256_mul_ps(_mm256_sub_ps(vl, vb) ,_mm256_add_ps(pu, pd))), vopthx
                 , _mm256_mul_ps(_mm256_add_ps(_mm256_mul_ps(_mm256_sub_ps(VU[ijc+Nx/8],vb) ,_mm256_add_ps(pd,pb)) , _mm256_mul_ps(_mm256_sub_ps(VU[ijc-Nx/8],vb) ,_mm256_add_ps(pu,VP[ij-Nx/8]))),vopthy)),_mm256_fmsub_ps(vb,vtwo,VU[ijc2]));


            } 
            
            for(int k = 1; k < 8; k++){
                U(i,k,prevlayer) = 2 * U(i, k, currentlayer) - U(i, k, prevlayer) + 0.0001f*( 
                ((U(i,k+1,currentlayer)-U(i, k, currentlayer)) *(P(i-1, k)+P(i, k)) + (U(i,k-1,currentlayer)-U(i, k, currentlayer)) *(P(i-1, k-1)+P(i, k-1)))*opthx
                 + ((U(i+1,k,currentlayer)-U(i, k, currentlayer)) *(P(i, k-1)+P(i, k)) + (U(i-1,k,currentlayer)-U(i, k, currentlayer)) *(P(i-1, k-1)+P(i-1, k)))*opthy);
            }

        }

        U((int)Sy,(int)Sx, prevlayer) += (0.0001f)*f(n);
        prevlayer = (prevlayer + 1) % 2;
        currentlayer = (currentlayer + 1) % 2;
            
    }
    ull end = rdtsc();

    printf("%lf\n", ((end - begin)/(CHz)));

    FILE * file_name;
    file_name = fopen("data0000150033.dat","w");
    for(int i = 0; i < Ny; i++){
        for(int j = 0; j < Nx; j++){
                fprintf(file_name, "%d %d %.10lf\n", j, i, U(i,j,prevlayer));
            }
        
    }
    fclose(file_name);

    return 0;
}