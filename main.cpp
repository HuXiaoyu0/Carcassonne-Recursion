#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <string>
#include <vector>
#include <cassert>

#include "tile.h"
#include "location.h"
#include "board.h"

using namespace std;

// this global variable is set in main.cpp and is adjustable from the command line
// (you are not allowed to make your own global variables)
int GLOBAL_TILE_SIZE = 11;


// ==========================================================================
// Helper function that is called when an error in the command line
// arguments is detected.
void usage(int argc, char *argv[]) {
  std::cerr << "USAGE: " << std::endl;
  std::cerr << "  " << argv[0] << " <filename>  -board_dimensions <h> <w>" << std::endl;
  std::cerr << "  " << argv[0] << " <filename>  -board_dimensions <h> <w>  -all_solutions" << std::endl;
  std::cerr << "  " << argv[0] << " <filename>  -board_dimensions <h> <w>  -allow_rotations" << std::endl;
  std::cerr << "  " << argv[0] << " <filename>  -all_solutions  -allow_rotations" << std::endl;
  std::cerr << "  " << argv[0] << " <filename>  -tile_size <odd # >= 11>" << std::endl;
  exit(1);
}

// The fllowing 3 bool funcions are used to see whether a location in the board is valid
//If one tile is at the bound of the board, the bound side should be parsture
bool OuterValid(Board &board, Tile* one, int i, int j){
	bool value = true;
	if (i==0)
		value = value && one->getNorth() == "pasture";
	//if (i !=0 && board.getTile(i-1,j) == NULL)
		//value = value && one->getNorth() == "pasture";
	if (i==board.numRows()-1)
		value = value && one->getSouth() == "pasture";
	//if (i != board.numRows()-1 && board.getTile(i+1,j) == NULL)
		//value = value && one->getSouth() == "pasture";
	if (j==0)
		value = value && one->getWest() == "pasture";
	//if (j != 0 && board.getTile(i,j-1) == NULL)
		//value = value && one->getWest() == "pasture";
	if (j==board.numColumns()-1)
		value = value && one->getEast() == "pasture";
	//if (j != board.numColumns()-1 && board.getTile(i,j+1) == NULL)
		//value = value && one->getEast() == "pasture";
	return value;
}
//Test whether a tile is ok with its nearbytiles
bool InnerValid(Board &board, Tile* one, int i, int j){
	bool value = true;

	if (board.getTile(i,j) != NULL)
		return false;

	if (i == 0 && j ==0)
		{
			if ( board.getTile(i+1,j)==NULL && board.getTile(i,j+1)==NULL)
				return false;
			else{
				if (board.getTile(i+1,j)!=NULL)
					value = value && board.getTile(i+1,j)->getNorth()==one->getSouth();
				if (board.getTile(i,j+1)!=NULL)
					value = value && board.getTile(i,j+1)->getWest()==one->getEast();
				return value;

			}
		}
		else if (i == 0 && j == board.numColumns()-1)
		{
			if ( board.getTile(i+1,j)==NULL && board.getTile(i,j-1)==NULL)
				return false;
			else{
				if (board.getTile(i+1,j)!=NULL)
					value = value && board.getTile(i+1,j)->getNorth()==one->getSouth();
				if (board.getTile(i,j-1)!=NULL)
					value = value && board.getTile(i,j-1)->getEast()==one->getWest();
				return value;

			}
		}
		else if (i == board.numRows()-1 && j == 0)
		{
			if ( board.getTile(i-1,j)==NULL && board.getTile(i,j+1)==NULL)
				return false;
			else{
				if (board.getTile(i-1,j)!=NULL)
					value = value && board.getTile(i-1,j)->getSouth()==one->getNorth();
				if (board.getTile(i,j+1)!=NULL)
					value = value && board.getTile(i,j+1)->getWest()==one->getEast();
				return value;

			}
		}
		else if (i == board.numRows()-1 && j == board.numColumns()-1)
		{
			if ( board.getTile(i-1,j)==NULL && board.getTile(i,j-1)==NULL)
				return false;
			else{
				if (board.getTile(i-1,j)!=NULL)
					value = value && board.getTile(i-1,j)->getSouth()==one->getNorth();
				if (board.getTile(i,j-1)!=NULL)
					value = value && board.getTile(i,j-1)->getEast()==one->getWest();
				return value;

			}
		}
		else if (i == 0)
		{
			if (board.getTile(i,j-1)==NULL && board.getTile(i+1,j)==NULL && board.getTile(i,j+1)==NULL)
				return false;
			else{
				if (board.getTile(i,j-1)!=NULL)
					value = value && board.getTile(i,j-1)->getEast()==one->getWest();
				if (board.getTile(i+1,j)!=NULL)
					value = value && board.getTile(i+1,j)->getNorth()==one->getSouth();
				if (board.getTile(i,j+1)!=NULL)
					value = value && board.getTile(i,j+1)->getWest()==one->getEast();
				return value;

			}
		}
		else if (i == board.numRows()-1)
		{
			if (board.getTile(i-1,j)==NULL && board.getTile(i,j-1)==NULL && board.getTile(i,j+1)==NULL)
				return false;
			else{
				if (board.getTile(i-1,j)!=NULL)
					value = value && board.getTile(i-1,j)->getSouth()==one->getNorth();
				if (board.getTile(i,j-1)!=NULL)
					value = value && board.getTile(i,j-1)->getEast()==one->getWest();
				if (board.getTile(i,j+1)!=NULL)
					value = value && board.getTile(i,j+1)->getWest()==one->getEast();
				return value;

			}
		}
		else if (j == 0)
		{
			if (board.getTile(i,j+1)==NULL && board.getTile(i+1,j)==NULL && board.getTile(i-1,j)==NULL)
				return false;
			else{
				if (board.getTile(i,j+1)!=NULL)
					value = value && board.getTile(i,j+1)->getWest()==one->getEast();
				if (board.getTile(i+1,j)!=NULL)
					value = value && board.getTile(i+1,j)->getNorth()==one->getSouth();
				if (board.getTile(i-1,j)!=NULL)
					value = value && board.getTile(i-1,j)->getSouth()==one->getNorth();
				return value;

			}
		}
		else if (j == board.numColumns()-1)
		{
			if (board.getTile(i,j-1)==NULL && board.getTile(i+1,j)==NULL && board.getTile(i-1,j)==NULL)
				return false;
			else{
				if (board.getTile(i,j-1)!=NULL)
					value = value && board.getTile(i,j-1)->getEast()==one->getWest();
				if (board.getTile(i+1,j)!=NULL)
					value = value && board.getTile(i+1,j)->getNorth()==one->getSouth();
				if (board.getTile(i-1,j)!=NULL)
					value = value && board.getTile(i-1,j)->getSouth()==one->getNorth();
				return value;

			}
		}
		else
		{
			if (board.getTile(i-1,j)==NULL && board.getTile(i+1,j)==NULL && board.getTile(i,j+1)==NULL && board.getTile(i,j-1)==NULL)
				return false;
			else{
				if (board.getTile(i-1,j)!=NULL)
					value = value && board.getTile(i-1,j)->getSouth()==one->getNorth();
				if (board.getTile(i+1,j)!=NULL)
					value = value && board.getTile(i+1,j)->getNorth()==one->getSouth();
				if (board.getTile(i,j+1)!=NULL)
					value = value && board.getTile(i,j+1)->getWest()==one->getEast();
				if (board.getTile(i,j-1)!=NULL)
					value = value && board.getTile(i,j-1)->getEast()==one->getWest();
				return value;

			}
		}
		}
