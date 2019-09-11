#include<bits/stdc++.h>
#include <iostream> 
#include <string.h> 
#include <algorithm> 
#include <list>
#include <iterator>
#include <exception>
using namespace std; 

void addEdge(vector<int> adjlst[], int a, int b) 
{ 
    adjlst[a].push_back(b); //b is connected to vertex a
    adjlst[b].push_back(a); //a is connected to vertex b
	
} 

void printGraph(vector<int> adjlst[], int num_verts) 
{ 

    for (int v = 1; v < num_verts + 1; ++v) 
    { 
		sort(adjlst[v].begin(), adjlst[v].end()); //sort each list from least to greatest
		
		if(adjlst[v].size() != 0) //only printing vertices with connections
		{ 
			cout << v; //print the vertex
			for (int x : adjlst[v]){
			   cout << "-> " << x; //print vertex's connections
			}
			cout << endl;
		}
    }

	cout << endl;
}


//***********************This section used for printing path**************************

//count reachable vertices from v
int reachableVertCount(vector<int> adjlst[], int v, bool visited[]) 
{ 
  // Mark the current node as visited 
  visited[v] = true; 
  int count = 1; 
  
  // Recur for all vertices adjacent to this vertex 
  for (int x : adjlst[v]){
      if (x != -1 && !visited[x]){ //if the vert is not removed and not visited yet it is reachable
          count += reachableVertCount(adjlst, x, visited); //add reachable vert and call recursively
	  }
  }
  
  return count; 
}


// This function removes edge u->v from graph by replacing adjcent vertex value with -1. 
int removeEdge(vector<int> adjlst[], int u, int v, int num_verts, bool temporary) 
{ 
	int index_v = -100;
	int index_u = -100;
	
	// Find v in adjacency list of u and replace it with -1 
	vector<int>::iterator iv = find(adjlst[u].begin(), adjlst[u].end(), v);
			if (iv != adjlst[u].end()){
				index_v = distance(adjlst[u].begin(), iv);
			}
		*iv = -1;
	
		
	// Find u in adjacency list of v and replace it with -1 
	vector<int>::iterator iu = find(adjlst[v].begin(), adjlst[v].end(), u);
			if (iu != adjlst[v].end()){
				index_u = distance(adjlst[v].begin(), iu);
			}
		*iu = -1; 


		bool visited[num_verts];
		memset(visited, false, num_verts); 
		int afterRemoved = reachableVertCount(adjlst, u, visited);
		
		
		// for some reason my addEdge function had a segmentation fault
		// so to add back an edge that was only removed temporarily for counting paths
		// I add the vertices back using vector indexing
		// it took me eternity to fix this problem
		if (temporary && index_v > 0){
			adjlst[u].at(index_v) = v;
			adjlst[v].at(index_u) = u;
		}
		
		return afterRemoved;
	
} 

bool isValidNextEdge(vector<int> adjlst[], int u, int v, int num_verts) 
{   
	// If v is the only adjacent vertex of u, the edge is valid
		int count = 0;  // stores count of adjacent vertices
			
		for (int x : adjlst[u])
		{
			if (x != -1){ 
				count++;
			}
		}
				  
		if (count == 1){
			return true; 
		}
			  
	// If there are multiple adjacents, then u-v is not a bridge and is valid  
			// counts vertices reachable from u before removing edge
				bool visited[num_verts]; 
				memset(visited, false, num_verts); 
				int beforeRemoved = reachableVertCount(adjlst, u, visited); 
		
			// removes edge (u, v) and then counts new # of vertices reachable from u
				int afterRemoved = removeEdge(adjlst, u, v, num_verts, true);
				
					  
			// If number of vertices reachable before edge removal is greater than after removal, then edge (u, v) is a bridge and not valid
				if (beforeRemoved > afterRemoved){
					return false;
				}
				else{
					return true;
				}
				
} 

void findPath(vector<int> adjlst[], int cur_vert, int num_verts, int num_edges) 
{ 
	
	for (int x : adjlst[cur_vert]) 
	{	  
		// If edge cur_vert->x is not removed and it's a a valid next edge, it is part of path
		if (x != -1 && isValidNextEdge(adjlst, cur_vert, x, num_verts)) 
		{
			cout << "(" << cur_vert << "," << x << ")" << " ";
			removeEdge(adjlst, cur_vert, x, num_verts, false); 
			findPath(adjlst, x, num_verts, num_edges);
			
		}
				
	}
} 
//****************************************************************************


void oneStrokeDraw(vector<int> adjlst[], int num_verts, int num_edges, string file_name)
{
	
	int odd_degree_verts = 0; // number of vertices with an odd # of edges coming off
	int count = 0; // counts number of edges coming off vertex
	vector<int> startVerts; // vector to hold potential starting vertices
	
	 for (int v = 1; v < num_verts + 1; ++v) 
     { 
		if(adjlst[v].size() != 0)
		{ 
			count = 0;
			for (int x : adjlst[v]){
			   count++;
			}
			
			if(count%2 == 1){
				odd_degree_verts++;
				startVerts.push_back(v);
			}
		}
    }
	
	// Euler's rule: 2 or 0 odd degree vertices = graph can be drawn in one stroke
	if(odd_degree_verts == 2 | odd_degree_verts == 0)
	{
		cout << "true" << endl;
		cout << "starting vertices: ";

		switch (odd_degree_verts)
		{
			case 0:
					// 0 odd degree verts = all vertices are starting points
					for(int i = 1; i < num_verts + 1; i++){
						cout << i << " ";
					}
					cout << endl;
					cout << "[";
					findPath(adjlst, 1, num_verts, num_edges);
					cout << "]";
					cout << " is the path to draw.";
				break;
			case 2:
					// 2 odd degree verts = start at one of the verts with odd degrees
					for (int i = 0; i < startVerts.size(); i++){
						cout << startVerts.at(i) << " ";
					}
					cout << endl;
					cout << "[";
					findPath(adjlst, startVerts.at(0), num_verts, num_edges);
					cout << "]";
					cout << " is the path to draw.";
				break;
		}
		
		return;
	}
	else
	{
		cout << "false";
		return;
	}

}


  
int main(int argc, char **argv) 
{ 
	
	for(int i = 1; i < argc; i++)
	{

		ifstream inFS;
		string file_name = argv[i];
		
		cout << argv[i] << ": " << endl;
		
		inFS.open(file_name);
		
		if (inFS.is_open())
		{

			int v1; //paired with v2
			int v2;
			vector<int> adjlst[100]; //100 is the max number of vertices the graph can have for this code
			
			int num_verts;
			int num_edges;
			
			inFS >> num_verts >> num_edges;
			
			while(!inFS.eof())
			{
				
				//reads the pair of vertices
				inFS >> v1 >> v2;
				
				//adds edge to the pair of vertices
				if(!inFS.eof()){	
					addEdge(adjlst, v1, v2);
				}
			}
			
			printGraph(adjlst, num_verts); //print adjacency list
			oneStrokeDraw(adjlst, num_verts, num_edges, file_name); //prints true if can be drawn, lists start vertices, calls findPath
			
		}
		else
		{
			cout << "Unable to open file.";
		}
	
		inFS.close();
	}

	return 0;
	
}