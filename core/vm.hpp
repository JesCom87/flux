# pragma once
 #include "bytecode.hpp"
# include "ast .hpp"
# include <unordered_map>
# include <s tring>

class Compiler {
public:
  explicit Compiler(Chunk& out): out (out) {}
  void compile(const std::shared_ptr<Node>& program);

private:
  Chunk& out;
  void stmt(const std::shared_ptr<Node>& n);
  void expr(const std::shared_ptr<Node>& n);
  void block(const std::shared_ptr<Node>& n);
};

class VM {
public:
  explicit VM(const Chunk& c): chunk(c) {}
  void run();

private:
  const Chunk& chunk;
  std::vector<double> stack;
  std::unordered_map<uint16_t,double> globals; // name-id -> value

  uint8_t fetch(size_t& ip) const { return chunk.code[ip++]; }
  uint16_t fetch16(size_t& ip) const { uint16_t lo=chunk.code[ip++], hi=chunk.code[ip++]; return (hi<<8)|lo; }
  int16_t fetchS16(size_t& ip) const { return (int16_t)fetch16(ip); }

  void push(double v){ stack.push_back(v); }
  double pop(){ double v=stack.back(); stack.pop_back(); return v; }
};
