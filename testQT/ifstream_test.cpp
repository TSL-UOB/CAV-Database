// ifstream::is_open
#include <iostream>     // std::cout
#include <fstream>      // std::ifstream

int main () {
  std::ifstream ifs ("agent_config.txt");

  if (ifs.is_open()) {
    // print file:
    char c = ifs.get();
    while (ifs.good()) {
      std::cout << c;
      c = ifs.get();
    }
  }
  else {
    // show message:
    std::cout << "Error opening file";
  }

  return 0;
}