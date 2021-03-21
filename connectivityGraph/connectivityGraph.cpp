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
	p_nnumber corners[2];
	string    id;
};

typedef vector<Pin>           v_pin;        // Vector of pins

struct Net{                                // Net representation, list of pins
	v_pin  pins;
	string id;
};
struct route_region{
	p_nnumber corners[2];
};

typedef vector<Obstacle> v_obstacle;         // Vector of obstacles
typedef vector<Net>      v_net;              // Vector of Nets
typedef vector<char>     v_char;             // Vector of characters
typedef vector<v_char>   vv_char;            // Matrix 2D of characters
typedef vector<nnumber>  v_nnumber;          // Vector of normal number
typedef vector<route_region> v_route_region; // Vector of Route Regions

/////////////////////////////////////////////////////////////////////////
// Global Variables

p_nnumber       layout_dimension;
v_obstacle      my_obstacles;
v_net           my_nets;
vv_char         my_layout;
v_route_region  my_route_regions;

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
					
					for(int j=0; j<3; j++){    // 3 words (ID Corner1 Corner2)
						
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

// Get route regions of layout, place obstacles and pins into layout(small dimension)
void get_route_regions(){
	
	my_layout.assign(layout_dimension.first, v_char(layout_dimension.second,'-')); // initializate layout
	v_nnumber x_obs(layout_dimension.first,-1), y_obs(layout_dimension.second,-1);


	for(int i=0; i<my_obstacles.size(); i++){
		
		x_obs[my_obstacles[i].corners[0].first]  = 0;
		x_obs[my_obstacles[i].corners[1].first]  = 1;
		y_obs[my_obstacles[i].corners[0].second] = 0;
		y_obs[my_obstacles[i].corners[1].second] = 1;

		for(int x=my_obstacles[i].corners[0].first; x<= my_obstacles[i].corners[1].first; x++)
			for(int y=my_obstacles[i].corners[0].second; y<=my_obstacles[i].corners[1].second; y++)
				my_layout[x][y]='#';

	}

	for(int i=0; i<my_nets.size();i++){
		for(int j=0;j<my_nets[i].pins.size(); j++){
			my_layout[my_nets[i].pins[j].position.first][my_nets[i].pins[j].position.second] = 'A'+i;
		}
	}

	nnumber id_route_region = 0;
	nnumber pos_x, pos_y;
	route_region new_region;

	for(int i=0; i<layout_dimension.first; i++){
		for(int j=0; j<layout_dimension.second; j++){
			
			if(my_layout[i][j] != '-')continue;
		
			new_region.corners[0] = {i,j};

			for(pos_x=i; pos_x<layout_dimension.first; pos_x++){
				
				if(pos_x != i && x_obs[pos_x] == 0)break;
				
				for(pos_y=j; pos_y<layout_dimension.second; pos_y++){
					
					if(pos_y!=j && y_obs[pos_y]==0)break;
					my_layout[pos_x][pos_y] = 'a'+id_route_region;
					new_region.corners[1] = {pos_x, pos_y};
					if(pos_y!=j && y_obs[pos_y]==1)break;
				
				}

				if(pos_x!=i && x_obs[pos_x]==1)break;
			}
			cerr << char('a'+id_route_region) << " -> ";
			cerr << new_region.corners[0].first << " " << new_region.corners[0].second << " ";
			cerr << new_region.corners[1].first << " " << new_region.corners[1].second << "\n";
			my_route_regions.push_back(new_region);
			id_route_region++;
		}
	}

}

// Main functions to route
bool route(const string &file_name){
	
	bool check_open;
	check_open = read_file(file_name);

	if(!check_open)
		return 0;

	// Def route regions & connectivity graph
	get_route_regions();

	// begin route
	return 1;
}

// Graph final layout
void graph_layout(){
	for(int i=0; i<layout_dimension.first; i++){
		for(int j=0; j<layout_dimension.second; j++){
			//if(islower(my_layout[i][j])) cout << "- ";
			/*else*/ cout << my_layout[i][j] << " ";
		}
		cout << "\n";
	}
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
