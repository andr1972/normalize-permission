#include <iostream>
#include <vector>
#include <sys/stat.h>
#include <string>
#include <algorithm>
#include <filesystem>

using std::cout; using std::cin;
using std::endl; using std::string;
namespace fs = std::filesystem;

int beforePerm = 0, afterPerm = 0;
bool recursive = false;
bool dry = false;

bool needChange(const string& path) {
    const char *cpath = path.c_str();
    struct stat st;
    if(stat(cpath, &st)) return false;//bad file, not try change attributes
    mode_t perm = st.st_mode & 0777;
    return perm == beforePerm;
}

void changePerm(const string& path, bool isDir) {
    const char *cpath = path.c_str();
    struct stat st;
    if(stat(cpath, &st)) return;
    mode_t perm = st.st_mode;
    chmod(cpath, afterPerm);
}


void listDirectory(string currentDir) {
    fs::path dirPath(currentDir);
    string pwd = canonical(dirPath).string();
    std::vector<string> fileNames;
    std::vector<string> dirNames;
    for (const auto &file: fs::directory_iterator(pwd)) {
        string name = file.path().filename().string();
        if (file.is_directory())
            dirNames.push_back(name);
        else
            fileNames.push_back(name);
    }
    std::sort(dirNames.begin(), dirNames.end());
    std::sort(fileNames.begin(), fileNames.end());
    for (const string& name: fileNames) {
        string path =  pwd+ "/" + name;
        if (needChange(path)) {
            cout << path << endl;
            if (!dry)
                changePerm(path, false);
        }
    }
    if (recursive)
    for (const string& name: dirNames) {
        string path =  pwd+ "/" + name;
        listDirectory(path);
    }
}

void help() {
    cout << "changes permission of files, not directories" << endl;
    cout << "call: normalizeAttr octBefore octAfter" << endl;
    cout << "-r: recursive" << endl;
    cout << "-dry: only output names, not change" << endl;
    cout << "example: ~/normalizeAttr 755 644 -r -dry" << endl;
}

bool processParameters(int argc, char ** argv) {
    int n = 0, nErrors = 0;
    for (int i = 1; i < argc; i++) {
        string arg = argv[i];
        if (arg[0] == '-') {
            if (arg == "-r")
                recursive = true;
            else if (arg == "-dry")
                dry = true;
            else {
                cout << "unknown option: " << arg << endl;
                nErrors++;
            }
        }
        else {
            size_t processedChars = 0;
            int number = std::stoi(arg, &processedChars, 8);
            if (processedChars != arg.length()) {
                cout << "bad permission: " << arg << endl;
                nErrors++;
            }
            else if (number >= 0777) {
                cout << "permission must be from 000 to 777: " << arg << endl;
                nErrors++;
            }
            if (n==0)
                beforePerm = number;
            else if (n==1)
                afterPerm = number;
            else {
                cout << "must be only two permissions: " << arg << endl;
                nErrors++;
            }
            n++;
        }
    }
    if (n<2) {
        cout << "must be two permissions" << endl;
        nErrors++;
    }
    return nErrors == 0;
}

int main(int argc,  char**argv) {
    if (argc == 1) {
        help();
        return 0;
    }
    if (!processParameters(argc,argv)) {
        cout << "---------------" <<endl;
        help();
        return 1;
    }
    listDirectory(".");
    return EXIT_SUCCESS;
}
