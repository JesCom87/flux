#include "lexer.hpp"
#include <cctype>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <algorithm>
#include <stdexcept>

static const std::unordered_set<std::string> KW = {
  "let","fn","return","if","else","while","true","false"
};

static bool isIdentStart(char c){ return std::isalpha((unsigned char)c) || c=='_'; }
static bool isIdentCont(char c){ return std::isalnum((unsigned char)c) || c=='_'; }

Lexer::Lexer(std::string src): s(std::move(src)) {}

char Lexer::peek() const { return i>=s.size()? '\0' : s[i]; }

char Lexer::advance(){
  char c = peek();
  if(c=='\0') return c;
  ++i;
  if(c=='\n'){ ++line; col=1; } else { ++col; }
  return c;
}

void Lexer::skipWS(){
  while(true){
    char c=peek();
    if(c==' '||c=='\t'||c=='\r'||c=='\n'){ advance(); continue; }
    if(c=='/' && i+1<s.size() && s[i+1]=='/'){
      while(peek()!='\n' && peek()!='\0') advance();
      continue;
    }
    break;
  }
}

Token Lexer::makeIdent(int L,int C){
  std::string t;
  while(isIdentCont(peek())) t.push_back(advance());
  Token tok;
  tok.kind = KW.count(t)? TokKind::Keyword : TokKind::Identifier;
  tok.lexeme = std::move(t);
  tok.line=L; tok.col=C; return tok;
}

Token Lexer::makeNumber(int L,int C){
  std::string t;
  while(std::isdigit((unsigned char)peek())) t.push_back(advance());
  if(peek()=='.'){ t.push_back(advance());
    while(std::isdigit((unsigned char)peek())) t.push_back(advance());
  }
  return Token{TokKind::Number,std::move(t),L,C};
}

Token Lexer::makeString(int L,int C){
  advance(); // "
  std::string t;
  while(peek()!='"' && peek()!='\0') t.push_back(advance());
  if(peek()=='"') advance();
  return Token{TokKind::String,std::move(t),L,C};
}

std::string Lexer::twoOrOne(char f){
  char c2 = peek();
  std::string pair; pair.push_back(f); pair.push_back(c2);
  if(pair=="=="||pair=="!="||pair=="<="||pair==">="||pair=="&&"||pair=="||"){
    advance(); return pair;
  }
  return std::string(1,f);
}

std::vector<Token> Lexer::scan(){
  std::vector<Token> out;
  while(true){
    skipWS();
    char c=peek();
    if(c=='\0') break;
    int L=line,C=col;
    if(isIdentStart(c)){ out.push_back(makeIdent(L,C)); continue; }
    if(std::isdigit((unsigned char)c)){ out.push_back(makeNumber(L,C)); continue; }
    if(c=='"'){ out.push_back(makeString(L,C)); continue; }
    char ch=advance();
    if(std::string(",;(){}").find(ch)!=std::string::npos){
      out.push_back(Token{TokKind::Punct,std::string(1,ch),L,C}); continue;
    }
    auto op = twoOrOne(ch);
    static const std::unordered_set<std::string> OPS={
      "==","!=", "<=",">=","&&","||","+","-","*","/","%","<",">","!","="
    };
    if(OPS.count(op)){ out.push_back(Token{TokKind::Operator,op,L,C}); continue; }
    out.push_back(Token{TokKind::Punct,std::string(1,ch),L,C});
  }
  out.push_back(Token{TokKind::End,"",line,col});
  return out;
}
