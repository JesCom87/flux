// lexer.flux — converts source text into tokens
import core.token

module core.lexer

func isAlpha(c: char): bool = (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_'
func isDigit(c: char): bool = (c >= '0' && c <= '9')
func isAlnum(c: char): bool = isAlpha(c) || isDigit(c)

record Lexer {
  src: string
  i: int
  line: int
  col: int
  tokens: Token[]
}

func new(src: string): Lexer =
  return Lexer{ src, 0, 1, 1, [] }

func peek(mut lx: Lexer): char =
  if lx.i >= len(lx.src) then return '\0'
  return lx.src[lx.i]

func advance(mut lx: Lexer): char =
  let c = peek(lx)
  if c == '\n' { lx.line += 1; lx.col = 1 } else { lx.col += 1 }
  lx.i += 1
  return c

func match(mut lx: Lexer, want: char): bool =
  if peek(lx) == want { advance(lx); return true } else { return false }

func add(mut lx: Lexer, kind: TokenKind, lex: string, line: int, col: int):
  lx.tokens.push(Token{ kind, lex, line, col })

func skipWhitespace(mut lx: Lexer):
  loop {
    let c = peek(lx)
    if c == ' ' || c == '\r' || c == '\t' || c == '\n' { advance(lx); continue }
    // line comment //
    if c == '/' && lx.i+1 < len(lx.src) && lx.src[lx.i+1] == '/' {
      while peek(lx) != '\n' && peek(lx) != '\0' { advance(lx) }
      continue
    }
    break
  }

func identifier(mut lx: Lexer, startLine: int, startCol: int):
  var s = ""
  while isAlnum(peek(lx)) { s += advance(lx) }
  let kind = (s in KEYWORDS) ? TokenKind.Keyword : TokenKind.Identifier
  add(lx, kind, s, startLine, startCol)

func number(mut lx: Lexer, startLine: int, startCol: int):
  var s = ""
  while isDigit(peek(lx)) { s += advance(lx) }
  if peek(lx) == '.' { s += advance(lx); while isDigit(peek(lx)) { s += advance(lx) } }
  add(lx, TokenKind.Number, s, startLine, startCol)

func string(mut lx: Lexer, startLine: int, startCol: int):
  advance(lx) // opening "
  var s = ""
  while peek(lx) != '"' && peek(lx) != '\0' {
    s += advance(lx)
  }
  if peek(lx) == '"' { advance(lx) }
  add(lx, TokenKind.String, s, startLine, startCol)

func twoOrOne(mut lx: Lexer, a: char, b: char): string =
  // try two-char operator like ==, !=, <=, >=, &&, ||
  let c1 = a
  let c2 = peek(lx)
  if (c1 == '=' && c2 == '=') || (c1 == '!' && c2 == '=') ||
     (c1 == '<' && c2 == '=') || (c1 == '>' && c2 == '=') ||
     (c1 == '&' && c2 == '&') || (c1 == '|' && c2 == '|') {
    advance(lx) // consume second
    return string(c1) + string(c2)
  }
  return string(c1)

func scan(mut lx: Lexer): Token[] =
  while peek(lx) != '\0' {
    skipWhitespace(lx)
    let c = peek(lx)
    if c == '\0' { break }
    let startLine = lx.line
    let startCol  = lx.col
    if isAlpha(c) { identifier(lx, startLine, startCol); continue }
    if isDigit(c) { number(lx, startLine, startCol); continue }
    if c == '"'   { string(lx, startLine, startCol); continue }

    // operators and punct
    let ch = advance(lx)
    if string(ch) in PUNCT {
      add(lx, TokenKind.Punct, string(ch), startLine, startCol); continue
    }
    var op = twoOrOne(lx, ch, peek(lx))
    if op in OPERATORS {
      add(lx, TokenKind.Operator, op, startLine, startCol); continue
    }

    // unknown -> treat as punct to keep going
    add(lx, TokenKind.Punct, string(ch), startLine, startCol)
  }
  add(lx, TokenKind.EOF, "", lx.line, lx.col)
  return lx.tokens
