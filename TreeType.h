#include <iostream>
#include <cstdlib>

using namespace std;

template <class ItemType>
struct TreeNode {
    ItemType info;
    TreeNode *left;
    TreeNode *right;
};


/* Rotation functions ------------------------------------------------ */
template <class ItemType>
TreeNode<ItemType>* RotateRight(TreeNode<ItemType> *T) {
    TreeNode<ItemType> *S = T->left;
    TreeNode<ItemType> *B = S->right;
    S->right = T;
    T->left = B;
    return S;
}

template <class ItemType>
TreeNode<ItemType>* RotateRightLeft(TreeNode<ItemType> *T) {
    TreeNode<ItemType> *S = T->right;
    T->right = RotateRight(S);
    return RotateLeft(T);
}

template <class ItemType>
TreeNode<ItemType>* RotateLeftRight(TreeNode<ItemType> *T) {
    TreeNode<ItemType> *S = T->left;
    T->left = RotateLeft(S);
    return RotateRight(T);
}

template <class ItemType>
TreeNode<ItemType>* RotateLeft(TreeNode<ItemType> *T) {
    TreeNode<ItemType> *S = T->right;
    TreeNode<ItemType> *B = S->left;
    S->left = T;
    T->right = B;
    return S;
}
/* End rotation functions ------------------------------------------------ */



// Determine the height of the subtree starting with root
template <class ItemType>
int Height(TreeNode<ItemType> *root) {
    if (root == NULL) {
        return 0;
    }

    int left = Height(root->left);
    int right = Height(root->right);

    if (left > right) {
        return 1 + left;
    } else {
        return 1 + right;
    }
}

// Determine the difference in height between the left and right subtree
template <class ItemType>
int Difference(TreeNode<ItemType> *root) {
    return Height<ItemType>(root->left) - Height(root->right);
}

// Perform a depth-first traversal of the tree and print its contents.
template <class ItemType>
void DepthFirstPrint(TreeNode<ItemType> *root) {
    // Base case: We went past the end of the tree, and there is nothing to print.
    if (root == NULL) {
        return;
    }

    // First print the left subtree...
    DepthFirstPrint(root->left);
    // ...then print ourselves...
    cout << root->info << " ";
    // ...then print the right subtree.
    DepthFirstPrint(root->right);
}

// A function to balance the tree at a given node if necessary
template <class ItemType>
TreeNode<ItemType>* Balance(TreeNode<ItemType> *T) {
    int balanceFactor = Difference(T);

    // Is the balance factor too heavy on the left side?
    if (balanceFactor > 1) {
        // If the left subtree is heavier on the left side, we do a simple right rotation
        if (Difference(T->left) > 0) { // BOOK BUG: This line is incorrect in the book
            return RotateRight(T);
        } else {
            // Otherwise, the left subtree is heavier on the right side, so we do a left-right
            // rotation
            return RotateLeftRight(T);
        }
    }
    else if (balanceFactor < -1) {
        // If the right subtree is heavier on the right side, we do a simple left rotation
        if (Difference(T->right) < 0) { // BOOK BUG: This line is incorrect in the book
            return RotateLeft(T);
        } else {
            // Otherwise, the right subtree is heavier on the left side, so we do a right-left
            // rotation
            return RotateRightLeft(T);
        }
    }
    else {
        return T;
    }
}

// A recursive function InsertNode for inserting a new node at the correct position
// inside a binary search tree. It is intended to be called from inside a TreeType
// object.
template <class ItemType>
void InsertNode(TreeNode<ItemType> *&tree, ItemType item) {
    // Base case: We're trying to insert past the end of the tree.
    if (tree == NULL) {
        tree = new TreeNode<ItemType> {item, NULL, NULL};
    }
    // Does the item belong in the left subtree?
    else if (item < tree->info) {
        InsertNode(tree->left, item);
        // As we pull back from recursion, see if we need to balance the tree
        tree->left = Balance(tree->left);
    }
    // Otherwise, does the element belong in the right subtree?
    else {
        InsertNode(tree->right, item);
        // As we pull back from recursion, see if we need to balance the tree
        tree->right = Balance(tree->right);
    }
}

// A recursive function CountNodes for finding the number of nodes in a tree.
// It is intended to be called from inside a TreeType object. 
template <class ItemType>
int CountNodes(TreeNode<ItemType> *root) {
    // Base case: We went past the end of the tree
    if (root == NULL) {
        // Return 0 because there is no node for us to count
        return 0;
    }

    // Recursive case: Count ourselves, then the number of nodes on both the
    // left and right subtrees.
    return 1 + CountNodes<ItemType>(root->left) + CountNodes<ItemType>(root->right);
}

// This function is used as part of the delete process to find the predecessor of
// a tree node. It does this by finding the largest value in the subtree whose root
// is *tree.
template<class ItemType>
ItemType GetPredecessor(TreeNode<ItemType> *tree) {
    // To find the largest value, travel as far to the right as possible
    while (tree->right != NULL) {
        tree = tree->right;
    }

    // Return the largest value.
    return tree->info;
}

// This function is used as part of the delete process. Its job is to find a node to
// delete, then initiate the deletion process by calling DeleteNode().
template <class ItemType>
void Delete(TreeNode<ItemType> *&root, ItemType item) {
    // First step: Recursively try to find the node to delete.
    // Because of the preconditions, we can assume the item is in the
    // tree, and we do not need to perform any NULL checks.
    if (item < root->info) {
        // Item is in the left subtree
        Delete(root->left, item);
    } else if (item > root->info) {
        // Item is in the right subtree
        Delete(root->right, item);
    } else {
        // Item is contained at this node: delete it
        DeleteNode(root);
    }
}

template <class ItemType>
class TreeType {
    public:
        TreeType() {
            root = NULL;
        }

        int BalanceFactor() {
            // Return the balance factor of the root
            return Difference<ItemType>(root);
        }

        /**
         * Determines the number of elements in the tree.
         * Preconditions: None
         * Postconditions: The number of elements in the tree is returned.
         */
        int GetLength() {
            // Because we can't make this recursive, immediately call out to
            // our recursive CountNodes function and pass along its return value.
            return CountNodes<ItemType>(root);
        }

        /**
         * Function: Adds item to the tree.
         * Preconditions:
         *  - Tree is not full.
         *  - Item is not in the tree.
         * Postconditions:
         *  - Item is in tree.
         *  - Binary search property is maintained.
         */
        void PutItem(ItemType item){
            InsertNode<ItemType>(root, item);
            // Additional step: Do we need to balance the root?
            root = Balance(root);
        }
    
        // Perform a depth-first traversal to print the contents of the tree.
        void PrintTree() {
            cout << "Tree contents: ";
            DepthFirstPrint(root);
            cout << endl;
        }
    private:
        TreeNode<ItemType> *root;
};
