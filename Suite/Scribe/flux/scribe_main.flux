module ScribeMain

import LetterScribe
import CrossScribe
import NumberScribe

func main(input: string, output: string) {
    let tokens = LetterScribe.lex(input)
    let ast = LetterScribe.parse(tokens)
    let linked = CrossScribe.link([ast])
    NumberScribe.assemble(linked, output)
    PRINT "Generated " + output

