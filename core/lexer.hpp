# pragma once
 #include "token.hpp"
# include <vec tor>
# include <string>
# include <unord ered_set>

class Lexer {
public:
  explicit Lexer(std: :string src);
  std::vector<Token> scan();

private:
  char peek() const;
  char advance();
  void skipWS();
  Token makeIdent(int line, int col);
  Token makeNumber(int line, int col);
  Token makeString(int line, int col);
  std::string twoOrOne(char first);

  std::string s;
  size_t i{0};
  int line{1}, col{1};
};
