// NOTE: Per assignment rules, only this single include is allowed here.
#include "VectorStore.h"

// =====================================
// Helper functions
// =====================================

// Helper function to print n spaces for tree visualization
void printNSpace(int n) {
    for (int i = 0; i < n; i++) {
        cout << " ";
    }
}

// =====================================
// AVLTree<K, T> implementation
// =====================================

template <class K, class T>
void AVLTree<K, T>::printTreeStructure() const {
    int height = this->getHeight();
		if (this->root == NULL)
		{
			cout << "NULL\n";
			return;
		}
		queue<AVLNode *> q;
		q.push(root);
		AVLNode *temp;
		int count = 0;
		int maxNode = 1;
		int level = 0;
		int space = pow(2, height);
		printNSpace(space / 2);
		while (!q.empty())
		{
			temp = q.front();
			q.pop();
			if (temp == NULL)
			{
				cout << " ";
				q.push(NULL);
				q.push(NULL);
			}
			else
			{
				cout << temp->data;
				q.push(temp->pLeft);
				q.push(temp->pRight);
			}
			printNSpace(space);
			count++;
			if (count == maxNode)
			{
				cout << endl;
				count = 0;
				maxNode *= 2;
				level++;
				space /= 2;
				printNSpace(space / 2);
			}
			if (level == height)
				return;
		}
}


//TODO: Implement all AVLTree<K, T> methods here
template <class K, class T>
typename AVLTree<K, T>::AVLNode* AVLTree<K, T>::rotateRight(AVLNode*& node) {
    if (!node || !node->pLeft) return node;

    AVLNode* x = node->pLeft;
    AVLNode* r = x->pRight;

    x->pRight = node;
    node->pLeft = r;

    return x;
}

template <class K, class T>
typename AVLTree<K, T>::AVLNode* AVLTree<K, T>::rotateLeft(AVLNode*& node) {
    if (!node || !node->pRight) return node;

    AVLNode* y = node->pRight;
    AVLNode* l = y->pLeft;

    y->pLeft = node;
    node->pRight = l;

    return y;
}

template <class K, class T>
int AVLTree<K, T>::height(AVLNode* node) {
    if (!node) return 0;
    return 1 + max(height(node->pLeft), height(node->pRight));
}

template <class K, class T>
BalanceValue AVLTree<K, T>::getBalance(AVLNode* node) {
    int lHeight = height(node->pLeft);
    int rHeight = height(node->pRight);
    BalanceValue balance = (BalanceValue)(lHeight - rHeight);
    return balance;
}

template <class K, class T>
typename AVLTree<K, T>::AVLNode* AVLTree<K, T>::insertHelper(AVLNode* node, const K& key, const T& value) {
    if (!node) {
        return new AVLNode(key, value);
    }

    if (key < node->key) {
        node->pLeft = insertHelper(node->pLeft, key, value);
    } else if (key > node->key) {
        node->pRight = insertHelper(node->pRight, key, value);
    } else {
        return node;
    }

    node->balance = getBalance(node);

    //Left-Left
    if (node->balance > 1 && key < node->pLeft->key) {
        return rotateRight(node);
    }
    //Right-Right
    if (node->balance < -1 && key > node->pRight->key) {
        return rotateLeft(node);
    }
    //Left-Right
    if (node->balance > 1 && key > node->pLeft->key) {
        node->pLeft = rotateLeft(node->pLeft);
        return rotateRight(node);
    }
    //Right-Left
    if (node->balance < -1 && key < node->pRight->key) {
        node->pRight = rotateRight(node->pRight);
        return rotateLeft(node);
    }

    return node;
}

template <class K, class T>
void AVLTree<K, T>::insert(const K& key, const T& value) {
    this->root = insertHelper(this->root, key, value);
}

template <class K, class T>
typename AVLTree<K, T>::AVLNode* AVLTree<K, T>::minNode(AVLNode* node) {
    AVLNode* current = node;

    while (current->pLeft) current = current->pLeft;

    return current;
}

