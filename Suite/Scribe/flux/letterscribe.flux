// letterscribe.flux
// Combined Lexer + Parser for Flux

// -------------------------
// Lexer: tokenize source
// -------------------------
func tokenize(content: String) -> List[String] {
    tokens: List[String] = []
    buffer: String = ""
    
    for c in content {
        if c == ' ' || c == '\n' || c == '\t' {
            if buffer != "" {
                tokens.append(buffer)
                buffer = ""
            }
        } else {
            buffer += c
        }
    }
    
    if buffer != "" {
        tokens.append(buffer)
    }
    
    return tokens
}

// -------------------------
// Parser: build AST from tokens
// -------------------------
type ASTNode {
    value: String
    children: List[ASTNode]
}

func parse(tokens: List[String]) -> ASTNode {
    root: ASTNode = ASTNode(value: "PROGRAM", children: [])
    i: Int = 0

    while i < tokens.length {
        t: String = tokens[i]
        if t == "PRINT" {
            node: ASTNode = ASTNode(value: "PRINT", children: [])
            i += 1
            if i < tokens.length {
                arg: ASTNode = ASTNode(value: tokens[i], children: [])
                node.children.append(arg)
            }
            root.children.append(node)
        }
        // Add more parsing rules here as needed
        i += 1
    }
    
    return root
}

// -------------------------
// Example usage (for testing)
// -------------------------
func main() {
    source: String = 'PRINT "Hello, Flux!"'
    toks: List[String] = tokenize(source)
    println("TOKENS:", toks)
    
    ast: ASTNode = parse(toks)
    println("AST:", ast)
}

main()
