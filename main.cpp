// Justin Akrami 4-10
#include <iostream>
#include <string>
#include <cstring>
#include <sstream>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>


using namespace std;


/*void doSingularExec(char* args) {
  int status;
  if (fork() > 0) {
    waitpid(-1, &status, 0);

  } else {
    execvp(args[0], args);
  }
}*/
void doPipes(char*** finalArrays) {
  cout << "IN THIS FUNCTION" << endl;
  int status;
  int pipes[2];
  int pid;
  int pipeIn = 0;

    while (*finalArrays != nullptr) {
      pipe(pipes);

      if (fork() > 0) {
        waitpid(-1, &status, 0);
        close(pipes[1]);
        pipeIn = pipes[0];
        finalArrays++;
        //exit(status);
    } else {
      cout << "am i in here" << endl;
      dup2(pipeIn, 0);
      if (*(finalArrays + 1) != nullptr) {
        dup2(pipes[1], 1);
      }
      close(pipes[0]);
      execvp((*finalArrays)[0], *finalArrays);
      exit(EXIT_FAILURE);
    }
  }
}


vector<string> splitWord(string input) {
  bool lastWord = false;
  vector<string> output;
  int tracker = 0;
  int count = 0;
  if (input.length() == 1) {
    output.push_back(input);
    return output;
  }
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

        if (i == input.length() - 1) {
          lastWord = true;
        }


    }
    count++;


  }
  if (!lastWord) {
    output.push_back(input.substr(tracker, input.length() - tracker));
  }


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
      cout << "Word from stringStream: " << temp << endl;
      vector<string> supply = splitWord(temp);
      cout << supply.size() << endl;
      for (int i = 0; i < supply.size(); i++) {
        inputTokens.push_back(supply[i]);
      }


    }

    for (int i = 0; i < inputTokens.size(); i++) {
      cout << "I: " << i << " " << inputTokens[i] << endl;
    }

    cout << inputTokens.size() << endl;

    // Now go through input tokens and parse it.
    bool inputArrow = false;
    bool outputArrow = false;
    bool breaker = false;
    bool background = false;
    int pipecount = 0;

    vector<vector<string>> myList;

    vector<string> charArray;
    for (int i = 0; i < inputTokens.size(); i++) {

      string temp = inputTokens[i];

      //FIRST IF STATEMENT CHECKING FOR <
    /*  if (temp == "<") {
        continue;
      } */
        /*if ((i == 1 || i == 0) && inputArrow == false) {
          inputArrow = true;
          continue;
        } else {
          cout << "ERROR: Can only use < at beginning of input and only once" << endl;
          breaker = true;
          break;
        }
      }*/

      // SECOND IF STATEMENT THAT COUNTS HOW MANY PIPES WE NEED
      if (temp == "|") {

        vector<string> inserter;
        for (int i = 0; i < charArray.size(); i++) {
          inserter.push_back(charArray[i]);
        }
        myList.push_back(inserter);
        charArray.clear();
        pipecount++;
        continue;
      }



      // THIRD IF STATEMENT THAT SEES IF NEED TO REDIRECT OUTPUT
      if (temp == ">") {
        if (outputArrow == false && (i == inputTokens.size() - 3 || i == inputTokens.size() - 2)) {
          outputArrow = true;
          continue;

        } else {

          cout << "ERROR: CAN ONLY USE > ONCE AND AS THE LAST COMMAND" << endl;
          breaker = true;
          break;
        }
      }

      // Check TO SEE IF WE NEEDA RUN IN BACKGROUND

      if (temp == "&") {
        if (i == inputTokens.size() - 1) {
          background = true;
          continue;
        } else {
          cout << temp << endl;
          cout << "ERROR: CAN ONLY USE & AT END OF STATEMENT" << endl;
          breaker = true;
          break;
        }


      }

    charArray.push_back(temp);





  }
  // Put the final command into a vector to be turned into an array.
  vector<string> inserter;
  for (int i = 0; i < charArray.size(); i++) {
    inserter.push_back(charArray[i]);
  }
  myList.push_back(inserter);

  // END OF FOR LOOP


  // Check for errors for input REDIRECT
  string inputFile;
  string outputFile;
  for (int i = 0; i < myList.size(); i++) {
    for (int j = 0; j < myList[i].size(); j++) {
      if (myList[i][j] == "<") {
        if (inputArrow || i != 0) {
          cout << "ERROR: Can only use < at beginning of input and only once" << endl;
          breaker = true;
        } else {
          inputArrow = true;
          myList[i].erase(myList[i].begin() + j);
          inputFile = myList[i][j+1];
          myList[i].erase(myList[i].begin() + j + 1);

        }
      }
    }
  }

  // Now check for output REDIRECT

  if (outputArrow) {
    vector<string> x = myList[myList.size()-1];
    outputFile = x[x.size() - 1];
  }





  for (int i = 0; i < myList.size(); i++) {
    cout << "SIZE OF EACH VECTOR" << endl;
    cout << myList[i].size() << endl;
  }


    if (breaker) {
      if (!dontDisplayShell) {
        cout << "shell: " << endl;
      }
      continue;
    }


// CHANGE EACH ONE INTO A CHAR* ARRAY FOR EXEC
  char** finalArrays[myList.size() + 1];
  for (int i = 0; i < myList.size(); i++) {
    vector<string> tempVect = myList[i];
    char* argv[tempVect.size() + 1];
    for (int j = 0; j < tempVect.size(); j++) {
      argv[j] = const_cast<char*>(tempVect[j].c_str());
    }
    argv[tempVect.size()] = nullptr;
    finalArrays[i] = argv;
  }
  finalArrays[myList.size()] = nullptr;



// Now set up file descriptors and PIPES

int in, out;
char* charInputFile;
char* charOutputFile;
charInputFile = const_cast<char*>(inputFile.c_str());
charOutputFile = const_cast<char*>(outputFile.c_str());
if (inputArrow) {
  in = open(charInputFile, O_RDONLY);
  dup2(in, 0);
  close(in);
}

if (outputArrow) {
  out = open(charOutputFile, O_WRONLY);
  dup2(out, 1);
  close(out);
}




//NOW SET UP PIPES

if (myList.size() == 1) {
  int status;
  char* execList = *finalArrays[0];
  if (fork() > 0) {
      waitpid(-1, &status, 0);
      //exit(status);
    } else {
      cout << "in child process" << endl;
      //cout << "output: " << execvp(execList[0], execList) << endl;
    }

} else {
  doPipes(finalArrays);
}



//in = open(inputFile, O_RDONLY);









  /*  char* argv[inputTokens.size() + 1];
    for (int i = 0; i < inputTokens.size(); i++) {
      argv[i] = const_cast<char*>(inputTokens[i].c_str());
    }
    cout << inputTokens.size() << endl;
    argv[inputTokens.size()] = nullptr;
*/

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
