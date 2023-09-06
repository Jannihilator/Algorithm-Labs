#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

// data structure that represents a node in the tree
struct RECT {
	int x1; // holds the key
	int y1;
	int x2;
	int y2;
	string id;
	RECT():x1(0), y1(0), x2(0), y2(0), id(""){}
    RECT& operator=(RECT rhs){
        this->id = rhs.id;
        this->x1 = rhs.x1;
        this->y1 = rhs.y1;
        this->x2 = rhs.x2;
        this->y2 = rhs.y2;
        return *this;
    }

};

struct COVER{
    bool valid;
    string id;
};

bool cmp_cover(COVER a, COVER b){
    if(stoi(a.id.substr(1))<stoi(b.id.substr(1)))
        return true;
    else return false;
}

bool cmp(string a, string b){
    if(stoi(a.substr(1))<stoi(b.substr(1)))
        return true;
    else return false;
}

inline int cal_area(RECT rect) { return (rect.x2-rect.x1)*(rect.y2-rect.y1); }
inline bool operator==(const RECT& lhs, const RECT& rhs){
	if(lhs.x1 == rhs.x1 && lhs.y1 == rhs.y1 && lhs.x2 == rhs.x2 && lhs.y2 == rhs.y2)
		return true;
	else 
		return false;
}

inline bool operator<(const RECT& lhs, const RECT& rhs){
	if(lhs.x1 < rhs.x1)
		return true;
	else if(lhs.x1 == rhs.x1){
        if(lhs.y1 < rhs.y1)
            return true;
        else if(lhs.y1 == rhs.y1){
			if(cal_area(lhs)<cal_area(rhs))
                return true;
			else if(cal_area(lhs)==cal_area(rhs)){
				if(stoi(lhs.id.substr(1))<stoi(rhs.id.substr(1)))
					return true;
				else return false;
			}
				
            else return false;
		}
            // if((lhs.x2-lhs.x1)*(lhs.y2-lhs.y1)<(rhs.x2-rhs.x1)*(rhs.y2-rhs.y1))
            
        else return false;
    }		
    else return false;
}

inline bool operator<=(const RECT& lhs, const RECT& rhs){
	if(lhs < rhs)
		return true;
	else if (lhs == rhs)
		return true;
    else return false;
}



struct Node {
	RECT rect;
	Node *parent; // pointer to the parent
	Node *left; // pointer to left child
	Node *right; // pointer to right child
	int color; // 1 -> Red, 0 -> Black
	Node():left(0),right(0),parent(0),rect(),color(1){}
};

typedef Node *NodePtr;

// class RBTree implements the operations in Red Black Tree
class RBTree {
private:
	NodePtr root;
	NodePtr TNULL;
	int minArea;

	// initializes the nodes with appropirate values
	// all the pointers are set to point to the null pointer
	void initializeNULLNode(NodePtr node, NodePtr parent) {
		node->parent = parent;
		node->left = nullptr;
		node->right = nullptr;
		node->color = 0;
	}

	void preOrderHelper(NodePtr node) {
		if (node != TNULL) {
			cout<<node->rect.x1<<" ";
			preOrderHelper(node->left);
			preOrderHelper(node->right);
		} 
	}

	void inOrderHelper(NodePtr node) {
		if (node != TNULL) {
			inOrderHelper(node->left);
			cout<<node->rect.x1<<" ";
			inOrderHelper(node->right);
		} 
	}

	void postOrderHelper(NodePtr node) {
		if (node != TNULL) {
			postOrderHelper(node->left);
			postOrderHelper(node->right);
			cout<<node->rect.x1<<" ";
		} 
	}

	NodePtr searchTreeHelper(NodePtr node, RECT key) {
        if (node == TNULL)
            return nullptr;
		if (key == node->rect) {
			return node;
		}
		if (key < node->rect) {
			return searchTreeHelper(node->left, key);
		} 
		else return searchTreeHelper(node->right, key);
	}


