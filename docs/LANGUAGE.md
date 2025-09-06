# Flux (bootstrap subset)

## Lexical
- Line comments: `// ...`
- Tokens: identifiers `[A-Za-z_][A-Za-z0-9_]*`, numbers `123` or `123.45`, strings `"text"`.
- Punct: `, ; ( ) { }`
- Operators: `= == + - * / % < <= > >= ! && ||`

## Grammar (simplified)
Program    := Stmt*
Stmt       := "let" IDENT ("=" Expr)? ";" 
            | "fn" IDENT "(" ParamList? ")" Block
            | "return" Expr? ";"
            | "if" "(" Expr ")" Block ("else" Block)?
            | "while" "(" Expr ")" Block
            | Expr ";"
ParamList  := IDENT ("," IDENT)*
Block      := "{" Stmt* "}"
Expr       := assignment
assignment := IDENT "=" assignment | logic_or
logic_or   := logic_and ("||" logic_and)*
logic_and  := equality ("&&" equality)*
equality   := relation (("=="|"!=") relation)*
relation   := add (("<"|"<="|">"|">=") add)*
add        := mul (("+"|"-") mul)*
mul        := unary (("*"|"/"|"%") unary)*
unary      := ("!"|"-") unary | call
call       := primary ("(" ArgList? ")")*
ArgList    := Expr ("," Expr)*
primary    := NUMBER | STRING | "true" | "false" | IDENT | "(" Expr ")"

## Builtins
- `print(x, ...)` — prints arguments with spaces and newline.
