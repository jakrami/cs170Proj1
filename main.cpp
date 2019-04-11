// Justin Akrami 4-10
#include <iostream>
#include <string>

using namespace std;

void runShell(bool dontDisplayShell) {

  string input;
  if (!dontDisplayShell) {
    cout << "shell: " << endl;
  }
  while(cin >> input) {
    if (cin.eof()) {
      return;
    }

    cout << input + "!" << endl;

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
