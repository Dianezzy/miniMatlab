#include"filemanager.h"

extern Command C;//Caller global command class

/*
The command input in GUI is parsed to determine whether it is a file operation, and the command to be run is passed into the Expr, command running class.
*/
bool FileManager::Command(string sentence, Matrix &ret) {
	//Determine whether it is a file operation
	if (sentence[0] == 'f' && sentence[1] == 'i' && sentence[2] == 'l' && sentence[3] == 'e' && sentence[4] == ':') {
		char FileName[20];
		int i;

		//Read in the file name
		for (i = 5; i < sentence.length(); i++) {
			FileName[i - 5] = sentence[i];
		}
		FileName[i - 5] = '\0';//Add '\0' at the end of char*, to make it can be read as a string. 

		return ReadFile(FileName, ret);
		
	}
	//If it is not a file operation, enter the command execution directly
	else {
		C.Load(sentence);//command Load
		return C.Execute(ret);//command Execute
	}
}

/*
Read the command from the file and Execute it.
*/
bool FileManager::ReadFile(char * FileName, Matrix &ret)
{
	ifstream inFile(FileName);//open the file
	ostringstream buf;//Read the file into the ostringstream object buf
	char inChar=0;

	//If the file does not exist, an error is reported
	//As long as the file exists, even if it is empty, the content can be read
	if (!inFile.get(inChar)) {
		error("The file is not exist.");
		return 0;
	}
	else {
		buf.put(inChar);
	}

	while (buf&&inFile.get(inChar)) {
		//Read by character
		buf.put(inChar);

		//Execution by line
		if (inChar == '\n') {
			C.Load(buf.str());
			C.Execute(ret);
			buf.str("");//Clear command buffer
		}
	}

	//Execute the last line of command and return the result
	C.Load(buf.str());
	return C.Execute(ret);
}
