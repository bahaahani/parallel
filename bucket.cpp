#include <mpi.h>
#include <iostream>
#include <vector>
#include <cstdlib>

// Define the bucket structure
struct Bucket
{
    std::vector<int> data;
};

// Function to distribute elements to buckets
void distribute(const std::vector<int> &arr, std::vector<Bucket> &buckets, int num_buckets)
{
    for (int i = 0; i < arr.size(); i++)
    {
        int index = arr[i] % num_buckets; // Distribute using modulo for simplicity
        buckets[index].data.push_back(arr[i]);
    }
}

// Function to sort the buckets individually
void sortBuckets(std::vector<Bucket> &buckets)
{
    for (auto &bucket : buckets)
    {
        std::sort(bucket.data.begin(), bucket.data.end());
    }
}

// Function to merge the sorted buckets
void merge(const std::vector<Bucket> &buckets, std::vector<int> &arr)
{
    int index = 0;
    for (const auto &bucket : buckets)
    {
        for (int num : bucket.data)
        {
            arr[index++] = num;
        }
    }
}

// Function to print the array
void print(const std::vector<int> &arr)
{
    for (int num : arr)
    {
        std::cout << num << " ";
    }
    std::cout << std::endl;
}

int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);

    int world_size, world_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    int n;
    std::vector<int> data;
    int num_buckets = world_size;
    std::vector<Bucket> buckets(num_buckets);

    if (world_rank == 0)
    {
        // Initialize data
        n = 100; // Example size
        data.resize(n);
        for (int i = 0; i < n; i++)
        {
            data[i] = rand() % (num_buckets * 10);
        }
    }

    // Broadcast the total number of elements to all processes
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Each process calculates the number of elements it will receive
    int local_size = (n / world_size) + (world_rank < (n % world_size) ? 1 : 0);
    std::vector<int> local_data(local_size);

    // Perform the cyclic distribution
    std::vector<int> sendcounts(world_size), displs(world_size);
    if (world_rank == 0)
    {
        for (int i = 0; i < world_size; ++i)
        {
            sendcounts[i] = (n / world_size) + (i < (n % world_size) ? 1 : 0);
            displs[i] = (i > 0) ? displs[i - 1] + sendcounts[i - 1] : 0;
        }
    }

    MPI_Scatterv(data.data(), sendcounts.data(), displs.data(), MPI_INT,
                 local_data.data(), local_size, MPI_INT, 0, MPI_COMM_WORLD);

    // Distribute elements to local buckets
    distribute(local_data, buckets, num_buckets);

    // Sort local buckets
    sortBuckets(buckets);

    // Prepare data for gathering
    std::vector<int> sorted_local_data;
    for (const auto &bucket : buckets)
    {
        sorted_local_data.insert(sorted_local_data.end(), bucket.data.begin(), bucket.data.end());
    }

    // Gather the sorted data back to root
    std::vector<int> recvcounts(world_size), rdispls(world_size);
    if (world_rank == 0)
    {
        for (int i = 0; i < world_size; ++i)
        {
            recvcounts[i] = sendcounts[i];
            rdispls[i] = displs[i];
        }
    }

    MPI_Gatherv(sorted_local_data.data(), sorted_local_data.size(), MPI_INT,
                data.data(), recvcounts.data(), rdispls.data(), MPI_INT, 0, MPI_COMM_WORLD);

    // Root process merges and prints the sorted data
    if (world_rank == 0)
    {
        print(data);
    }

    MPI_Finalize();
    return 0;
}
