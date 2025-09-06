// fluxc.cpp — Flux lexer + parser (C++17, single-file, no deps)
// Builds an AST (JSON) for a Flux subset: let/fn/return/if/else/while,
// blocks, function calls, unary/binary ops, literals, identifiers.
// Comments: // ... to end of line.

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <algorithm>
#include <stdexcept>
#include <unordered_set>
#include <utility> // for std::move
using namespace std;

// ----- Token types -----
struct Token {
    string kind;   // "Identifier","Keyword","Number","String","Bool","Operator","Punct","EOF"
    string lexeme;
    int line;
    int col;
};

static const unordered_set<string> KEYWORDS = {
    "let","fn","return","if","else","while","true","false"
};

static const vector<string> OPERATORS = {
    "==","!=", "<=", ">=", "&&","||", "+","-","*","/","%","<",">","!","="
};

static const unordered_set<char> PUNCT = {',',';','(',')','{','}'};

// ----- Lexer -----
struct Lexer {
    string s;
    size_t i=0, n=0;
    int line=1, col=1;

    explicit Lexer(const string& src): s(src), n(src.size()) {}

    char peek() const { return i>=n ? '\0' : s[i]; }

    char advance() {
        char c = peek();
        if (c=='\0') return c;
        ++i;
        if (c=='\n') { ++line; col=1; } else { ++col; }
        return c;
    }

    void skip_ws() {
        for (;;) {
            char c = peek();
            if (c==' '||c=='\t'||c=='\r'||c=='\n') { advance(); continue; }
            // // comment
            if (c=='/' && i+1<n && s[i+1]=='/') {
                while (peek()!='\n' && peek()!='\0') advance();
                continue;
            }
            break;
        }
    }

    Token make(const string& kind, const string& lex, int ln, int cl) {
        return Token{kind, lex, ln, cl};
    }

    Token ident(int ln, int cl) {
        string out;
        while (isalnum(peek()) || peek()=='_') out.push_back(advance());
        if (KEYWORDS.count(out)) {
            if (out=="true"||out=="false") return make("Bool", out, ln, cl);
            return make("Keyword", out, ln, cl);
        }
        return make("Identifier", out, ln, cl);
    }

    Token number(int ln, int cl) {
        string out;
        while (isdigit(peek())) out.push_back(advance());
        if (peek()=='.') {
            out.push_back(advance());
            while (isdigit(peek())) out.push_back(advance());
        }
        return make("Number", out, ln, cl);
    }

    Token string_ (int ln, int cl) {
        advance(); // consume opening "
        string out;
        while (peek()!='"' && peek()!='\0') {
            char c = advance();
            if (c=='\\') { // basic escapes
                char e = peek();
                if (e=='"' || e=='\\' || e=='n' || e=='t' || e=='r') {
                    c = advance();
                    if (c=='n') out.push_back('\n');
                    else if (c=='t') out.push_back('\t');
                    else if (c=='r') out.push_back('\r');
                    else out.push_back(c);
                } else {
                    out.push_back('\\');
                }
            } else {
                out.push_back(c);
            }
        }
        if (peek()=='"') advance();
        return make("String", out, ln, cl);
    }

    string two_or_one(char first) {
        char c2 = peek();
        string pair; pair.push_back(first); pair.push_back(c2);
        if (pair=="=="||pair=="!="||pair=="<="||pair==">="||pair=="&&"||pair=="||") {
            advance();
            return pair;
        }
        return string(1, first);
    }

    vector<Token> scan() {
        vector<Token> out;
        for (;;) {
            skip_ws();
            char c = peek();
            if (c=='\0') break;
            int ln=line, cl=col;

            if (isalpha(c) || c=='_') { out.push_back(ident(ln,cl)); continue; }
            if (isdigit(c)) { out.push_back(number(ln,cl)); continue; }
            if (c=='"') { out.push_back(string_(ln,cl)); continue; }

            char ch = advance();
            if (PUNCT.count(ch)) { out.push_back(make("Punct", string(1,ch), ln, cl)); continue; }
            string op = two_or_one(ch);
            if (find(OPERATORS.begin(), OPERATORS.end(), op) != OPERATORS.end()) {
                out.push_back(make("Operator", op, ln, cl)); continue;
            }
            // Fallback: unknown char as punct
            out.push_back(make("Punct", string(1,ch), ln, cl));
        }
        out.push_back(Token{"EOF","",line,col});
        return out;
    }
};