//If a tile is near a NULL in the board, the side to that NULL should be parsture
bool NullValid(Board &board){	                                                                   // to check if the location is already on the locations
	for ( int i = 0; i < board.numRows(); i++){
		for (int j = 0; j < board.numColumns(); j++){
			if (board.getTile(i, j) == NULL) continue;
			if (i != 0){
				if (board.getTile(i-1,j) == NULL && board.getTile(i,j)->getNorth() != "pasture")
					return false;
			}
			if (i != board.numRows()-1){
			    if(board.getTile(i+1,j) == NULL && board.getTile(i,j)->getSouth() != "pasture")
					return false;
			}
			if (j != 0){
			    if(board.getTile(i,j-1) == NULL && board.getTile(i,j)->getWest() != "pasture")
                    return false;
			}
			if (j != board.numColumns()-1){
				if(board.getTile(i,j+1) == NULL && board.getTile(i,j)->getEast() != "pasture")
					return false;
			}
		}}
	return true;
}


//Test if a solution is already been found
//If yes, the sum of the abusolute values of the differences between all locations should be zero
//The search_from_location will garantee that the one to the very left corner will be store first and not be deleted
bool IsDupli(vector<Location> locs, vector<vector<Location> > solutions){
	if (solutions.size() == 0)
	return false;
	
	vector<vector<Location> >::iterator ite = solutions.begin();
		
	for (; ite != solutions.end(); ite++){

		int sumR = 0;
	    int sumC = 0;
	    int sumRo = 0;


		int diffR = (*ite)[0].row - locs[0].row;
		int diffC = (*ite)[0].column - locs[0].column;
		int diffRo = (*ite)[0].rotation - locs[0].rotation;

		for (int i = 1; i<locs.size(); i++){

		sumR = sumR + abs((*ite)[i].row - locs[i].row - diffR);
		sumC = sumC + abs((*ite)[i].column - locs[i].column - diffC);
		sumRo = sumRo + abs((*ite)[i].rotation - locs[i].rotation - diffRo);
		}

	    if (sumR == 0 && sumC == 0 && sumRo == 0)
		return true;
	
	}
		return false;
}

