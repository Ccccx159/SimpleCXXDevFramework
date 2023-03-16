#pragma once

#include <functional>
#include <iostream>
#include <queue>
#include <stack>

#include "logger/logger.hpp"

namespace myutils {

enum class RBColor { BLACK, RED };

template <typename Tkey, typename Tval>
class RBTree;

template <typename Tkey = int, typename Tval = int>
struct RBData {
  Tkey _key;
  Tval _value;
};

template <typename Tkey = int, typename Tval = int>
class RBNode {
 public:
  using TRBData = struct RBData<Tkey, Tval>;
  RBNode(const Tkey& k, const Tval& v) {
    _data._key = k;
    _data._value = v;
  }
  RBNode() {}
  ~RBNode() {}

 public:
  const TRBData& getData() const { return _data; }

  void setData(const Tkey& k, const Tval& v) {
    _data._key = k;
    _data._value = v;
  }

 public:
  RBNode<Tkey, Tval>* _parent = nullptr;
  RBNode<Tkey, Tval>* _lchild = nullptr;
  RBNode<Tkey, Tval>* _rchild = nullptr;
  RBColor _color = RBColor::RED;
  std::pair<int, int> _pt;

 private:
  TRBData _data;
  friend class RBTree<Tkey, Tval>;
};

template <typename Tkey = int, typename Tval = int>
class RBTree {
 public:
  using TRBData = struct RBData<Tkey, Tval>;
  using TRBNode = RBNode<Tkey, Tval>;
  using TFCompare = std::function<int(const TRBNode*, const TRBNode*)>;

  RBTree(TFCompare fc) : _compareFunc(fc) {
    _nil = new TRBNode;
    if (_nil) {
      _nil->_color = RBColor::BLACK;
    }
  }

  virtual ~RBTree() {
    if (_root != nullptr && _root != _nil) {
      PostorderTraversal(_root);
    }
    delete _nil;
  }

 public:
  // 插入节点
  int Insert(const Tkey& key, const Tval& val) {
    TRBNode* node = new TRBNode(key, val);
    if (nullptr == node) {
      LOG(ERROR) << "allocate mem failed!...";
      return -1;
    }
    if (nullptr == _root) {
      node->_color = RBColor::BLACK;
      node->_lchild = node->_rchild = _nil;
      _root = node;
      _nodeNum++;
      return 0;
    }
    TRBNode* cur = _root;
    while (cur != _nil) {
      int comp = _compareFunc(node, cur);
      if (comp == 0) {
        cur->setData(key, val);
        return 0;
      } else if (comp < 0 && cur->_lchild != _nil) {
        cur = cur->_lchild;
      } else if (comp < 0 && cur->_lchild == _nil) {
        cur->_lchild = node;
        break;
      } else if (comp > 0 && cur->_rchild != _nil) {
        cur = cur->_rchild;
      } else if (comp > 0 && cur->_rchild == _nil) {
        cur->_rchild = node;
        break;
      }
    }
    node->_parent = cur;
    node->_lchild = node->_rchild = _nil;
    _nodeNum++;
    return FixIns(node);
  }

  // 删除结点
  int Delete(const Tkey& key) {
    TRBNode* cur = nullptr;
    Tval fakeVal;
    TRBNode fakeNode(key, fakeVal);
    if (0 != PreorderTraversal(&cur, &fakeNode, _root)) {
      LOG(ERROR) << "Query Failed! Cannot find any node by the key";
      return -1;
    }
    TRBNode* x = _nil;
    // 删除节点具有非空双子，则找到其后继节点 suc，将 suc 的 data
    // 拷贝给删除节点，
    // 具有非空双子的节点的后继节点必然不可能双子非空，因此问题转换为删除空儿子节点
    // 或单儿子节点的节点。
    if (cur->_rchild != _nil && cur->_lchild != _nil) {
      TRBNode* suc = GetSuccessor(key);
      cur->setData(suc->_data._key, suc->_data._value);
      cur = suc;
    }

    TRBNode* child = cur->_lchild == _nil ? cur->_rchild : cur->_lchild;
    TRBNode* parent = cur->_parent;
    if (parent == nullptr) {
      _root = child;
    } else {
      if (cur == parent->_lchild) parent->_lchild = child;
      if (cur == parent->_rchild) parent->_rchild = child;
    }
    child->_parent = parent;
    // 删除红色节点不影响红黑树结构，因此当且仅当删除节点为黑色节点时才进行
    // Fixup
    if (cur->_color == RBColor::BLACK) FixDel(child);
    delete cur;
    _nodeNum--;
    return 0;
  }