template <class K, class T>
typename AVLTree<K, T>::AVLNode* AVLTree<K, T>::removeHelper(AVLNode* node, const K& key) {
    if (!node) return node;

    if (key < node->key) {
        node->pLeft = removeHelper(node->pLeft, key);
    }
    else if (key > node->key) {
        node->pRight = removeHelper(node->pRight, key);
    } else {
        if ((node->pLeft == nullptr) || (node->pRight == nullptr)) {
            AVLNode* temp = node->pLeft ? node->pLeft : node->pRight;

            if (!temp) {
                temp = node;
                node = nullptr;
            } else *node = *temp;
            delete temp;
        } else {
            AVLNode* temp = minNode(node->pRight);

            node->key = temp->key;
            node->pRight = removeHelper(node->pRight, temp->key);
        }
    }

    if (!node) return node;

    node->balance = getBalance(node);

    if (node->balance > 1 && getBalance(node->pLeft) >= 0) {
        return rotateRight(node);
    }
    if (node->balance < -1 && getBalance(node->pRight) <= 0) {
        return rotateLeft(node);
    }
    if (node->balance > 1 && getBalance(node->pLeft) < 0) {
        node->pLeft = rotateLeft(node->pLeft);
        return rotateRight(node);
    }
    if (node->balance < -1 && getBalance(node->pRight) > 0) {
        node->pRight = rotateRight(node->pRight);
        return rotateLeft(node);
    }

    return node;
}

template <class K, class T>
void AVLTree<K, T>::remove(const K& key) {
    this->root = removeHelper(this->root, key);
}

template <class K, class T>
bool AVLTree<K, T>::contains(const K& key) const {
    if (!this->root) return false;

    AVLNode* current = this->root;
    while (current) {
        if (key == current->key) return true;

        if (key < current->key) current = current->pLeft;
        else if (key > current->key) current = current->pRight;
    }
    return false;
}

template <class K, class T>
int AVLTree<K, T>::getHeight() const {
    if (!this->root) return 0;

    queue<AVLNode*> q;
    q.push(this->root);

    int height = 0;
    while (!q.empty()) {
        int levelSize = q.size();
        for (int i = 0; i < levelSize; ++i) {
            AVLNode* current = q.front(); q.pop();
            if (current->pLeft) q.push(current->pLeft);
            if (current->pRight) q.push(current->pRight);
        }
        ++height;
    }

    return height;
}

template <class K, class T>
int AVLTree<K, T>::getSize() const {
    if (!this->root) return 0;

    queue<AVLNode*> q;
    q.push(this->root);

    int size = 0;
    while (!q.empty()) {
        AVLNode* current = q.front(); q.pop();
        if (current->pLeft) q.push(current->pLeft);
        if (current->pRight) q.push(current->pRight);
        ++size;
    }

    return size;
}

template <class K, class T>
bool AVLTree<K, T>::empty() const {
    return (!this->root? true : false);
}

template <class K, class T>
void AVLTree<K, T>::clearHelper(AVLNode* node) {
	if (!node) return;

	clearHelper(node->pLeft);
	clearHelper(node->pRight);
	delete node;
}

template <class K, class T>
void AVLTree<K, T>::clear() {
	clearHelper(this->root);
	this->root = nullptr;
}

template <class K, class T>
void AVLTree<K, T>::inorderTraversalHelper(AVLNode* node, void (*action)(const T&)) const {
	if (!node) return;

	inorderTraversalHelper(node->pLeft, action);
	action(node->data);
	inorderTraversalHelper(node->pRight, action);
}

template <class K, class T>
void AVLTree<K, T>::inorderTraversal(void (*action)(const T&)) const {
	inorderTraversalHelper(this->root, action);
}

// =====================================
// RedBlackTree<K, T> implementation
// =====================================

template <class K, class T>
void RedBlackTree<K, T>::printTreeStructure() const {
    if (this->root == nullptr) {
        cout << "NULL\n";
        return;
    }
    
    queue<RBTNode*> q;
    q.push(root);
    RBTNode* temp;
    int count = 0;
    int maxNode = 1;
    int level = 0;
    
    // Calculate tree height for spacing
    int height = 0;
    queue<RBTNode*> heightQ;
    heightQ.push(root);
    while (!heightQ.empty()) {
        int size = heightQ.size();
        height++;
        for (int i = 0; i < size; i++) {
            RBTNode* node = heightQ.front();
            heightQ.pop();
            if (node->left) heightQ.push(node->left);
            if (node->right) heightQ.push(node->right);
        }
    }
    
    int space = pow(2, height);
    printNSpace(space / 2);
    
    while (!q.empty()) {
        temp = q.front();
        q.pop();
        
        if (temp == nullptr) {
            cout << "NULL";
            q.push(nullptr);
            q.push(nullptr);
        } else {
            // Print node data and color as tuple: (data, color)
            cout << "(" << temp->data << ",";
            if (temp->color == RED) {
                cout << "R)";
            } else {
                cout << "B)";
            }
            
            q.push(temp->left);
            q.push(temp->right);
        }
        
        printNSpace(space);
        count++;
        
        if (count == maxNode) {
            cout << endl;
            count = 0;
            maxNode *= 2;
            level++;
            space /= 2;
            printNSpace(space / 2);
        }
        
        if (level == height) {
            return;
        }
    }
}

