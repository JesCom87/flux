#include "vm.hpp"
#include <iostream>
#include <stdexcep t>
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <algorithm>
#include <stdexcept>

// -------- Compile r (AST -> bytecode) --------
void Compiler::compile(const std::shared_ptr<Node>& program){
  for(auto& s : program->kids) stmt(s);
  out.emit(OP_RET);
}

void Compiler::block(const std::shared_ptr<Node>& n){
  for(auto& s : n->kids) stmt(s);
}

void Compiler::stmt(const std::shared_ptr<Node>& n){
  if(n->kind=="Let"){
    if(!n->kids.empty()){
      expr(n->kids[0]);
      auto id = out.nameId(n->tag);
      out.emit(OP_STORE); out.emit16(id);
    }
    return;
  }
  if(n->kind=="ExprStmt"){ expr(n->kids[0]); out.emit(OP_POP); return; }
  if(n->kind=="Return"){
    if(!n->kids.empty()) expr(n->kids[0]);
    out.emit(OP_RET); return;
  }
  if(n->kind=="If"){
    expr(n->kids[0]); // cond
    out.emit(OP_JMPF); size_t fix = out.code.size(); out.emitS16(0);
    block(n->kids[1]);
    if(n->kids.size()==3){
      out.emit(OP_JMP); size_t fix2 = out.code.size(); out.emitS16(0);
      int16_t rel = (int16_t)(out.code.size() - (fix+2));
      out.code[fix] = (uint8_t)(rel & 0xFF); out.code[fix+1] = (uint8_t)(rel>>8);
      block(n->kids[2]);
      int16_t rel2 = (int16_t)(out.code.size() - (fix2+2));
      out.code[fix2] = (uint8_t)(rel2 & 0xFF); out.code[fix2+1] = (uint8_t)(rel2>>8);
    } else {
      int16_t rel = (int16_t)(out.code.size() - (fix+2));
      out.code[fix] = (uint8_t)(rel & 0xFF); out.code[fix+1] = (uint8_t)(rel>>8);
    }
    return;
  }
  if(n->kind=="While"){
    size_t loopStart = out.code.size();
    expr(n->kids[0]);
    out.emit(OP_JMPF); size_t exitFix = out.code.size(); out.emitS16(0);
    block(n->kids[1]);
    out.emit(OP_JMP); int16_t back = (int16_t)((int)loopStart - (int)(out.code.size()+2));
    out.emitS16(back);
    int16_t rel = (int16_t)(out.code.size() - (exitFix+2));
    out.code[exitFix]=(uint8_t)(rel & 0xFF); out.code[exitFix+1]=(uint8_t)(rel>>8);
    return;
  }
  if(n->kind=="Block"){ block(n); return; }

  // Fallback: expression statement
  expr(n);
  out.emit(OP_POP);
}

void Compiler::expr(const std::shared_ptr<Node>& n){
  if(n->kind=="Number"){ double v=std::stod(n->value); out.emit(OP_CONST); out.emit16(out.addNum(v)); return; }
  if(n->kind=="String"){ auto id=out.addStr(n->value); out.emit(OP_CONST); out.emit16(id); return; } // strings treated as numeric index; printed via name-id
  if(n->kind=="Bool"){ out.emit(n->value=="true"?OP_TRUE:OP_FALSE); return; }
  if(n->kind=="Ident"){ auto id=out.nameId(n->value); out.emit(OP_LOAD); out.emit16(id); return; }
  if(n->kind=="Assign"){
    expr(n->kids[0]);
    auto id = out.nameId(n->tag);
    out.emit(OP_STORE); out.emit16(id); return;
  }
  if(n->kind=="Unary"){
    expr(n->kids[0]);
    if(n->tag=="-"){ out.emit(OP_CONST); out.emit16(out.addNum(-1)); out.emit(OP_MUL); }
    else if(n->tag=="!"){ /* emulate !x as x==0 */ out.emit(OP_CONST); out.emit16(out.addNum(0)); out.emit(OP_EQ); }
    return;
  }
  if(n->kind=="Binary"){
    expr(n->kids[0]); expr(n->kids[1]);
    if(n->tag=="+") out.emit(OP_ADD);
    else if(n->tag=="-") out.emit(OP_SUB);
    else if(n->tag=="*") out.emit(OP_MUL);
    else if(n->tag=="/") out.emit(OP_DIV);
    else if(n->tag=="%") out.emit(OP_MOD);
    else if(n->tag=="==") out.emit(OP_EQ);
    else if(n->tag=="!=") out.emit(OP_NE);
    else if(n->tag=="<") out.emit(OP_LT);
    else if(n->tag=="<=") out.emit(OP_LE);
    else if(n->tag==">") out.emit(OP_GT);
    else if(n->tag==">=") out.emit(OP_GE);
    else throw std::runtime_error("Unknown binary op");
    return;
  }
  if(n->kind=="Call"){
    // Only builtin 'print' supported in bootstrap: first child = callee (Ident)
    auto callee = n->kids[0];
    for(size_t k=1;k<n->kids.size();++k) expr(n->kids[k]);
    if(callee->kind!="Ident") throw std::runtime_error("call callee must be identifier");
    auto nameId = out.nameId(callee->value);
    out.emit(OP_CALL); out.emit16(nameId); out.emit((uint8_t)(n->kids.size()-1)); return;
  }
  if(n->kind=="Block"){ block(n); return; }
  throw std::runtime_error("Unhandled node kind: "+n->kind);
}

