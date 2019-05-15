/*****************************************
 * Instructions
 *  - Replace 'uwuserid' with your uWaterloo User ID
 *  - Select the current calendar term and enter the year
 *  - List students with whom you had discussions and who helped you
 *
 * uWaterloo User ID:  jachiang@uwaterloo.ca
 * Submitted for ECE 250
 * Department of Electrical and Computer Engineering
 * University of Waterloo
 * Calender Term of Submission:  (Winter) 2018
 *
 * By submitting this file, I affirm that
 * I am the author of all modifications to
 * the provided code.
 *
 * The following is a list of uWaterloo User IDs of those students
 * I had discussions with in preparing this project:
 *    - 
 *
 * The following is a list of uWaterloo User IDs of those students
 * who helped me with this project (describe their help; e.g., debugging):
 *    - 
 *****************************************/

//Implement Gaurd Statements to avoid having the same file included twice. Avoids Duplicate Definitions.
#ifndef WEIGHTED_GRAPH_H
#define WEIGHTED_GRAPH_H

#ifndef nullptr
#define nullptr 0
#endif

#include <iostream> //Input/Output Library
#include <limits> //Set variable storage bounds.
#include <algorithm>
#include "Exception.h" //Include Exception file to throw exceptions at methods that the compiler cannot handle.
#include "ece250.h" //Testing Dynamic Memory File

// include whatever classes you want

class Weighted_graph {
	private:
		// your implementation here
		//  you can add both private member variables and private member functions
		int num_vertices; //Stores the total number of vertices created.
		int num_edges; //Stores number of edges.
		double **A; //Pointer to pointer array.
		int *deg;
		
		static const double INF; //Infinity Variable

	public:
		Weighted_graph(int = 50); //Constructor
		~Weighted_graph(); //Destructor

		//Accessors
		
		int degree( int ) const; //Returns the degree of vertex n.
		int edge_count() const; //Return the number of edges in the graph.
		double adjacent( int, int ) const; //Returns the weight of the edge connecting vertices m and n.
		
		//Mutators
		
		double distance( int, int ); //Returns the shortest distance between vertices m and n.
		void insert( int, int, double ); //Inserts or modifies an edge between 2 vertices
		
	// Friends
	friend std::ostream &operator<<( std::ostream &, Weighted_graph const & );
};

const double Weighted_graph::INF = std::numeric_limits<double>::infinity();

//Constructor
Weighted_graph::Weighted_graph(int n):
num_vertices(n<=0 ? 1 : n), //If user inputs an invalid number of vertices, change the number of vertices to 1 by default.
num_edges(0) //Number of Edges is 0.
{
	deg = new int [n]; //Create an array storing the degree of each vertice.

	//Create an Adjacency matrix.
	A = new double *[n]; //Allocate an array of n pointers.
	double *temp = new double [n*n]; //Create a long array storing the vertices.
	for (int i=0; i<n; ++i){	
		A[i] = &(temp[n*i]); //Assign each pointer in the array to the designated start of each vertice.
		for (int j=0;j<n;++j){
			A[i][j] = INF; //Set every entry to infinity. Vertices are not connected.	
		}
		A[i][i] = 0; //Overwriting: The distance between a vertice and itself is 0. 
		deg[i] = 0;
	}
}

//Destructor
Weighted_graph::~Weighted_graph(){
	delete [] A[0]; //Deallocate the long temp array.
	delete [] A; //Deallocate the pointer to the pointer array.
	delete [] deg; //Deallocate the degree array.
}

int Weighted_graph::degree(int n) const{
	if (n<0 || n>=num_vertices){
		throw illegal_argument(); //If argument does not correspond to an existing vertex.
	}
	return deg[n]; //Return the degree stored in the degree array. 
}

int Weighted_graph::edge_count() const{
	return num_edges; //Return the variable storing the number of edges.
}

double Weighted_graph::adjacent(int n, int m) const{
	if (n<0 || m<0 || n>=num_vertices || m>=num_vertices){
		throw illegal_argument(); //If argument does not correspond to an existing vertex.
	}
	return A[n][m]; //Return the matrix index which contains the edge weight.
} 

void Weighted_graph::insert(int n, int m, double w){
	if (n<0 || m<0 || n>=num_vertices || m>=num_vertices){
		throw illegal_argument(); //If argument does not correspond to an existing vertex.
	}
	A[n][m] = w; //Set designated index to weight w. 
	A[m][n] = w;  
	num_edges++; //An edge is created between vertex n and w.
	deg[n] = deg[n]+1; //Increase the degree by 1 for n.
	deg[m] = deg[m]+1; //Increase the degree by 1 for m.
}

double Weighted_graph::distance(int n, int m){
	if (n<0 || m<0 || n>=num_vertices || m>=num_vertices){
		throw illegal_argument(); //If argument does not correspond to an existing vertex.
	}
	//Use Dijkstra's Algorithm
	//Set the Table.
	bool *visited = new bool[num_vertices]; //Create a list to store if the vertex has been stored or not.
	double *dist = new double[num_vertices]; //Create a list to store the distance.
	
	//Initialize the table.
	for (int i=0;i<num_vertices;++i){
		visited[i] = false; //No vertex has been visited yet.
		dist[i] = INF; //Everything is infinity as of the moment.
	}

	int current = n; //The current vertex right now is m.
	int min = current; //Min stores the vertex with the shortest distance to the current vertex.
	int minedge = -1; ///Minedge stores the value of the edge from current to the next vertex.
	
	visited[current] = true; //Set new current vertex to true which we will visit.
	dist[current]=0; //The distance from starting vertex to itself is 0.
	
	for (int ctr=1;ctr<num_vertices;++ctr){
	
		//Go through the current vertex of the adjacency matrix.
		for (int i=0;i<num_vertices;++i){
			
			//Locate all adjacent vertices of current vertex.
			if (A[current][i]!=INF && visited[i]==false){
				
				//Update the default table.
				if (dist[i]>dist[current]+A[current][i]){
					dist[i] = dist[current] + A[current][i]; //The distance between the neighbour and current vertex is updated.
				}
				
 			}	

 			//Find what is the minimum distance.
			if ((dist[i]<minedge || minedge==-1) && visited[i]==false){
				minedge = dist[i]; //Contains Edge Value
				min = i; //Contains Minimum Vertex
			}

		}
		
		if (min==m){ //When we reach the other vertex, this is deemed the shortest distance.
			double minimum = dist[min];
			delete []dist; //Deallocate the tables we created.
			delete []visited; 
			return minimum;
		}
		
		minedge = -1; //Reset minedge.
		current = min; //We will visit the minimum distance vertex from the current.
		visited[min]=true; //Set this to visited.
		
		ctr++; //Counter goes up every time we visit a vertex.
	}
	return INF; //Return INF if nothing is returned from our search in the while loop.
}



// You can modify this function however you want:  it will not be tested

std::ostream &operator<<( std::ostream &out, Weighted_graph const &graph ) {
	for (int i=0; i<graph.num_vertices; ++i){	
		for (int j=0;j<graph.num_vertices;++j){
			out << graph.A[i][j] << " "; 
		}
		out << std::endl; 
	}
	return out;
}

// Is an error showing up in ece250.h or elsewhere?
// Did you forget a closing '}' ?

#endif
