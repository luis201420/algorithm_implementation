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
typedef pair<nnumber,char>    p_nnumberchar;// Pair of number and char

struct Pin{                                 // Pin representation, position (X,Y)
	p_nnumber position;
	nnumber initial_region;            // initial node of graph
	char direction; 		   // direction (vertical or horizontal)
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
	p_nnumber tracks;                  // number of {Vertical, horizontal} tracks
};

typedef vector<Obstacle> v_obstacle;            // Vector of obstacles
typedef vector<Net>      v_net;                 // Vector of Nets
typedef vector<char>     v_char;                // Vector of characters
typedef vector<v_char>   vv_char;               // Matrix 2D of characters
typedef vector<nnumber>  v_nnumber;             // Vector of normal number
typedef vector<route_region> v_route_region;    // Vector of Route Regions
typedef vector<p_nnumberchar>  vp_nnumberchar;  // Vector of number-char pairs
typedef vector<vp_nnumberchar>  vvp_nnumberchar;// Vector of vector of number-char pairs

/////////////////////////////////////////////////////////////////////////
// Global Variables

p_nnumber       layout_dimension;
v_obstacle      my_obstacles;
v_net           my_nets;
vv_char         my_layout;
v_route_region  my_route_regions;
vvp_nnumberchar my_graph;

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

	// Add obstacles on layout
	for(int i=0; i<my_obstacles.size(); i++){
		
		x_obs[my_obstacles[i].corners[0].first]  = 0;
		x_obs[my_obstacles[i].corners[1].first]  = 1;
		y_obs[my_obstacles[i].corners[0].second] = 0;
		y_obs[my_obstacles[i].corners[1].second] = 1;

		for(int x=my_obstacles[i].corners[0].first; x<= my_obstacles[i].corners[1].first; x++)
			for(int y=my_obstacles[i].corners[0].second; y<=my_obstacles[i].corners[1].second; y++)
				my_layout[x][y]='#';

	}
	
	// Add nets on layout
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
			//cerr << char('a'+id_route_region) << " -> ";
			//cerr << new_region.corners[0].first << " " << new_region.corners[0].second << " ";
			//cerr << new_region.corners[1].first << " " << new_region.corners[1].second << "\n";
			my_route_regions.push_back(new_region);
			id_route_region++;
		}
	}

}

// Get main graph
void get_graph(){
	
	nnumber xp,yp;
	nnumber x11,x12,y11,y12,x21,x22,y21,y22;
	my_graph.resize(my_route_regions.size());

	for(int i=0; i<my_route_regions.size();i++){
		
		x11 = my_route_regions[i].corners[0].first;
		x21 = my_route_regions[i].corners[1].first;
		y11 = my_route_regions[i].corners[0].second;
		y21 = my_route_regions[i].corners[1].second;
		
		my_route_regions[i].tracks = {abs(y11-y21)+1, abs(x11-x21)+1};

		// Get initial node to each pin
		for(int j=0; j<my_nets.size();j++){
			for(int k=0;k<my_nets[j].pins.size();k++){
				xp=my_nets[j].pins[k].position.first;
				yp=my_nets[j].pins[k].position.second;
				if(x11<=xp && xp<=x21 && (abs(yp-y11)==1 || abs(yp-y21)==1)){
					my_nets[j].pins[k].initial_region = i;
					my_nets[j].pins[k].direction = 'H';
				}
				if(y11<=yp && yp<=y21 && (abs(xp-x11)==1 || abs(xp-x21)==1)){
					my_nets[j].pins[k].initial_region = i;
					my_nets[j].pins[k].direction = 'V';
				}
			}
		}

		for(int j=i+1;j<my_route_regions.size();j++){
			
			x12 = my_route_regions[j].corners[0].first;
			x22 = my_route_regions[j].corners[1].first;
			y12 = my_route_regions[j].corners[0].second;
			y22 = my_route_regions[j].corners[1].second;

			if(x11 == x12 && x21 == x22 && (abs(y11-y22)==1 || abs(y21-y12)==1)){
				my_graph[i].push_back({j,'H'});
				my_graph[j].push_back({i,'H'});
			}

			if(y11 == y12 && y21 == y22 && (abs(x11-x22)==1 || abs(x21-x12)==1)){
				my_graph[i].push_back({j,'V'});
				my_graph[j].push_back({i,'V'});
			}
		}
	}
}

