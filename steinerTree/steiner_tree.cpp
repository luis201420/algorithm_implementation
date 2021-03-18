/////////////////////////////////////////////////////////////////////////
// * Algorithm: Steiner Tree construction heuristics 2D
// * Author: Luis Enrique Murillo Vizcardo
// * Complexity: O(n^2)
/////////////////////////////////////////////////////////////////////////

#include <bits/stdc++.h>

#define INF 1e9

using namespace std;

/////////////////////////////////////////////////////////////////////////
// Types

typedef int                        nnumber;             // type normal numbers
typedef vector<nnumber>            v_nnumber;           // vector of normal numbers
typedef pair<nnumber,nnumber>      p_nnumber;           // pair of normal numbers
typedef vector<bool>               v_bool;              // vector of booleans

struct pin{
	v_nnumber position;
};

typedef vector<pin>                vect_pins;
typedef vector<v_nnumber>          vv_nnumber;
typedef vector<char>               v_char;
typedef vector<v_char>             vv_char;

/////////////////////////////////////////////////////////////////////////
// Global Variables

vect_pins  my_pins  ;
v_bool     used_pins;
nnumber    n_usedpins, n_pins, max_X, max_Y;
vv_nnumber L_tree;

/////////////////////////////////////////////////////////////////////////
//Functions

// Convert string (v1,v2,...) to type pin
pin stringtoPin(const string &my_string){
	
	string  cur_value;
	pin     new_pin  ;

	cur_value = "";
	
	for(int i=1;i<my_string.size()-1;i++){
		if(my_string[i]==','){

			new_pin.position.push_back(atoi(cur_value.c_str()));
			max_X = max(max_X,atoi(cur_value.c_str()));
			cur_value = "";

		}
		else cur_value += my_string[i];
	}
	new_pin.position.push_back(atoi(cur_value.c_str()));
	max_Y = max(max_Y,atoi(cur_value.c_str()));

	return new_pin;
}

// Read input file and initializate the list of pins
void read_file(const string &file_name){
	
	ifstream my_stream(file_name);
	string   line                ;

	max_X = max_Y = 0;

	if(my_stream.is_open()){
		while(getline(my_stream,line))
			my_pins.push_back(stringtoPin(line));

		my_stream.close();

		used_pins.assign(my_pins.size(),0);
		n_usedpins = 0;
		n_pins     = my_pins.size();

		cout << "The file content " << n_pins << " pin(s) to route.\n";
	}
	else cout << "Unable to open file.\n";
}

// Get manhatan distance between points (X,Y)
nnumber get_distance(const v_nnumber &p1, const v_nnumber &p2){
	return abs(p1[0]-p2[0]) + abs(p1[1]-p2[1]);
}

// Return the positions of nearest pair pins O(n^2)
p_nnumber find_nearest_pins(){
	
	nnumber min_distance = INF;
	nnumber distance          ;
	p_nnumber nearest_pins   ;

	for(int i=0;i<n_pins;i++){
		for(int j=i+1;j<n_pins;j++){
				
			distance = get_distance(my_pins[i].position,my_pins[j].position);

			if(min_distance > distance){
				nearest_pins = {i,j};
				min_distance = distance;
			}
		}
	}
	return nearest_pins;
}

// Find nearest pin to MBB(Minimum Bounding Box)
// Return nearest pin, nearest mbb point and L point
void find_nearest_pin_MBB(const vv_nnumber &mbb, nnumber &nearest_pin, v_nnumber &nearest_mbbpoint, v_nnumber &L_point){
	
	nnumber    cur_x, cur_y, x1, x2, y1, y2, mbb_y, mbb_x;
	nnumber    min_distance, distance;
	v_nnumber  mbb_point;

	// Get position of the pin pair
	x1 = mbb[0][0];
	y1 = mbb[0][1];
	x2 = mbb[1][0];
	y2 = mbb[1][1];
	
	min_distance = INF;

	for(int i=0;i<n_pins;i++){
		if(used_pins[i])continue;
		
		// Get position of current pin
		cur_x = my_pins[i].position[0];
		cur_y = my_pins[i].position[1];

		// Get vertical lines of MBB
		if(min(y1,y2) < cur_y && cur_y < max(y1,y2)) mbb_y = cur_y;       // if cur_y is between Y range of MBB
		else if(cur_y < min(y1,y2))                  mbb_y = min(y1,y2);  // if cur_y is down of Y range
		else                                         mbb_y = max(y1,y2);  // if cur_y is up of Y range

		// Line (x1,y1) - (x1,y2) 
		mbb_point = {x1,mbb_y};
		distance = get_distance(mbb_point,my_pins[i].position);
		if(distance < min_distance){
			min_distance     = distance;
			L_point          = {x1,y2};
			nearest_mbbpoint = mbb_point;
			nearest_pin      = i;
		}
		// Line (x2,y2) - (x2,y1)
		mbb_point = {x2,mbb_y};
		distance = get_distance(mbb_point,my_pins[i].position);
		if(distance < min_distance){
			min_distance = distance;
			L_point          = {x2,y1};
			nearest_mbbpoint = mbb_point;
			nearest_pin      = i;
		}

		// Get horizontal lines of MBB
		if(min(x1,x2) < cur_x && cur_x < max(x1,x2)) mbb_x = cur_x;       // if cur_x is between X range of MBB
		else if(cur_x < min(x1,x2))                  mbb_x = min(x1,x2);  // if cur_x is down of X range
		else                                         mbb_x = max(x1,x2);  // if cur_x is up of X range

		// Line (x1,y1) - (x2,y1)
		mbb_point = {mbb_x,y1};
		distance = get_distance(mbb_point,my_pins[i].position);
		if(distance < min_distance){
			min_distance = distance;
			L_point          = {x2,y1};
			nearest_mbbpoint = mbb_point;
			nearest_pin      = i;
		}

		// Line (x2,y2) - (x1,y2)
		mbb_point = {mbb_x,y2};
		distance = get_distance(mbb_point,my_pins[i].position);
		if(distance < min_distance){
			min_distance     = distance;
			L_point          = {x1,y2};
			nearest_mbbpoint = mbb_point;
			nearest_pin      = i;
		}
	}
	
}

