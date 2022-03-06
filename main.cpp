#include <iostream>
#include <vector>
#include <sys/stat.h>
#include <string>
#include <algorithm>
#include <filesystem>

using std::cout; using std::cin;
using std::endl; using std::string;
namespace fs = std::filesystem;

bool needChange(const string& path) {
    const char *cpath = path.c_str();
    struct stat st;
    if(stat(cpath, &st)) return false;//bad file, not try change attributes
    mode_t perm = st.st_mode;
    mode_t mask = S_IRUSR | S_IWUSR | S_IXUSR |
                  S_IRGRP | S_IWGRP | S_IXGRP |
                  S_IROTH | S_IWOTH | S_IXOTH;
    return (perm & mask) == mask;
}

void changePerm(const string& path, bool isDir) {
    const char *cpath = path.c_str();
    struct stat st;
    if(stat(cpath, &st)) return;
    mode_t perm = st.st_mode;
    if (isDir)
        chmod(cpath, perm & ~S_IWOTH);
    else
        chmod(cpath, perm & ~(S_IWOTH | S_IXUSR | S_IXGRP | S_IXOTH));
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
            changePerm(path, false);
        }
    }
    for (const string& name: dirNames) {
        string path =  pwd+ "/" + name;
        listDirectory(path);
        if (needChange(path)) {
            cout << path << endl;
            changePerm(path, true);
        }
    }
}

int main() {
    listDirectory(".");
    return EXIT_SUCCESS;
}
