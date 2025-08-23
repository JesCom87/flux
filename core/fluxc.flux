// fluxc.flux — Flux compiler front-end (Lexer + Parser)
// Self-hosting goal: written in Flux itself

// ---------------------------
// Data structures
// ---------------------------

struct Token {
    kind: string
    lexeme: string
    line: int
    col: int
}

struct Node {
    kind: string
    tag: string
    value: string
    kids: Node[]
}

// ---------------------------
// Globals
// ---------------------------

let KEYWORDS = ["let","fn","return","if","else","while","true","false"]
let OPERATORS = ["==","!=", "<=" ,">=","&&","||","+","-","*","/","%","<",">","!","="]
let PUNCT = [",",";","(",")","{","}"]

// ---------------------------
// Lexer
// ---------------------------

fn is_alpha(c: char) -> bool {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == '_')
}

fn is_digit(c: char) -> bool {
    return (c >= '0' && c <= '9')
}

fn is_alnum(c: char) -> bool {
    return is_alpha(c) || is_digit(c)
}

struct Lexer {
    src: string
    i: int
    line: int
    col: int
}

fn Lexer_new(src: string) -> Lexer {
    return Lexer{ src, 0, 1, 1 }
}

fn peek(lex: &Lexer) -> char {
    if lex.i >= len(lex.src) { return '\0' }
    return lex.src[lex.i]
}

fn advance(lex: &Lexer) -> char {
    let c = peek(lex)
    if c == '\0' { return c }
    lex.i = lex.i + 1
    if c == '\n' {
        lex.line = lex.line + 1
        lex.col = 1
    } else {
        lex.col = lex.col + 1
    }
    return c
}

fn skip_ws(lex: &Lexer) {
    while true {
        let c = peek(lex)
        if c == ' ' || c == '\t' || c == '\r' || c == '\n' {
            advance(lex)
            continue
        }
        if c == '/' && lex.i+1 < len(lex.src) && lex.src[lex.i+1] == '/' {
            while peek(lex) != '\n' && peek(lex) != '\0' {
                advance(lex)
            }
            continue
        }
        break
    }
}

fn scan_tokens(lex: &Lexer) -> Token[] {
    let out: Token[] = []

    while true {
        skip_ws(lex)
        let c = peek(lex)
        if c == '\0' { break }

        let line = lex.line
        let col  = lex.col

        // identifiers and keywords
        if is_alpha(c) {
            let s = ""
            while is_alnum(peek(lex)) {
                s = s + advance(lex)
            }
            let kind = "Identifier"
            for kw in KEYWORDS {
                if s == kw { kind = "Keyword" }
            }
            push(out, Token{kind, s, line, col})
            continue
        }

        // numbers
        if is_digit(c) {
            let s = ""
            while is_digit(peek(lex)) {
                s = s + advance(lex)
            }
            if peek(lex) == '.' {
                s = s + advance(lex)
                while is_digit(peek(lex)) {
                    s = s + advance(lex)
                }
            }
            push(out, Token{"Number", s, line, col})
            continue
        }

        // strings
        if c == '"' {
            advance(lex)
            let s = ""
            while peek(lex) != '"' && peek(lex) != '\0' {
                s = s + advance(lex)
            }
            if peek(lex) == '"' { advance(lex) }
            push(out, Token{"String", s, line, col})
            continue
        }

        // punctuation
        let ch = advance(lex)
        for p in PUNCT {
            if ch == p {
                push(out, Token{"Punct", ch, line, col})
                goto next
            }
        }

        // operators
        let op = string(ch)
        let c2 = peek(lex)
        let pair = op + c2
        let matched = false
        for o in OPERATORS {
            if pair == o {
                advance(lex)
                push(out, Token{"Operator", pair, line, col})
                matched = true
                break
            }
        }
        if matched { goto next }

        for o in OPERATORS {
            if op == o {
                push(out, Token{"Operator", op, line, col})
                matched = true
                break
            }
        }
        if matched { goto next }

        // fallback
        push(out, Token{"Unknown", op, line, col})

        next:
    }

    push(out, Token{"EOF", "", lex.line, lex.col})
    return out
}

