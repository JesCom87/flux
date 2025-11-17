module CrossScribe

struct Symbol {
    name: string
    address: int
}

func link(modules: list<ASTNode>) -> list<ASTNode> {
    var globalSymbols = []
    var linkedModules = []

    for mod in modules {
        for node in mod.children {
            if node.type == "IDENTIFIER" {
                globalSymbols.append(Symbol(node.value, 0))
            }
        }
        linkedModules.append(mod)
    }

    return linkedModules
}