//TODO: Implement all other RedBlackTree<K, T> methods here
template <class K, class T>
void RedBlackTree<K, T>::RBTNode::recolorToRed() {
    this->color = RED;
}

template <class K, class T>
void RedBlackTree<K, T>::RBTNode::recolorToBlack() {
    this->color = BLACK;
}

template <class K, class T>
void RedBlackTree<K, T>::rotateRight(RBTNode* node) {
    if (!node || !node->left) return;

    RBTNode* x = node->left;
    RBTNode* r = x->right;

    node->left = r;
    if (r) r->parent = node;

    x->right = node;
    x->parent = node->parent;
    node->parent = x;

    if (!x->parent) {
        this->root = x;
    } else if (x->parent->left == node) {
        x->parent->left = x;
    } else {
        x->parent->right = x;
    }
}

template <class K, class T>
void RedBlackTree<K, T>::rotateLeft(RBTNode* node) {
    if (!node || !node->right) return;

    RBTNode* y = node->right;
    RBTNode* l = y->left;

    node->right = l;
    if (l) l->parent = node;

    y->left= node;
    y->parent = node->parent;
    node->parent = y;

    if (!y->parent) {
        this->root = y;
    } else if (y->parent->left == node) {
        y->parent->left = y;
    } else {
        y->parent->right = y;
    }
}

template <class K, class T>
typename RedBlackTree<K, T>::RBTNode* RedBlackTree<K, T>::lowerBoundNode(const K& key) const {
    RBTNode* cur = this->root;

    while (cur) {
        if (cur->key >= key) {
            return cur;
		}
        cur = cur->right;
	}

    return nullptr;
}

template <class K, class T>
typename RedBlackTree<K, T>::RBTNode* RedBlackTree<K, T>::upperBoundNode(const K& key) const {
    RBTNode* cur = this->root;

    while (cur) {
        if (cur->key > key) {
            return cur;
		}
        cur = cur->right;
    }

    return nullptr;
}

template <class K, class T>
bool RedBlackTree<K, T>::empty() const {
    return (!this->root ? true : false);
}

template <class K, class T>
int RedBlackTree<K, T>::size() const {
    if (!this->root) return 0;

    queue<RBTNode*> q; q.push(this->root);
    int size = 0;

    while (!q.empty()) {
        RBTNode* cur = q.front(); q.pop();
        ++size;

        if (cur->left) q.push(cur->left);
        if (cur->right) q.push(cur->right);
    }

    return size;
}

template <class K, class T>
void RedBlackTree<K, T>::clearHelper(RBTNode* node) {
	if (!node) return;

	clearHelper(node->left);
	clearHelper(node->right);
	delete node;
}

template <class K, class T>
void RedBlackTree<K, T>::clear() {
	clearHelper(this->root);
	this->root = nullptr;
}

template <class K, class T>
bool RedBlackTree<K, T>::isRed(RBTNode* node) {
    return (node && node->color == RED);
}

template <class K, class T>
void RedBlackTree<K, T>::fixInsert(RBTNode* node) {
    while (node != this->root && isRed(node->parent)) {
        RBTNode* parent = node->parent;
        RBTNode* grand = parent->parent;

        // parent is left child
        if (parent == grand->left) {
            RBTNode* uncle = grand->right;

            // Case 1: uncle is RED → recolor
            if (isRed(uncle)) {
                parent->color = BLACK;
                uncle->color = BLACK;
                grand->color = RED;
                node = grand;
            }
            else {
                // Case 2: node is right child → rotate left
                if (node == parent->right) {
                    node = parent;
                    rotateLeft(node);

                    parent = node->parent;
                    grand = parent->parent;
                }
                // Case 3: node is left child → rotate right
                parent->color = BLACK;
                grand->color = RED;
                rotateRight(grand);
            }
        }

        // parent is right child
        else {
            RBTNode* uncle = grand->left;

            if (isRed(uncle)) {
                parent->color = BLACK;
                uncle->color = BLACK;
                grand->color = RED;
                node = grand;
            }
            else {
                if (node == parent->left) {
                    node = parent;
                    rotateRight(node);

                    parent = node->parent;
                    grand = parent->parent;
                }
                parent->color = BLACK;
                grand->color = RED;
                rotateLeft(grand);
            }
        }
    }

    this->root->color = BLACK;
}

