#pragma once

#include <functional>

namespace myutils {

// 声明红黑树颜色枚举
enum class RBColor { BLACK, RED };

// 申明红黑树存储数据类型模板
template <typename TKey = int, typename TVal = int>
class RBData {
 public:
  TKey key;
  TVal value;
};

// 声明红黑树结点类型
template <typename Tkey = int, typename Tval = int>
class RBNode {
 public:
  RBNode(RBData<Tkey, T2>&& data) : _data(std::move(data)) {
    _color = RBColor::RED;
    _parent = nullptr;
    _lchild = nullptr;
    _rchild = nullptr;
  }
  ~RBNode();

 public:
  const RBData<Tkey, Tval>& getData() { return _data; }

 public:
  RBNode<Tkey, Tval>* _parent;
  RBNode<Tkey, Tval>* _lchild;
  RBNode<Tkey, Tval>* _rchild;
  RBColor _color;

 private:
  RBData<Tkey, Tval> _data;
};

template <typename Tkey = int, typename Tval = int>
class RBTree {
 public:
  RBTree(RBNode<>);
  ~RBTree();

 public:
  // 创建一颗红黑树
  int Create();

  // 销毁红黑树
  int Destroy();

  // 插入节点
  int Ins();

  // 删除结点
  int Del();

  // 查询
  int Query();

  // 获取前驱节点
  int GetPredecessor();

  // 获取后继节点
  int GetSuccessor();

  // 获取当前红黑树的节点个数
  const size_t& GetTreeNodeum();

  // 获取红黑树的根
  const RBNode<Tkey, Tval>& Root();

  // 先序遍历，先中后代表根节点位置
  int PreorderTraversal();
  // 中序遍历
  int InorderTraversal();
  // 后序遍历
  int PostorderTraversal();

 private:
  // 插入节点后的变色旋转
  int FixIns();

  // 删除节点后的变色旋转
  int FixDel();

 private:
  RBNode<Tkey, Tval>* _root;
  RBNode<Tkey, Tval>* _nil;
  size_t _nodeNum;
  std::function<bool(RBNode<Tkey, Tval>*, RBNode<Tkey, Tval>*)> _compareFunc;
};

}  // namespace myutils