// ---------------------------
// Parser skeleton (to be filled out)
// ---------------------------

struct Parser {
    toks: Token[]
    i: int
}

fn Parser_new(toks: Token[]) -> Parser {
    return Parser{toks, 0}
}

// TODO: implement stmt(), expr(), block(), fn(), if(), while(), etc.

// ---------------------------
// Main
// ---------------------------

fn main(args: string[]) {
    if len(args) < 2 {
        print("usage: fluxc <file.flux>\n")
        return
    }

    let src = read_file(args[1])
    let lex = Lexer_new(src)
    let toks = scan_tokens(&lex)

    for t in toks {
        print(t.kind + " " + t.lexeme + "\n")
    }
// fluxc.flux — Flux front-end in Flux
// Features: Lexer, Parser, AST, and JSON-ish pretty print
// Goal: self-hosting Flux front-end that can be evolved to codegen/VM

// ==========================
// Minimal standard intrinsics
// (Assume these are provided by Flux runtime. If not, provide host shims.)
// ==========================
// print(s:string)
// read_file(path:string)->string
// exit(code:int)
// len(x) -> int
// push<T>(arr:&T[], v:T)  // appends v to arr
// string(c:char)->string

// ==========================
// Data structures
// ==========================
struct Token {
    kind: string
    lexeme: string
    line: int
    col: int
}

struct Node {
    kind: string
    tag: string
    value: string
    kids: Node[]
}

// Helpers to build AST nodes
fn Leaf(kind:string, value:string) -> Node {
    return Node{ kind, "", value, [] }
}
fn Tag(kind:string, tag:string, kids:Node[]) -> Node {
    return Node{ kind, tag, "", kids }
}
fn Node0(kind:string) -> Node {
    return Node{ kind, "", "", [] }
}

// ==========================
// Language tables
// ==========================
let KEYWORDS = ["let","fn","return","if","else","while","true","false"]
let OPERATORS = ["==","!=", "<=" ,">=","&&","||","+","-","*","/","%","<",">","!","="]
let PUNCT = [",",";","(",")","{","}"]

// ==========================
// Character helpers
// ==========================
fn is_alpha(c:char) -> bool {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == '_')
}
fn is_digit(c:char) -> bool {
    return (c >= '0' && c <= '9')
}
fn is_alnum(c:char) -> bool {
    return is_alpha(c) || is_digit(c)
}

// ==========================
// Lexer
// ==========================
struct Lexer {
    src: string
    i: int
    line: int
    col: int
    n: int
}

fn Lexer_new(src:string) -> Lexer {
    return Lexer{ src, 0, 1, 1, len(src) }
}

fn lx_peek(l:&Lexer) -> char {
    if l.i >= l.n { return '\0' }
    return l.src[l.i]
}
fn lx_advance(l:&Lexer) -> char {
    let c = lx_peek(l)
    if c == '\0' { return c }
    l.i = l.i + 1
    if c == '\n' {
        l.line = l.line + 1
        l.col = 1
    } else {
        l.col = l.col + 1
    }
    return c
}
fn lx_skip_ws(l:&Lexer) {
    while true {
        let c = lx_peek(l)
        if c == ' ' || c == '\t' || c == '\r' || c == '\n' {
            lx_advance(l); continue
        }
        if c == '/' && (l.i + 1) < l.n && l.src[l.i+1] == '/' {
            while lx_peek(l) != '\n' && lx_peek(l) != '\0' { lx_advance(l) }
            continue
        }
        break
    }
}

fn lx_ident(l:&Lexer, line:int, col:int) -> Token {
    let s = ""
    var t = s
    while is_alnum(lx_peek(l)) {
        t = t + lx_advance(l)
    }
    var kind = "Identifier"
    for kw in KEYWORDS { if t == kw { kind = "Keyword" } }
    return Token{ kind, t, line, col }
}

