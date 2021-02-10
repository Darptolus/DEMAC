// Define Operations

#ifndef N_SIZE
#define N_SIZE 10
#endif

#define END_ID 999 //Finish program
#define ACK_ID 1
#define REC_ID 2
#define SND_ID 3
#define VAR_ID 10 //Vector addition (Reduction)
#define VAE_ID 11 //Vector addition (element by element)
#define GMM_ID 15 //Matrix Multiplication (N_SIZE x N_SIZE)

//Vector addition (Reduction)
int v_add(int vect[N_SIZE]){
  int sum = 0, i;
  for (i = 0; i < N_SIZE; i++){
    sum = vect[i] + sum;
  }
  return sum;
}
//Vector addition (element by element)

// Matrix Multiplication (N_SIZE x N_SIZE)
void mult_m(int mat1[][N_SIZE], int mat2[][N_SIZE], int res[][N_SIZE]){ 
    int i, j, k; 
    for (i = 0; i < N_SIZE; i++){ 
        for (j = 0; j < N_SIZE; j++){ 
            res[i][j] = 0; 
            for (k = 0; k < N_SIZE; k++) 
                res[i][j] += mat1[i][k] * mat2[k][j]; 
        } 
    } 
} 

// Convolution

// Relu