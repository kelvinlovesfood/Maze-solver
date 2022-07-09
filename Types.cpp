#include <string>
#include <iostream>
#include "Types.h"

Types::Types() {
   this->env = nullptr;
   this->rows = 0;
   this->cols = 0;
};

Types::~Types() {
   this->delete_env();
};

int Types::getRows() {
   return this->rows;
}

int Types::getCols() {
   return this->cols;
}

char** Types::getEnv() {
   return this->env;
}

void Types::make_env() {
   std::string memory = "";
   std::string lineToRead = "";
   int rectCheck = 0;
   while (!std::cin.eof()) {
      if (this->rows == 1) {
         this->cols = lineToRead.length();
      }
      getline(std::cin, lineToRead);
      memory.append(lineToRead);
      this->rows += 1;
   }

   this->env = new char*[this->cols];
   for (int i = 0; i != this->cols; ++i) {
      this->env[i] = new char[this->rows];
   }

   int i = 0;
   int yCount = 0;
   std::string sub = "";
   while (i <= memory.length()) {
      sub = memory.substr(i, this->cols);
      for (int x=0; x<this->cols; ++x) {
         this->env[x][yCount] = sub[x];
      }
      i += this->cols;
      yCount += 1;
   }
}

void Types::delete_env() {
   if (this->rows >= 0 && this->cols >= 0) {
      for (int i = 0; i != this->cols; ++i) {
         delete this->env[i];
      }
      delete this->env;
   }
}