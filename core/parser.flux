// parser.flux — Pratt parser for the LANGUAGE.md grammar
import core.token
import core.ast

module core.parser

record Parser {
  toks: Token[]
  i: int
}

func at(mut p: Parser): Token = p.toks[p.i]
func eat(mut p: Parser): Token { let t = p.toks[p.i]; p.i += 1; return t }
func match(mut p: Parser, kind: TokenKind, lex: string = ""): bool =
  let t = at(p)
  if t.kind != kind { return false }
  if lex != "" && t.lexeme != lex { return false }
  p.i += 1; return true
func expect(mut p: Parser, kind: TokenKind, lex: string = ""): Token =
  let t = at(p)
  if t.kind != kind || (lex != "" && t.lexeme != lex) {
    panic("Parse error at line "+t.line+": expected "+kind+" "+lex+" got "+t.lexeme)
  }
  p.i += 1; return t

// Precedence
func prec(op: string): int =
  if op == "||" { return 1 }
  if op == "&&" { return 2 }
  if op == "==" || op == "!=" { return 3 }
  if op == "<" || op == "<=" || op == ">" || op == ">=" { return 4 }
  if op == "+" || op == "-" { return 5 }
  if op == "*" || op == "/" || op == "%" { return 6 }
  return 0

func parse(mut p: Parser): Node =
  var nodes: Node[] = []
  while at(p).kind != TokenKind.EOF {
    nodes.push(parseStmt(p))
  }
  return Program(nodes)

func parseStmt(mut p: Parser): Node =
  let t = at(p)
  if t.kind == TokenKind.Keyword && t.lexeme == "let" { return parseLet(p) }
  if t.kind == TokenKind.Keyword && t.lexeme == "fn"  { return parseFn(p) }
  if t.kind == TokenKind.Keyword && t.lexeme == "return" { return parseReturn(p) }
  if t.kind == TokenKind.Keyword && t.lexeme == "if" { return parseIf(p) }
  if t.kind == TokenKind.Keyword && t.lexeme == "while" { return parseWhile(p) }
  // ExprStmt
  let e = parseExpr(p)
  expect(p, TokenKind.Punct, ";")
  return Tag(NodeKind.Block, "expr;", [e])

func parseLet(mut p: Parser): Node =
  expect(p, TokenKind.Keyword, "let")
  let name = expect(p, TokenKind.Identifier).lexeme
  var init: Node[] = []
  if match(p, TokenKind.Operator, "=") {
    init.push(parseExpr(p))
  }
  expect(p, TokenKind.Punct, ";")
  return Tag(NodeKind.Let, name, init)

func parseFn(mut p: Parser): Node =
  expect(p, TokenKind.Keyword, "fn")
  let name = expect(p, TokenKind.Identifier).lexeme
  expect(p, TokenKind.Punct, "(")
  var params: Node[] = []
  if at(p).kind == TokenKind.Identifier {
    params.push(Leaf(NodeKind.Ident, eat(p).lexeme))
    while match(p, TokenKind.Punct, ",") {
      params.push(Leaf(NodeKind.Ident, expect(p, TokenKind.Identifier).lexeme))
    }
  }
  expect(p, TokenKind.Punct, ")")
  let body = parseBlock(p)
  // encode params as first child, body as second
  return Tag(NodeKind.Fn, name, [Tag(NodeKind.Block, "params", params), body])

func parseReturn(mut p: Parser): Node =
  expect(p, TokenKind.Keyword, "return")
  if at(p).lexeme == ";" { eat(p); return Tag(NodeKind.Return, "", []) }
  let e = parseExpr(p); expect(p, TokenKind.Punct, ";")
  return Tag(NodeKind.Return, "", [e])

func parseIf(mut p: Parser): Node =
  expect(p, TokenKind.Keyword, "if")
  expect(p, TokenKind.Punct, "(")
  let cond = parseExpr(p)
  expect(p, TokenKind.Punct, ")")
  let thenB = parseBlock(p)
  var kids = [cond, thenB]
  if at(p).kind == TokenKind.Keyword && at(p).lexeme == "else" {
    eat(p)
    kids.push(parseBlock(p))
  }
  return Tag(NodeKind.If, "", kids)

