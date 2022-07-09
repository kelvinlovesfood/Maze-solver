
#include "PathPlanner.h"

#include <iostream>

// Used for legibility in adjacency checking
enum {
   UP,
   RIGHT,
   DOWN,
   LEFT,
};

PathPlanner::PathPlanner(Env env, int rows, int cols){
   // Duplicate env into member variable so all methods can access it
   this->env = env;
   this->rows = rows;
   this->cols = cols;
   // Initialised as member variables so all methods can access these
   this->open = new NodeList(4*rows*cols);
   this->closed = new NodeList(4*rows*cols);
   this->path = new NodeList(4*rows*cols);
   this->S = nullptr;
   this->G = nullptr;
}

PathPlanner::~PathPlanner(){
   delete this->open;
   delete this->closed;
   delete this->path;
}

void PathPlanner::initialPosition(int row, int col){
   this->S = new Node(row, col, 0);
   this->open->addBack(this->S);
}

NodeList* PathPlanner::getReachableNodes(){
   for (int y=0; y<this->rows; ++y) {
      for (int x=0; x<this->cols; ++x) {
         if (this->env[x][y] == SYMBOL_START) {
            this->initialPosition(x, y);
         } 
      }
   }

   NodePtr P = this->S;

   bool nextExists = true;
   while (nextExists == true) { 
      nextExists =  false;

      if (this->checkAdjacent(this->env, P, UP, SYMBOL_EMPTY) == true
      || this->checkAdjacent(this->env, P, UP, SYMBOL_GOAL) == true) {
         // Create new node so that open/closed/path lists are separate in memory
         NodePtr up = new Node(P->getRow(), P->getCol()-1, P->getDistanceToS()+1);
         if (this->open->containsNode(up) == false) {
            this->open->addBack(up);
         }
      }
      if (this->checkAdjacent(this->env, P, RIGHT, SYMBOL_EMPTY) == true
      || this->checkAdjacent(this->env, P, RIGHT, SYMBOL_GOAL) == true) {
         // Increment/decrement row/col by 1 to create a node adjacent relative to P
         NodePtr right = new Node(P->getRow()+1, P->getCol(), P->getDistanceToS()+1);
         if (this->open->containsNode(right) == false) {
            this->open->addBack(right);
         }
      }
      if (this->checkAdjacent(this->env, P, DOWN, SYMBOL_EMPTY) == true
      || this->checkAdjacent(this->env, P, DOWN, SYMBOL_GOAL) == true) {
         NodePtr down = new Node(P->getRow(), P->getCol()+1, P->getDistanceToS()+1);
         // Check against open list to avoid u turns
         if (this->open->containsNode(down) == false) {
            this->open->addBack(down);
         }
      }
      if (this->checkAdjacent(this->env, P, LEFT, SYMBOL_EMPTY) == true
      || this->checkAdjacent(this->env, P, LEFT, SYMBOL_GOAL) == true) {
         NodePtr left = new Node(P->getRow()-1, P->getCol(), P->getDistanceToS()+1);
         if (this->open->containsNode(left) == false) {
            this->open->addBack(left);
         }
      }

      NodePtr beenThere = new Node(P->getRow(), P->getCol(), P->getDistanceToS());
      this->closed->addBack(beenThere);

      // Choose most recently node in open list that doesn't exist in closed
      // Basically depth search 
      for (int i=0; i<this->open->getLength() && nextExists == false; ++i) {
         if (this->closed->containsNode(this->open->get(i)) == false) {
            P = this->open->get(i);
            nextExists = true;
         }  
      }
   }

   // Return deep copy
   NodeList* reachables = new NodeList(*this->open);
   return reachables;
}