// Route BFS algorithm
v_nnumber get_path(nnumber &source, nnumber &target){

	vector<bool> visited(my_route_regions.size(),0);
	v_nnumber cur_path, new_path;
	queue<v_nnumber> q;
	q.push({source});
	nnumber v,u;

	while(!q.empty()){
		
		cur_path = q.front(); q.pop();
		
		v = *cur_path.rbegin();

		if(v == target) return cur_path;
		if(visited[v]) continue;

		visited[v] = 1;
		//cerr << v << "->";
		
		for(int i=0;i<my_graph[v].size();i++){

			u = my_graph[v][i].first;
			//cerr << u << endl;

			if(my_graph[v][i].second == 'V' && my_route_regions[v].tracks.first && my_route_regions[u].tracks.first){
				new_path = cur_path;
				new_path.push_back(u);
				q.push(new_path);
			}
			if(my_graph[v][i].second == 'H' && my_route_regions[v].tracks.second && my_route_regions[u].tracks.second){
				new_path = cur_path;
				new_path.push_back(u);
				q.push(new_path);
			}
				
		}
	}
	return v_nnumber();

}

// Update layout
void update_layout(v_nnumber &path, Net &net, char net_id){
	
	char cur_dir;
	nnumber x1,x2,y1,y2;
	nnumber x11,x21,y11,y21;
	nnumber x12,x22,y12,y22;
	bool ver;

	if(net.pins[0].direction == 'V'){
		
		cur_dir = 'V';
		my_route_regions[path[0]].tracks.first--;
		
		x1 = my_route_regions[path[0]].corners[0].first;
		x2 = my_route_regions[path[0]].corners[1].first;

		y1 = net.pins[0].position.second;

		for(int i=x1;i<=x2;i++) my_layout[i][y1] = net_id;
		
	}
	else{
		cur_dir = 'H';
		my_route_regions[path[0]].tracks.second--;

		y1 = my_route_regions[path[0]].corners[0].second;
		y2 = my_route_regions[path[0]].corners[1].second;

		x1 = net.pins[0].position.first;

		for(int i=y1;i<=y2;i++) my_layout[x1][i] = net_id;
	}

	for(int i=0;i<path.size()-1;i++){	

			int j = i;
			ver = 1;
			
			while(j<path.size() && ver){
				
				x11 = my_route_regions[path[j]].corners[0].first;
				x21 = my_route_regions[path[j]].corners[1].first;
				y11 = my_route_regions[path[j]].corners[0].second;
				y21 = my_route_regions[path[j]].corners[1].second;
					
				j++;
				
				if(j>=path.size())break;

				x12 = my_route_regions[path[j]].corners[0].first;
				x22 = my_route_regions[path[j]].corners[1].first;
				y12 = my_route_regions[path[j]].corners[0].second;
				y22 = my_route_regions[path[j]].corners[1].second;
			
				// H
				if(x11 == x12 && x21 == x22 && (abs(y11-y22)==1 || abs(y21-y12)==1)){
					if(cur_dir != 'H'){
						my_route_regions[path[j-1]].tracks.second--;
						ver = 0;
					}
					my_route_regions[path[j]].tracks.second--;
				}

				// V
				if(y11 == y12 && y21 == y22 && (abs(x11-x22)==1 || abs(x21-x12)==1)){
					if(cur_dir != 'V'){
						my_route_regions[path[j-1]].tracks.first--;
						ver = 0;
					}
					my_route_regions[path[j]].tracks.first--;
				}
			}

			//cerr << i <<" " << j << " " << cur_dir << endl;
			
			for(int k=i;k<j;k++){
				if(i==0){
					if(cur_dir=='V'){
						x1 = my_route_regions[path[k]].corners[0].first;
						x2 = my_route_regions[path[k]].corners[1].first;
					
						if(cur_dir == net.pins[0].direction)	
							y1 = net.pins[0].position.second;
						else
							y1 = my_route_regions[path[k]].corners[0].second;
						
						while(x1<=x2){
							my_layout[x1][y1]=net_id;
							x1++;
						}
					}
					else{
						y1 = my_route_regions[path[k]].corners[0].second;
						y2 = my_route_regions[path[k]].corners[1].second;
						
						if(cur_dir == net.pins[0].direction)
							x1 = net.pins[0].position.first;
						else
							x1 = my_route_regions[path[k]].corners[0].first;

						while(y1<=y2){
							my_layout[x1][y1]=net_id;
							y1++;
						}
					}
				}
				else if(j==path.size()){
					if(cur_dir=='V'){
						x1 = my_route_regions[path[k]].corners[0].first;
						x2 = my_route_regions[path[k]].corners[1].first;
						
						if(cur_dir == net.pins[1].direction)
							y1 = net.pins[1].position.second;
						else
							y1 = my_route_regions[path[k]].corners[0].second;
						
						while(x1<=x2){
							my_layout[x1][y1]=net_id;
							x1++;
						}
					}
					else{
						y1 = my_route_regions[path[k]].corners[0].second;
						y2 = my_route_regions[path[k]].corners[1].second;
						
						if(cur_dir == net.pins[1].direction)
							x1 = net.pins[1].position.first;
						else
							x1 = my_route_regions[path[k]].corners[0].first;

						while(y1<=y2){
							my_layout[x1][y1]=net_id;
							y1++;
						}
					}
				}
				else{
					if(cur_dir=='V'){
						x1 = my_route_regions[path[k]].corners[0].first;
						x2 = my_route_regions[path[k]].corners[1].first;
						
						y1 = my_route_regions[path[k]].corners[0].second;
						
						while(x1<=x2){
							my_layout[x1][y1]=net_id;
							x1++;
						}
					}
					else{
						y1 = my_route_regions[path[k]].corners[0].second;
						y2 = my_route_regions[path[k]].corners[1].second;
						
						x1 = my_route_regions[path[k]].corners[0].first;

						while(y1<=y2){
							my_layout[x1][y1]=net_id;
							y1++;
						}
					}
				}
			}

			if(cur_dir == 'V') cur_dir = 'H';
			else cur_dir = 'V';
			i = j-2;

	}

	if(net.pins[1].direction == 'V'){
		
		//if(cur_dir != 'V'){
			my_route_regions[*path.rbegin()].tracks.first--;
		
			x1 = my_route_regions[*path.rbegin()].corners[0].first;
			x2 = my_route_regions[*path.rbegin()].corners[1].first;

			for(int i=x1;i<=x2;i++) my_layout[i][net.pins[1].position.second] = net_id;
		//}
	}
	else{
		//if(cur_dir != 'H'){
			my_route_regions[*path.rbegin()].tracks.second--;

			y1 = my_route_regions[*path.rbegin()].corners[0].second;
			y2 = my_route_regions[*path.rbegin()].corners[1].second;

			for(int i=y1;i<=y2;i++) my_layout[net.pins[1].position.first][i] = net_id;
		//}
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

	// Get graph
	get_graph();

	v_nnumber path;

	// begin route
	for(int i=0;i<my_nets.size();i++){
		
		path = get_path(my_nets[i].pins[0].initial_region, my_nets[i].pins[1].initial_region);
		/*for(int j=0;j<path.size();j++)
			cerr << path[j] << " ";
		cerr << endl;*/
	
		update_layout(path,my_nets[i],'A'+i);
	}
	return 1;
}

// Graph final layout
void graph_layout(){
	for(int i=0; i<layout_dimension.first; i++){
		for(int j=0; j<layout_dimension.second; j++){
			if('a'<=my_layout[i][j] && my_layout[i][j]<='z') cout << "   ";
			//if(islower(my_layout[i][j])) cout << "- ";
			else cout << my_layout[i][j] << "  ";
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
