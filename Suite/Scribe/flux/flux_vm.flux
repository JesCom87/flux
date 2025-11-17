module FluxVM

func run(op_file: string) {
    let ops = readfile(op_file).split("\n")
    var stack = []

    for op in ops {
        let parts = op.split(" ")
        let cmd = parts[0]

        if cmd == "CONST" {
            stack.append(parts[1])
        } else if cmd == "LOAD" {
            stack.append("VAR:" + parts[1])
        } else if cmd == "PRINT" {
            println(stack.pop())
        } else {
            println("Unknown OP: " + cmd)
        }
    }
}

// Example
run("output.op")
