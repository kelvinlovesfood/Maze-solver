/* An issue I had in the beginning was failing to realise that nodes could be
 * owned by two or more lists. This lead to lots of difficult segmentation faults
 * to debug as there was no transparency as to whether I was for example either: 
 * accessing a pointer that had changed ownership, or deleting a pointer twice.
 * 
 * Therefore the approach I took in my implementation was to initialise 
 * the node lists as PathPlanner member objects. Compared to when I was initialising
 * them within the methods, the lists were now viewable from main. The benefit of
 * this was that I could identify and debug when I was accidentally sharing nodes between
 * many lists (instead of creating new nodes), thus fixing and avoiding many segmentation 
 * faults from double deletion, or calling NodeList methods on disowned pointers.
 */

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>

#include "Types.h"
#include "Node.h"
#include "NodeList.h"
#include "PathPlanner.h"

// Helper test functions
void testNode();
void testNodeList();

// Read a environment from standard input.
void readEnvStdin(Types* env);

// Print out a Environment to standard output with path.
// To be implemented for Milestone 3
void printPath(Types* env, NodeList* solution);

// A custom debug method used to verify nodes
void printList(NodeList* nodes);

void printEnv(Types* env);

int main(int argc, char** argv){
   // THESE ARE SOME EXAMPLE FUNCTIONS TO HELP TEST YOUR CODE
   // AS YOU WORK ON MILESTONE 2. YOU CAN UPDATE THEM YOURSELF
   // AS YOU GO ALONG.
   // COMMENT THESE OUT BEFORE YOU SUBMIT!!!
   // std::cout << "TESTING - COMMENT THE OUT TESTING BEFORE YOU SUBMIT!!!" << std::endl;
   // testNode();
   // testNodeList();
   // std::cout << "DONE TESTING" << std::endl << std::endl;

   // Load Environment 
   Types* env = new Types();
   readEnvStdin(env);
   // printEnv(env);
 
   // Solve using forwardSearch
   // THIS WILL ONLY WORK IF YOU'VE FINISHED MILESTONE 2
   PathPlanner* pathplanner = new PathPlanner(env->getEnv(), env->getRows(), env->getCols());
   NodeList* reachablePositions = new NodeList();
   reachablePositions = pathplanner->getReachableNodes();

   // Get the path
   // THIS WILL ONLY WORK IF YOU'VE FINISHED MILESTONE 3
   NodeList* solution = pathplanner->getPath();

   // print the path
   printPath(env, solution);

   // The following are used for debugging
   // std::cout << std::endl;
   // std::cout << std::endl << "Reachables: " << std::endl;
   // printList(reachablePositions);
   // std::cout << std::endl << "Solution: " << std::endl;
   // printList(solution);

   delete pathplanner;
   delete reachablePositions;
   delete solution;
   delete env;

   return EXIT_SUCCESS;
}

void printEnv(Types* env) {
   for (int y=0; y<env->getRows(); ++y) {
      for (int x=0; x<env->getCols(); ++x) {
         std::cout << env->getEnv()[x][y];
      }
      std::cout << std::endl;
   }
}

void readEnvStdin(Types* env) {
   env->make_env();
}

void printPath(Types* env, NodeList* solution) {
   for (int y=0; y<env->getRows(); y++) {
      for (int x=0; x<env->getCols(); x++) {
         // Because solution is generated from goal to start, the solution list is inverted
         // Therefore iterate backwards to print in correct order
         // Exclude first and last index of solution nodelist so that S and G are not overwritten
         for (int i=solution->getLength()-2; i>0; --i) {
            if (solution->get(i)->getRow() == x && solution->get(i)->getCol() == y) {
               // Choose arrow direction relative to which side the next solution node is on
               if (solution->get(i-1)->getCol() == solution->get(i)->getCol()-1) {
                  env->getEnv()[x][y] = '^';
               }
               else if (solution->get(i-1)->getRow() == solution->get(i)->getRow()+1) {
                  env->getEnv()[x][y] = '>';
               }
               else if (solution->get(i-1)->getCol() == solution->get(i)->getCol()+1) {
                  env->getEnv()[x][y] = 'V';
               }
               else if (solution->get(i-1)->getRow() == solution->get(i)->getRow()-1) {
                  env->getEnv()[x][y] = '<';
               }
            }
         }
         std::cout << env->getEnv()[x][y];
      }
      // Ensure no trailing newline character to fit with given expout
      if (y != env->getRows()-1) {
         std::cout << std::endl;
      }
   }
}

// A custom debug function
// prints node coordinates, distance, and memory address
void printList(NodeList* nodes){
   for (int i=0; i<nodes->getLength(); ++i) {
      std::cout << "(";
      std::cout << nodes->get(i)->getRow() << ", ";
      std::cout << nodes->get(i)->getCol() << ") ";
      std::cout << nodes->get(i)->getDistanceToS() << " at: ";
      std::cout << nodes->get(i) << std::endl;
   }
}

void testNode() {
   std::cout << "TESTING Node" << std::endl;

   // Make a Node and print out the contents
   Node* node = new Node(1, 1, 2);
   std::cout << node->getRow() << ",";
   std::cout << node->getCol() << ",";
   std::cout << node->getDistanceToS() << std::endl;
   delete node;

   // Change Node and print again
   node = new Node(4, 2, 3);
   std::cout << node->getRow() << ",";
   std::cout << node->getCol() << ",";
   std::cout << node->getDistanceToS() << std::endl;
   delete node;
}

void testNodeList() {
   std::cout << "TESTING NodeList" << std::endl;

   // Make a simple NodeList, should be empty size
   NodeList* nodeList = new NodeList();
   std::cout << "NodeList size: " << nodeList->getLength() << std::endl;

   // Add a Node to the NodeList, print size
   Node* b1 = new Node(1, 1, 1);
   nodeList->addBack(b1);
   std::cout << "NodeList size: " << nodeList->getLength() << std::endl;

   // Add second Nodetest
   Node* b2 = new Node(0, 0, 1);
   nodeList->addBack(b2);
   std::cout << "NodeList size: " << nodeList->getLength() << std::endl;

   // Test Get-ith - should be 0,0,1
   Node* getB = nodeList->get(1);
   std::cout << getB->getRow() << ",";
   std::cout << getB->getCol() << ",";
   std::cout << getB->getDistanceToS() << std::endl;

   // Print out the NodeList
   std::cout << "PRINTING OUT A NODELIST IS AN EXERCISE FOR YOU TO DO" << std::endl;
}
