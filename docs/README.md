# Flux Language — Minimal Subset (v0.1)

## Comments
- `// line comment`

## Tokens
- identifiers: `[A-Za-z_][A-Za-z0-9_]*`
- numbers: `123`, `3.14`
- strings: `"hello"`
- keywords: `let fn return if else while true false`
- operators: `+ - * / % == != < <= > >= && || ! =`
- punctuation: `, ; ( ) { }`

## Grammar (EBNF-ish)
Program     := Stmt*
Stmt        := LetStmt | FnDecl | ReturnStmt | IfStmt | WhileStmt | ExprStmt
LetStmt     := "let" Identifier ("=" Expr)? ";"
FnDecl      := "fn" Identifier "(" ParamList? ")" Block
ParamList   := Identifier ("," Identifier)*
ReturnStmt  := "return" Expr? ";"
IfStmt      := "if" "(" Expr ")" Block ("else" Block)?
WhileStmt   := "while" "(" Expr ")" Block
ExprStmt    := Expr ";"

Expr        := Assign
Assign      := Or ( "=" Assign )?
Or          := And ( "||" And )*
And         := Equality ( "&&" Equality )*
Equality    := Rel ( ("==" | "!=") Rel )*
Rel         := Add ( ("<" | "<=" | ">" | ">=") Add )*
Add         := Mul ( ("+" | "-") Mul )*
Mul         := Unary ( ("*" | "/" | "%") Unary )*
Unary       := ("!" | "-") Unary | Call
Call        := Primary ( "(" ArgList? ")" )*
ArgList     := Expr ("," Expr)*
Primary     := Number | String | "true" | "false" | Identifier | "(" Expr ")"

Block       := "{" Stmt* "}"

# Convergence 
    Hardware
CentralBoard=MainStructure|PrimeCard|OperatingCore 

# Flux
    Experience 
Hardware|Ableware=MachineOuterFace|SmallLowBack=Architecture|Engineering=Flux=Management|Administration=FrontHighBig|UserInterFace=Proware|Software

# Intelligence 
    Software 
Money|Time=Growth=Agenda|Outfit|Business=Balance=Means|Ends