	// fix the rb tree modified by the delete operation
	void fixDelete(NodePtr x) {
		NodePtr s;
		while (x != root && x->color == 0) {
			if (x == x->parent->left) {
				s = x->parent->right;
				// if(s==TNULL)
				// 	return;
				if (s->color == 1) {
					// case 3.1
					s->color = 0;
					x->parent->color = 1;
					leftRotate(x->parent);
					s = x->parent->right;
				}
				if (s->left->color == 0 && s->right->color == 0) {
					// case 3.2
					s->color = 1;
					x = x->parent;
				} else {
					if (s->right->color == 0) {
						// case 3.3
						s->left->color = 0;
						s->color = 1;
						rightRotate(s);
						s = x->parent->right;
					} 
					// case 3.4
					s->color = x->parent->color;
					x->parent->color = 0;
					s->right->color = 0;
					leftRotate(x->parent);
					x = root;
				}
			} else {
				s = x->parent->left;
				// if(s==TNULL)
				// 	return;
				if (s->color == 1) {
					// case 3.1
					s->color = 0;
					x->parent->color = 1;
					rightRotate(x->parent);
					s = x->parent->left;
				}

				if (s->left->color == 0 && s->right->color == 0) {
					// case 3.2
					s->color = 1;
					x = x->parent;
				} else {
					if (s->left->color == 0) {
						// case 3.3
						s->right->color = 0;
						s->color = 1;
						leftRotate(s);
						s = x->parent->left;
					} 

					// case 3.4
					s->color = x->parent->color;
					x->parent->color = 0;
					s->left->color = 0;
					rightRotate(x->parent);
					x = root;
				}
			} 
		}
		x->color = 0;
	}


	void rbTransplant(NodePtr u, NodePtr v){
		if (u->parent == nullptr) {
			root = v;
		} else if (u == u->parent->left){
			u->parent->left = v;
		} else {
			u->parent->right = v;
		}
		v->parent = u->parent;
	}

	void deleteNodeHelper(NodePtr node, RECT key) {
		// find the node containing key
		NodePtr z = TNULL;
		NodePtr x, y;
		while (node != TNULL){
			if (node->rect == key) {
				z = node;
			}

			if (key <= node->rect) {
				node = node->left;
			} else {
				node = node->right;
			}
		}
		if (z == TNULL) {
			cout<<"Couldn't find key in the tree"<<endl;
			return;
		} 

		y = z;
		int y_original_color = y->color;
		if (z->left == TNULL) {
			x = z->right;
			rbTransplant(z, z->right);
		} else if (z->right == TNULL) {
			x = z->left;
			rbTransplant(z, z->left);
		} else {
			y = maximum(z->left);
			y_original_color = y->color;
			x = y->left;
			if (y->parent == z) {
				x->parent = y;
			} else {
				rbTransplant(y, y->left);
				y->left = z->left;
				y->left->parent = y;
			}

			rbTransplant(z, y);
			y->right = z->right;
			y->right->parent = y;
			y->color = z->color;
		}
		delete z;
		if (y_original_color == 0){
			fixDelete(x);
		}
		
	}
	
	// fix the red-black tree
	void fixInsert(NodePtr k){
		NodePtr u;
		while (k->parent->color == 1) {
			if (k->parent == k->parent->parent->right) {
				u = k->parent->parent->left; // uncle
				if (u->color == 1) {
					// case 3.1
					u->color = 0;
					k->parent->color = 0;
					k->parent->parent->color = 1;
					k = k->parent->parent;
				} else {
					if (k == k->parent->left) {
						// case 3.2.2
						k = k->parent;
						rightRotate(k);
					}
					// case 3.2.1
					k->parent->color = 0;
					k->parent->parent->color = 1;
					leftRotate(k->parent->parent);
				}
			} else {
				u = k->parent->parent->right; // uncle

				if (u->color == 1) {
					// mirror case 3.1
					u->color = 0;
					k->parent->color = 0;
					k->parent->parent->color = 1;
					k = k->parent->parent;	
				} else {
					if (k == k->parent->right) {
						// mirror case 3.2.2
						k = k->parent;
						leftRotate(k);
					}
					// mirror case 3.2.1
					k->parent->color = 0;
					k->parent->parent->color = 1;
					rightRotate(k->parent->parent);
				}
			}
			if (k == root) {
				break;
			}
		}
		root->color = 0;
	}

