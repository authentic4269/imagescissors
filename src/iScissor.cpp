/* iScissor.cpp */
/* Main file for implementing project 1.  See TODO statments below
 * (see also correlation.cpp and iScissor.h for additional TODOs) */

#include <assert.h>
#include <iostream>
#include "correlation.h"
#include "iScissor.h"
#include <fstream>
using namespace std;
#include "PriorityQueue.h"

const double linkLengths[8] = { 1.0, SQRT2, 1.0, SQRT2, 1.0, SQRT2, 1.0, SQRT2 };

// two inlined routines that may help;

inline Node& NODE(Node* n, int i, int j, int width)
{
    return *(n + j * width + i);
}

inline unsigned char PIXEL(const unsigned char* p, int i, int j, int c, int width)
{
    return *(p + 3 * (j * width + i) + c);
}

/************************ TODO 1 ***************************
 *InitNodeBuf
 *	INPUT:
 *		img:	a RGB image of size imgWidth by imgHeight;
 *		nodes:	a allocated buffer of Nodes of the same size, one node corresponds to a pixel in img;
 *  OUPUT:
 *      initializes the column, row, and linkCost fields of each node in the node buffer.
 */

void InitNodeBuf(Node* nodes, const unsigned char* img, int imgWidth, int imgHeight)
{
  int j;
  int link;
  Node* firstnode = nodes;
  Node* cur;
  double *gradients = new double[8 * imgWidth * imgHeight];
  double maxlink = 0.0;
  double hi = 1.0;
  int sz = imgWidth * imgHeight * 3;
  unsigned char *selected = new unsigned char[imgWidth * imgHeight * 3];
  for (int i = 0; i < imgWidth * imgHeight * 3; i++)
  {
    selected[i] = 1;
  }
  for (int i = 0; i < 8; i++)
  {
    double *ptr = &gradients[i * imgWidth * imgHeight];
    const unsigned char *sel = selected;
    double ke[9] = {0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0};
    image_filter(ptr, img, sel, imgWidth, imgHeight, kernels[i], KERNEL_WIDTH, KERNEL_HEIGHT, 1.0, 0.0);
  }
  for (int i = 0; i < imgWidth; i++)
  {
    for (j = 0; j < imgHeight; j++)
    {
      cur = &(nodes[i * imgHeight + j]);
      (*cur).row = j;
      (*cur).column = i;
  //    cout << "\nRow: "; cout << j; cout << ", Column: "; cout << i; cout << ", Index: "; cout << (i * imgHeight + j);
      for (link = 0; link < 8; link++)
      {
        cur->linkCost[link] = gradients[link * imgWidth * imgHeight + i * imgHeight + j];
        if (cur->linkCost[link] > maxlink)
          maxlink = nodes->linkCost[link];
      }
    }
  }
  for (int i = 0; i < 8; i++)
  {
    nodes = firstnode;
    for (j = 0; j < imgWidth * imgHeight; j++)
    {
      nodes->linkCost[i] = maxlink - nodes->linkCost[i];
      nodes++;
    }
  }
  nodes = firstnode;
}


/************************ END OF TODO 1 ***************************/

static int offsetToLinkIndex(int dx, int dy)
{
    int indices[9] = { 3, 2, 1, 4, -1, 0, 5, 6, 7 };
    int tmp_idx = (dy + 1) * 3 + (dx + 1);
    //assert(tmp_idx >= 0 && tmp_idx < 9 && tmp_idx != 4);
    return indices[tmp_idx];
}

/************************ TODO 4 ***************************
 *LiveWireDP:
 *	INPUT:
 *		seedX, seedY:	seed position in nodes
 *		nodes:			node buffer of size width by height;
 *      width, height:  dimensions of the node buffer;
 *		selection:		if selection != NULL, search path only in the subset of nodes[j*width+i] if selection[j*width+i] = 1;
 *						otherwise, search in the whole set of nodes.
 *		numExpanded:		compute the only the first numExpanded number of nodes; (for debugging)
 *	OUTPUT:
 *		computes the minimum path tree from the seed node, by assigning
 *		the prevNode field of each node to its predecessor along the minimum
 *		cost path from the seed to that node.
 */

