/*
CS3210 - Programming Assignment, Spring 2025
Author: Suraj Sharma, 110231904
Email: suraj.sharma_ugt2023@ashoka.edu.in

# Algorithm:
We solve the problem of multilayer maximal points using an AVL tree (BBST), as insertion and searching require O(log n) time.
Each node in the AVL tree represents a layer, and each node has its own dynamic array that contains the points assigned to that layer.

The algorithm for assigning layers works as follows:
1. If a point `p` has a `y` value greater than the maximum `y` of all existing layers, it is assigned to the topmost layer.
2. If a point `q
` has a `y` value less than the maximum `y` of all existing layers, a new layer is created.
3. If a point `r` falls between two layers `M_k` and `M_{k+1}` such that `M_k <= r.y < M_{k+1}`, then `r` is assigned to the `M_k` layer, as all layers above it dominate `r`.

The algorithm processes points in decreasing order of `x` (right to left). As each point is encountered, a decision is made based on the conditions above.
Since searching and insertion in an AVL tree take O(log n) time, and sorting the `N` points takes O(n log n), the overall complexity of the algorithm is O(n log n).

# Running the Program:
## Input:
- The input file should be named `inputX`, where `X` is any number.

## Output:
- The corresponding output file will be generated with the name `outputX`, where `X` matches the input file number.

Also, the number of points and Global counter T is saved in a file named `nT_data`, which can be used for plotting results later.

To run this program, use the following commands:

gcc main.c -o main -lm  # Note the addition of the math library at compile time
./main inputX  # Provide the input file as a standard input file at the command line



*/



#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h> 

// Global counter
int T = 0;

// Structure for representing a point
typedef struct {
    int x;
    int y;
} Point;

// Structure for representing a dynamic array of points for each AVL node
typedef struct {
    Point* pts; 
    int sz;     // Size 
    int cap;    // Capacity
} DynArr;

// Fn to initialize a dynamic array
void initDynArr(DynArr* arr, int cap) {
    arr->pts = (Point*)malloc(cap * sizeof(Point));
    arr->sz = 0;
    arr->cap = cap;
}

// Fn for appending a point to the array
void appendDynArr(DynArr* arr, Point p) {
    if (arr->sz == arr->cap) {
        arr->cap *= 2;
        arr->pts = (Point*)realloc(arr->pts, arr->cap * sizeof(Point));
    }
    arr->pts[arr->sz++] = p;
    T += 2; // count: assignment, increment
}

// Fn to compare points (y-chord, increasing order)
int cmpYInc(const void* a, const void* b) {
    Point* p1 = (Point*)a;
    Point* p2 = (Point*)b;
    T += 1; // count: comparison
    return p1->y - p2->y; 
}

// Fn to compare points (x-coord, decreasing order)
int cmpXDesc(const void* a, const void* b) {
    Point* p1 = (Point*)a;
    Point* p2 = (Point*)b;
    T += 1; // count: comparison
    return p2->x - p1->x;
}

// Structure for representing the AVL Tree
typedef struct AVLNode {
    int maxY; // Max y-coord in this layer
    DynArr layerPts; // Points in this layer
    int height; // Height of the node
    struct AVLNode* left;
    struct AVLNode* right;
} AVLNode;

// Fn for creating a new AVL node
AVLNode* createAVL_N(Point p) {
    AVLNode* node = (AVLNode*)malloc(sizeof(AVLNode));
    node->maxY = p.y;
    initDynArr(&node->layerPts, 2); // initial capacity
    appendDynArr(&node->layerPts, p);
    node->left = node->right = NULL;
    node->height = 1; 
    return node;
}

// Fn to get the height of a node
int height(AVLNode* node) {
    if (node == NULL) return 0;
    return node->height;
}

// Fn calculate the balance factor of a node
int balanceFactor(AVLNode* node) {
    if (node == NULL) return 0;
    return height(node->left) - height(node->right);
}

// Fn to perform a right rotation
AVLNode* rightRotate(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;

    // Perform rotation
    x->right = y;
    y->left = T2;

    // Update heights
    y->height = 1 + fmax(height(y->left), height(y->right));
    x->height = 1 + fmax(height(x->left), height(x->right));

    T += 4; // count: assignments, fmax, height
    return x;
}

// Fn to perform a left rotation
AVLNode* leftRotate(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;

    // Perform rotation
    y->left = x;
    x->right = T2;

    // Update heights
    x->height = 1 + fmax(height(x->left), height(x->right));
    y->height = 1 + fmax(height(y->left), height(y->right));

    T += 4; // count: assignments, fmax, height
    return y;
}

