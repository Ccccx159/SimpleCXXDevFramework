#include <vector>

#include "myutils.hpp"
using namespace myutils;

int main(int argc, char* argv[]) {
  Logger_Init(argv[0]);
  LOG(INFO) << "Create Tree with root 10:";
  RBNode root(10, 10);
  RBTree T(root, [](const RBNode* n1, const RBNode* n2) {
    return n1->getData()._key - n2->getData()._key;
  });
  T.print();
  std::vector<std::pair<int, int>> data{{13, 13}, {15, 15}, {5, 5}, {14, 14},
                                        {9, 9}};
  for (auto it : data) {
    LOG(INFO) << "Insert node (" << it.first << ", " << it.second << ')';
    T.Ins(it.first, it.second);
    T.print();
  }

  LOG(INFO) << "Full Tree:";
  T.print();

  for (auto it : data) {
    LOG(WARNING) << "Delete node (" << it.first << ", " << it.second << ')';
    T.Del(it.first);
    T.print();
  }
  LOG(WARNING) << "Delete Root (10, 10)";
  T.Del(10);
  T.print();
  return 0;
}