func parseWhile(mut p: Parser): Node =
  expect(p, TokenKind.Keyword, "while")
  expect(p, TokenKind.Punct, "(")
  let cond = parseExpr(p)
  expect(p, TokenKind.Punct, ")")
  let body = parseBlock(p)
  return Tag(NodeKind.While, "", [cond, body])

func parseBlock(mut p: Parser): Node =
  expect(p, TokenKind.Punct, "{")
  var stmts: Node[] = []
  while !(at(p).kind == TokenKind.Punct && at(p).lexeme == "}") {
    stmts.push(parseStmt(p))
  }
  expect(p, TokenKind.Punct, "}")
  return Tag(NodeKind.Block, "block", stmts)

// Expressions (Pratt)
func parseExpr(mut p: Parser): Node = parseAssign(p)

func parseAssign(mut p: Parser): Node =
  let left = parseOr(p)
  if at(p).kind == TokenKind.Operator && at(p).lexeme == "=" {
    eat(p)
    let right = parseAssign(p)
    // treat as Assign with left ident as tag
    if left.kind != NodeKind.Ident { panic("Left side of assignment must be identifier") }
    return Tag(NodeKind.Assign, left.value, [right])
  }
  return left

func parseOr(mut p: Parser): Node = parseBin(p, parseAnd, ["||"])
func parseAnd(mut p: Parser): Node = parseBin(p, parseEquality, ["&&"])
func parseEquality(mut p: Parser): Node = parseBin(p, parseRel, ["==","!="])
func parseRel(mut p: Parser): Node = parseBin(p, parseAdd, ["<","<=",">",">="])
func parseAdd(mut p: Parser): Node = parseBin(p, parseMul, ["+","-"])
func parseMul(mut p: Parser): Node = parseBin(p, parseUnary, ["*","/","%"])

func parseBin(mut p: Parser, sub: func(Parser)->Node, ops: string[]): Node =
  var left = sub(p)
  while at(p).kind == TokenKind.Operator && (at(p).lexeme in ops) {
    let op = eat(p).lexeme
    let right = sub(p)
    left = Tag(NodeKind.Binary, op, [left, right])
  }
  return left

func parseUnary(mut p: Parser): Node =
  if at(p).kind == TokenKind.Operator && (at(p).lexeme == "!" || at(p).lexeme == "-") {
    let op = eat(p).lexeme
    let rhs = parseUnary(p)
    return Tag(NodeKind.Unary, op, [rhs])
  }
  return parseCall(p)

func parseCall(mut p: Parser): Node =
  var expr = parsePrimary(p)
  while at(p).kind == TokenKind.Punct && at(p).lexeme == "(" {
    eat(p)
    var args: Node[] = []
    if !(at(p).kind == TokenKind.Punct && at(p).lexeme == ")") {
      args.push(parseExpr(p))
      while at(p).kind != TokenKind.Punct || at(p).lexeme != ")" {
        expect(p, TokenKind.Punct, ",")
        args.push(parseExpr(p))
      }
    }
    expect(p, TokenKind.Punct, ")")
    // convert callee (expr) + args
    expr = Tag(NodeKind.Call, "", [expr] + args)
  }
  return expr

func parsePrimary(mut p: Parser): Node =
  let t = at(p)
  if t.kind == TokenKind.Number { eat(p); return Leaf(NodeKind.Number, t.lexeme) }
  if t.kind == TokenKind.String { eat(p); return Leaf(NodeKind.String, t.lexeme) }
  if t.kind == TokenKind.Keyword && (t.lexeme == "true" || t.lexeme == "false") {
    eat(p); return Leaf(NodeKind.Bool, t.lexeme)
  }
  if t.kind == TokenKind.Identifier { eat(p); return Leaf(NodeKind.Ident, t.lexeme) }
  if t.kind == TokenKind.Punct && t.lexeme == "(" {
    eat(p); let e = parseExpr(p); expect(p, TokenKind.Punct, ")"); return e
  }
  panic("Unexpected token: " + t.lexeme)
