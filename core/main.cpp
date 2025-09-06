#include "lexer.hpp"
#include "parser.hpp"
#include "vm.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <algorithm>
#include <stdexcept>

i nt  main(int argc, char** argv){
  if(argc<2){ std::cerr<<"usage: fluxc <file.fl ux>\n"; return 1; }
   std::ifstream in(argv[1]);
  i f(!in){ std::cerr<<"cannot open "<<argv[1]<<"\n"; return 1; }
  std::stringstream buf; buf << in.rdbuf();
  std::string src = buf.str();

  try{
    auto toks = Lexer(src).scan();
    Chunk chunk;
    auto ast = Parser(std::move(toks)).parse();
    Compiler comp(chunk);
    comp.compile(ast);
    VM vm(chunk);
    vm.run();
  }catch(const std::exception& e){
    std::cerr<<"Flux error: "<<e.what()<<"\n"; return 2;
  }
  return 0;
}
