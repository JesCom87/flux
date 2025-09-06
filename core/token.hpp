# pragma once
 #include <string>
# include <str ing_view>

enum class TokKind {
  End, Identifier, Number, String, Keyword, Punct, Operator
};

struct Token {
  TokKind kind{};
  std::string lexeme;
  int line{1};
  int col{1};
};
