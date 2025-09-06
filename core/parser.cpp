#include "parser.hpp"
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <algorithm>
#include <stdexcept>

static bool tokEq(const Token& t, TokKind k, const char* lex){
  if(t.kind!=k) return false;
  if(!lex) return true;
  return t.lexeme==lex;
}

Parser::Parser(std::vector<Token> toks): t(std::move(toks)) {}
const Token& Parser::at() const { return t[i]; }
const Token& Parser::eat(){ return t[i++]; }

bool Parser::match(TokKind k, const char* lex){
  if(!tokEq(at(),k,lex)) return false;
  ++i; return true;
}

const Token& Parser::expect(TokKind k, const char* lex){
  if(!tokEq(at(),k,lex)){
    throw std::runtime_error("Syntax error near token '" + at().lexeme + "'");
  }
  return eat();
}

std::shared_ptr<Node> Parser::parse(){
  auto prog = Node::make("Program");
  while(at().kind!=TokKind::End){
    prog->kids.push_back(stmt());
  }
  return prog;
}

std::shared_ptr<Node> Parser::stmt(){
  if(tokEq(at(),TokKind::Keyword,"let")) return let_();
  if(tokEq(at(),TokKind::Keyword,"fn"))  return fn_();
  if(tokEq(at(),TokKind::Keyword,"return")) return ret_();
  if(tokEq(at(),TokKind::Keyword,"if"))  return if_();
  if(tokEq(at(),TokKind::Keyword,"while")) return while_();
  auto e = expr(); expect(TokKind::Punct,";");
  auto n = Node::make("ExprStmt"); n->kids={e}; return n;
}

std::shared_ptr<Node> Parser::let_(){
  expect(TokKind::Keyword,"let");
  auto name = expect(TokKind::Identifier);
  auto n = Node::make("Let"); n->tag=name.lexeme;
  if(match(TokKind::Operator,"=")){
    n->kids.push_back(expr());
  }
  expect(TokKind::Punct,";");
  return n;
}

std::shared_ptr<Node> Parser::fn_(){
  expect(TokKind::Keyword,"fn");
  auto name = expect(TokKind::Identifier);
  expect(TokKind::Punct,"(");
  auto params = Node::make("Params");
  if(at().kind==TokKind::Identifier){
    params->kids.push_back([&]{ auto k=Node::make("Ident"); k->value=eat().lexeme; return k; }());
    while(match(TokKind::Punct,",")){
      auto id = expect(TokKind::Identifier);
      auto k=Node::make("Ident"); k->value=id.lexeme; params->kids.push_back(k);
    }
  }
  expect(TokKind::Punct,")");
  auto body = block();
  auto fn = Node::make("Fn"); fn->tag=name.lexeme; fn->kids={params,body}; return fn;
}

std::shared_ptr<Node> Parser::ret_(){
  expect(TokKind::Keyword,"return");
  if(tokEq(at(),TokKind::Punct,";")){ eat(); auto n=Node::make("Return"); return n; }
  auto e = expr(); expect(TokKind::Punct,";");
  auto n=Node::make("Return"); n->kids={e}; return n;
}

std::shared_ptr<Node> Parser::if_(){
  expect(TokKind::Keyword,"if");
  expect(TokKind::Punct,"("); auto cond=expr(); expect(TokKind::Punct,")");
  auto thenB = block();
  auto n = Node::make("If"); n->kids={cond,thenB};
  if(tokEq(at(),TokKind::Keyword,"else")){
    eat(); n->kids.push_back(block());
  }
  return n;
}

std::shared_ptr<Node> Parser::while_(){
  expect(TokKind::Keyword,"while");
  expect(TokKind::Punct,"("); auto cond=expr(); expect(TokKind::Punct,")");
  auto body = block();
  auto n = Node::make("While"); n->kids={cond,body}; return n;
}

