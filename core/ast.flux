// ast.flux — AST node definitions + printer
module core.ast

type NodeKind =
  | Program | Let | Fn | Return | If | While | Block
  | Call | Binary | Unary | Assign
  | Ident | Number | String | Bool

record Node {
  kind: NodeKind
  tag: string        // name/operator/etc
  value: string      // literal string
  kids: Node[]       // children
}

func Program(children: Node[]): Node = Node{NodeKind.Program, "", "", children}
func Leaf(kind: NodeKind, value: string): Node = Node{kind, "", value, []}
func Tag(kind: NodeKind, tag: string, kids: Node[]): Node = Node{kind, tag, "", kids}

func print(node: Node, indent: int = 0):
  let pad = "  " * indent
  if node.value != "" {
    println(pad + node.kind + ": " + node.value)
  } else if node.tag != "" {
    println(pad + node.kind + "(" + node.tag + ")")
  } else {
    println(pad + node.kind)
  }
  for k in node.kids { print(k, indent+1) }