//void LiveWireDP(int seedX, int seedY, Node* nodes, int width, int height, const unsigned char* selection, int numExpanded)
//{
//    int currentExpanded = 0;
//    CTypedPtrHeap<Node> pq;
//    for (int i = 0; i < width * height; i++) {
//        nodes[i].state = INITIAL;
//        nodes[i].totalCost = 0;
//        nodes[i].prevNode = NULL;
//    }
//    Node *seedPtr = &(nodes[width * seedY + seedX]);
//    (*seedPtr).totalCost = 0.0;
//    pq.Insert(seedPtr);
//    //printf("The x value of seed: %d The y value of seed: %d", (*seedPtr).column, (*seedPtr).row );
//    while (!pq.IsEmpty()) {
//        Node *qPtr = pq.ExtractMin();
//        (*qPtr).state = EXPANDED;
//        //printf("Expanded node at position (%d, %d) \n", (*qPtr).column, (*qPtr).row);
//   //     printf("The x value of added node: %d The y value of added node: %d", (*qPtr).column, (*qPtr).row );
//        currentExpanded++;
//        if (currentExpanded > numExpanded) {
//            break;
//        }
//        for (int i = 0; i < 8; i++) {
//            int offsetX, offsetY;
//            (*qPtr).nbrNodeOffset(offsetX, offsetY, i);
//            int rX = (*qPtr).column + offsetX;
//            int rY = (*qPtr).row + offsetY;
//            if (rX < width && rX >= 0 && rY < height && rY >= 0) {
//                if (!selection || selection[width * rY + rX]) {
//                    Node *rPtr = &(nodes[width * rY + rX]);
//                    if (! ((*rPtr).state == EXPANDED)) {
//                        if ((*rPtr).state == INITIAL) {
//                            (*rPtr).totalCost = (*qPtr).totalCost + (*qPtr).linkCost[i];
//                            (*rPtr).state = ACTIVE;
//                            (*rPtr).prevNode = qPtr;
//                            printf("Node: (%d,%d)   prevNode: (%d,%d)\n", rPtr->column, rPtr->row, qPtr->column, qPtr->row);
//                            pq.Insert(rPtr);
//                        }
//                        else if ((*rPtr).state == ACTIVE) {
//                            double sumCost = (*qPtr).totalCost + (*qPtr).linkCost[i];
//                            if (sumCost < (*rPtr).totalCost) {
//                                (*rPtr).totalCost = sumCost;
//                                (*rPtr).prevNode = qPtr;
//                                printf("Node: (%d,%d)   prevNode: (%d,%d)\n", rPtr->column, rPtr->row, qPtr->column, qPtr->row);
//                            }
//                        }
//                    }
//                }
//            }
//        }
//    }
//
//}

void LiveWireDP(int seedX, int seedY, Node* nodes, int width, int height, const unsigned char* selection, int numExpanded)
{
    int currentExpanded = 0;
    CTypedPtrHeap<Node> pq;
    for (int i = 0; i < width * height; i++) {
        nodes[i].state = INITIAL;
        nodes[i].totalCost = 0;
        nodes[i].prevNode = NULL;
    }
    Node *seedPtr = &(nodes[width * seedY + seedX]);
    (*seedPtr).totalCost = 0.0;
    pq.Insert(seedPtr);
    //printf("The x value of seed: %d The y value of seed: %d", (*seedPtr).column, (*seedPtr).row );
    while (!pq.IsEmpty()) {
        Node *qPtr = pq.ExtractMin();
        (*qPtr).state = EXPANDED;
        //printf("Expanded node at position (%d, %d) \n", (*qPtr).column, (*qPtr).row);
        //     printf("The x value of added node: %d The y value of added node: %d", (*qPtr).column, (*qPtr).row );
        currentExpanded++;
        if (currentExpanded > numExpanded) {
            break;
        }
        for (int i = 0; i < 8; i++) {
            int offsetX, offsetY;
            (*qPtr).nbrNodeOffset(offsetX, offsetY, i);
          //  printf("offsetX: %d   offsetY: %d\n", offsetX, offsetY);
            int rX = (*qPtr).column + offsetX;
            int rY = (*qPtr).row + offsetY;
            //printf("rX: %d   rY: %d   x: %d   y: %d\n", rX, rY, qPtr->column, qPtr-> row);
            if (rX < width && rX >= 0 && rY < height && rY >= 0) {
                if (!selection || selection[height * rX + rY]) {
                    Node *rPtr = &(nodes[height * rX + rY]);
                 //   printf("r column: %d   rX: %d   r row: %d   rY: %d\n", rPtr->column, rX, rPtr->row, rY);
                   // cout << "\nrPtr index in nodes: "; cout << (width * rY + rX); cout << ", rY: "; cout << rY; cout << ", rX: "; cout << rX;
                   // cout << "\nrPtr index in nodes: "; cout << (width * rY + rX); cout << ", rY: "; cout << (*rPtr).row; cout << ", rX: "; cout << (*rPtr).column;
                    if (! ((*rPtr).state == EXPANDED)) {
                        if ((*rPtr).state == INITIAL) {
                            (*rPtr).totalCost = (*qPtr).totalCost + (*qPtr).linkCost[i];
                            (*rPtr).state = ACTIVE;
                            (*rPtr).prevNode = qPtr;
                //            printf("Node: (%d,%d)   prevNode: (%d,%d)\n", rPtr->column, rPtr->row, qPtr->column, qPtr->row);
                            pq.Insert(rPtr);
                        }
                        else if ((*rPtr).state == ACTIVE) {
                            double sumCost = (*qPtr).totalCost + (*qPtr).linkCost[i];
                            if (sumCost < (*rPtr).totalCost) {
                                (*rPtr).totalCost = sumCost;
                                (*rPtr).prevNode = qPtr;
                         //       printf("Node: (%d,%d)   prevNode: (%d,%d)\n", rPtr->column, rPtr->row, qPtr->column, qPtr->row);
                            }
                        }
                    }
                }
            }
        }
    }
}


