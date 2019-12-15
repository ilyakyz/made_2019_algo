#ifndef DSU_hpp
#define DSU_hpp

#include <stdio.h>
#include <vector>

class DSU {
 public:
  explicit DSU(size_t size);

  int find(int element);
  void merge(int left, int right);
  void print() const;

 private:
  std::vector<int> parent;
  std::vector<int> rank;
};


#endif /* DSU_hpp */