  // 查询
  TRBData Query(const Tkey& key) {
    if (nullptr == _root || 0 == _nodeNum) {
      LOG(WARNING) << "This is an Empty RBTree...";
      return TRBData();
    }
    TRBNode* tar = nullptr;
    Tval fakeVal;
    TRBNode fakeNode(key, fakeVal);
    LOG_IF(ERROR, 0 != PreorderTraversal(&tar, &fakeNode, _root))
        << "Query Failed! Cannot find any node by the key";

    TRBData d = tar != nullptr ? tar->getData() : TRBData();
    return d;
  }

  // 获取前驱节点
  TRBNode* GetPredecessor(const Tkey& key) {
    std::stack<TRBNode*> st;
    std::stack<TRBNode*> str;
    TRBNode* cur = _root;
    while (true) {
      while (cur != _nil) {
        st.push(cur);
        cur = cur->_lchild;
      }
      if (st.empty()) break;
      cur = st.top();
      st.pop();
      if (key == cur->getData()._key && !str.empty()) {
        return str.top();
      }
      str.push(cur);
      cur = cur->_rchild;
    }

    LOG(ERROR)
        << "Node with key has no predecessor or There has no Node with key";
    return nullptr;
  }

  // 获取后继节点
  TRBNode* GetSuccessor(const Tkey& key) {
    std::stack<TRBNode*> st;
    std::stack<TRBNode*> str;
    TRBNode* cur = _root;
    while (true) {
      while (cur != _nil && cur != nullptr) {
        st.push(cur);
        cur = cur->_rchild;
      }
      if (st.empty()) break;
      cur = st.top();
      st.pop();
      if (key == cur->_data._key && !str.empty()) {
        return str.top();
      }
      str.push(cur);
      cur = cur->_lchild;
    }

    LOG(ERROR) << " Key Node has no successor or There has no Node with key";
    return nullptr;
  }

  // 获取当前红黑树的节点个数
  const size_t& GetTreeNodeNum(void) { return _nodeNum; }

  // 获取红黑树的根
  const TRBNode* Root() {
    if (nullptr == _root) {
      LOG(ERROR) << "This is an Empty RBTree...";
      return nullptr;
    }
    return _root;
  }

 private:
  // 插入节点后的变色旋转
  int FixIns(TRBNode* cur) {
    // 父亲是黑色节点，则维持不变
    if (cur->_parent->_color == RBColor::BLACK) return 0;
    // 父亲是红色结点
    while (cur->_parent != nullptr && cur->_parent->_color == RBColor::RED) {
      TRBNode* father = cur->_parent;
      TRBNode* grand = father->_parent;
      TRBNode* uncle =
          father == grand->_lchild ? grand->_rchild : grand->_lchild;

      // case1、父亲和叔叔是红色节点，祖父必为黑色，变色后祖父为新的当前节点，继续fix
      if (uncle->_color == RBColor::RED) {
        father->_color = uncle->_color = RBColor::BLACK;
        grand->_color = RBColor::RED;
        cur = grand;
      } else {
        // case2.1、父亲红，叔叔黑，且当前节点是左儿子，右旋
        if (cur == father->_lchild) {
          if (father == grand->_lchild) {
            father->_color = RBColor::BLACK;
            grand->_color = RBColor::RED;
          } else {
            cur = father;
          }
          // case2.1.1、父亲是祖父的左儿子，祖父为支点，祖父和父亲变色
          // case2.1.2、父亲是祖父的右儿子，父亲为支点
          RightRotate(father == grand->_lchild ? grand : father);
        } else {
          // case2.2、父亲红，叔叔黑，且当前节点是右儿子
          if (father == grand->_rchild) {
            father->_color = RBColor::BLACK;
            grand->_color = RBColor::RED;
          } else {
            cur = father;
          }
          // case2.2.1、父亲是祖父的左儿子，父亲为支点
          // case2.2.2、父亲是祖父的右儿子，祖父为支点，祖父和父亲变色
          LeftRotate(father == grand->_rchild ? grand : father);
        }
      }
    }
    if (cur == _root && cur->_color == RBColor::RED) {
      cur->_color = RBColor::BLACK;
    }
    return 0;
  }

