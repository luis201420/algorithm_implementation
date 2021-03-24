/////////////////////////////////////////////////////////////////////////
// * Algorithm: Dijkstra (Book)
// * Author: Luis Enrique Murillo Vizcardo
// * Complexity: O(n*log(n))
/////////////////////////////////////////////////////////////////////////

#include <bits/stdc++.h>

#define INF 1e9
#define word1 "NODE"
#define word2 "WEIGHT"
#define word3 "GOAL"
#define ALL(x) x.begin(),x.end()

using namespace std;

typedef int nnumber;                        // Normal number
typedef pair<nnumber,nnumber> p_nnumber;    // Pair of normal numbers
typedef vector<nnumber> v_nnumber;          // Vector of normal numbers
typedef vector<v_nnumber> vv_nnumber;       // Vector of normal number vector

struct edge{				    // Edge with target and N weights
	nnumber to_node;
	v_nnumber weights;
};
typedef vector<edge> v_edge;                // Vector of edges
typedef vector<v_edge> vv_edge;             // Vector of edge vectors
typedef vector<string> v_string;            // Vector of string
typedef vector<bool> v_bool;                // Vector of booleans
typedef vector<p_nnumber> v_pnnumber;       // Vector of normal number pair

/////////////////////////////////////////////////////////////////////////
// Global variable

nnumber n_nodes, n_weights;                 // number of nodes and weights on graph
vv_edge graph;
string source_node, target_node;            // source and target id node
map<string, nnumber> ids;                   // ids of nodes on the graph
v_string names;                             // original names of nodes
v_nnumber dist_shorest;                     // distance of shorest path

/////////////////////////////////////////////////////////////////////////
// Functions

// Read file and initializate the graph
bool read_file(const string &file_name){

	ifstream my_stream(file_name);
	string   word[7],s_weight    ;
	edge new_edge                ;

	if(my_stream.is_open()){
		
		for(int i=0;i<7;i++)
			my_stream >> word[i];

		if(word[0] != word1 || word[2] != word2 || word[4]!=word3){
			
			cout << "Incorrect values on input file\n";
			return 0;
		
		}

		n_nodes   = atoi(word[1].c_str());
		n_weights = atoi(word[3].c_str());

		source_node = word[5];
		target_node = word[6];

		names.push_back("");                      // fill position 0

		ids[source_node] = 1;                     // save position of node (ID)
		names.push_back(source_node);             // save name of node

		ids[target_node] = 2;
		names.push_back(target_node);

		graph.assign(n_nodes+1,v_edge());
		
		while( my_stream >> word[0] >> word[1]){
			
			for(int i=0;i<2;i++){
				
				if(ids[word[i]]==0){
					
					ids[word[i]] = names.size();
					names.push_back(word[i]);
				
				}
			}
			
			new_edge.to_node = ids[word[1]];

			for(int i=0; i<n_weights; i++){
			
				my_stream >> s_weight;
				new_edge.weights.push_back(atoi(s_weight.c_str()));
			
			}
			
			graph[ids[word[0]]].push_back(new_edge);
			
			new_edge.to_node = ids[word[0]];
			graph[ids[word[1]]].push_back(new_edge);
			
			new_edge.weights.clear();
		}

		my_stream.close();
		return 1;
	}
	else{
		cout << "Unable to open file.\n";
		return 0;
	}		
}

// Dijkstra algorithm
bool dijkstra(){
	
	nnumber source = ids[source_node];
	nnumber target = ids[target_node];

	v_bool group1(n_nodes+1,1);
	priority_queue<p_nnumber> group2;
	v_nnumber parent(n_nodes+1,-1);
	vv_nnumber dist(n_nodes+1,v_nnumber(n_weights,INF));

	dist[source] = v_nnumber(n_weights,0);
	group2.push({0,source});

	nnumber cur_node;
	edge    cur_edge;
	nnumber cur_cost,new_cost_value;
	v_nnumber new_cost(n_weights);

	while(!group2.empty()){
	
		cur_node = group2.top().second; group2.pop();
		
		if(!group1[cur_node]) continue;

		group1[cur_node] = 0;
		
		for(int i=0;i<graph[cur_node].size();i++){
			
			cur_edge = graph[cur_node][i];
			cur_cost = new_cost_value = 0;

			for(int j=0;j<n_weights;j++){
				cur_cost += dist[cur_edge.to_node][j];
				new_cost[j] = dist[cur_node][j] + cur_edge.weights[j];
				new_cost_value += new_cost[j];
			}
			if(new_cost_value < cur_cost){
				dist[cur_edge.to_node] = new_cost;
				group2.push({-new_cost_value,cur_edge.to_node});	
			}
		}
	}
	
	if(group1[target]) return 0;

	dist_shorest = dist[target];
	return 1;
}

// Main route function
bool route(const string &file_name){

	bool check_read = read_file(file_name);
	
	if(!check_read)
		return 0;

	// Dijkstra algorithm
	bool check_route = dijkstra();
	
	return check_route;
}

/////////////////////////////////////////////////////////////////////////
// Main function

int main(int argc, char* argv[]){
	
	if (argc == 2){
		bool check_route = route(argv[1]);
		if(check_route){
			cout << "Cost of shortest path between " << source_node << " and " << target_node <<" : \n";
			for(int i=0; i<n_weights;i++)
				cout << "Weight "<< i << " = " << dist_shorest[i] << "\n";
			
			cout << "Total cost: " << accumulate(ALL(dist_shorest),0) << "\n";
		}
		else
			cout << "Error!\n";
	}
	else
		cout << "Error: Be sure to put only the input file name.\n ./dijkstra file_name_input.txt\n";
	return 0;
}
