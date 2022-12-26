#include <mpi.h>
#include <cstdlib>
#include <ctime>
#include <cstdio>


void merge_arr(int a[], int l, int m, int r) {
    int b[r - l + 1];
    int h, i, j, k;
    i = 0;

    h = l;
    j = m + 1;

    while(h <= m && j <= r) {
        if(a[h] <= a[j]) {
            b[i] = a[h];
            h++;
        }
        else {
            b[i] = a[j];
            j++;
        }
        i++;
    }

    if(m < h) {
        for(k = j; k <= r; k++) {
            b[i] = a[k];
            i++;
        }
    }
    else {
        for(k = h; k <= m; k++) {
            b[i] = a[k];
            i++;
        }
    }

    i = 0;
    for(k = l; k <= r; k++) {
        a[k] = b[i];
        i ++;
    }
}

void merge_sort(int a[], int l, int r) {
    int m;
    if(l < r) {
        m = (l + r)/2;

        merge_sort(a, l, m);
        merge_sort(a, (m + 1), r);
        merge_arr(a, l, m, r);
    }
}


int main(int argc, char** argv) {
    int n = 100000;
    int arr[n];

    srand(time(nullptr));
    for(int i = 0; i < n; i++) {
        arr[i] = rand() % n;
//        printf("%d ", arr[i]);
    }
//    printf("\n");
    double start_time, end_time;
    int rank;
    int size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0){
        start_time = MPI_Wtime();
    }

    int arr_size = n/size;

    int sub_array[arr_size];

    MPI_Scatter(arr, arr_size, MPI_INT, sub_array, arr_size, MPI_INT, 0, MPI_COMM_WORLD);

    merge_sort(sub_array, 0, (arr_size - 1));

    int sorted[n];

    MPI_Gather(sub_array, arr_size, MPI_INT, sorted, arr_size, MPI_INT, 0, MPI_COMM_WORLD);

    if(rank == 0) {
        merge_sort(sorted, 0, (n - 1));

//        for(int i = 0; i < n; i++) {
//            printf("%d ", sorted[i]);
//        }
        end_time = MPI_Wtime();
        printf("%f ", end_time - start_time);
    }
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
}
