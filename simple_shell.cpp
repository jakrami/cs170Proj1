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

void handler(int sig) {
  wait(NULL);
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
      /*  cout << "I am here, temp is: " << temp << endl;
        if (temp == " " || temp == "") {
          tracker = i+1;
          count = 0;
          continue;
        }*/

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
  int stat;
  string input;

  if (!dontDisplayShell) {
    cout << "shell: " << endl;
  }
  while(getline(cin, input)) {
    if (cin.eof()) {
      return;
    }
    if (input == "") {
      if (!dontDisplayShell) {
        cout << "shell: " << endl;
      }
      continue;
    }


    vector<string> inputTokens;
    stringstream ss(input);
    while (ss.good()) {
      string temp;

      ss >> temp;
      //cout << "temp: " << temp << endl;
      //cout << "Word from stringStream: " << temp << endl;
      vector<string> supply = splitWord(temp);
      //cout << supply.size() << endl;
      for (int i = 0; i < supply.size(); i++) {
        inputTokens.push_back(supply[i]);
      }


    }



    // Now go through input tokens and parse it.
    bool inputArrow = false;
    bool outputArrow = false;
    bool breaker = false;
    bool background = false;
    int pipecount = 0;
    string outputFile;
    vector<vector<string>> myList;

    vector<string> charArray;
    for (int i = 0; i < inputTokens.size(); i++) {

      string temp = inputTokens[i];
      //cout << temp << endl;

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
          if (inputTokens[inputTokens.size() - 1] == "&") {
              outputFile = inputTokens[inputTokens.size() - 2];
              inputTokens.erase(inputTokens.begin() + inputTokens.size() - 2);
          } else {
            outputFile = inputTokens[inputTokens.size() - 1];
            inputTokens.erase(inputTokens.begin() + inputTokens.size() - 1);
          }
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
          //cout << temp << endl;
          cout << "ERROR: CAN ONLY USE & AT END OF STATEMENT" << endl;
          breaker = true;
          break;
        }


      }

    charArray.push_back(temp);



// hi

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
  //string outputFile;
  for (int i = 0; i < myList.size(); i++) {
    for (int j = 0; j < myList[i].size(); j++) {
      //cout << "myList[i][j] : " << myList[i][j] << endl;
      if (myList[i][j] == "" || myList[i][j] == " ") {
        myList[i].erase(myList[i].begin() + j);
      }
      if (myList[i][j] == "<") {
        if (inputArrow || i != 0) {
          cout << "ERROR: Can only use < at beginning of input and only once" << endl;
          breaker = true;
          break;
        } else {
          inputArrow = true;
          myList[i].erase(myList[i].begin() + j);

          inputFile = myList[i][j];

          myList[i].erase(myList[i].begin() + j);

        }
      }
    }
  }



  // Now check for output REDIRECT



    if (breaker) {
      if (!dontDisplayShell) {
        cout << "shell: " << endl;
      }
      continue;
    }


// CHANGE EACH ONE INTO A CHAR* ARRAY FOR EXEC
  /*char** finalArrays[myList.size() + 1];
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
*/


// Now set up file descriptors and PIPES

/*for (int i = 0; i < myList.size(); i++) {
  cout << "i: " << i << endl;
  for (int j = 0; j < myList[i].size(); j++) {
    cout << myList[i][j] << endl;
  }
}*/






//NOW SET UP PIPES

if (myList.size() == 1) {

  /*
  for (int i = 0; i < myList[0].size(); i++) {
    cout << myList[0][i] << endl;
  }*/
  /*if (background) {
    setpgid(0,0);
  }*/
  char* argv[myList[0].size() + 1];
  for (int j = 0; j < myList[0].size(); j++) {
    argv[j] = const_cast<char*>(myList[0][j].c_str());
  }
  argv[myList[0].size()] = nullptr;

  int status;
  /*char* execList = *finalArrays[0];
  for (int i = 0; i < 1; i++) {
    cout << execList[i] << endl;
  }*/
  if (fork() > 0) {
      if (background) {
        signal(SIGCHLD, handler);
        //waitpid(-1, &status, WNOHANG);
      } else {
        waitpid(-1, &status, 0);
      }



    } else {
       //cout << "in child process" << endl;
       int in, out;
       char* charInputFile;
       char* charOutputFile;

       charInputFile = const_cast<char*>(inputFile.c_str());
       charOutputFile = const_cast<char*>(outputFile.c_str());
       //cout << "inputarrow: " << inputArrow << endl;
       //cout << "outputArrow: " << outputArrow << endl;
       if (inputArrow) {
         //cout << "inside input" << endl;
         in = open(charInputFile, O_RDONLY);
       }
       if (outputArrow) {
         out = open(charOutputFile,  O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
       }
       if (inputArrow) {
         //cout << "inside input 2" << endl;
         dup2(in, 0);
         close(in);
       }

       if (outputArrow) {

         dup2(out, 1);
         close(out);
       }

       //cout << "argv[0]: " << argv[0] << endl;
      /* cout << argv[1] << endl;
       if (argv[1] == nullptr) {
         cout << "its null" << endl;
       }*/
       if (execvp(argv[0], argv) < 0) {
         perror("ERROR:");
         exit(-1);
       }


    }

} else {

  //cout << "in the else statement" << endl;

  int pid;
  int oldpipe[2];
  int newpipe[2];

  int status1;
  for (int i = 0; i < myList.size(); i++) {

    //if (i != myList.size() -1) {

      pipe(newpipe);
    //}
    pid = fork();

    if (pid == 0) {

      if (background) {
        setpgid(0,0);
      }

      if (i == 0) {
        if (inputArrow) {
          int in;
          char* charInputFile;
          charInputFile = const_cast<char*>(inputFile.c_str());
          in = open(charInputFile, O_RDONLY);
          if (inputArrow) {

            dup2(in, 0);
            close(in);
          }
        }

      }

      if (i != 0) {

        close(oldpipe[1]);
        dup2(oldpipe[0], 0);
        close(oldpipe[0]);

      }

      if (i != myList.size() - 1) {
        close(newpipe[0]);
        dup2(newpipe[1], 1);
        close(newpipe[1]);
      }

      if (i == myList.size() - 1) {
        int out;
        char* charOutputFile;
        charOutputFile = const_cast<char*>(outputFile.c_str());
        if (outputArrow) {
          out = open(charOutputFile,  O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
        }
        if (outputArrow) {
          dup2(out, 1);
          close(out);
        }
      }
      // now execute the command
      char* argv[myList[i].size() + 1];
      for (int j = 0; j < myList[i].size(); j++) {
        argv[j] = const_cast<char*>(myList[i][j].c_str());
      }
      argv[myList[i].size()] = nullptr;

      if (execvp(argv[0], argv) < 0) {
        perror("ERROR");
        exit(-1);
      }

    } else {
      if (i != 0) {
        close(oldpipe[0]);
        close(oldpipe[1]);
      }
      if (i != myList.size() - 1) {

        oldpipe[0] = newpipe[0];
        oldpipe[1] = newpipe[1];
      }
    }


  }

  for (int i = 0; i < myList.size(); i++) {
    if (background) {
      signal(SIGCHLD, handler);
      //waitpid(-1, &status1, WNOHANG);
    } else {
      waitpid(-1, &status1, 0);
    }

  }

  close(oldpipe[0]);
  close(oldpipe[1]);


}

    if (background) {
      waitpid(-1, &stat, WNOHANG);
    }
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
