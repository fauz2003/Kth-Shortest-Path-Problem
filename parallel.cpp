#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <climits>
#include <algorithm>
#include <mpi.h>
#include<omp.h>
#include <chrono>

using namespace std;

void explorePaths(int source, int destination, vector<vector<pair<int, int>>>& g, vector<vector<pair<int, vector<int>>>>& dis, int k, int rank) {
    priority_queue<pair<int, pair<int, vector<int>>>, vector<pair<int, pair<int, vector<int>>>>,
                   greater<pair<int, pair<int, vector<int>>>>>
        pq;

    pq.push({0, {source, {source}}});
    dis[source][0] = {0, {source}};

    while (!pq.empty()) {
        int u = pq.top().second.first;
        int d = pq.top().first;
        vector<int> path = pq.top().second.second;
        pq.pop();

        if (dis[u][k - 1].first < d) continue;

        for (auto &[dest, cost] : g[u]) {
            // Condition to start exploring based on rank and edge type
            if ((rank == 0 && u == source && dest % 2 == 0) || (rank == 1 && u == source && dest % 2 == 1) || (u != source)) {
                if (d + cost < dis[dest][k - 1].first) {
                    dis[dest][k - 1].first = d + cost;
                    vector<int> new_path = path;
                    new_path.push_back(dest);

                    pq.push({(d + cost), {dest, new_path}});
                    dis[dest][k - 1].second = new_path;
                    sort(dis[dest].begin(), dis[dest].end());
                }
            }
        }
    }
}

int main(int argc, char** argv) {
    omp_set_num_threads(4);
    int source=1;
    int destination=350;

    ifstream inputFile("aazan.txt"); // Open the file for reading
    MPI_Init(&argc, &argv);
    int edge_count;
    int **edges;
    int rank, sizep;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &sizep);
     string str;
	int count=0;
    int size=10000;
    int edges_per_process;
    double start_time, end_time; // Variables to hold start and end times

    start_time = MPI_Wtime(); // Start measuring time
    
  

        if (!inputFile.is_open()) {
            cout << "Error opening file!" << endl;
            return 0;
        }
    
        int **arr = new int*[size];
        for (int i = 0; i < size; ++i) {
            arr[i] = new int[size];
        }


        for(int i=0 ;i<size;i++)
        {
            #pragma omp parallel for
            for(int j=0; j< size;j++)
            {
                if(i==j)
                {
                    arr[i][j]=0;
                }
            }
        }


        string a,b;
    int x,y;
    int num=0;

        while(getline(inputFile,str) )
        {
            if(str[0] >= '0' && str[0] <= '9'){
                count++;
                int i=0;
                a="";
                b="";
                while(str[i] >= '0' && str[i] <= '9')
                {
                    a+=str[i];
                    i++;
                } 
            //	cout<<a<<" ";
        
            i++;
            while(str[i] >= '0' && str[i] <= '9')
                {
                    b+=str[i];
                    i++;
                } 
                
                x=stoi(a);
                y=stoi(b);
                if(x>=size)
                    break;
                if(y>=size){
                    y=size-1;
                }
                arr[x][y]=1;
                
            }
        
        }
        //x is the number of nodes 
        inputFile.close(); // Close the file when done
        
        

        



        count+=6;
        edges = new int*[count];
        #pragma omp parallel for
        for (int i = 0; i < count; ++i)
        {
            edges[i] = new int[3]; // Each edge has three elements
        }
        

         edge_count = 0;
    for (int i = 0; i < size; ++i) {
        //#pragma omp parallel for
        for (int j = 0; j < size; ++j) {
            if (arr[i][j] == 1) {
                edges[edge_count][0] = i ; // Adding 1 to make nodes 1-indexed
                edges[edge_count][1] = j ; // Adding 1 to make nodes 1-indexed
                edges[edge_count][2] = 1;
                edge_count++;
            }
        }
    }
        

      //  #pragma omp parallel for
    
      //  cout<<"\n\n\n";
        int n = size, m = edge_count, k = 3;
      

       // findKShortestPaths(edges, N, M, K,0,4);
       vector<vector<pair<int, int>>> g(n + 1);
        #pragma omp parallel for
    for (int i = 0; i < m; i++) {
        g[edges[i][0]].push_back({ edges[i][1], edges[i][2] });
    }

    vector<vector<pair<int, vector<int>>>> dis(n + 1, vector<pair<int, vector<int>>>(k, {INT_MAX, {}}));

  
        explorePaths(1, destination, g, dis, k,rank);
        //int recv[6];
    

    vector<int> lengths(k);
    #pragma omp parallel for
    for (int i = 0; i < k; i++) {
        lengths[i] = dis[destination][i].first;
    }

    // Collect lengths from all processes
    vector<int> all_lengths(sizep * k);
    MPI_Gather(lengths.data(), k, MPI_INT, all_lengths.data(), k, MPI_INT, 0, MPI_COMM_WORLD);

    // Print lengths (only by rank 0 process)
    if (rank == 0) {
        sort(all_lengths.begin(), all_lengths.end());

        #pragma omp parallel for
        for (int i = 0; i < k; i++) {
            cout << "Length " << (i + 1) << ": " << all_lengths[i] << endl;
        }
    }
     end_time = MPI_Wtime(); // End measuring time

    if(rank == 0) {
        printf("Parallel version execution time: %f seconds\n", end_time - start_time);
    }


    MPI_Finalize();
    
    return 0;


   
        

}