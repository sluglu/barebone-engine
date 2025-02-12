#include <iostream>
#include "Point.h"

using namespace std;



void main() {
    Octree octree(Point(0, 0, 0), 1.0, 4);
    octree.insert(Point(0.1, 0.1, 0.1));
    octree.insert(Point(0.2, 0.2, 0.2));
    octree.insert(Point(0.3, 0.3, 0.3));
    octree.insert(Point(0.4, 0.4, 0.4));
    octree.insert(Point(0.5, 0.5, 0.5));
}

