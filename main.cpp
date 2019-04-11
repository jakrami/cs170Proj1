// Justin Akrami 4-10
#include <iostream>
#include <string>
#include <cstring>
#include <sstream>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

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
      inputTokens.push_back(temp);
    }

    char* argv[inputTokens.size() + 1];
    for (int i = 0; i < inputTokens.size(); i++) {
      argv[i] = const_cast<char*>(inputTokens[i].c_str());
    }
    cout << inputTokens.size() << endl;
    argv[inputTokens.size()] == nullptr;
    if (argv[1] == nullptr) {
      cout << "it's null" << endl;
    }
    /*
    cout << "NOW PRINTING OUT VECTOR OF TOKENS" << endl;
    for (int i = 0; i < inputTokens.size(); i++) {
      cout << inputTokens[i] << endl;
    }
    */

    // NOW STARTING TO DO THE ACTUAL EXECVP STUFF\

    int status;

    if (fork() > 0) {
      waitpid(-1, &status, 0);
      //exit(status);
    } else {
      cout << "in child process" << endl;
      cout << "output: " << execvp(argv[0], argv) << endl;
    }


    cin.clear();




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
