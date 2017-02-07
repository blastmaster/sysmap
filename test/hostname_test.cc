#include "utils.hpp"

#include <iostream>

using namespace std;
using namespace adafs::utils;

string get_hostname()
{
    string host;
    auto prog = exec::which("hostname");
    auto read_host = [&host](const string& s) {
                        if (!s.empty()) {
                            host = s;
                            return true;
                        }
                        return false;
                    };
    if (!prog.empty()) {
        auto res = exec::for_each_line(prog, {}, read_host);
        if (res) {
            return host;
        }
    } // if we cannot use the hostname binary try to read Hostname from /proc
    else {
        auto res = file::for_each_line("/proc/sys/kernel/hostname", read_host);
        if (res) {
            return host;
        }
    }

    return host;
}

string filename_prefix_host(const string& name)
{
    auto host = get_hostname();
    return host + "-" + name;
}

int main()
{
    auto h = get_hostname();
    cout << h << "\n";
    string filename("foo.txt");
    cout << filename << " -> " << filename_prefix_host(filename) << "\n";
    return 0;
}