fn lx_number(l:&Lexer, line:int, col:int) -> Token {
    let s = ""
    var t = s
    while is_digit(lx_peek(l)) { t = t + lx_advance(l) }
    if lx_peek(l) == '.' {
        t = t + lx_advance(l)
        while is_digit(lx_peek(l)) { t = t + lx_advance(l) }
    }
    return Token{ "Number", t, line, col }
}

fn lx_string(l:&Lexer, line:int, col:int) -> Token {
    // consume opening "
    lx_advance(l)
    let s = ""
    var t = s
    while lx_peek(l) != '"' && lx_peek(l) != '\0' {
        t = t + lx_advance(l)
    }
    if lx_peek(l) == '"' { lx_advance(l) }
    return Token{ "String", t, line, col }
}

fn lx_two_or_one(l:&Lexer, first:string) -> string {
    let c2 = lx_peek(l)
    let pair = first + c2
    for o in OPERATORS {
        if pair == o {
            lx_advance(l)
            return pair
        }
    }
    return first
}

fn lex(src:string) -> Token[] {
    let out: Token[] = []
    let L = Lexer_new(src)

    while true {
        lx_skip_ws(&L)
        let c = lx_peek(&L)
        if c == '\0' { break }

        let line = L.line
        let col  = L.col

        if is_alpha(c) {
            push(out, lx_ident(&L, line, col))
            continue
        }
        if is_digit(c) {
            push(out, lx_number(&L, line, col))
            continue
        }
        if c == '"' {
            push(out, lx_string(&L, line, col))
            continue
        }

        // single char advance
        let ch = lx_advance(&L)

        // punctuation
        var is_p = false
        for p in PUNCT {
            if ch == p {
                push(out, Token{"Punct", string(ch), line, col})
                is_p = true
                break
            }
        }
        if is_p { continue }

        // operators
        let op = lx_two_or_one(&L, string(ch))
        var matched = false
        for o in OPERATORS {
            if op == o {
                push(out, Token{"Operator", op, line, col})
                matched = true
                break
            }
        }
        if matched { continue }

        // fallback
        push(out, Token{"Unknown", string(ch), line, col})
    }

    push(out, Token{"EOF","",L.line,L.col})
    return out
}

// ==========================
// Parser
// ==========================
struct Parser {
    toks: Token[]
    i: int
}

fn P_new(toks:Token[]) -> Parser { return Parser{ toks, 0 } }
fn P_at(p:&Parser) -> Token { return p.toks[p.i] }
fn P_eat(p:&Parser) -> Token { let t = p.toks[p.i]; p.i = p.i + 1; return t }

fn P_match(p:&Parser, kind:string, lex:string) -> bool {
    let t = P_at(p)
    if t.kind != kind { return false }
    if lex != "" && t.lexeme != lex { return false }
    p.i = p.i + 1
    return true
}
fn P_expect(p:&Parser, kind:string, lex:string) -> Token {
    let t = P_at(p)
    if t.kind != kind || (lex != "" && t.lexeme != lex) {
        print("SyntaxError: expected " + kind + " " + lex + " got " + t.kind + " " + t.lexeme + " at " + t.line + ":" + t.col + "\n")
        exit(1)
    }
    p.i = p.i + 1
    return t
}

fn parse_program(toks:Token[]) -> Node {
    let p = P_new(toks)
    let kids: Node[] = []
    while P_at(&p).kind != "EOF" {
        push(kids, parse_stmt(&p))
    }
    return Tag("Program","",kids)
}

fn parse_stmt(p:&Parser) -> Node {
    let t = P_at(p)
    if t.kind=="Keyword" && t.lexeme=="let" { return parse_let(p) }
    if t.kind=="Keyword" && t.lexeme=="fn"  { return parse_fn(p) }
    if t.kind=="Keyword" && t.lexeme=="return" { return parse_return(p) }
    if t.kind=="Keyword" && t.lexeme=="if" { return parse_if(p) }
    if t.kind=="Keyword" && t.lexeme=="while" { return parse_while(p) }

    let e = parse_expr(p)
    P_expect(p,"Punct",";")
    return Tag("ExprStmt","",[e])
}

