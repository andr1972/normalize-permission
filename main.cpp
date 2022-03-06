/*goal: correct Linux attributes after copying to usb drive */
#include <iostream>
//#include <vector>
#include <sys/stat.h>
#include <string>
#include <filesystem>

using std::cout; using std::cin;
using std::endl; using std::string;
using std::filesystem::directory_iterator;

char* permissions(const char *filename){
    struct stat st;
    char *modeval = (char*)malloc(sizeof(char) * 9 + 1);
    if(stat(filename, &st) == 0){
        mode_t perm = st.st_mode;
        modeval[0] = (perm & S_IRUSR) ? 'r' : '-';
        modeval[1] = (perm & S_IWUSR) ? 'w' : '-';
        modeval[2] = (perm & S_IXUSR) ? 'x' : '-';
        modeval[3] = (perm & S_IRGRP) ? 'r' : '-';
        modeval[4] = (perm & S_IWGRP) ? 'w' : '-';
        modeval[5] = (perm & S_IXGRP) ? 'x' : '-';
        modeval[6] = (perm & S_IROTH) ? 'r' : '-';
        modeval[7] = (perm & S_IWOTH) ? 'w' : '-';
        modeval[8] = (perm & S_IXOTH) ? 'x' : '-';
        modeval[9] = '\0';
        return modeval;
    }
    else{
        return "bad";
    }
}

int main() {
    string path = "..";

    for (const auto & file : directory_iterator(path))
        cout << file.path().filename().string() << "   " << file.is_directory() <<
        "  " << permissions(file.path().string().c_str()) << endl;

    return EXIT_SUCCESS;
}