// Justin Akrami 4-10
#include <iostream>
#include <string>
#include <cstring>
#include <sstream>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;


vector<string> splitWord(string input) {
  vector<string> output;
  int tracker = 0;
  int count = 0;
  for (int i = 0; i < input.length(); i++) {
    if (input[i] == '<' || input[i] == '>' || 
        input[i] == '|' || input[i] == '&') {

        string temp;
        if (tracker == 0) {
           temp = input.substr(tracker, i);
        }
        else {
           temp = input.substr(tracker, count-1);
        }
        
        output.push_back(temp);
        temp = input[i];
        output.push_back(temp);
        tracker = i+1;
        count = 0;
        

    }
    count++;

    
  }
  output.push_back(input.substr(tracker, input.length() - tracker));

  return output;
}

void runShell(bool dontDisplayShell) {

  string input;
  if (!dontDisplayShell) {
    cout << "shell: " << endl;
  }
  while(getline(cin, input)) {
    if (cin.eof()) {
      return;
    }


    vector<string> inputTokens;
    stringstream ss(input);
    while (ss.good()) {
      string temp;
      ss >> temp;
      vector<string> supply = splitWord(temp);
      for (int i = 0; i < supply.size(); i++) {
        inputTokens.push_back(supply[i]);
      }
      
    
    }

    /*for (int i = 0; i < inputTokens.size(); i++) {
      cout << inputTokens[i] << endl;
    }*/


    // Now go through input tokens and parse it.
    bool inputArrow = false; 
    bool outputArrow = false;
    bool breaker = false;

    vector<vector<string>> myList;

    vector<string> charArray;
    for (int i = 0; i < inputTokens.size(); i++) {

      string temp = inputTokens[i];

      //FIRST IF STATEMENT CHECKING FOR < 
      if (temp == "<") {
        if ((i == 1 || i == 0) && inputArrow == false) {
          input = true;
        } else {
          cout << "ERROR: Can only use < at beginning of input and only once" << endl;
          breaker = true;
          break;
        }
      }

      // SECOND IF STATEMENT THAT COUNTS HOW MANY PIPES WE NEED
      



    }
    if (breaker) {
      if (!dontDisplayShell) {
        cout << "shell: " << endl;
      }
      continue;
    }




    char* argv[inputTokens.size() + 1];
    for (int i = 0; i < inputTokens.size(); i++) {
      argv[i] = const_cast<char*>(inputTokens[i].c_str());
    }
    cout << inputTokens.size() << endl;
    argv[inputTokens.size()] = nullptr;
    

    // NOW STARTING TO DO THE ACTUAL EXECVP STUFF\

   /* int status;

    if (fork() > 0) {
      waitpid(-1, &status, 0);
      //exit(status);
    } else {
      cout << "in child process" << endl;
      cout << "output: " << execvp(argv[0], argv) << endl;
    }


    cin.clear();

*/


    if (!dontDisplayShell) {
      cout << "shell: " << endl;
    }

  }
}





int main(int argc, char* argv[]) {

  bool dontDisplayShell = false;
  if (argc > 1) {
    string check = argv[1];

    if (check == "-n") {

      dontDisplayShell = true;
    } else {
      cout << "Error: Please only use -n to not display shell" << endl;
      return 1;
    }
  }
  runShell(dontDisplayShell);

  return 0;
}