  // 删除节点后的变色旋转，修复红黑树性质，根本目标将黑色节点上移
  int FixDel(TRBNode* cur) {
    // 由于删除问题到最后删除的必然是一个没有子节点，或者单儿子节点的节点，如果删除
    // 的是根节点，那么当前的红黑树必然有且仅有一个儿子，那么在删除之后 cur
    // 成为了根节点， 且当前红黑树仅有一个根节点，因此直接将当前节点 cur
    // 颜色设置为 BLACK 即可。

    // 非根节点则按以下情况处理
    while (cur != _root && cur->_color == RBColor::BLACK) {
      TRBNode* par = cur->_parent;
      TRBNode* bro = cur == par->_lchild ? par->_rchild : par->_lchild;

      // 由于以 cur 为根的子树被删除了一个黑色节点，因此导致 cur 所在子树和
      // cur的兄弟
      // 节点为根的子树无法满足性质5（每条路径上的黑色节点数量相同）。所以根据
      // cur 的兄 弟节点进行分析：Red 或 Black
      if (bro->_color == RBColor::RED) {
        // 当 bro
        // 为红色时，parent节点必为黑色，因此通过互换bro和par的颜色并以par为
        // 支点旋转，不破坏红黑树性质。
        par->_color = RBColor::RED;
        bro->_color = RBColor::BLACK;
        if (cur == par->_lchild) {
          LeftRotate(par);
        } else {
          RightRotate(par);
        }
      } else {
        // bro是黑色时，且左右儿子都是黑色节点，如果par是红色，则仅需要将par变黑，
        // 即能完成修复，保持性质5。如果 par 是黑色，则将 par
        // 设置为cur，重新进行循环
        if (bro->_lchild->_color == RBColor::BLACK &&
            bro->_rchild->_color == RBColor::BLACK) {
          bro->_color = RBColor::RED;
          cur = par;
          continue;
        }
        // 当 bro 左右儿子颜色不一致时，根据 cur 和 bro
        // 的位置，分别做一次右旋（左 旋）和左旋（右旋）和对应的变色处理。
        if (cur == par->_lchild && bro->_rchild->_color == RBColor::BLACK) {
          // cur是左儿子，且 bro
          // 的左儿子是红色节点，红色节点上移(右旋)，为保持性质5 ，变换 bro 和
          // bro 左儿子的颜色
          RBColor tmpColor = bro->_color;
          bro->_color = bro->_lchild->_color;
          bro->_lchild->_color = tmpColor;
          RightRotate(bro);
          // bro = cur->_parent->_rchild;
        } else if (cur == par->_rchild &&
                   bro->_lchild->_color == RBColor::BLACK) {
          // 对称操作
          RBColor tmpColor = bro->_color;
          bro->_color = bro->_rchild->_color;
          bro->_rchild->_color = tmpColor;
          LeftRotate(bro);
          // bro = cur->_parent->_lchild;
        }
        par = cur->_parent;
        bro = cur == par->_lchild ? par->_rchild : par->_lchild;
        if (cur == par->_lchild && bro->_rchild->_color == RBColor::RED) {
          // cur 是左儿子，bro
          // 的右儿子是红色节点，红色节点上移则需要以父节点为支
          // 点进行左旋。旋转之后，为了保持性质5，需要变色。左旋后，bro 成为
          // par的 par。因此 bro 维持原 par 颜色不变；为了维持 bro
          // 右侧分支的性质5，因此 需要将 bro 的右儿子
          // 变为黑色；原par成为了cur所在子分支的根，由于删除
          // 了黑色节点，为了维持性质5，因此需要将该 par 变为黑色；
          bro->_color = par->_color;
          bro->_rchild->_color = RBColor::BLACK;
          par->_color = RBColor::BLACK;
          LeftRotate(par);
          cur = _root;
        } else if (cur == par->_rchild &&
                   bro->_lchild->_color == RBColor::RED) {
          // 镜像操作
          bro->_color = par->_color;
          bro->_lchild->_color = RBColor::BLACK;
          par->_color = RBColor::BLACK;
          RightRotate(par);
          cur = _root;
        }
      }
    }
    cur->_color = RBColor::BLACK;
    return 0;
  }

  // 左旋
  void LeftRotate(TRBNode* n) {
    TRBNode* f = n->_parent;
    TRBNode* rc = n->_rchild;

    n->_parent = rc;
    n->_rchild = rc->_lchild;

    if (f != nullptr) {
      if (n == f->_lchild) f->_lchild = rc;
      if (n == f->_rchild) f->_rchild = rc;
    } else {
      _root = rc;
    }

    if (rc->_lchild != _nil) rc->_lchild->_parent = n;

    rc->_parent = f;
    rc->_lchild = n;
  }

  // 右旋
  void RightRotate(TRBNode* n) {
    TRBNode* f = n->_parent;
    TRBNode* lc = n->_lchild;

    n->_parent = lc;
    n->_lchild = lc->_rchild;

    if (f != nullptr) {
      if (n == f->_lchild) f->_lchild = lc;
      if (n == f->_rchild) f->_rchild = lc;
    } else {
      _root = lc;
    }

    if (lc->_rchild != _nil) lc->_rchild->_parent = n;

    lc->_parent = f;
    lc->_rchild = n;
  }

  // 先序遍历。查找
  int PreorderTraversal(TRBNode** dst, const TRBNode* fake, TRBNode* n) {
    if (n == _nil || n == nullptr) {
      *dst = nullptr;
      return -1;
    }

    int comp = _compareFunc(fake, n);
    if (comp == 0) {
      *dst = n;
      return 0;
    } else if (comp < 0 && n->_lchild != _nil) {
      return PreorderTraversal(dst, fake, n->_lchild);
    } else if (comp > 0 && n->_rchild != _nil) {
      return PreorderTraversal(dst, fake, n->_rchild);
    } else {
      *dst = nullptr;
      return -1;
    }
  }

