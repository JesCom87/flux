// token.flux — token types and token struct
module core.token

type TokenKind =
  | Identifier
  | Number
  | String
  | Keyword
  | Operator
  | Punct
  | EOF

record Token {
  kind: TokenKind
  lexeme: string
  line: int
  col: int
}

const KEYWORDS = ["let","fn","return","if","else","while","true","false"]
const OPERATORS = ["==","!=", "<=" ,">=","&&","||","+","-","*","/","%","<",">","!","="]
const PUNCT = [",",";","(",")","{","}"]
