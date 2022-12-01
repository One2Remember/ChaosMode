#include "UnorderedMap.h"
#include "UnorderedSet.h"

#include <iostream>
#include <fstream>
#include <string>

void TestMap(std::ostream &, int, int);
void TestSet(std::ostream &, int, int);

int main(int argc, char* argv[]) {
  if(argc < 5) {
    std::cout << "Need the following arguments:\nFile Name\nNumber of Iterations\nSize of Map/Set to Construct\nTest map or set? Map: 0, Set: 1" << std::endl;
  }

  std::ofstream file;
  file.open(argv[1]);

  
  // testing mape
  if(std::stoi(argv[4]) == 0) {
    file << "Testing ChaosMode for UnorderedMap with MapSize = " << argv[2] << " and # of iterations = " << argv[3] << std::endl;
    TestMap(file, std::stoi(argv[2]), std::stoi(argv[3]));
  } 
  // testing set
  else if(std::stoi(argv[4]) == 1) {
    file << "Testing ChaosMode for UnorderedSet with SetSize = " << argv[2] << " and # of iterations = " << argv[3] << std::endl;
    TestSet(file, std::stoi(argv[2]), std::stoi(argv[3]));
  } 
  // error
  else {
    std::cout << "Error: Argument 4 should be 0 for map or 1 for set" << std::endl;
  }

  file.close();
}


void TestMap(std::ostream & file, int iterations, int mapSize) {
  for(int i = 0; i < iterations; i++) {
    UnorderedMap<int, std::string> myMap;
    for(int i = 0; i < mapSize; i++) {
      myMap.insert({i, std::to_string(i)});
    }
    for(auto it = myMap.begin(); it != myMap.end(); ++it) {
      file << "Key: " << it->first << ", Value: " << it->second << std::endl;
    }
    file << "~~~~~~~~~~~~~~~~~~" << std::endl;
  }
}

void TestSet(std::ostream & file, int iterations, int setSize) {
  for(int i = 0; i < iterations; i++) {
    UnorderedSet<int> mySet;
    for(int i = 0; i < setSize; i++) {
      mySet.insert(i);
    }
    for(auto it = mySet.begin(); it != mySet.end(); ++it) {
      file << "Key: " << *it << std::endl;
    }
    file << "~~~~~~~~~~~~~~~~~~" << std::endl;
  }
}