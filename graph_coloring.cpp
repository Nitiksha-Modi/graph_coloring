#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<string.h>
#include<stdlib.h>
#include<map>
#include<set>
#include <cstdio>
#include<algorithm>

using namespace std;

//function to split a string
vector<string> split(string str, string dlm){
    char* cstr=const_cast<char*>(str.c_str());
    char* current;
    vector<string> arr;
    current=strtok(cstr,dlm.c_str());
    while(current!=NULL){
        arr.push_back(current);
        current=strtok(NULL,dlm.c_str());
	}
    return arr;
}

//priority queue with change key
class MinIndexedPQ {
    int NMAX, N, *heap, *index, *keys;

    void swap(int i, int j) {
        int t = heap[i]; heap[i] = heap[j]; heap[j] = t;
        index[heap[i]] = i; index[heap[j]] = j;
    }

    void bubbleUp(int k)    {
        while(k > 1 && keys[heap[k/2]] > keys[heap[k]])   {
            swap(k, k/2);
            k = k/2;
        }
    }

    void bubbleDown(int k)  {
        int j;
        while(2*k <= N) {
            j = 2*k;
            if(j < N && keys[heap[j]] > keys[heap[j+1]])
                j++;
            if(keys[heap[k]] <= keys[heap[j]])
                break;
            swap(k, j);
            k = j;
        }
    }

public:
    // Create an empty MinIndexedPQ which can contain atmost NMAX elements
    MinIndexedPQ(int NMAX)  {
        this->NMAX = NMAX;
        N = 0;
        keys = new int[NMAX + 1];
        heap = new int[NMAX + 1];
        index = new int[NMAX + 1];
        for(int i = 0; i <= NMAX; i++)
            index[i] = -1;
    }
	
    ~MinIndexedPQ() {
        delete [] keys;
        delete [] heap;
        delete [] index;
    }

    // check if the PQ is empty
    bool isEmpty()  {
        return N == 0;
    }

    // check if i is an index on the PQ
    bool contains(int i)    {
        return index[i] != -1;
    }

    // return the number of elements in the PQ
    int size()  {
        return N;
    }

    // associate key with index i; 0 < i < NMAX
    void insert(int i, int key) {
        N++;
        index[i] = N;
        heap[N] = i;
        keys[i] = key;
        bubbleUp(N);
    }

    // returns the index associated with the minimal key
    int minIndex()  {
        return heap[1];
    }

    // returns the minimal key
    int minKey()    {
        return keys[heap[1]];
    }

    // delete the minimal key and return its associated index
    // Warning: Don't try to read from this index after calling this function
    int deleteMin() {
        int min = heap[1];
        swap(1, N--);
        bubbleDown(1);
        index[min] = -1;
        heap[N+1] = -1;
        return min;
    }

    // returns the key associated with index i
    int keyOf(int i)    {
        return keys[i];
    }

    // change the key associated with index i to the specified value
    void changeKey(int i, int key)  {
        keys[i] = key;
        bubbleUp(index[i]);
        bubbleDown(index[i]);
    }

    // decrease the key associated with index i to the specified value
    void decreaseKey(int i, int key)    {
        keys[i] = key;
        bubbleUp(index[i]);
    }

    // increase the key associated with index i to the specified value
    void increaseKey(int i, int key)    {
        keys[i] = key;
        bubbleDown(index[i]);
    }

    // delete the key associated with index i
    void deleteKey(int i)   {
        int ind = index[i];
        swap(ind, N--);
        bubbleUp(ind);
        bubbleDown(ind);
        index[i] = -1;
    }
};

int main()
{
	//input graph-------------------------------------------------------------
	//input filename
    string filename;
    cout << "Enter the filename: ";
    getline(cin, filename);
    
    //read the file and make graph
    ifstream inFile(filename.c_str());   
    if(inFile.is_open()){
    	vector<string> arr;
    	map<int, set<int> > graph; //some files mention the edges twice
        string line;
        int flag = 0;
        int n_vertices = -1;
        int m_edges = -1;
        while(getline(inFile, line)){
        	//find the parameters 
	        arr = split(line," ");
	        if(arr[0] == "p"){
	        	flag = 1;
	        	n_vertices = atoi(arr[2].c_str());
	        	m_edges = atoi(arr[3].c_str());
	        	break;
			}
    	}
    	
    	//add edges to the graph
    	int degree[n_vertices+1] = {0};
    	if(flag == 1 && n_vertices != -1){
    		while(getline(inFile,line)){
    			arr = split(line, " ");
    			if(arr[0] == "e"){
    				int v1 = atoi(arr[1].c_str());
    				int v2 = atoi(arr[2].c_str());
    				graph[v1].insert(v2);
					graph[v2].insert(v1);
					degree[v1] += 1;
					degree[v2] += 1;
				}
			}
		}
		else{
			cerr<< "Missing parameters" << endl;
		}
		
		//check whether the graph is empty
		if(graph.size() == 0){
			cout << "Empty instance" << endl;
			return -1;
		}
		//----------------------------------------------------------------------
		
		
		//start dsatur algorithm to color the graph
		//----------------------------------------------------------------------
		//add vertices to the priority queue
		int saturation[n_vertices + 1] = {0};
		int color[n_vertices + 1] = {0};
		MinIndexedPQ pq(n_vertices);
		
		for(int i = 1; i <= n_vertices+1; i++)
		{
			pq.insert(i, -1*degree[i]);
		}
		while(!pq.isEmpty())
		{
			int current_node = pq.deleteMin();
			int min_color = 1;
			int color_ind[n_vertices + 1] = {0};
			int ind;
			for (set<int>::iterator it=graph[current_node].begin(); it!=graph[current_node].end(); ++it)
			{
				 color_ind[color[*it]] = 1;
			}
			for(int k=1; k<=n_vertices+1; k++)
			{
				if(color_ind[k] == 0)
				{
					ind = k;
					printf("%d\n", ind);
					break;
				}
			}
			color[current_node] = ind;
			color_ind[n_vertices + 1] = {0};
		}
		//----------------------------------------------------------------------
		return 0;
	}
	else{
		cerr << "Unable to open file" << endl;
	}
}
