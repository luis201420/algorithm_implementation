/////////////////////////////////////////////////////////////////////////
// * Algorithm: Connectivity Graph (Book)
// * Author: Luis Enrique Murillo Vizcardo
// * Complexity:
/////////////////////////////////////////////////////////////////////////

#include <bits/stdc++.h>

#define INF 1e9
#define word1 "DIMENSION"
#define word2 "OBSTACLES"
#define word3 "NETS"

using namespace std;

/////////////////////////////////////////////////////////////////////////
// Types

typedef int                   nnumber;      // Type normal numbers
typedef pair<nnumber,nnumber> p_nnumber;    // Pair of normal numbers

struct Pin{                                 // Pin representation, position (X,Y)
	p_nnumber position;
};

struct Obstacle{                            // Obstacle representation, 4 corners (Rectangular shape)
	p_nnumber corners[4];
	string    id;
};

typedef vector<Pin>           v_pin;        // Vector of pins

struct Net{                                // Net representation, list of pins
	v_pin  pins;
	string id;
};

typedef vector<Obstacle> v_obstacle;       // Vector of obstacles
typedef vector<Net>      v_net;            // Vector of Nets

/////////////////////////////////////////////////////////////////////////
// Global Variables

p_nnumber  layout_dimension;
v_obstacle my_obstacles;
v_net      my_nets;

/////////////////////////////////////////////////////////////////////////
// Functions

// Convert string (X,Y) to pair of normal numbers
p_nnumber stringToPair(const string &s_pair){
	
	p_nnumber r_pair;
	string    cur_s;
	
	cur_s = "";

	for(int i=1; i<s_pair.size()-1; i++){
		
		if(s_pair[i]==','){
			r_pair.first = atoi(cur_s.c_str());
			cur_s="";
		}
		else cur_s+=s_pair[i];
	}

	r_pair.second = atoi(cur_s.c_str());
	return r_pair;
}

// Read input file and initializate the list of pins/obstacles
bool read_file(const string &file_name){

	ifstream my_stream(file_name);
	string   word                ;
	nnumber  n_obs, n_nets, n_pin;
	Obstacle new_obstacle        ;
	Net      new_net             ;
	Pin      new_pin             ;

	if(my_stream.is_open()){
		
		while( my_stream >> word){
			if(word == word1){                          // Dimension
				
				my_stream >> word;                  // read next word
				layout_dimension = stringToPair(word);
			
			}
			else if(word == word2){                     // Obstacles
				
				my_stream >> word;                  // read next word
				n_obs = atoi(word.c_str());         // get number of obstacles
				
				for(int i = 0; i<n_obs ; i++){
					
					for(int j=0; j<5; j++){    // 5 words (ID Corner1 Corner2 Corner3 Corner4)
						
						my_stream >> word;
						
						if(j==0) new_obstacle.id = word;                     // get id obstacle
						else new_obstacle.corners[j-1] = stringToPair(word); // get corners
					
					}
					
					my_obstacles.push_back(new_obstacle);       // add obstacle to my obstacle list
				}
			}
			else{                                      // Nets
				my_stream >> word;                 // read next word
				n_nets = atoi(word.c_str());       // get number of nets
				
				for(int i=0; i<n_nets; i++){
					
					my_stream >> word;
					new_net.id = word;         // get id of net
					my_stream >> word;
					n_pin = atoi(word.c_str());// get pin number of net

					for(int j=0; j<n_pin; j++){
						my_stream >> word;
						new_pin.position = stringToPair(word);   // get pins position
						new_net.pins.push_back(new_pin);         // add pin to current net
					}

					my_nets.push_back(new_net);                      // add net to my nets list
					new_net.pins.clear();                            // clear pins
				}
			}
		}
		my_stream.close();

		return 1;
	}
	else{
		cout << "Unable to open file.\n";
		return 0;
	}	
}

// Main functions to route
bool route(const string &file_name){
	
	bool check_open;
	check_open = read_file(file_name);

	if(!check_open)
		return 0;

	// Def route regions & connectivity graph
	

	// begin route
	return 1;
}

// Graph final layout
void graph_layout(){
}

/////////////////////////////////////////////////////////////////////////
// Main Function

int main(int argc, char* argv[]){
	
	if (argc == 2){
		bool check_route = route(argv[1]);
		if(check_route)
			graph_layout();
		else
			cout << "Error!\n";
	}
	else
		cout << "Error: Be sure to put only the input file name.\n ./connectivityGraph file_name_input.txt\n";

	return 0;
}
