#pragma once
#include <stdio.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdlib.h>
#include "expr.h"
using namespace std;

class FileManager {
private:
	
public:
	bool Command(string sentence, Matrix &ret);
	bool ReadFile(char* FileName, Matrix &ret);
};