// Fn to insert a point into the AVL Tree
AVLNode* insertAVL(AVLNode* node, Point p) {
    if (node == NULL) {
        T += 1; // count: comparison
        return createAVL_N(p);
    }
    T += 1; // count: comparison
    if (p.y <= node->maxY) {
        node->left = insertAVL(node->left, p);
        T += 1; // count: assignment
    } else {
        node->right = insertAVL(node->right, p);
        T += 1; // count: assignment
    }

    // Update height
    node->height = 1 + fmax(height(node->left), height(node->right));
    T += 3; // count: fmax, height, assignment

    // Get the balance factor
    int balance = balanceFactor(node);
    T += 1; // count: balanceFactor

    // Perform rotations if the node is unbalanced
    if (balance > 1 && p.y <= node->left->maxY) {
        return rightRotate(node);
    }
    if (balance < -1 && p.y > node->right->maxY) {
        return leftRotate(node);
    }
    if (balance > 1 && p.y > node->left->maxY) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }
    if (balance < -1 && p.y <= node->right->maxY) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

// Fn to add a point to a layer
AVLNode* addPointToLayer(AVLNode* root, Point p) {
    AVLNode* curr = root;
    AVLNode* bestLayer = NULL;

    // Traverse the AVL Tree to find the largest maxY such that maxY <= p.y
    while (curr != NULL) {
        if (p.y >= curr->maxY) {
            bestLayer = curr;
            curr = curr->right; // Check if there's a better layer (larger maxY)
        } else {
            curr = curr->left;
        }
    }

    if (bestLayer == NULL) {
        // Case 1: No layer can accommodate the point (p.y < maxY for all layers)
        root = insertAVL(root, p);
    } else if (p.y > bestLayer->maxY) {
        // update maxY 
        bestLayer->maxY = p.y;
        appendDynArr(&bestLayer->layerPts, p);
    } else {
        appendDynArr(&bestLayer->layerPts, p);
    }

    return root;
}

// Fn to perform reverse in-order traversal of the AVL Tree and write to output file
void reverseInorderTraversalAndWrite(FILE* outFile, AVLNode* root) {
    if (root != NULL) {
        // Traverse the right subtree first (higher maxY)
        reverseInorderTraversalAndWrite(outFile, root->right);

        // Sort points in the current layer by y-coordinate in increasing order
        qsort(root->layerPts.pts, root->layerPts.sz, sizeof(Point), cmpYInc);
        T += root->layerPts.sz * log2(root->layerPts.sz); // count: qsort

        // Write points to the output file
        for (int i = 0; i < root->layerPts.sz; i++) {
            fprintf(outFile, "%d, %d\n", root->layerPts.pts[i].x, root->layerPts.pts[i].y);
        }
        fprintf(outFile, "\n"); // Blank line between layers

        // Traverse the left subtree (lower maxY)
        reverseInorderTraversalAndWrite(outFile, root->left);
    }
}

void freeAVL(AVLNode* root) {
    if (root != NULL) {
        freeAVL(root->left);
        freeAVL(root->right);
        free(root->layerPts.pts);
        free(root);
    }
}

int extractNumber(const char* filename) {
    int num = 0;
    const char* ptr = filename;
    while (*ptr && !isdigit(*ptr)) ptr++; // Skip non-digits
    while (*ptr && isdigit(*ptr)) {
        num = num * 10 + (*ptr - '0');
        ptr++;
    }
    return num;
}

// Main Fn
int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Extract the number from the input filename
    int num = extractNumber(argv[1]);
    char outputFilename[100];
    snprintf(outputFilename, sizeof(outputFilename), "output%d", num);

    // Open input file
    FILE* inputFile = fopen(argv[1], "r");
    if (inputFile == NULL) {
        perror("Error opening input file");
        return EXIT_FAILURE;
    }

    // Read number of points
    int n;
    if (fscanf(inputFile, "%d", &n) != 1) {
        fprintf(stderr, "Error reading number of points\n");
        fclose(inputFile);
        return EXIT_FAILURE;
    }

    // Allocate memory for points
    Point* pts = (Point*)malloc(n * sizeof(Point));
    if (pts == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        fclose(inputFile);
        return EXIT_FAILURE;
    }

    // Read points from input file
    for (int i = 0; i < n; i++) {
        if (fscanf(inputFile, "%d %d", &pts[i].x, &pts[i].y) != 2) {
            fprintf(stderr, "Error reading point at index %d\n", i);
            free(pts);
            fclose(inputFile);
            return EXIT_FAILURE;
        }
    }

    fclose(inputFile);

    qsort(pts, n, sizeof(Point), cmpXDesc);

    // Initialize
    AVLNode* root = NULL;

    // Insert points into AVL Tree
    for (int i = 0; i < n; i++) {
        root = addPointToLayer(root, pts[i]);
        T += 1; // count: loop iteration
    }

    FILE* outFile = fopen(outputFilename, "w");
    if (outFile == NULL) {
        perror("Error opening output file");
        free(pts);
        freeAVL(root);
        return EXIT_FAILURE;
    }

    // Write layers to output file in reverse in-order (decreasing maxY)
    reverseInorderTraversalAndWrite(outFile, root);

   
    fclose(outFile);

    free(pts);
    freeAVL(root);

	// Append n and T to nT_data file
	FILE* nTFile = fopen("nT_data", "a");
	if (nTFile == NULL) {
	    perror("Error opening nT_data file");
	    return EXIT_FAILURE;
	}
	fprintf(nTFile, "%d,%d\n", n, T); 
	fclose(nTFile);

    return EXIT_SUCCESS;
}
