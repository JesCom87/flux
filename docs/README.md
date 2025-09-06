<<<<<<< HEAD
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

===================================================

# ∆       Unity
# ∆∆      Duality 
# ∆∆∆     Trinity 

# • Eminence 

## Modus Operandi Equilibrium 
    Church
Maker=God|Devil=Christianity|Judaism|Islam
Satisfy=Measure|Weight 

## Worldwide Services Council 
    Government 
Overseer=Democratic|Republican=Constitutional|Federal|Presidential 
Model=Structure|Properties<>Function=Behavior|Change 

## Tailor Tomorrow Today 
    Business 
Leader=Management|Administration=Accounting|Finance|Economics 
Work=Matter|Energy=Dimension|Motion|Exertion 

# =================

# • Synergy 

## Convergence 
    Hardware
CentralBoard=MainStructure|PrimeCard|OperatingCore 

## Flux
    Experience 
Hardware|Ableware=MachineOuterFace|SmallLowBack=Architecture|Engineering=Flux=Management|Administration=FrontHighBig|UserInterFace=Proware|Software

## Intelligence 
    Software 
Money|Time=Growth=Agenda|Outfit|Business=Balance=Means|Ends

# =================

# • Optimum 

## Business 
    
    - Economics 
Leakage|Injection=Import|Export=Taxation|Spending=Investments|Savings=Input|Output 
    
    Finance 
Buy|Sell=Support|Resistant=Bullish|Bearish 
    
    Accounting 
Increase|Decrease=Revenue|Expense=Debit|Credit 
    
    Marketing 
Research|Positioning=Auditing|Scanning=Branding|Advertising 
    
    Supply Chain 
Vending|Listing=Logisting|Provisioning=Bearing|Sourcing 
    
    Human Resources 
Vacationing|Scheduling=Educating|Training=Onboarding|Recruitming

## Outfit
    
    Revolutionary 
Robotics|Defense|Aerospace|Energy
    
    Infrastructure 
Air|Water|Land
    
    Industry
Accommodation|Education|Information|Transportation|Construction 

## Agenda 
    
    Scheduling 
Calendar|Clock 
    
    Productivity 
Projects|Tasks 
    
    Collaboration 
Spaces|Rooms

=======
![Flux Pyramid](docs/flux_pyramid.svg)
>>>>>>> 2959154 (Initial Flux repo upload)
