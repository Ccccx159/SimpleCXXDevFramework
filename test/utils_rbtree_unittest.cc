#include <vector>

#include "myutils.hpp"
using namespace myutils;

int main(int argc, char* argv[]) {
  Logger_Init(argv[0]);
  DLOG(INFO) << "Create Tree with root 10:";
  RBNode<> root(10, 10);
  RBTree<> T([](const RBNode<>* n1, const RBNode<>* n2) {
    return n1->getData()._key - n2->getData()._key;
  });
  T.print();
  std::vector<std::pair<int, int>> data{{10, 10}, {13, 13}, {15, 15},
                                        {5, 5},   {14, 14}, {9, 9}};
  for (auto it : data) {
    DLOG(INFO) << "Insert node (" << it.first << ", " << it.second << ')';
    T.Insert(it.first, it.second);
    T.print();
  }
  LOG(INFO) << "finish insert";
  DLOG(INFO) << "Full Tree:";
  T.print();

  for (auto it : data) {
    DLOG(WARNING) << "Delete node (" << it.first << ", " << it.second << ')';
    T.Delete(it.first);
    T.print();
  }
  LOG(INFO) << "finish delete";

  return 0;
}