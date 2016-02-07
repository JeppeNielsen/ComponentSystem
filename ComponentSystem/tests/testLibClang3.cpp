//
//  testLibClang3.cpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 31/01/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include "ClangUtil.hpp"

#include <clang-c/Index.h>

using namespace std;

string path = "";
string className = "";

CXChildVisitResult visitor(CXCursor cursor, CXCursor parent,
    CXClientData clientData)
{
    //
    // For now, only want to process the main file of interest...
    //
    //if ( !clang_Location_isFromMainFile(clang_getCursorLocation(cursor)) )
        //return CXChildVisit_Continue;

    string cursorSpelling = toStdString(cursor);

    CXTranslationUnit tu = clang_Cursor_getTranslationUnit(cursor);
    CXSourceRange range = clang_getCursorExtent(cursor);

    if ( cursor.kind == CXCursor_ClassDecl || cursor.kind == CXCursor_StructDecl)
    {
        if ( className == "" || className == cursorSpelling )
        {
            cout << "Found class " << cursorSpelling << endl;

            //
            // Now we'll tokenize the range, which encompasses the whole class,
            // and annotate it.
            //
            CXToken* tokens = 0;
            unsigned int numTokens;
            clang_tokenize(tu, range, &tokens, &numTokens);

            CXCursor cursors[numTokens];
            clang_annotateTokens(tu, tokens, numTokens, cursors);
    
            cout << std::left << setw(28) << "Token" << setw(28) << "Cursor" << setw(35) <<
                "Cursor Kind"  << setw(34) << "Cursor Type" << endl;
            cout << "=================================================================================" <<
                endl;
            for ( unsigned int idx=0; idx<numTokens; ++idx )
            {
                CXType type = clang_getCursorType(cursors[idx]);
                string cursorSpelling = toStdString(cursors[idx]);
                string tokenSpelling = toStdString(tokens[idx], tu);
                string typeSpelling = toStdString(type);
                cout << std::left << setw(28) << tokenSpelling << setw(28) << cursorSpelling <<
                    setw(35) << toStdString(cursors[idx].kind) << setw(34) <<  typeSpelling << endl;
            }
            if ( numTokens > 0 )
                cout << endl << endl;
        }
    }

    return CXChildVisit_Continue;
}

int main()
{
    const char* filename = "/Projects/ComponentSystem/ComponentSystem/LibCode/VelocitySystem.hpp";
  
  
  int numArgs = 5;
  char *cmdArgs[] = {
  "c++",
  "-std=c++14",
  "-stdlib=libc++",
  "-I/Projects/ComponentSystem/ComponentSystem/ComponentSystem/",
  "-I/Projects/ComponentSystem/ComponentSystem/meta/"
  };


    CXIndex index = clang_createIndex(1,1);
    
    // create Translation Unit
  CXTranslationUnit TU = clang_parseTranslationUnit(index, filename, cmdArgs, numArgs, NULL, 0, 0);
  if (TU == NULL) {
    printf("Cannot parse translation unit\n");
    return 1;
  }
  
    CXCursor startCursor = clang_getTranslationUnitCursor(TU);
    clang_visitChildren(startCursor, visitor, 0);

}