template <class K, class T>
void RedBlackTree<K, T>::insert(const K& key, const T& value) {
    RBTNode* newNode = new RBTNode(key, value);

    if (!this->root) {
        newNode->color = BLACK;  // root always black
        this->root = newNode;
        return;
    }

    RBTNode* cur = this->root;
    RBTNode* parent = nullptr;

    while (cur) {
        parent = cur;
        if (key < cur->key) cur = cur->left;
        else                cur = cur->right;
    }

    newNode->parent = parent;
    if (key < parent->key)
        parent->left = newNode;
    else
        parent->right = newNode;

    fixInsert(newNode);
}

template <class K, class T>
typename RedBlackTree<K, T>::RBTNode* RedBlackTree<K, T>::maxNode(RBTNode* node) {
    RBTNode* current = node;

    while (current && current->right) {
        current = current->right;
    }

    return current;
}

template <class K, class T>
void RedBlackTree<K, T>::fixRemove(RBTNode* x, RBTNode* xParent) {
    // Loop until we've fixed the problem or reached root
    while (x != this->root && !isRed(x)) {
        
        // Determine if x is left or right child
        // Special handling for when x is nullptr
        bool xIsLeftChild;
        if (x != nullptr) {
            xIsLeftChild = (x == xParent->left);
        } else {
            // x is nullptr, check which child of parent is nullptr
            xIsLeftChild = (xParent->left == nullptr);
        }
        
        if (xIsLeftChild) {
            // ============================================
            // X IS LEFT CHILD - Handle left-side cases
            // ============================================
            RBTNode* s = xParent->right;  // sibling
            
            // Case 2: Red sibling
            if (isRed(s)) {
                s->color = BLACK;
                xParent->color = RED;
                rotateLeft(xParent);
                s = xParent->right;  // Update sibling after rotation
            }
            
            // Case 3: Black sibling, both children black
            if (!isRed(s->left) && !isRed(s->right)) {
                s->color = RED;
                x = xParent;
                xParent = x->parent;
            } else {
                // Case 4: Near child red, far child black
                if (!isRed(s->right)) {
                    if (s->left) s->left->color = BLACK;
                    s->color = RED;
                    rotateRight(s);
                    s = xParent->right;  // Update sibling after rotation
                }
                
                // Case 5: Far child red (terminal case)
                s->color = xParent->color;
                xParent->color = BLACK;
                if (s->right) s->right->color = BLACK;
                rotateLeft(xParent);
                x = this->root;  // Force loop to exit - we're done!
            }
            
        } else {
            // ============================================
            // X IS RIGHT CHILD - Handle right-side cases (MIRROR)
            // ============================================
            RBTNode* s = xParent->left;  // sibling (on left now)
            
            // Case 2: Red sibling
            if (isRed(s)) {
                s->color = BLACK;
                xParent->color = RED;
                rotateRight(xParent);  // Mirror: rotateRight
                s = xParent->left;     // Update sibling after rotation
            }
            
            // Case 3: Black sibling, both children black
            if (!isRed(s->right) && !isRed(s->left)) {
                s->color = RED;
                x = xParent;
                xParent = x->parent;
            } else {
                // Case 4: Near child red, far child black
                // Near child is s->right (closer to x)
                // Far child is s->left (farther from x)
                if (!isRed(s->left)) {
                    if (s->right) s->right->color = BLACK;
                    s->color = RED;
                    rotateLeft(s);     // Mirror: rotateLeft
                    s = xParent->left; // Update sibling after rotation
                }
                
                // Case 5: Far child red (terminal case)
                s->color = xParent->color;
                xParent->color = BLACK;
                if (s->left) s->left->color = BLACK;  // Far child is left
                rotateRight(xParent);  // Mirror: rotateRight
                x = this->root;        // Force loop to exit - we're done!
            }
        }
    }
    
    // If x is red (or became red), color it black to absorb extra black
    if (x) {
        x->color = BLACK;
    }
}

