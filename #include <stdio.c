#include <stdio.h>
#include <mpi.h>

int main(){
    int done =0, n, pid, pnum,i;
    double PI = 3.1415926535897932;
    double mypi,pi,h,sum,x;
    MPI_STATUS status;

    MPI_INIT(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &pnum);
    MPI_Comm_rank(MPI_COMM_WORLD, &pid);

    if (pid==0){
        printf("Enter number of intervals");
        scanf("%d", &n);
    }

    MPI_Bcast( &n, 1 , MPI_INT , int root , MPI_COMM_WORLD);
    h = 1.0 / (double)n;
    sum = 0.0;

    for (i=pid+1;i<=n;i+=pnum){
        x = h *((double)i-0.5);
        sum +=h *4.0 / (1.0 + x*x);
    }
    mpi=sum;
    if(pid !=0){
        MPI_Send( &mypi ,1 , MPI_DOUBLE ,0 ,0 , MPI_Comm_WORLD);
    }
    else{
        pi = mypi;
        for(i=1;i<pnum;i++){
            MPI_Recv( &mypi ,1 , MPI_DOUBLE , i , 0 , MPI_Comm_WORLD, &status);
            pi += mypi;
        }
        printf("Approx pi is ,", pi);
    }
MPI_Finalize();
return 0;
}