module NumberScribe

// Emit very small OP instructions understood by flux_vm
// We use: CONST "str", PRINT, ADD, MUL, HALT

func emit_for_ast(ast: dict) -> list<string> {
    let out = []
    for stmt in ast["body"] {
        if stmt["type"] == "print" {
            out.push("CONST " + stmt["value"])
            out.push("PRINT")
        } else if stmt["type"] == "raw" {
            // pass-through: if token is numeric op like ADD/MUL
            if stmt["value"] == "ADD" or stmt["value"] == "MUL" {
                out.push(stmt["value"])
            } else {
                // unknown raw -> comment in OP (flux_vm may ignore unknown)
                out.push("// RAW " + stmt["value"])
            }
        }
    }
    out.push("HALT")
    return out
}

func assemble(ast: dict, out_path: string) {
    let lines = emit_for_ast(ast)
    let text = join(lines, "\n")
    WRITEFILE(out_path, text)
}
