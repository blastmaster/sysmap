#include <iostream>
#include <vector>
#include <map>
#include <string>

#include "utils/execution.hpp"

using namespace std;
using namespace sysmap::utils::exec;

void do_some(const string& cmd, const vector<string>& args, const string input)
{
    map<string, string> envp {};
    auto res = execute(cmd, args,
            [](string& s) { if (s.empty()) return false; cout << s << "\n"; return true; },
            [](string& s) { if (s.empty()) return false; cout << s << "\n"; return true; },
            envp,
            input.c_str(),
            5);
}

int main(int argc, char** argv)
{
    string input = "Hallo Welt!\n";
    vector<string> args { };
    do_some("/bin/cat", args, input);

    return 0;
}
