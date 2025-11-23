module scribe

import std.io
import std.string

// Very small placeholder compiler.
// It does NOT compile real code.
// It only generates a tiny OP program so we can bootstrap.

func main(input_file: string, output_file: string) {
    let text = read_file(input_file)

    let result = ""
    result += "OP_PRINT \"Compiled " + basename(input_file) + "\"\n"
    result += "OP_HALT\n"

    write_file(output_file, result)
}
