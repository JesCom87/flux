module NumberScribe

func assemble(ast: ASTNode, output: string) {
    var ops = []

    for node in ast.children {
        if node.type == "IDENTIFIER" {
            ops.append("LOAD " + node.value)
        } else if node.type == "NUMBER" {
            ops.append("CONST " + node.value)
        } else if node.type == "STRING" {
            ops.append("CONST \"" + node.value + "\"")
        } else if node.type == "SYMBOL" {
            ops.append("OP " + node.value)
        }
    }

    writefile(output, join(ops, "\n"))
}