template <class K, class T>
void RedBlackTree<K, T>::remove(const K& key) {
    // Step 1: Find the node to delete
    RBTNode* z = this->root;
    while (z) {
        if (key == z->key) {
            break;
        } else if (key < z->key) {
            z = z->left;
        } else {
            z = z->right;
        }
    }
    
    if (!z) return;  // Key not found
    
    RBTNode* toDelete = z;  // The node we'll actually delete
    Color deletedColor;     // Track the color
    RBTNode* x;            // Replacement node
    RBTNode* xParent;      // Parent of replacement
    
    // Step 2: Handle 2-children case
    if (z->left && z->right) {
        // Find predecessor (maxNode in left subtree)
        RBTNode* pred = maxNode(z->left);
        
        // Swap data (not the nodes themselves)
        z->key = pred->key;
        z->data = pred->data;
        
        // Now delete the predecessor instead
        toDelete = pred;
    }
    
    // Step 3: toDelete now has at most 1 child
    deletedColor = toDelete->color;
    xParent = toDelete->parent;
    
    // Identify replacement
    if (toDelete->left) {
        x = toDelete->left;
    } else {
        x = toDelete->right;  // Could be nullptr
    }
    
    // Step 4: Replace toDelete with x
    if (!xParent) {
        // Deleting root
        this->root = x;
    } else if (toDelete == xParent->left) {
        xParent->left = x;
    } else {
        xParent->right = x;
    }
    
    // Update x's parent (if x exists)
    if (x) {
        x->parent = xParent;
    }
    
    // Step 5: Delete the node
    delete toDelete;
    
    // Step 6: Fix violations if we deleted a black node
    if (deletedColor == BLACK) {
        fixRemove(x, xParent);
    }
    
    // Step 7: Ensure root is black
    if (this->root) {
        this->root->color = BLACK;
    }
}

template <class K, class T>
typename RedBlackTree<K, T>::RBTNode* RedBlackTree<K, T>::find(const K& key) const {
	if (!this->root) return nullptr;
	
	RBTNode* current = this->root;
	while (current) {
		if (current->key == key) return current;
		else if (current->key < key) current = current->right;
		else current = current->left;
	}

	return nullptr;
}

template <class K, class T>
bool RedBlackTree<K, T>::contains(const K& key) const {
	if (!this->root) return false;
	
	RBTNode* current = this->root;
	while (current) {
		if (current->key == key) return true;
		else if (current->key < key) current = current->right;
		else current = current->left;
	}

	return false;
}

template <class K, class T>
typename RedBlackTree<K, T>::RBTNode* RedBlackTree<K, T>::lowerBound(const K& key, bool& found) const{
    RBTNode* res = nullptr;
    RBTNode* cur = this->root;

    while (cur) {
        if (cur->key >= key) {
            res = cur;
            cur = cur->left;
        } else cur = cur->right;
    }

    if (res) {
        found = true;
    }
    return res;
}

template <class K, class T>
typename RedBlackTree<K, T>::RBTNode* RedBlackTree<K, T>::upperBound(const K& key, bool& found) const{
    RBTNode* res = nullptr;
    RBTNode* cur = this->root;

    while (cur) {
        if (cur->key > key) {
            res = cur;
            cur = cur->left;
        } else cur = cur->right;
    }

    if (res) {
        found = true;
    }
    return res;
}

// =====================================
// VectorRecord implementation
// =====================================

// Overload operator << for VectorRecord - prints only the id
std::ostream& operator<<(std::ostream& os, const VectorRecord& record) {
    os << record.id;
    return os;
}

// =====================================
// VectorStore implementation
// =====================================

//TODO: Implement all VectorStore methods here

// Explicit template instantiation for the type used by VectorStore
template class AVLTree<double, VectorRecord>;
template class AVLTree<double, double>;
template class AVLTree<int, double>;
template class AVLTree<int, int>;
template class AVLTree<double, string>;
template class AVLTree<int, string>;

template class RedBlackTree<double, VectorRecord>;
template class RedBlackTree<double, double>;
template class RedBlackTree<int, double>;
template class RedBlackTree<int, int>;
template class RedBlackTree<double, string>;
template class RedBlackTree<int, string>;



