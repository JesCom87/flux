module ScribeMain

import LetterScribe
import LetterScribeParser
import CrossScribe
import NumberScribe

func main(input_file: string, output_file: string) {
    let tokens = LetterScribe.lex(input_file)
    let ast = LetterScribeParser.parse(tokens)
    let linked = CrossScribe.link([ast])
    NumberScribe.assemble(ast, output_file)
}

// Example execution
main("test.flux", "output.op")
