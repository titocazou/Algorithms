#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <stack>
#include <set>
#include <map>
#include <stack>
#include<utility>
#include <algorithm>
#include <limits>


// A* algorithm implementation for a pacman game.
// Expected inputs to the algorithm :
// 35 35
// 35 1
// 37 37
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// %-------%-%-%-----------%---%-----%-%
// %-%%%%%%%-%-%%%-%-%%%-%%%-%%%%%%%-%-%
// %-------%-------%-%-----%-----%-%---%
// %%%%%-%%%%%-%%%-%-%-%-%%%-%%%%%-%-%%%
// %---%-%-%-%---%-%-%-%---%-%---%-%---%
// %-%%%-%-%-%-%%%-%%%%%-%%%-%-%%%-%%%-%
// %-------%-----%---%---%-----%-%-%---%
// %%%-%%%%%%%%%-%%%%%%%-%%%-%%%-%-%-%-%
// %-------------%-------%-%---%-----%-%
// %-%-%%%%%-%-%%%-%-%-%%%-%-%%%-%%%-%-%
// %-%-%-----%-%-%-%-%-----%---%-%-%-%-%
// %-%-%-%%%%%%%-%-%%%%%%%%%-%%%-%-%%%-%
// %-%-%-%-----%---%-----%-----%---%---%
// %%%-%%%-%-%%%%%-%%%%%-%%%-%%%-%%%%%-%
// %-----%-%-%-----%-%-----%-%---%-%-%-%
// %-%-%-%-%-%%%-%%%-%%%-%%%-%-%-%-%-%-%
// %-%-%-%-%-----------------%-%-%-----%
// %%%-%%%%%%%-%-%-%%%%%-%%%-%-%%%-%%%%%
// %-------%-%-%-%-----%---%-----%-%---%
// %%%%%-%-%-%%%%%%%%%-%%%%%%%%%%%-%-%%%
// %---%-%-----------%-%-----%---%-%---%
// %-%%%-%%%%%-%%%%%%%%%-%%%%%-%-%-%%%-%
// %-%---%------%--------%-----%-------%
// %-%-%-%%%%%-%%%-%-%-%-%-%%%%%%%%%%%%%
// %-%-%---%-----%-%-%-%-------%---%-%-%
// %-%-%%%-%%%-%-%-%-%%%%%%%%%-%%%-%-%-%
// %-%---%-%---%-%-%---%-%---%-%-%-----%
// %-%%%-%%%-%%%%%-%%%-%-%-%%%%%-%-%%%%%
// %-------%---%-----%-%-----%---%-%---%
// %%%-%-%%%%%-%%%%%-%%%-%%%-%-%%%-%-%%%
// %-%-%-%-%-%-%-%-----%-%---%-%---%-%-%
// %-%-%%%-%-%-%-%-%%%%%%%%%-%-%-%-%-%-%
// %---%---%---%-----------------%-----%
// %-%-%-%-%%%-%%%-%%%%%%%-%%%-%%%-%%%-%
// %.%-%-%-------%---%-------%---%-%--P%
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// First line is the initial X Y position of Pacman
// Second line is the X Y position of the food
// Third line is the grid zie (rows x columns)
// Fourth line to the end is the grid where '%' are obstacle
// 'P' is pacman, '.' is the food and '-' are navigable nodes
//
// Expected output:
// N
// X0 Y0
// X1 Y1
// .  .
// .  .
// .  .
// First line is the total of move required to reach the food
// Second line to the end are all the X Y position (one per line) required
// to reach the food from the initial pacman position
//
//
// Convention used: top left corner is [0][0]. X is positive downward, Y is positive eastward


using namespace std;

class PacMan{
public:
  PacMan (vector<string> grid, int rows, int columns,int food_x, int food_y, int pacman_x, int  pacman_y) :
    grid_(grid),
    rows_(rows),
    columns_(columns),
    food_(make_pair(food_x, food_y)),
    pacman_(make_pair(pacman_x, pacman_y)),
    g_cost_map_(rows,vector<int>(columns, std::numeric_limits<int>::max())),
    f_cost_map_(g_cost_map_) {
    g_cost_map_[pacman_x][pacman_y] = 0;
    f_cost_map_[pacman_x][pacman_y] = heuristic_cost(pacman_);
    open_set_.insert(pacman_);
  }

  // public functions
  void run_A_star (); // main routine that runs the A* search algorithm

private:
  // private functions
  int heuristic_cost (const pair<int,int>& start_node) const;  // function that calculate the heuristic cost to reach the food from a given node
  pair<int,int> find_lowest_f_cost_node() const; // function to find node with the lowest f cost from the open_set_
  vector<pair<int,int>> get_all_neighbors(const pair<int,int>& node) const;  // function that returns all the current valid neighbors of a node
  void reconstruct_path(); // function that generate path taken once the food is reached

  // Class members
  vector<string> grid_; // vector of string representing the pacman grid. '-' are accessible nodes while '%' are walls
  int rows_; // number of rows in the grid
  int columns_; // number of columns in the grid
  pair<int,int> food_; // coordinates <X,Y> of the food
  pair<int,int> pacman_; // current coordinate <X,Y> of pacman
  vector<vector<int>> g_cost_map_; // 2D map of cost to reach each node
  vector<vector<int>> f_cost_map_; // 2D map of total cost @ each node. f[X][Y] = g[X][Y] + (heuristic cost for this node)
  set<pair<int,int>> open_set_; // set of nodes that should be visited
  set<pair<int,int>> closed_set_; // set of nodes that already have been visited
  map<pair<int,int>,pair<int,int>> previous_node_map_; // dictionary that links a node to the node it came from
};


