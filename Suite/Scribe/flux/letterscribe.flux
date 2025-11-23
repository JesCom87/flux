module LetterScribe

// Very small, intentionally simple lexer+parser for demo purposes.

func read_all(path: string) -> string {
    return READFILE(path)
}

func lex(content: string) -> list<string> {
    let tokens = []
    var buf = ""
    var in_str = false
    for ch in content {
        if ch == '"' {
            buf += ch
            if in_str { tokens.push(buf); buf = ""; in_str = false; }
            else in_str = true
            continue
        }
        if in_str {
            buf += ch
            continue
        }
        if ch == ' ' or ch == '\n' or ch == '\t' or ch == '\r' {
            if buf != "" { tokens.push(buf); buf = "" }
            continue
        }
        buf += ch
    }
    if buf != "" { tokens.push(buf) }
    return tokens
}

func parse_tokens(tokens: list<string>) -> dict {
    // Very small parser that recognizes:
    // PRINT "text"
    let i = 0
    let stmts = []
    while i < tokens.length {
        let t = tokens[i]
        if t == "PRINT" {
            stmts.push({"type":"print", "value": tokens[i+1]})
            i += 2
            continue
        }
        // fallback: treat tokens as raw
        stmts.push({"type":"raw", "value": t})
        i += 1
    }
    return {"type":"module", "body": stmts}
}

func lex_and_parse_file(path: string) -> dict {
    let content = read_all(path)
    let tokens = lex(content)
    let ast = parse_tokens(tokens)
    return ast
}