	void printHelper(NodePtr root, string indent, bool last, ofstream &ofs) {
		// print the tree structure on the screen
	   	if (root != TNULL) {
		   ofs<<indent;
		   if (last) {
		      ofs<<"R----";
		      indent += "     ";
		   } else {
		      ofs<<"L----";
		      indent += "|    ";
		   }
            
           string sColor = root->color?"RED":"BLACK";
		   ofs<<root->rect.id<<"("<<sColor<<")"<<endl;
		   printHelper(root->left, indent, false, ofs);
		   printHelper(root->right, indent, true, ofs);
		}
		// cout<<root->left->data<<endl;
	}

public:
	RBTree() {
		TNULL = new Node;
		TNULL->color = 0;
		TNULL->left = nullptr;
		TNULL->right = nullptr;
		root = TNULL;
	}

	// Pre-Order traversal
	// Node->Left Subtree->Right Subtree
	void preorder() {
		preOrderHelper(this->root);
	}

	// In-Order traversal
	// Left Subtree -> Node -> Right Subtree
	void inorder() {
		inOrderHelper(this->root);
	}

	// Post-Order traversal
	// Left Subtree -> Right Subtree -> Node
	void postorder() {
		postOrderHelper(this->root);
	}

	// search the tree for the key k
	// and return the corresponding node
	NodePtr searchTree(RECT k) {
		return searchTreeHelper(this->root, k);
	}

	// find the node with the minimum key
	NodePtr minimum(NodePtr node) {
		while (node->left != TNULL) {
			node = node->left;
		}
		return node;
	}

	// find the node with the maximum key
	NodePtr maximum(NodePtr node) {
		while (node->right != TNULL) {
			node = node->right;
		}
		return node;
	}

	// find the successor of a given node
	NodePtr successor(NodePtr x) {
		// if the right subtree is not null,
		// the successor is the leftmost node in the
		// right subtree
		if (x->right != TNULL) {
			return minimum(x->right);
		}

		// else it is the lowest ancestor of x whose
		// left child is also an ancestor of x.
		NodePtr y = x->parent;
		while (y != TNULL && x == y->right) {
			x = y;
			y = y->parent;
		}
		return y;
	}

	// find the predecessor of a given node
	NodePtr predecessor(NodePtr x) {
		// if the left subtree is not null,
		// the predecessor is the rightmost node in the 
		// left subtree
		if (x->left != TNULL) {
			return maximum(x->left);
		}

		NodePtr y = x->parent;
		while (y != TNULL && x == y->left) {
			x = y;
            if(y==this->root){
                y = nullptr;
                break;
            }
			y = y->parent;
		}

		return y;
	}

	// rotate left at node x
	void leftRotate(NodePtr x) {
		NodePtr y = x->right;
		x->right = y->left;
		if (y->left != TNULL) {
			y->left->parent = x;
		}
		y->parent = x->parent;
		if (x->parent == nullptr) {
			this->root = y;
		} else if (x == x->parent->left) {
			x->parent->left = y;
		} else {
			x->parent->right = y;
		}
		y->left = x;
		x->parent = y;
	}

	// rotate right at node x
	void rightRotate(NodePtr x) {
		NodePtr y = x->left;
		x->left = y->right;
		if (y->right != TNULL) {
			y->right->parent = x;
		}
		y->parent = x->parent;
		if (x->parent == nullptr) {
			this->root = y;
		} else if (x == x->parent->right) {
			x->parent->right = y;
		} else {
			x->parent->left = y;
		}
		y->right = x;
		x->parent = y;
	}