//Give a position, this function search for solution fro that position
//If end has been reached, return true; Otherwise return faulse.
bool search_from_loc(Location loc, Board board, vector<Board> &boards, vector<Tile*> tiles, vector<Location> locs, vector<vector<Location> > &solutions, vector<bool> IsUsed){
	
	int count = 0;
	for (int i = 0; i<board.numRows(); i++){
		for (int j = 0; j<board.numColumns(); j++){
			if (board.getTile(i,j) != NULL)
				count++;
		}}


	if (count == tiles.size()){

        if (NullValid(board)) {
			if (!IsDupli(locs,solutions)){

	boards.push_back(board);
	

	solutions.push_back(locs);
	board.Clear_all();
	locs.clear();
	for (int t = 0; t<tiles.size(); t++)
		IsUsed[t] = false; 
	
			}}}
	for (int t = 0; t<tiles.size(); t++){
	  for (int i = loc.row-1; i <= loc.row+1; i++){
		for (int j = loc.column-1; j <= loc.column+1; j++){
			
			if (IsUsed[t]) continue;

			if (i < 0 || i >= board.numRows()) continue;
			if (j < 0 || j >= board.numColumns()) continue;
			if (i == loc.row+1 && j == loc.column+1) continue;
			if (i == loc.row-1 && j == loc.column-1) continue;
			if (i == loc.row+1 && j == loc.column-1) continue;
			if (i == loc.row-1 && j == loc.column+1) continue;

			if (board.getTile(i,j) != NULL) continue;
			

			if (InnerValid(board,tiles[t],i,j) && OuterValid(board,tiles[t],i,j)){
				board.setTile(i,j,tiles[t]);

				locs[t].row = i;
				locs[t].column = j;
				IsUsed[t] = true;
			}
			else
				continue;
			bool ReachEnd = search_from_loc(locs[t],board,boards,tiles,locs,solutions,IsUsed);
			board.Clear_one(i,j);
			locs[t].row = -1;
			locs[t].column = -1;
			locs[t].rotation = 0;
			IsUsed[t] = false;
		    if (ReachEnd)
				return true;
			else
				continue;
		}}}
	return false;
}

//Put the first tile ar every position of the board and start searching
void RightlyPlaceTiles(Location loc, Board board, vector<Board> &boards, vector<Tile*> tiles, vector<Location> locs, vector<vector<Location> > &solutions, vector<bool> IsUsed){

	for (int i = 0; i<board.numRows(); i++){
		for (int j = 0; j<board.numColumns(); j++){


			board.setTile(i,j,tiles[0]);

		

			loc.row = i;
			loc.column = j;
			IsUsed[0] = true;
			locs[0].row = i;
			locs[0].column = j;
			locs[0].rotation = 0;
			search_from_loc(loc, board, boards, tiles, locs, solutions, IsUsed);
			board.Clear_one(i,j);
			
		}}
}