// Main algorithm function
void route(const string &file_name){
	
	read_file(file_name);
	
	p_nnumber  nearest_pins;
	p_nnumber  nearest_pointpin;
	vv_nnumber pos_nearest_pins(2);

	nearest_pins = find_nearest_pins();

	used_pins[nearest_pins.first] = used_pins[nearest_pins.second] = 1; // mark used pins
	n_usedpins += 2;

	pos_nearest_pins[0] = my_pins[nearest_pins.first].position;
	pos_nearest_pins[1] = my_pins[nearest_pins.second].position;

	if(n_usedpins == n_pins){
		// add L (p1,p2,p3) to tree
		L_tree.push_back(pos_nearest_pins[0]); 
		L_tree.push_back({pos_nearest_pins[0][0],pos_nearest_pins[1][1]});
		L_tree.push_back(pos_nearest_pins[1]);
		return;
	}

	nnumber   nearest_pin;
	v_nnumber nearest_mbbpoint, L_point;

	while(n_usedpins < n_pins){
			
		find_nearest_pin_MBB(pos_nearest_pins, nearest_pin, nearest_mbbpoint, L_point);
	
		// add L (p1,p2,p3) to tree
		L_tree.push_back(pos_nearest_pins[0]); 
		L_tree.push_back(L_point);
		L_tree.push_back(pos_nearest_pins[1]);

		// upgrade MBB points
		pos_nearest_pins[0] = nearest_mbbpoint;
		pos_nearest_pins[1] = my_pins[nearest_pin].position;

		used_pins[nearest_pin] = 1; // mark used to nearest pin
		n_usedpins++;

	}

	// add L (p1,p2,p3) to tree
	L_tree.push_back(pos_nearest_pins[0]); 
	L_tree.push_back({pos_nearest_pins[0][0],pos_nearest_pins[1][1]});
	L_tree.push_back(pos_nearest_pins[1]);	

}

void graph_tree(){
	vv_char  graph(max_X+3, v_char(max_Y+3,' '));
	nnumber wirelenght = 0;

	//Add the L's conection in the graphic (*)
	for(int i=0; i<L_tree.size(); i+=3){
		for(int k=0;k<2;k++){
			if(L_tree[i+k][0]==L_tree[i+k+1][0]){
				for(int j=min(L_tree[i+k][1],L_tree[i+k+1][1]); j <= max(L_tree[i+k][1],L_tree[i+k+1][1]); j++)
					graph[L_tree[i+k][0]+1][j+1]='*';
			}
			else
				for(int j=min(L_tree[i+k][0],L_tree[i+k+1][0]); j <= max(L_tree[i+k][0],L_tree[i+k+1][0]); j++)
					graph[j+1][L_tree[i+k][1]+1]='*';
		}
	}

	// Add the pins in the graphic(X)
	for(int i=0; i<n_pins; i++)
		graph[my_pins[i].position[0]+1][my_pins[i].position[1]+1]='A'+i;
	
	// Print graphic
	for(int i=0; i<graph.size(); i++){
		for(int j=0;j<graph[i].size();j++){
			if(graph[i][j]=='*')wirelenght++;
			if(i==0 || i == graph.size()-1) cout << '-';
		       	else if(j==0 || j==graph[i].size()-1) cout << '|';
			else cout << graph[i][j];
		}
		cout << "\n";
	}
	cout << "The wirelenght is: " << wirelenght << ".\n";
}

/////////////////////////////////////////////////////////////////////////


int main( int argc, char* argv[]){
	
	if(argc==2){
		route(argv[1]);
		graph_tree();
	}
	else
		cout << "Error: Be sure to put only the input file name.\n ./steiner_tree file_name_input.txt\n";

	return 0;
}
