# pragma once
 #include <string>
# include <mem ory>
# include <vector>

struct Node  {
  std::string kind;        // "Program","Let","Fn","Return","If","While","Block","Binary","Unary","Call","Number","String","Bool","Ident","Assign","ExprStmt","Params"
  std::string tag;         // auxiliary (e.g., name or operator)
  std::string value;       // literals / identifier
  std::vector<std::shared_ptr<Node>> kids;
  static std::shared_ptr<Node> make(std::string k){ auto n = std::make_shared<Node>(); n->kind=std::move(k); return n; }
};