// ----- AST -----
struct Node {
    string kind;   // e.g., "Program","Let","Fn","Params","Block","Return","If","While","Assign","Binary","Unary","Call","Number","String","Bool","Ident","ExprStmt"
    string tag;    // extra label (e.g., function or variable name)
    string value;  // literal value or identifier text for leaves
    vector<Node> kids;

    // JSON printing
    void print_json(ostream& os, int indent=0) const {
        auto ind = [&](int k){ for(int i=0;i<k;i++) os<<' '; };
        os << "{\n";
        ind(indent+2); os << "\"kind\": " << json_str(kind) << ",\n";
        ind(indent+2); os << "\"tag\": "  << json_str(tag)  << ",\n";
        ind(indent+2); os << "\"value\": "<< json_str(value)<< ",\n";
        ind(indent+2); os << "\"kids\": [";
        if (!kids.empty()) os << "\n";
        for (size_t i=0;i<kids.size();++i) {
            kids[i].print_json(os, indent+4);
            if (i+1<kids.size()) os << ",";
            os << "\n";
        }
        if (!kids.empty()) ind(indent+2);
        os << "]\n";
        ind(indent); os << "}";
    }

    static string json_str(const string& s) {
        ostringstream o;
        o << "\"";
        for (char c : s) {
            switch (c) {
                case '\\': o << "\\\\"; break;
                case '\"': o << "\\\""; break;
                case '\n': o << "\\n"; break;
                case '\r': o << "\\r"; break;
                case '\t': o << "\\t"; break;
                default: o << c; break;
            }
        }
        o << "\"";
        return o.str();
    }
};

static Node Leaf(const string& kind, const string& value) {
    return Node{kind, "", value, {}};
}
static Node Tag(const string& kind, const string& tag, vector<Node> kids = {}) {
    return Node{kind, tag, "", std::move(kids)};
}

// ----- Parser (Pratt for expressions) -----
struct Parser {
    const vector<Token>& toks;
    size_t i=0;

    explicit Parser(const vector<Token>& t): toks(t) {}

    const Token& at() const { return toks[i]; }
    const Token& eat() { return toks[i++]; }

    bool match(const string& kind, const string& lex="") {
        const Token& t = at();
        if (t.kind != kind) return false;
        if (!lex.empty() && t.lexeme != lex) return false;
        ++i; return true;
    }

    const Token& expect(const string& kind, const string& lex="") {
        const Token& t = at();
        if (t.kind!=kind || (!lex.empty() && t.lexeme!=lex)) {
            ostringstream msg;
            msg << "SyntaxError: expected " << kind;
            if (!lex.empty()) msg << " '"<<lex<<"'";
            msg << " but got " << t.kind << " '"<<t.lexeme<<"' at " << t.line << ":" << t.col;
            throw runtime_error(msg.str());
        }
        ++i; return toks[i-1];
    }

    Node parse() {
        vector<Node> nodes;
        while (at().kind != "EOF") {
            nodes.push_back(stmt());
        }
        return Node{"Program","","", std::move(nodes)};
    }

    Node stmt() {
        const Token& t = at();
        if (t.kind=="Keyword" && t.lexeme=="let")    return let_();
        if (t.kind=="Keyword" && t.lexeme=="fn")     return fn_();
        if (t.kind=="Keyword" && t.lexeme=="return") return ret_();
        if (t.kind=="Keyword" && t.lexeme=="if")     return if_();
        if (t.kind=="Keyword" && t.lexeme=="while")  return while_();
        Node e = expr();
        expect("Punct",";");
        return Tag("ExprStmt","", {e});
    }

    Node let_() {
        expect("Keyword","let");
        string name = expect("Identifier").lexeme;
        vector<Node> kids;
        if (match("Operator","=")) kids.push_back(expr());
        expect("Punct",";");
        return Tag("Let", name, std::move(kids));
    }

    Node fn_() {
        expect("Keyword","fn");
        string name = expect("Identifier").lexeme;
        expect("Punct","(");
        vector<Node> params;
        if (at().kind=="Identifier") {
            params.push_back(Leaf("Ident", eat().lexeme));
            while (match("Punct",",")) {
                params.push_back(Leaf("Ident", expect("Identifier").lexeme));
            }
        }
        expect("Punct",")");
        Node body = block();
        return Tag("Fn", name, { Tag("Params","", params), body });
    }

