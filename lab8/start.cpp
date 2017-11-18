// Starting code for Checkpoints 2 and 3.  This includes
// functions to read the grid and to output it.

#include <fstream>
#include <iostream>
#include <list>
#include <vector>
using namespace std;


// A simple class to represent a point location.  It only has a
// constructor and a two public member variables.  This is one of the
// few times that you are allowed to use non-private member variables.

class Point {
public:
  Point(int x0, int y0) : x(x0), y(y0) {}
  int x,y;
};


// Input the grid and the start location.  The input is a sequence of
// x y locations, terminated by x==0 and y==0.  The last input, which
// follows 0 0 input, is the start location.
//
// The grid is represented as a 2d vector of bools, with each location
// that is blocked --- meaning that no path can go through --- being
// represented by the value "true".  The grid is large enough to
// include all blocked points and include the starting location.  The
// first coordinate of the vector of vectors is the x coordinate, and
// the second is the y coordinate.  The format of the input is
// specified in the lab handout.

void read_grid(istream& istr, vector<vector<bool> > & blocked_grid,
	       int & start_x, int & start_y) {

  // Read the x y locations into a list of Points.  Keep track of the
  // max x and max y values so that the size of the grid can be
  // determined.
  int x, y;
  int max_x = 0, max_y = 0;  // keep track of the max coordinate values
  list<Point> blocked_points;
  while ((istr >> x >> y) && ! (x==0 && y==0)) {
    blocked_points.push_back(Point(x,y));
    if (x > max_x) max_x = x;
    if (y > max_y) max_y = y;
  }

  // Now that a 0 0 location has been read, read the start location.
  // If this is beyond the max x or y value then update these values.
  istr >> start_x >> start_y;
  if (start_x > max_x) max_x = start_x;
  if (start_y > max_y) max_y = start_y;

  // Make a vector of vectors with all entries marked false.   
  vector<bool> one_row_of_ys(max_y+1, false);
  vector<vector<bool> > empty_grid(max_x+1, one_row_of_ys);
  blocked_grid = empty_grid;

  // For Point in the list, mark the location in the list as blocked.
  list<Point>::iterator p;
  for (p = blocked_points.begin(); p != blocked_points.end(); ++p) {
    blocked_grid[p->x][p->y] = true;
  }
}


// Output the grid to cout.  The form of the output is explained in
// the cout statement below.

void print_grid(vector<vector<bool> > const& blocked_grid, int start_x, int start_y) {

  cout << "Here is the grid with the origin in the upper left corner, x increasing \n"
       << "horizontally and y increasing down the screen. An 'X' represents a blocked\n"
       << "location and the 'S' represents the starting location.\n\n";

  for (int y=0; y<blocked_grid[0].size(); ++y) {
    for (int x=0; x<blocked_grid.size(); ++x) {
      if (x == start_x && y == start_y)
        cout << " S";
      else if (blocked_grid[x][y])
        cout << " X";
      else 
        cout << " .";
    }
    cout << '\n';
  }
}

int find_paths_free(int start_x, int start_y) {
  if (start_x == 0 && start_y == 0) {
    return 1;
  }
  if (start_x == 0 && start_y > 0) {
    return find_paths_free(start_x, start_y-1);
  }
  if (start_x > 0 && start_y == 0) {
    return find_paths_free(start_x-1, start_y);
  }
  if (start_x > 0 && start_y > 0) {
    return find_paths_free(start_x-1, start_y) + find_paths_free(start_x, start_y-1);
  }
  return 0;
}

/*int find_paths_free(int start_x, int start_y, bool& check) {
  if (start_x == 0 && start_y == 0) {
    if (check) {
      std::cout << "(0, 0).\n";
      check = false;
    }
    return 1;
  }
  if (check) std::cout << "(" << start_x << ", " << start_y << ") to ";
  if (start_x == 0 && start_y > 0) {
    return find_paths_free(start_x, start_y-1, check);
  }
  if (start_x > 0 && start_y == 0) {
    return find_paths_free(start_x-1, start_y, check);
  }
  if (start_x > 0 && start_y > 0) {
    return find_paths_free(start_x-1, start_y, check) + find_paths_free(start_x, start_y-1, check);
  }
  return 0;
}*/

int find_paths_blocked(vector<vector<bool> > const& grid, int start_x, int start_y, bool& check) {
  if (start_x == 0 && start_y == 0) {
    if (check) {
      std::cout << "(0, 0).\n";
      check = false;
    }
    // the origin should never be blocked
    return 1;
  }
  if (check) std::cout << "(" << start_x << ", " << start_y << ") to ";
  if (start_x == 0 && start_y > 0) {
    return (!grid[start_x][start_y-1] ? find_paths_blocked(grid, start_x, start_y-1, check) : 0);
  }
  if (start_x > 0 && start_y == 0) {
    return (!grid[start_x-1][start_y] ? find_paths_blocked(grid, start_x-1, start_y, check) : 0);
  }
  if (start_x > 0 && start_y > 0) {
    return (!grid[start_x-1][start_y] ? find_paths_blocked(grid, start_x-1, start_y, check) : 0) +
           (!grid[start_x][start_y-1] ? find_paths_blocked(grid, start_x, start_y-1, check) : 0);
  }
  return 0;
}



int main(int argc, char* argv[]) {
  if (argc != 2) {
    cerr << "Usage: " << argv[0] << " grid-file\n";
    return 1;
  }
  ifstream istr(argv[1]);
  if (!istr) {
    cerr << "Could not open " << argv[1] << endl;
    return 1;
  }
  
  vector<vector<bool> > blocked_grid;
  int start_x, start_y;
  read_grid(istr, blocked_grid, start_x, start_y);
  print_grid(blocked_grid, start_x, start_y);

  // Start here with your code...

  // std::cout << start_x << " " << start_y << std::endl;
  /*int num_paths1 = find_paths_free(0, 0);
  int num_paths2 = find_paths_free(1, 1);
  int num_paths3 = find_paths_free(2, 1);
  int num_paths4 = find_paths_free(2, 2);
  std::cout << "num_paths1: " << num_paths1 << std::endl;
  std::cout << "num_paths2: " << num_paths2 << std::endl;
  std::cout << "num_paths3: " << num_paths3 << std::endl;
  std::cout << "num_paths4: " << num_paths4 << std::endl;
  int num_paths5 = find_paths_blocked(blocked_grid, 0, 0);
  int num_paths6 = find_paths_blocked(blocked_grid, 1, 1);
  int num_paths7 = find_paths_blocked(blocked_grid, 2, 2);
  int num_paths8 = find_paths_blocked(blocked_grid, 2, 3);
  int num_paths9 = find_paths_blocked(blocked_grid, start_x, start_y);
  std::cout << "num_paths5: " << num_paths5 << std::endl;
  std::cout << "num_paths6: " << num_paths6 << std::endl;
  std::cout << "num_paths7: " << num_paths7 << std::endl;
  std::cout << "num_paths8: " << num_paths8 << std::endl;
  std::cout << "num_paths9: " << num_paths9 << std::endl;*/
  std::cout << "Legal path to (4, 4):" << std::endl;
  bool check = true;
  find_paths_blocked(blocked_grid, start_x, start_y, check);

  return 0;
}
