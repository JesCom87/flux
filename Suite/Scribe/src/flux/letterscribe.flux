// letterscribe.flux  (very small Flux lex example)
scribe LetterScribe {
  fun lex_line(line) {
    // split by spaces naive
    let parts = split(line, ' ')
    return parts
  }
}