	// insert the key to the tree in its appropriate position
	// and fix the tree
	void insert(RECT key) {
		// Ordinary Binary Search Insertion
		NodePtr node = new Node;
		node->parent = nullptr;
		node->rect = key;
		node->left = TNULL;
		node->right = TNULL;
		node->color = 1; // new node must be red

		NodePtr y = nullptr;
		NodePtr x = this->root;

		while (x != TNULL) {
			y = x;
			if (node->rect < x->rect) {
				x = x->left;
			} else {
				x = x->right;
			}
		}

		// y is parent of x
		node->parent = y;
		if (y == nullptr) {
			root = node;
		} else if (node->rect < y->rect) {
			y->left = node;
		} else {
			y->right = node;
		}

		// if new node is a root node, simply return
		if (node->parent == nullptr){
			node->color = 0;
			return;
		}

		// if the grandparent is null, simply return
		if (node->parent->parent == nullptr) {
			return;
		}

		// Fix the tree
		fixInsert(node);
	}

	NodePtr getRoot(){
		return this->root;
	}

	// delete the node from the tree
	void deleteNode(RECT rect) {
		deleteNodeHelper(this->root, rect);
	}

	// print the tree structure on the screen
	void prettyPrint() {
		ofstream ofs ("graph.txt", std::ofstream::out | std::ofstream::trunc);
	    if (root) {
    		printHelper(this->root, "", true, ofs);
	    }
	}

	void area_traverse(NodePtr current, vector<string> *r_list, RECT key){
		if(current!=TNULL){
			area_traverse(current->left, r_list, key);
			if(current->rect.x1 == key.x1 && current->rect.y1 == key.y1){
				if(cal_area(current->rect)<=minArea)	
					minArea = cal_area(current->rect);
					(*r_list).push_back(current->rect.id);
			}
				
			// if(current->rect.x1 == key.x1 && current->rect.y1 == key.y1 && cal_area(current->rect) < min){
			// 	(*r_list).clear();
			// 	min = cal_area(current->rect);
			// 	(*r_list).push_back(current->rect.id);
			// }
				
			area_traverse(current->right, r_list, key);
		}

	}

	void search_traverse(NodePtr current, vector<string> *r_list, RECT key){
		if(current!=TNULL){
			search_traverse(current->left, r_list, key);
			if(current->rect == key)
				(*r_list).push_back(current->rect.id);
			// if(current->rect.x1 == key.x1 && current->rect.y1 == key.y1 && cal_area(current->rect) < min){
			// 	(*r_list).clear();
			// 	min = cal_area(current->rect);
			// 	(*r_list).push_back(current->rect.id);
			// }
				
			search_traverse(current->right, r_list, key);
		}

	}

	vector<string> searching(RECT key){
		vector<string> r_list;
		NodePtr found = nullptr;
        NodePtr node = this->root;
		while (node != TNULL) {
            if (key == node->rect){
                found = node;
				break;
            }
			if (key < node->rect) {
				node = node->left;
			} else {
				node = node->right;
			}
		}
		if(found){
			search_traverse(found, &r_list, key);
			sort(r_list.begin(), r_list.end(), cmp);
		}
			
		return r_list;
	}

    vector<string> area(RECT key) {
		vector<string> r_list;
		NodePtr found = nullptr;
        NodePtr node = this->root;
		while (node != TNULL) {
            if (key.x1 == node->rect.x1){
                found = node;
				break;
            }
			if (key.x1 <= node->rect.x1) {
				node = node->left;
			} else {
				node = node->right;
			}
		}
		if(found){
			minArea = INT8_MAX;
			area_traverse(found, &r_list, key);
			sort(r_list.begin(), r_list.end(), cmp);
		}
			
		return r_list;
	}

    short int cover(RECT key, RECT rect){
        if(rect.x1 <= key.x1-20 && rect.y1 <= key.y1-20 && rect.x2 >= key.x2+20 && rect.y2 >= key.y2+20)
            return 1;
        else if (rect.x1 > key.x1 || rect.y1 > key.y1 || rect.x2 < key.x2 || rect.y2 < key.y2)
            return 0;
        else return -1;
    }