fn parse_let(p:&Parser) -> Node {
    P_expect(p,"Keyword","let")
    let name = P_expect(p,"Identifier","").lexeme
    let kids: Node[] = []
    if P_match(p,"Operator","=") {
        push(kids, parse_expr(p))
    }
    P_expect(p,"Punct",";")
    return Tag("Let", name, kids)
}

fn parse_fn(p:&Parser) -> Node {
    P_expect(p,"Keyword","fn")
    let name = P_expect(p,"Identifier","").lexeme
    P_expect(p,"Punct","(")
    let params: Node[] = []
    if P_at(p).kind=="Identifier" {
        push(params, Leaf("Ident", P_eat(p).lexeme))
        while P_match(p,"Punct",",") {
            push(params, Leaf("Ident", P_expect(p,"Identifier","").lexeme))
        }
    }
    P_expect(p,"Punct",")")
    let body = parse_block(p)
    return Tag("Fn", name, [Tag("Params","",params), body])
}

fn parse_return(p:&Parser) -> Node {
    P_expect(p,"Keyword","return")
    if P_at(p).kind=="Punct" && P_at(p).lexeme==";" {
        P_eat(p)
        return Tag("Return","",[])
    }
    let e = parse_expr(p)
    P_expect(p,"Punct",";")
    return Tag("Return","",[e])
}

fn parse_if(p:&Parser) -> Node {
    P_expect(p,"Keyword","if")
    P_expect(p,"Punct","(")
    let cond = parse_expr(p)
    P_expect(p,"Punct",")")
    let thenB = parse_block(p)
    let kids: Node[] = [cond, thenB]
    if P_at(p).kind=="Keyword" && P_at(p).lexeme=="else" {
        P_eat(p)
        push(kids, parse_block(p))
    }
    return Tag("If","",kids)
}

fn parse_while(p:&Parser) -> Node {
    P_expect(p,"Keyword","while")
    P_expect(p,"Punct","(")
    let cond = parse_expr(p)
    P_expect(p,"Punct",")")
    let body = parse_block(p)
    return Tag("While","",[cond,body])
}

fn parse_block(p:&Parser) -> Node {
    P_expect(p,"Punct","{")
    let kids: Node[] = []
    while !(P_at(p).kind=="Punct" && P_at(p).lexeme=="}") {
        push(kids, parse_stmt(p))
    }
    P_expect(p,"Punct","}")
    return Tag("Block","",kids)
}

// --------------------------
// Expressions (Pratt)
// --------------------------
fn parse_expr(p:&Parser) -> Node { return parse_assign(p) }

fn parse_assign(p:&Parser) -> Node {
    let left = parse_or(p)
    if P_at(p).kind=="Operator" && P_at(p).lexeme=="=" {
        P_eat(p)
        let right = parse_assign(p)
        if left.kind != "Ident" {
            print("SyntaxError: left of '=' must be identifier\n")
            exit(1)
        }
        return Tag("Assign", left.value, [right])
    }
    return left
}

fn parse_or(p:&Parser) -> Node { return parse_bin(p, parse_and,   ["||"]) }
fn parse_and(p:&Parser) -> Node { return parse_bin(p, parse_eq,    ["&&"]) }
fn parse_eq(p:&Parser)  -> Node { return parse_bin(p, parse_rel,   ["==","!="]) }
fn parse_rel(p:&Parser) -> Node { return parse_bin(p, parse_add,   ["<","<=",">",">="]) }
fn parse_add(p:&Parser) -> Node { return parse_bin(p, parse_mul,   ["+","-"]) }
fn parse_mul(p:&Parser) -> Node { return parse_bin(p, parse_unary, ["*","/","%"]) }

type SubFn = fn(p:&Parser)->Node

fn parse_bin(p:&Parser, sub:SubFn, ops:string[]) -> Node {
    var left = sub(p)
    while P_at(p).kind=="Operator" && contains(ops, P_at(p).lexeme) {
        let op = P_eat(p).lexeme
        let right = sub(p)
        left = Tag("Binary", op, [left,right])
    }
    return left
}

