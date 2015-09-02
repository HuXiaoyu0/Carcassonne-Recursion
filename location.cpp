#include <cassert>
#include "location.h"


// CONSTRUCTOR
Location::Location (int r, int c, int rot) : row(r), column(c), rotation(rot) { 
  assert (rotation == 0 || 
          rotation == 90 || 
          rotation == 180 || 
          rotation == 270); 
}


// helper function to output the placement of this tile within a solution
std::ostream& operator<<(std::ostream &ostr, const Location &loc) {
  ostr << "(" << loc.row << "," << loc.column << "," << loc.rotation << ")";
  return ostr;
}
