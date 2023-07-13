#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include <sys/stat.h>
#include <dirent.h>

using namespace std;

string reverse_string(string s) {
    reverse(s.begin(), s.end());
    return s;
}

void reverse_file_contents(const string& filename,const string& outname) {
    ifstream file(filename);
    string contents="",contestLine="";
    while(getline(file,contestLine)){
        contents+=contestLine;
        if(!file.eof())contents+='\n';
    }
    string buf= reverse_string(contents);
   
    ofstream out(outname);
    if (!out.is_open()) {
        cerr << "Error: could not open file " << filename << endl;
        return;
    }
    out << buf;
    file.close();
    out.close();
}

void reverse_copy(const string& src_dir,const string& src_sub_dir) {
    string dst_name = src_sub_dir;
    dst_name=reverse_string(dst_name);

    cout<<src_dir+"/"+dst_name<<"      "<<src_sub_dir<<endl;

    if(mkdir((src_dir+"/"+dst_name).c_str(), 0777))
    {
        cerr<<"Error in mkdir \n";
        return;
    }

    DIR* dir = opendir((src_dir+"/"+src_sub_dir).c_str());
    if (dir == nullptr) {
        cerr << "Error: could not open directory " << src_dir << endl;
        return;
    }
    dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        if (entry->d_type == DT_REG) {
            string src_filename = src_dir+"/"+src_sub_dir + "/" + entry->d_name;
            string buf=entry->d_name;
            buf=reverse_string(buf);

            string dst_filename = src_dir +"/"+ dst_name + "/" + buf;

            cout<<src_filename<<"    "<<dst_filename<<endl;
            reverse_file_contents(src_filename,dst_filename);
        }
    }
    closedir(dir);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <directory>" << endl;
        return 1;
    }
    string src_dir = argv[1];

    string src_sub_dir = "";
    int pos=-1;
    for(int i=src_dir.size()-1;i>=0;i--){
        if(src_dir[i]=='/'){
            pos=i;break;
        }
        else src_sub_dir+=src_dir[i];
    }

    if(pos<0){
        cerr << "Bad directory name"<< endl;
        return 1;
    }

    src_sub_dir=reverse_string(src_sub_dir);
    src_dir.erase(pos);

    reverse_copy(src_dir, src_sub_dir);
    cout<<src_dir+'/'+src_sub_dir<<endl;
    return 0;
}