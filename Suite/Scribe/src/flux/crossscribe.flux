// crossscribe.flux (toy)
scribe CrossScribe {
  fun lower_ast_to_ir(ast){
    // For simple program 'print("Hello")' we output IR instructions matching OPS
    // We'll just pass through a textual op list in this bootstrap
    return ast // no-op in this simple mode
  }
}