void HandleCommandLineArguments(int argc, char *argv[], std::string &filename, 
                                int &rows, int &columns, bool &all_solutions, bool &allow_rotations) {

  // must at least put the filename on the command line
  if (argc < 2) {
    usage(argc,argv);
  }
  filename = argv[1];

  // parse the optional arguments
  for (int i = 2; i < argc; i++) {
    if (argv[i] == std::string("-tile_size")) {
      i++;
      assert (i < argc);
      GLOBAL_TILE_SIZE = atoi(argv[i]);
      if (GLOBAL_TILE_SIZE < 11 || GLOBAL_TILE_SIZE % 2 == 0) {
        std::cerr << "ERROR: bad tile_size" << std::endl;
        usage(argc,argv);
      }
    } else if (argv[i] == std::string("-all_solutions")) {
      all_solutions = true;
    } else if (argv[i] == std::string("-board_dimensions")) {
      i++;
      assert (i < argc);
      rows = atoi(argv[i]);
      i++;
      assert (i < argc);
      columns = atoi(argv[i]);
      if (rows < 1 || columns < 1) {
        usage(argc,argv);
      }
    } else if (argv[i] == std::string("-allow_rotations")) {
      allow_rotations = true;
    } else {
      std::cerr << "ERROR: unknown argument '" << argv[i] << "'" << std::endl;
      usage(argc,argv);
    }
  }
}


// ==========================================================================
void ParseInputFile(int argc, char *argv[], const std::string &filename, std::vector<Tile*> &tiles) {

  // open the file
  std::ifstream istr(filename.c_str());
  if (!istr) {
    std::cerr << "ERROR: cannot open file '" << filename << "'" << std::endl;
    usage(argc,argv);
  }
  assert (istr);

  // read each line of the file
  std::string token, north, east, south, west;
  while (istr >> token >> north >> east >> south >> west) {
    assert (token == "tile");
    Tile *t = new Tile(north,east,south,west);
    tiles.push_back(t);
  }
}


// ==========================================================================
int main(int argc, char *argv[]) {

  std::string filename;
  int rows = -1;
  int columns = -1;
  bool all_solutions = false;
  bool allow_rotations = false;
  HandleCommandLineArguments(argc, argv, filename, rows, columns, all_solutions, allow_rotations);


  // load in the tiles
  std::vector<Tile*> tiles;
  ParseInputFile(argc,argv,filename,tiles);


  // confirm the specified board is large enough
  if (rows < 1  ||  columns < 1  ||  rows * columns < tiles.size()) {
    std::cerr << "ERROR: specified board is not large enough" << rows << "X" << columns << "=" << rows*columns << " " << tiles.size() << std::endl;
    usage(argc,argv);
  }
   
  Board board(rows,columns);
  Location loc(-1,-1,0);
  const int n = tiles.size();
  vector<Board> boards;
  vector<bool> IsUsed(n,false);
  vector<Location> locs(n,loc);
  vector<vector<Location> > solutions;

  RightlyPlaceTiles(loc, board, boards, tiles, locs, solutions, IsUsed);
  


  //for (int i = 0; i<tiles.size(); i++)


  //for (int i = 0; i < 5; i++) {

    // generate a random tile layouts
   
    //std::vector<Location> locations;
    
	/*
    // print the solution
    std::cout << "probably-not-a-Solution: ";
    for (int i = 0; i < locations.size(); i++) {
      std::cout << locations[i];
    }
    std::cout << std::endl;
	*/

    // print the ASCII art board representation
    int i = -1;
	int count = 0;
	for (vector<Board>::iterator ite = boards.begin(); ite != boards.end(); ite++){
		i++;
		count++;
		std::cout<<"Solution: ";
		for (int j = 0; j<tiles.size(); j++)
		std::cout<<solutions[i][j];
		std::cout<<endl;
		ite->Print();
	}
	if (count == 0)
		std:cout<<"No Solutions."<<std::endl;
	else
	    cout<<"Found "<<count<<" Solution(s)."<<std::endl;
  //}
  
  
  // delete the tiles
  for (int t = 0; t < tiles.size(); t++) {
    delete tiles[t];
  }
}
// ==========================================================================
