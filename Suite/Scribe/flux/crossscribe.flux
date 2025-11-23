module CrossScribe

func link(ast_list: list<dict>) -> dict {
    let combined = {"type":"linked", "body":[]}
    for a in ast_list { combined["body"] += a["body"] }
    return combined
}
