#include <iostream>

#include "utils/execution.hpp"


using namespace std;
using namespace adafs::utils::exec;


void do_some(const string& cmd, const vector<string>& args)
{
    map<string,string> envp;
    int count = 0;
    auto res = for_each_line(cmd, args,
            [&count](string& s) {
                if (!s.empty()) {
                    ++count;
                    return true;
                }
                return false;
            },
            [](string& s) { return true; },
            envp,
            nullptr,
            5);

    cout << "count: " << count << "\n";

}


int main(int argc, char** argv)
{

    vector<string> args {"-A1"};
    do_some("ls", args);

    return 0;
}