NodeList* PathPlanner::getPath(){
   int goalCount = 0;
   // Goal checking is done separately from the other checks because we have to consider
   // that many goals may exist, and that a fully completed open list is required to
   // deem any goal position reachable or unreachable.
   for (int i=0; i<this->open->getLength(); ++i) {
      NodePtr goal = this->open->get(i);
      if (this->env[this->open->get(i)->getRow()][this->open->get(i)->getCol()] == SYMBOL_GOAL) {
         goalCount += 1;
         if (goalCount == 1) {
            this->G = goal;
         }
         else {
            if (goal->getDistanceToS() < this->G->getDistanceToS()) {
               this->G = goal;
            }
         }
      }
   }

   // Admitted secondary method exit. 
   // If this is placed anywhere else there will be segmentation faults.
   if (goalCount == 0) {
      std::cout << "No solution found. Terminating..." << std::endl;
      exit(EXIT_FAILURE);
   }

   NodePtr goal = new Node(this->G->getRow(), this->G->getCol(), this->G->getDistanceToS());
   this->path->addBack(goal);

   NodePtr P = this->G;
   for (int i=0; i<this->open->getLength(); ++i) {  
      if (this->checkAdjacent(this->env, P, UP, SYMBOL_EMPTY) == true
      || this->checkAdjacent(this->env, P, UP, SYMBOL_START) == true) {
         // Increment/decrement row/col by 1 to check for immediate adjacencies
         // with a distance one less of P
         if (this->open->get(P->getRow(), P->getCol()-1)->getDistanceToS() == P->getDistanceToS()-1) {
            P = this->open->get(P->getRow(), P->getCol()-1);
            NodePtr up = new Node(P->getRow(), P->getCol(), P->getDistanceToS());
            this->path->addBack(up);
         }
      }
      if (this->checkAdjacent(this->env, P, RIGHT, SYMBOL_EMPTY) == true
      || this->checkAdjacent(this->env, P, RIGHT, SYMBOL_START) == true) {
         if (this->open->get(P->getRow()+1, P->getCol())->getDistanceToS() == P->getDistanceToS()-1) {
            P = this->open->get(P->getRow()+1, P->getCol());
            // Create new nodes again so open list and path list are separate in memory
            NodePtr right = new Node(P->getRow(), P->getCol(), P->getDistanceToS());
            this->path->addBack(right);
         }
      }
      if (this->checkAdjacent(this->env, P, DOWN, SYMBOL_EMPTY) == true
      || this->checkAdjacent(this->env, P, DOWN, SYMBOL_START) == true) {
         if (this->open->get(P->getRow(), P->getCol()+1)->getDistanceToS() == P->getDistanceToS()-1) {
            P = this->open->get(P->getRow(), P->getCol()+1);
            NodePtr down = new Node(P->getRow(), P->getCol(), P->getDistanceToS());
            this->path->addBack(down);
         }
      }
      if (this->checkAdjacent(this->env, P, LEFT, SYMBOL_EMPTY) == true
      || this->checkAdjacent(this->env, P, LEFT, SYMBOL_START) == true) {
         if (this->open->get(P->getRow()-1, P->getCol())->getDistanceToS() == P->getDistanceToS()-1) {
            P = this->open->get(P->getRow()-1, P->getCol());
            NodePtr left = new Node(P->getRow(), P->getCol(), P->getDistanceToS());
            this->path->addBack(left);
         }
      }
   }
   
   // Return deep copy
   NodeList* solution = new NodeList(*this->path);
   return solution;
}

// Directionality is parameterised so the function can be reused
// By limiting the capability of this method (i.e. not making nodes, not editing lists),
// it can be used modularly in more methods.
bool PathPlanner::checkAdjacent(Env env, NodePtr P, int direction, char target) {
   bool existsAdjacent = false;
   if (direction == UP) {
      // Once again incrementing/decrementing row/col by 1 to check for immediate adjacencies
      // where the character in env is the one we are looking for
      if (this->env[P->getRow()][P->getCol()-1] == target) {
         existsAdjacent = true;
      }
   }
   else if (direction == RIGHT) {
      if (this->env[P->getRow()+1][P->getCol()] == target) {
         existsAdjacent = true;
      }
   }
   else if (direction == DOWN) {
      if (this->env[P->getRow()][P->getCol()+1] == target) {
         existsAdjacent = true;
      }
   }
   else if (direction == LEFT) {
      if (this->env[P->getRow()-1][P->getCol()] == target) {
         existsAdjacent = true;
      }
   }
   return existsAdjacent;
}