/************************ END OF TODO 4 ***************************/

/************************ TODO 5 ***************************
 *MinimumPath:
 *	INPUT:
 *		nodes:				a node buffer of size width by height;
 *		width, height:		dimensions of the node buffer;
 *		freePtX, freePtY:	an input node position;
 *	OUTPUT:
 *		insert a list of nodes along the minimum cost path from the seed node to the input node.
 *		Notice that the seed node in the buffer has a NULL predecessor.
 *		And you want to insert a *pointer* to the Node into path, e.g.,
 *		insert nodes+j*width+i (or &(nodes[j*width+i])) if you want to insert node at (i,j), instead of nodes[nodes+j*width+i]!!!
 *		after the procedure, the seed should be the head of path and the input code should be the tail
 */


void MinimumPath(CTypedPtrDblList <Node>* path, int freePtX, int freePtY, Node* nodes, int width, int height)
{
    Node *n = &(nodes[freePtY * width + freePtX]);
    while (n != NULL) {
        CTypedPtrDblElement<Node>* elem;
        Node *data;
        elem = (*path).AddHead(n);
        data = (*elem).Data();
        n = (*data).prevNode;
        if (n != NULL) {
            printf("adding node at position (%d,%d) \n", n->column, n->row);
        }
    }

	// Extra credit
}
/************************ END OF TODO 5 ***************************/

/************************ An Extra Credit Item ***************************
 *SeeSnap:
 *	INPUT:
 *		img:				a RGB image buffer of size width by height;
 *		width, height:		dimensions of the image buffer;
 *		x,y:				an input seed position;
 *	OUTPUT:
 *		update the value of x,y to the closest edge based on local image information.
 */

void SeedSnap(int& x, int& y, unsigned char* img, int width, int height)
{
	//Extra credit
}

//generate a cost graph from original image and node buffer with all the link costs;
void MakeCostGraph(unsigned char* costGraph, const Node* nodes, const unsigned char* img, int imgWidth, int imgHeight)
{
    int graphWidth = imgWidth * 3;
    int graphHeight = imgHeight * 3;
    int dgX = 3;
    int dgY = 3 * graphWidth;

    int i, j;
    for (j = 0; j < imgHeight; j++) {
        for (i = 0; i < imgWidth; i++) {
            int nodeIndex = j * imgWidth + i;
            int imgIndex = 3 * nodeIndex;
            int costIndex = 3 * ((3 * j + 1) * graphWidth + (3 * i + 1));

            const Node* node = nodes + nodeIndex;
            const unsigned char* pxl = img + imgIndex;
            unsigned char* cst = costGraph + costIndex;

            cst[0] = pxl[0];
            cst[1] = pxl[1];
            cst[2] = pxl[2];

            //r,g,b channels are grad info in seperate channels;
            DigitizeCost(cst	   + dgX, node->linkCost[0]);
            DigitizeCost(cst - dgY + dgX, node->linkCost[1]);
            DigitizeCost(cst - dgY      , node->linkCost[2]);
            DigitizeCost(cst - dgY - dgX, node->linkCost[3]);
            DigitizeCost(cst	   - dgX, node->linkCost[4]);
            DigitizeCost(cst + dgY - dgX, node->linkCost[5]);
            DigitizeCost(cst + dgY	   ,  node->linkCost[6]);
            DigitizeCost(cst + dgY + dgX, node->linkCost[7]);
        }
    }
}

