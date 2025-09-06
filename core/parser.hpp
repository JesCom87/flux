# pragma once
 #include "token.hpp"
# include "ast .hpp"
# include <vector>
# include <memor y>

class Parser {
public:
  explicit Parser(std::vecto r<Token> toks);
  std::shared_ptr<Node> parse();

private:
  const Token& at() const;
  const Token& eat();
  bool match(TokKind k, const char* lex=nullptr);
  const Token& expect(TokKind k, const char* lex=nullptr);

  std::shared_ptr<Node> stmt();
  std::shared_ptr<Node> let_();
  std::shared_ptr<Node> fn_();
  std::shared_ptr<Node> ret_();
  std::shared_ptr<Node> if_();
  std::shared_ptr<Node> while_();
  std::shared_ptr<Node> block();

  // expressions
  std::shared_ptr<Node> expr();
  std::shared_ptr<Node> assign();
  std::shared_ptr<Node> logic_or();
  std::shared_ptr<Node> logic_and();
  std::shared_ptr<Node> equality();
  std::shared_ptr<Node> relation();
  std::shared_ptr<Node> add();
  std::shared_ptr<Node> mul();
  std::shared_ptr<Node> unary();
  std::shared_ptr<Node> call();
  std::shared_ptr<Node> primary();

  std::shared_ptr<Node> bin(std::shared_ptr<Node>(Parser::*sub)(), const std::vector<std::string>& ops);

  std::vector<Token> t;
  size_t i{0};
};
