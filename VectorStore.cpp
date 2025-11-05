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
    AVLNode* l = y->pRight;

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

    int lHeight = height(node->pLeft);
    int rHeight = height(node->pRight);
    node->balance = (BalanceValue)(lHeight - rHeight);

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