fn contains(xs:string[], x:string) -> bool {
    for a in xs { if a == x { return true } }
    return false
}

fn parse_unary(p:&Parser) -> Node {
    if P_at(p).kind=="Operator" && (P_at(p).lexeme=="!" || P_at(p).lexeme=="-") {
        let op = P_eat(p).lexeme
        let rhs = parse_unary(p)
        return Tag("Unary", op, [rhs])
    }
    return parse_call(p)
}

fn parse_call(p:&Parser) -> Node {
    var expr = parse_primary(p)
    while P_at(p).kind=="Punct" && P_at(p).lexeme=="(" {
        P_eat(p)
        let args: Node[] = []
        if !(P_at(p).kind=="Punct" && P_at(p).lexeme==")") {
            push(args, parse_expr(p))
            while !(P_at(p).kind=="Punct" && P_at(p).lexeme==")") {
                P_expect(p,"Punct",",")
                push(args, parse_expr(p))
            }
        }
        P_expect(p,"Punct",")")
        expr = Tag("Call","",[expr] + args)
    }
    return expr
}

fn parse_primary(p:&Parser) -> Node {
    let t = P_at(p)
    if t.kind=="Number" { P_eat(p); return Leaf("Number", t.lexeme) }
    if t.kind=="String" { P_eat(p); return Leaf("String", t.lexeme) }
    if t.kind=="Keyword" && (t.lexeme=="true" || t.lexeme=="false") {
        P_eat(p); return Leaf("Bool", t.lexeme)
    }
    if t.kind=="Identifier" { P_eat(p); return Leaf("Ident", t.lexeme) }
    if t.kind=="Punct" && t.lexeme=="(" {
        P_eat(p)
        let e = parse_expr(p)
        P_expect(p,"Punct",")")
        return e
    }
    print("SyntaxError: unexpected token " + t.kind + " " + t.lexeme + " at " + t.line + ":" + t.col + "\n")
    exit(1)
    return Node0("Error")
}

// ==========================
// AST Serializer (JSON-ish)
// ==========================
fn escape(s:string)->string {
    // minimal escapes for quotes and backslashes
    let out = ""
    for i in 0..len(s) {
        let c = s[i]
        if c == '"'  { out = out + "\\\"" }
        else if c == '\\' { out = out + "\\\\" }
        else { out = out + c }
    }
    return out
}

fn node_to_json(n:&Node, indent:int)->string {
    let ind = repeat(" ", indent)
    let ind2 = repeat(" ", indent+2)

    var s = ind + "{\n"
    s = s + ind2 + "\"kind\": \"" + escape(n.kind) + "\",\n"
    s = s + ind2 + "\"tag\": \"" + escape(n.tag) + "\",\n"
    s = s + ind2 + "\"value\": \"" + escape(n.value) + "\",\n"
    s = s + ind2 + "\"kids\": ["
    if len(n.kids) == 0 {
        s = s + "]\n"
    } else {
        s = s + "\n"
        for i in 0..len(n.kids) {
            s = s + node_to_json(&n.kids[i], indent+4)
            if i+1 < len(n.kids) { s = s + ",\n" } else { s = s + "\n" }
        }
        s = s + ind2 + "]\n"
    }
    s = s + ind + "}"
    return s
}

fn repeat(s:string, n:int)->string {
    let out = ""
    var r = out
    for i in 0..n { r = r + s }
    return r
}

// ==========================
// Main
// ==========================
fn main(args:string[]) {
    if len(args) < 2 {
        print("usage: fluxc <file.flux>\n")
        exit(2)
    }
    let path = args[1]
    let src = read_file(path)
    let toks = lex(src)
    // Uncomment to see tokens:
    // for t in toks { print(t.kind + " " + t.lexeme + " @" + t.line + ":" + t.col + "\n") }

    let ast = parse_program(toks)
    let json = node_to_json(&ast, 0)
    print(json + "\n")
}}