// return heuristic cost by computing l1 norm between a given node and the food
int PacMan::heuristic_cost(const pair<int, int>& start_node) const{
  return std::abs(food_.first - start_node.first) + std::abs(food_.second - start_node.second);
}

// iterate through open set and return node with lowest f cost
pair<int,int> PacMan::find_lowest_f_cost_node() const{
  int min_score = std::numeric_limits<int>::max();
  pair<int,int> lowest_node;

  for (set<pair<int,int>>::iterator it=open_set_.begin(); it!=open_set_.end(); ++it) {
    if (f_cost_map_[it->first][it->second] <= min_score) {
      lowest_node = *it;
      min_score = f_cost_map_[it->first][it->second];
    }
  }
  return lowest_node;
}

// return all valid neighbors of a given node
vector<pair<int,int>> PacMan::get_all_neighbors(const pair<int,int>& node) const{
  vector<pair<int,int>> neighbors;

  int x = node.first;
  int y = node.second;

  // check that neighbor is within the grid and is not a wall
  if (y+1 < columns_ && grid_[x][y+1]!= '%')
    neighbors.push_back(make_pair(x, y+1));
  if (y-1 >= 0 && grid_[x][y-1]!= '%')
    neighbors.push_back(make_pair(x, y-1));
  if (x+1 < rows_ && grid_[x+1][y]!= '%')
    neighbors.push_back(make_pair(x+1, y));
  if (x-1 >= 0 && grid_[x-1][y]!= '%')
    neighbors.push_back(make_pair(x-1, y));

  return neighbors;
}

// navigate through previous_node_map_ and display path taken
void PacMan::reconstruct_path() {
  pair<int,int> node = pacman_; // start from current pacman position (which should be at food_ by now)
  int count = 0; // count to keep track of number of nodes visited for optimal solution
  stack<pair<int,int>> path; // stack that will be used to arrange path forward in time

  // navigate through previous_node_map_ (which goes backward) and add each entry to a stack
  while(previous_node_map_.count(node)) {
    path.push(node);
    node = previous_node_map_[node];
    count++; // keep track of number of nodes visited
  }
  path.push(node); // don't forget the initial node which was never added to previous_node_map_

  cout << count << '\n'; // display count

  // go through the stack of nodes and display them one by one. This ends up display a forward in time path
  while (!path.empty()) {
    node = path.top();
    path.pop();
    cout << node.first << " " << node.second << '\n';
  }
}


// main A* routine
void PacMan::run_A_star () {
  while (!open_set_.empty()) {
    pacman_ = find_lowest_f_cost_node();

    // if we reached the food we are done. display the path
    if (pacman_ == food_) {
      reconstruct_path();
    }

    // erase current node from open_set_
    open_set_.erase(pacman_);

    // add current node to closed_set_
    closed_set_.insert(pacman_);

    // get all neighbors
    vector<pair<int,int>> neighbors = get_all_neighbors(pacman_);

    int g_cost; // g cost of node
    int moving_cost = 1; // moving cost to a neighbor is 1 unless it is food (which is 0 in that case)

    // iterate through neighbors
    for (vector<pair<int,int>>::iterator it=neighbors.begin(); it!=neighbors.end(); ++it) {
      // if neighbor is in closed set don't bother
      if (closed_set_.count(*it))
        continue;

      // if neighbor is food, it costs nothing to go there
      if (*it == food_)
        moving_cost = 0 ;

      // compute g cost of neighbor
      g_cost = g_cost_map_[pacman_.first][pacman_.second] + moving_cost; // g cost is current node cost + cost of moving to neighbor

      if(open_set_.count(*it) == 0) { // if neighbor is not in open set, add it and continue to logic below
        open_set_.insert(*it);
      } else if (g_cost_map_[it->first][it->second] <= g_cost) { // if current neighbor cost is higher than current g cost, continue to logic below
        continue;
      }
      // at this point, either this is the first time the neighboring node is visited and the cost goes from infinity to g_cost
      // or g_cost is smaller than the current neighbor cost. In both cases, update the g cost, compute the f cost and keep track
      // of the previous node
      g_cost_map_[it->first][it->second] = g_cost;
      f_cost_map_[it->first][it->second] = g_cost + heuristic_cost(*it);
      previous_node_map_.insert(make_pair(*it, pacman_));
    }
  }
}

int main(void) {
  int rows, columns, pacman_x, pacman_y, food_x, food_y;

  // parse terminal inputs for pacman and food position as well as the grid size
  cin >> pacman_x >> pacman_y; // pacman coordinates
  cin >> food_x >> food_y; // food coordinates
  cin >> rows >> columns; // rows and columns

  vector <string> grid; // grid_ is a vector of strings

  // parse terminal inputs and generate the grid
  for(int i=0; i<rows; i++) {
    string s; cin >> s; // input row by row
    grid.push_back(s);
  }

  // instantiate pacman class
  PacMan pacman(grid,rows, columns, food_x, food_y, pacman_x, pacman_y);

  // run A *
  pacman.run_A_star();

  return 0;
}