    Node ret_() {
        expect("Keyword","return");
        if (match("Punct",";")) return Tag("Return","",{});
        Node e = expr(); expect("Punct",";");
        return Tag("Return","",{e});
    }

    Node if_() {
        expect("Keyword","if");
        expect("Punct","(");
        Node cond = expr();
        expect("Punct",")");
        Node thenB = block();
        vector<Node> kids{cond, thenB};
        if (at().kind=="Keyword" && at().lexeme=="else") {
            eat();
            kids.push_back(block());
        }
        return Tag("If","", std::move(kids));
    }

    Node while_() {
        expect("Keyword","while");
        expect("Punct","(");
        Node cond = expr();
        expect("Punct",")");
        Node body = block();
        return Tag("While","", {cond, body});
    }

    Node block() {
        expect("Punct","{");
        vector<Node> kids;
        while (!(at().kind=="Punct" && at().lexeme=="}")) {
            kids.push_back(stmt());
        }
        expect("Punct","}");
        return Tag("Block","", std::move(kids));
    }

    // --- Expressions ---
    Node expr() { return assign(); }

    Node assign() {
        Node left = or_();
        if (at().kind=="Operator" && at().lexeme=="=") {
            eat();
            Node right = assign();
            if (left.kind!="Ident") throw runtime_error("SyntaxError: left of '=' must be identifier");
            return Tag("Assign", left.value, {right});
        }
        return left;
    }

    Node or_()        { return bin([&]{ return and_(); }, {"||"}); }
    Node and_()       { return bin([&]{ return equality(); }, {"&&"}); }
    Node equality()   { return bin([&]{ return rel(); }, {"==","!="}); }
    Node rel()        { return bin([&]{ return add(); }, {"<","<=",">",">="}); }
    Node add()        { return bin([&]{ return mul(); }, {"+","-"}); }
    Node mul()        { return bin([&]{ return unary(); }, {"*","/","%"}); }

    using SubFn = function<Node()>;
    Node bin(SubFn sub, const unordered_set<string>& ops) {
        Node left = sub();
        while (at().kind=="Operator" && ops.count(at().lexeme)) {
            string op = eat().lexeme;
            Node right = sub();
            left = Tag("Binary", op, {left, right});
        }
        return left;
    }

    Node unary() {
        if (at().kind=="Operator" && (at().lexeme=="!" || at().lexeme=="-")) {
            string op = eat().lexeme;
            Node rhs = unary();
            return Tag("Unary", op, {rhs});
        }
        return call();
    }

    Node call() {
        Node e = primary();
        while (at().kind=="Punct" && at().lexeme=="(") {
            eat(); // (
            vector<Node> args;
            if (!(at().kind=="Punct" && at().lexeme==")")) {
                args.push_back(expr());
                while (!(at().kind=="Punct" && at().lexeme==")")) {
                    expect("Punct",",");
                    args.push_back(expr());
                }
            }
            expect("Punct",")");
            vector<Node> kids; kids.push_back(e);
            kids.insert(kids.end(), args.begin(), args.end());
            e = Tag("Call","", std::move(kids));
        }
        return e;
    }

    Node primary() {
        const Token& t = at();
        if (t.kind=="Number")  { eat(); return Leaf("Number", t.lexeme); }
        if (t.kind=="String")  { eat(); return Leaf("String", t.lexeme); }
        if (t.kind=="Bool")    { eat(); return Leaf("Bool",   t.lexeme); }
        if (t.kind=="Identifier") { eat(); return Leaf("Ident",  t.lexeme); }
        if (t.kind=="Punct" && t.lexeme=="(") {
            eat();
            Node e = expr();
            expect("Punct",")");
            return e;
        }
        ostringstream msg;
        msg << "SyntaxError: unexpected " << t.kind << " '"<<t.lexeme<<"' at " << t.line << ":" << t.col;
        throw runtime_error(msg.str());
    }
};

// ----- main -----
int main(int argc, char** argv) {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    if (argc < 2) {
        cerr << "usage: fluxc <file.flux>\n";
        return 1;
    }
    // Read file
    ifstream in(argv[1]);
    if (!in) { cerr << "error: cannot open " << argv[1] << "\n"; return 1; }
    string src((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());

    try {
        Lexer lx(src);
        vector<Token> toks = lx.scan();
        Parser ps(toks);
        Node ast = ps.parse();
        ast.print_json(cout, 0);
        cout << "\n";
        return 0;
    } catch (const exception& e) {
        cerr << e.what() << "\n";
        return 2;
    }
}