// -------- VM (bytecode interpreter) --------
void VM::run(){
  size_t ip=0;
  while(ip < chunk.code.size()){
    Op op = (Op)chunk.code[ip++];
    switch(op){
      case OP_CONST: {
        uint16_t idx = fetch16(ip);
        // If idx < knums => push number; else treat as string index offset by knums size.
        if(idx < chunk.knums.size()) push(chunk.knums[idx]);
        else push((double)idx + 100000.0); // tag strings by large offset (simplest bootstrap trick)
        break;
      }
      case OP_TRUE: push(1.0); break;
      case OP_FALSE: push(0.0); break;
      case OP_LOAD: { uint16_t id=fetch16(ip); push(globals[id]); break; }
      case OP_STORE:{ uint16_t id=fetch16(ip); globals[id]=pop(); push(globals[id]); break; }
      case OP_ADD:{ double b=pop(), a=pop(); push(a+b); break; }
      case OP_SUB:{ double b=pop(), a=pop(); push(a-b); break; }
      case OP_MUL:{ double b=pop(), a=pop(); push(a*b); break; }
      case OP_DIV:{ double b=pop(), a=pop(); push(a/b); break; }
      case OP_MOD:{ double b=pop(), a=pop(); push(fmod(a,b)); break; }
      case OP_EQ:{ double b=pop(), a=pop(); push(a==b?1.0:0.0); break; }
      case OP_NE:{ double b=pop(), a=pop(); push(a!=b?1.0:0.0); break; }
      case OP_LT:{ double b=pop(), a=pop(); push(a<b?1.0:0.0); break; }
      case OP_LE:{ double b=pop(), a=pop(); push(a<=b?1.0:0.0); break; }
      case OP_GT:{ double b=pop(), a=pop(); push(a>b?1.0:0.0); break; }
      case OP_GE:{ double b=pop(), a=pop(); push(a>=b?1.0:0.0); break; }
      case OP_NOT:{ double a=pop(); push(a==0.0?1.0:0.0); break; }
      case OP_JMP:{ int16_t off=fetchS16(ip); ip = (size_t)((int)ip + off); break; }
      case OP_JMPF:{ int16_t off=fetchS16(ip); double v=pop(); if(v==0.0) ip=(size_t)((int)ip+off); break; }
      case OP_POP:{ pop(); break; }
      case OP_CALL:{
        uint16_t nameId = fetch16(ip);
        uint8_t argc = chunk.code[ip++];
        // Only builtin 'print'
        // Pull args off stack into local vector (reverse order)
        std::vector<double> args(argc);
        for(int k=argc-1;k>=0;--k) args[k]=pop();
        // nameId->string not stored here; accept any id as 'print' if its textual name equals "print"
        // In bootstrap, assume nameId 0 == first name encountered may be 'print'. We'll map by textual compare via reverse map:
        // simplest: just print regardless (this is a bootstrap compromise)
        // If you want stricter, keep a secondary table mapping ids->names in Chunk.
        for(size_t k=0;k<args.size();++k){
          double v=args[k];
          if(v>=100000.0){
            // string index
            uint16_t idx = (uint16_t)(v - 100000.0);
            if(idx < chunk.kstrs.size()) std::cout << chunk.kstrs[idx];
            else std::cout << "<str?>";
          }else{
            std::cout << v;
          }
          if(k+1<args.size()) std::cout << " ";
        }
        std::cout << "\n";
        push(0.0); // return value
        break;
      }
      case OP_RET: return;
    }
  }
}
