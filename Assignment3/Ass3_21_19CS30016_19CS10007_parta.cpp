#include<stdio.h>
#include<unistd.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/wait.h>
#include<sys/types.h>
#include <sys/stat.h>
#include <wait.h>
#include<iostream>
using namespace std;
#define shmsize 100
#define shmmode (SHM_R|SHM_W)
#define shmkey (key_t)31


unsigned int sizeof_dm(int rows, int cols, size_t sizeElement){
    size_t size = rows * (sizeof(void *) + (cols * sizeElement));
    return size;
}

typedef struct _process_data 
{
    double *A;
    double *B;
    double *C;
    int veclen, i, j;
    int y;
} ProcessData;

void create_index(double **m, int rows, int cols, size_t sizeElement)
{
    int i;  
    size_t sizeRow = cols * sizeElement;
    m[0] = 0;
    for(i=1; i<rows; i++){      
        m[i] = (m[i-1]+sizeRow);
    }
}

void mult(ProcessData *arg)
{
    ProcessData *val; 
    val = arg;
    int ptr;
    double res = 0; 
    double *m1,*m2,*m3;
    m1 = val->A; m2 = val->B; m3 = val->C;
    int r = val->i; int c = val->j;
    int valA,valB;
    //cout<<"i is "<<r<<" j is "<<c<<'\n';
    for(ptr = 0;ptr<val->veclen;ptr++)
    {
        valA = m1[r*(val->veclen)+ptr];
        valB = m2[ptr*(val->y)+c];
        //cout<<"valA "<<valA<<" valB "<<valB<<'\n';
        res = res + valA*valB;
    }
    //cout << "res is "<<res<<'\n';
    arg->C[r*(val->y)+c] = res;
   // cout << "update "<<arg->C[r*(val->veclen)+c] << "\n";
    return;
}

void print_matriz(double *matrix, int Rows, int Cols,int c){
    printf("\nMatrix C:\n");
        for (int i = 0; i < Rows; i++) 
        {
        for (int j = 0; j < Cols; j++)
        {
               // cout<<"i is "<<i<<"j is "<<j<<'\n';
                cout<< matrix[i * c + j]<<" ";
        }
        cout<<"\n";
    }
}

int main() 
{
    double *A,*B,*C; void **mat;
    int c1,r1,c2,r2,shmId,i,j;
     cout<< "Enter the rows and columns of matrix A:\n";
     cin >> r1 >> c1;
     cout << "Enter the rows and columns of matrix B:\n";
     cin >> r2 >> c2;
    if(r2!=c1) 
    {
        printf("\nCannot Multiply");
        return 0;
    }
    size_t sizeMatrixA = sizeof_dm(r1,c1,sizeof(double));
    size_t sizeMatrixB = sizeof_dm(r2,c2,sizeof(double));
    size_t sizeMatrixC = sizeof_dm(r1,c2,sizeof(double));
    //size_t sizeMatrix = sizeMatrixA + sizeMatrixB + sizeMatrixC;
    shmId = shmget(IPC_PRIVATE, sizeMatrixA, IPC_CREAT|0600);
    A = ( double*)shmat(shmId, NULL, 0);
    //create_index(A, r1, c1, sizeof(double));   
    
    shmId = shmget(IPC_PRIVATE, sizeMatrixB, IPC_CREAT|0600);
    B =  ( double* )shmat(shmId, NULL, 0);
  //  create_index(B, r2, c2, sizeof(double)); 

    shmId = shmget(IPC_PRIVATE, sizeMatrixC, IPC_CREAT|0600);
    C = ( double* )shmat(shmId, NULL, 0);
   // create_index(C, r1, c2, sizeof(double));   

     printf("Enter the matrix A row wise: \n");
     double v;

     for( i = 0; i < r1*c1; i++)
    {
        cin >> v;
        A[i] = v;
    }

    printf("Enter the matrix B row wise: \n");
    for( i = 0; i < r2*c2; i++)
    {
        cin >> v;
        B[i] = v;
    }
    ProcessData p;
    p.A = A;
    p.B = B;
    p.C = C;
    p.veclen = c1;
    p.y = c2;
    int flag = 0;
    int process = r1*c2;  i = 0; j = -1;
    for(int n = 0; n<process;n++)
    {
        if(j==c2-1)
        {
            i++;
            j=-1;
        }
        j++;
        p.i = i;
        p.j = j;
        //cout<<"i is "<<p.i<<" j is "<<p.j<<'\n'; 
        if (fork() == 0) 
        {
        // do the job specific to the child process
          mult(&p);
        // don't forget to exit from the child
            exit(0);
        }
    }
    int status;
    for (int n = 0; n < process; ++n)
        wait(&status);
    if(status == 0)
    {
        cout<<"\n";
        print_matriz(C,r1,c2,c2);
    }
    
    shmdt(A); shmdt(B),shmdt(C);
        shmctl(shmId, IPC_RMID, 0);
    return 0 ;
}