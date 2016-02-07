//
//  testLibClang2.cpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 31/01/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include <iostream>
#include <clang-c/Index.h>
#include <clang-c/Platform.h>


void printDiagnostics(CXTranslationUnit translationUnit);
void printTokenInfo(CXTranslationUnit translationUnit,CXToken currentToken);
void printCursorTokens(CXTranslationUnit translationUnit,CXCursor currentCursor);

CXChildVisitResult cursorVisitor(CXCursor cursor, CXCursor parent, CXClientData client_data);
CXChildVisitResult functionDeclVisitor(CXCursor cursor, CXCursor parent, CXClientData client_data);

int main ()
{
     const char* filename = "/Projects/ComponentSystem/ComponentSystem/LibCode/VelocitySystem.hpp";
  
  
  int numArgs = 5;
  char *cmdArgs[] = {
  "c++",
  "-std=c++14",
  "-stdlib=libc++",
  "-I/Projects/ComponentSystem/ComponentSystem/ComponentSystem/",
  "-I/Projects/ComponentSystem/ComponentSystem/meta/"
  
  //"/Projects/ComponentSystem/ComponentSystem/ComponentSystem/GameSystem.hpp"
  };



    CXIndex index = clang_createIndex(0, 0);
    
    if(index == 0){
        fprintf(stderr, "error creating index\n");
        return 1;
    }

    CXTranslationUnit translationUnit = clang_parseTranslationUnit(index, filename, cmdArgs, numArgs, NULL, 0, 0);
    //CXTranslationUnit translationUnit = clang_parseTranslationUnit(index, 0, argv, argc, 0, 0, CXTranslationUnit_None);
    
    if(translationUnit == 0){
        fprintf(stderr, "error creating translationUnit\n");
        return 1;
    }
    
    printDiagnostics(translationUnit);

    
    CXCursor rootCursor = clang_getTranslationUnitCursor(translationUnit);
    
    printCursorTokens(translationUnit,rootCursor);
    
    unsigned int res = clang_visitChildren(rootCursor, *cursorVisitor,0);
    
    clang_disposeTranslationUnit(translationUnit);
    clang_disposeIndex(index);
    return 0;
}

void printDiagnostics(CXTranslationUnit translationUnit){
    int nbDiag = clang_getNumDiagnostics(translationUnit);
    printf("There is %i diagnostics\n",nbDiag);
    
    for (unsigned int currentDiag = 0; currentDiag < nbDiag; ++currentDiag) {
        CXDiagnostic diagnotic = clang_getDiagnostic(translationUnit, currentDiag); 
        CXString errorString = clang_formatDiagnostic(diagnotic,clang_defaultDiagnosticDisplayOptions());
        fprintf(stderr, "%s\n", clang_getCString(errorString));
        clang_disposeString(errorString);
    }
}

void printTokenInfo(CXTranslationUnit translationUnit,CXToken currentToken)
{
    CXString tokenString = clang_getTokenSpelling(translationUnit, currentToken);
    CXTokenKind kind = clang_getTokenKind(currentToken);
    
    switch (kind) {
        case CXToken_Comment:
            printf("Token : %s \t| COMMENT\n", clang_getCString(tokenString));        
            break;
        case CXToken_Identifier:
            printf("Token : %s \t| IDENTIFIER\n", clang_getCString(tokenString));                    
            break;
        case CXToken_Keyword:
            printf("Token : %s \t| KEYWORD\n", clang_getCString(tokenString));                    
            break;
        case CXToken_Literal:
            printf("Token : %s \t| LITERAL\n", clang_getCString(tokenString));                    
            break;
        case CXToken_Punctuation:
            printf("Token : %s \t| PUNCTUATION\n", clang_getCString(tokenString));                    
            break;
        default:
            break;
    }    
}

void printCursorTokens(CXTranslationUnit translationUnit,CXCursor currentCursor)
{
    CXToken *tokens;
    unsigned int nbTokens;
    CXSourceRange srcRange;
    
    srcRange = clang_getCursorExtent(currentCursor);
    
    clang_tokenize(translationUnit, srcRange, &tokens, &nbTokens);
    
    for (int i = 0; i < nbTokens; ++i)
    {
        CXToken currentToken = tokens[i];
        
        printTokenInfo(translationUnit,currentToken);
    }
    
    clang_disposeTokens(translationUnit,tokens,nbTokens);
}

CXChildVisitResult cursorVisitor(CXCursor cursor, CXCursor parent, CXClientData client_data){
    
    CXCursorKind kind = clang_getCursorKind(cursor);
    CXString name = clang_getCursorSpelling(cursor);
    if (kind == CXCursor_FunctionDecl || kind == CXCursor_ObjCInstanceMethodDecl)
    {
        printf("method '%s'\n",clang_getCString(name));

        // visit method childs
        int nbParams = 0;
        clang_visitChildren(cursor, *functionDeclVisitor,&nbParams);
        
        printf("nb Params : %i'\n",nbParams);
        
        
        CXSourceLocation location = clang_getCursorLocation(cursor);   
        
        CXString filename;
        unsigned int line, column;
        
        clang_getPresumedLocation(location, &filename, &line, &column);
        
        printf("source location : %s, (%i,%i)\n",clang_getCString(filename),line,column);
        return CXChildVisit_Continue;
    }
    //printf("cursor '%s' -> %i\n",clang_getCString(name),kind);
    return CXChildVisit_Recurse;
}

CXChildVisitResult functionDeclVisitor(CXCursor cursor, CXCursor parent, CXClientData client_data){
    CXCursorKind kind = clang_getCursorKind(cursor);
    CXType type = clang_getCursorType(cursor);
    
    if (kind == CXCursor_ParmDecl){
        CXString name = clang_getCursorSpelling(cursor);
        printf("\tparameter: '%s' of type '%i'\n",clang_getCString(name),type.kind);
        int *nbParams = (int *)client_data;
        (*nbParams)++;
    }

    return CXChildVisit_Continue;
    
}