    vector<COVER> overlap(RECT key){
        vector<COVER> r_list;

        NodePtr node = this->root;
        NodePtr pre = nullptr;
        COVER info;

        while (node != TNULL) {
            pre = node;
            
            if (key < node->rect) {
                node = node->left;
            } else {
                node = node->right;
            }
        }
        while(pre != nullptr){
            if(cover(key, pre->rect)){
                info.id = pre->rect.id;
                info.valid = cover(key, pre->rect)+1;
                r_list.push_back(info);
            }     
            pre = this->predecessor(pre);
        }
        sort(r_list.begin(), r_list.end(), cmp_cover);
        return r_list;
    }
};  



int main(int argc, char *argv[]) {
	char ch;
	string iid;
	int ix1, iy1, ix2, iy2;
	RECT rect;
    NodePtr output;
	RBTree bst;
    vector<COVER> rlist;
	vector<string> r_list;
	ifstream file (argv[1]);
    ofstream ofs (argv[2], std::ofstream::out | std::ofstream::trunc);

	if(!file.is_open()){
		cout<<"Failed to open file"<<endl;	
	} else {
		while(file>>ch){
			switch(ch){
				case 'I':
					file>>iid>>ix1>>iy1>>ix2>>iy2;
					//ofs<<iid<<" "<<ix1<<" "<<iy1<<" "<<ix2<<" "<<iy2<<endl;
                    rect.id = iid;
					rect.x1 = ix1;
					rect.y1 = iy1;
					rect.x2 = ix2;
					rect.y2 = iy2;
					bst.insert(rect);
					break;
				case 'O':
					file>>ix1>>iy1>>ix2>>iy2;
					ofs<<ch<<endl;
                    //ofs<<" "<<ix1<<" "<<iy1<<" "<<ix2<<" "<<iy2<<endl;
                    rect.x1 = ix1;
					rect.y1 = iy1;
					rect.x2 = ix2;
					rect.y2 = iy2;
                    rlist = bst.overlap(rect);
                    for (int i = 0; i < rlist.size(); i++) {
                        if(rlist[i].valid)
                            ofs << rlist[i].id << endl;
                        else
                            ofs << rlist[i].id << " " << "violate" << endl;
                    }
					break;
				case 'S':
					file>>ix1>>iy1>>ix2>>iy2;
					ofs<<ch<<endl;;
                    //ofs<<" "<<ix1<<" "<<iy1<<" "<<ix2<<" "<<iy2<<endl;
                    rect.x1 = ix1;
					rect.y1 = iy1;
					rect.x2 = ix2;
					rect.y2 = iy2;
                    r_list = bst.searching(rect);
                    if(!r_list.empty()){
						for (int i = 0; i < r_list.size(); i++) {
                            ofs << r_list[i] << endl;
						}
					}
					break;
				case 'D':
					file>>ix1>>iy1>>ix2>>iy2;
					//ofs<<ix1<<" "<<iy1<<" "<<ix2<<" "<<iy2<<endl;
					rect.x1 = ix1;
					rect.y1 = iy1;
					rect.x2 = ix2;
					rect.y2 = iy2;
					bst.deleteNode(rect);
					break;
				case 'A':
					file>>ix1>>iy1;
					ofs<<ch<<endl;;
                    //ofs<<" "<<ix1<<" "<<iy1<<endl;
                    rect.x1 = ix1;
					rect.y1 = iy1;
                    rect.x2 = 0;
					rect.y2 = 0;
					r_list = bst.area(rect);
                    if(!r_list.empty()){
						for (int i = 0; i < r_list.size(); i++) {
                            ofs << r_list[i] << endl;
						}
					}
					break;

			}
		}
	file.close();
    ofs.close();
    //bst.prettyPrint();
	return 0;
	}
}
