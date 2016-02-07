//
//  testLibLoading.cpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 24/01/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include <iostream>
#include <dlfcn.h>
#include <vector>
#include <string>
#include <stdio.h>
#include <iostream>
#include "Vector2.hpp"
#include "ScriptEngine.hpp"

bool my_popen (const std::string& cmd,std::vector<std::string>& out ) {
    FILE*           fp;
    const int       SIZEBUF = 1234;
    char            buf [SIZEBUF];
    out = std::vector<std::string> ();
    if ((fp = popen(cmd.c_str (), "r")) == NULL) {
        return false;
    }
    std::string  cur_string = "";
    while (fgets(buf, sizeof (buf), fp)) {
        cur_string += buf;
    }
    out.push_back (cur_string.substr (0, cur_string.size () - 1));
    pclose(fp);
    return true;
}


int main_load_lib() {

/*
{
    std::vector<std::string> output;
    my_popen(
        "/Users/Jeppe/Downloads/clang+llvm-3.7.0-x86_64-apple-darwin/bin/clang++ "
        "-dynamiclib "
        "-std=c++14 -stdlib=libc++ "
        //"-v "
        " /Projects/ComponentSystem/testLibrary/testLibrary/Vector2.cpp "
        " -o engine.so"
    , output);
    
    for (auto o : output) {
        std::cout << o << std::endl;
    }
}
*/

{
    std::cout << "Compiling script code...."<< std::endl;
    std::vector<std::string> output;
    my_popen(
        "/Users/Jeppe/Downloads/clang+llvm-3.7.0-x86_64-apple-darwin/bin/clang++ "
        "-dynamiclib "
        "-std=c++14 -stdlib=libc++ "
        //"-v "
        " /Projects/ComponentSystem/testLibrary/testLibrary/hello.cpp "
        " -o testLibrary.so"
        " engine.so"
    , output);
    
    if (output.empty() || (output.size()>0 && output[0].size()==0)) {
        std::cout << "Compilation succes.."<< std::endl;
    } else {
        for (auto o : output) {
            std::cout << o << std::endl;
        }
        return -1;
    }
}

    // /Users/Jeppe/Downloads/clang+llvm-3.7.0-x86_64-apple-darwin/bin/clang++

    //Vector2 vector { 3,4 };
    //float length = vector.Length();
    //std::cout << "Length : "<< length << std::endl;

    using std::cout;
    using std::cerr;
    cout << "C++ dlopen demo\n\n";
    // open the library
    cout << "Opening testLibrary...\n";
    //void* handle = dlopen("/Projects/ComponentSystem/testLibrary/Build/Build/Products/Debug/testLibrary", RTLD_LAZY);
    void* handle = dlopen("./testLibrary.so", RTLD_LAZY);
    
    if (!handle) {
        cerr << "Cannot open library: " << dlerror() << '\n';
        return 1;
    }
    
    cout << "Loading symbol hello...\n";
    typedef int (*hello_t)();
    
    dlerror();
    hello_t hello = (hello_t) dlsym(handle, "hello");
    const char *dlsym_error = dlerror();
    if (dlsym_error) {
        cerr << "Cannot load symbol 'hello': " << dlsym_error << '\n';
        dlclose(handle);
        return 1;
    }
    // use it to do the calculation
    cout << "Calling hello...\n";
    int ret = hello();
    std::cout << "Return : "<< ret<<std::endl;
    
    // close the library
    cout << "Closing library...\n";
    dlclose(handle);
    
    return 0;
}