  // 后序遍历，销毁所有节点
  int PostorderTraversal(TRBNode* n) {
    if (nullptr == n) {
      return 0;
    }
    PostorderTraversal(n->_lchild);
    PostorderTraversal(n->_rchild);
    delete n;
    return 0;
  }

 private:
  TRBNode* _root = nullptr;
  TRBNode* _nil = nullptr;
  size_t _nodeNum = 0;
  TFCompare _compareFunc;

#ifndef NDEBUG
 private:
  class Canvas {
   public:
    static const int HEIGHT = 10;
    static const int WIDTH = 50;
    static char buffer[HEIGHT][WIDTH + 1];
    static RBColor color[HEIGHT][WIDTH + 1];
    static void draw() {
      for (int i = 0; i < HEIGHT; i++) {
        buffer[i][WIDTH] = '\0';
        for (int j = 0; j <= WIDTH; j++) {
          if (color[i][j] == RBColor::BLACK) {
            printf("\033[0;34m%c\033[0m", buffer[i][j]);
          } else if (color[i][j] == RBColor::RED) {
            printf("\033[0;31m%c\033[0m", buffer[i][j]);
          } else {
            printf("%c", buffer[i][j]);
          }
        }
        std::cout << std::endl;
      }
    }

    static void put(int r, int c, const std::string& s, RBColor& cl) {
      int len = s.length();
      int idx = 0;
      for (int i = c; (i < WIDTH) && (idx < len); i++) {
        buffer[r][i] = s[idx++];
        color[r][i] = cl;
      }
    }
    static void put(int r, int c, char ch, int num) {
      while (num > 0 && c < WIDTH) buffer[r][c++] = ch, num--;
    }

    static void resetBuffer() {
      for (int i = 0; i < HEIGHT; i++) memset(buffer[i], ' ', WIDTH);
    }
  };

  void initX(TRBNode* p, int& x) {
    if (p == nullptr || p == _nil) return;
    initX(p->_lchild, x);
    p->_pt.first = x++;
    initX(p->_rchild, x);
  }
  void initY() {
    if (_root == nullptr) return;

    using Node = std::pair<TRBNode*, int>;

    _root->_pt.second = 1;

    std::queue<Node> q;
    q.push(Node(_root, _root->_pt.second));
    while (!q.empty()) {
      auto p = q.front();
      q.pop();
      if (p.first->_lchild != nullptr) {
        p.first->_lchild->_pt.second = p.second + 1;
        q.push(Node(p.first->_lchild, p.second + 1));
      }
      if (p.first->_rchild != nullptr) {
        p.first->_rchild->_pt.second = p.second + 1;
        q.push(Node(p.first->_rchild, p.second + 1));
      }
    }
  }

  void initCoordinate() {
    int x = 0;
    initX(_root, x);
    initY();
  }

 public:
  // 格式化打印红黑树
  void print(void) {
    if (_root == _nil || _root == nullptr) return;
    initCoordinate();
    const int widthZoom = 2;
    // printf("width zoom: %d\n", widthZoom);
    Canvas::resetBuffer();
    std::queue<TRBNode*> q;
    q.push(_root);
    int x, y, val;
    while (!q.empty()) {
      auto p = q.front();
      q.pop();
      x = p->_pt.first;
      y = p->_pt.second;
      val = p->_data._key;
      Canvas::put(2 * y, widthZoom * x, std::to_string(val), p->_color);
      if (p->_lchild != _nil) {
        q.push(p->_lchild);
        Canvas::put(2 * y + 1, widthZoom * ((p->_lchild->_pt.first + x) / 2),
                    '/', 1);
      }
      if (p->_rchild != _nil) {
        q.push(p->_rchild);
        Canvas::put(2 * y + 1,
                    widthZoom * ((x + p->_rchild->_pt.first) / 2) + 1, '\\', 1);
      }
    }
    Canvas::draw();
  }
#else
 public:
  void print(void) {}
#endif
};

#ifndef NDEBUG
template <typename Tkey, typename Tval>
char RBTree<Tkey, Tval>::Canvas::buffer[RBTree<Tkey, Tval>::Canvas::HEIGHT]
                                       [RBTree<Tkey, Tval>::Canvas::WIDTH + 1];

template <typename Tkey, typename Tval>
RBColor
    RBTree<Tkey, Tval>::Canvas::color[RBTree<Tkey, Tval>::Canvas::HEIGHT]
                                     [RBTree<Tkey, Tval>::Canvas::WIDTH + 1];
#endif

}  // namespace myutils