std::shared_ptr<Node> Parser::block(){
  expect(TokKind::Punct,"{");
  auto b = Node::make("Block");
  while(!tokEq(at(),TokKind::Punct,"}")){
    b->kids.push_back(stmt());
  }
  expect(TokKind::Punct,"}");
  return b;
}

// Expressions (Pratt-ish)
std::shared_ptr<Node> Parser::expr(){ return assign(); }

std::shared_ptr<Node> Parser::assign(){
  auto left = logic_or();
  if(tokEq(at(),TokKind::Operator,"=")){
    eat();
    auto right = assign();
    if(left->kind!="Ident") throw std::runtime_error("Left of '=' must be identifier");
    auto n = Node::make("Assign"); n->tag=left->value; n->kids={right}; return n;
  }
  return left;
}

static bool inOps(const std::vector<std::string>& ops, const std::string& s){
  for(auto& o:ops) if(o==s) return true;
  return false;
}

std::shared_ptr<Node> Parser::bin(std::shared_ptr<Node>(Parser::*sub)(), const std::vector<std::string>& ops){
  auto left = (this->*sub)();
  while(at().kind==TokKind::Operator && inOps(ops, at().lexeme)){
    auto op = eat().lexeme;
    auto right = (this->*sub)();
    auto n = Node::make("Binary"); n->tag=op; n->kids={left,right}; left=n;
  }
  return left;
}

std::shared_ptr<Node> Parser::logic_or(){ return bin(&Parser::logic_and, {"||"}); }
std::shared_ptr<Node> Parser::logic_and(){ return bin(&Parser::equality, {"&&"}); }
std::shared_ptr<Node> Parser::equality(){ return bin(&Parser::relation, {"==","!="}); }
std::shared_ptr<Node> Parser::relation(){ return bin(&Parser::add, {"<","<=",">",">="}); }
std::shared_ptr<Node> Parser::add(){ return bin(&Parser::mul, {"+","-"}); }
std::shared_ptr<Node> Parser::mul(){ return bin(&Parser::unary, {"*","/","%"}); }

std::shared_ptr<Node> Parser::unary(){
  if(tokEq(at(),TokKind::Operator,"!") || tokEq(at(),TokKind::Operator,"-")){
    auto op = eat().lexeme; auto rhs = unary();
    auto n = Node::make("Unary"); n->tag=op; n->kids={rhs}; return n;
  }
  return call();
}

std::shared_ptr<Node> Parser::call(){
  auto expr = primary();
  while(tokEq(at(),TokKind::Punct,"(")){
    eat(); // (
    std::vector<std::shared_ptr<Node>> args;
    if(!tokEq(at(),TokKind::Punct,")")){
      args.push_back(expr = this->expr());
      while(tokEq(at(),TokKind::Punct,",")){
        eat();
        args.push_back(this->expr());
      }
    }
    expect(TokKind::Punct,")");
    auto callee = expr;
    auto n = Node::make("Call");
    n->kids.clear();
    n->kids.push_back(callee);
    n->kids.insert(n->kids.end(), args.begin(), args.end());
    expr = n;
  }
  return expr;
}

std::shared_ptr<Node> Parser::primary(){
  if(at().kind==TokKind::Number){ auto v=eat().lexeme; auto n=Node::make("Number"); n->value=v; return n; }
  if(at().kind==TokKind::String){ auto v=eat().lexeme; auto n=Node::make("String"); n->value=v; return n; }
  if(tokEq(at(),TokKind::Keyword,"true") || tokEq(at(),TokKind::Keyword,"false")){
    auto v=eat().lexeme; auto n=Node::make("Bool"); n->value=v; return n;
  }
  if(at().kind==TokKind::Identifier){ auto v=eat().lexeme; auto n=Node::make("Ident"); n->value=v; return n; }
  if(tokEq(at(),TokKind::Punct,"(")){ eat(); auto e=expr(); expect(TokKind::Punct,")"); return e; }
  throw std::runtime_error("Unexpected token: "+at().lexeme);
}
