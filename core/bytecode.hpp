# pragma once
 #include <string>
# include <vec tor>
# include <unordered_map>

enum  Op : uint8_t {
  OP_CONST,   // push const pool index (u16)
  OP_TRUE,
  OP_FALSE,
  OP_LOAD,    // load var by name id (u16)
  OP_STORE,   // store var
  OP_ADD, OP_SUB, OP_MUL, OP_DIV, OP_MOD,
  OP_EQ, OP_NE, OP_LT, OP_LE, OP_GT, OP_GE,
  OP_NOT,
  OP_JMP,     // jump rel (s16)
  OP_JMPF,    // jump if false (s16)
  OP_POP,
  OP_CALL,    // call builtin (name id) with argc (u8)
  OP_RET,
};

struct Chunk {
  std::vector<uint8_t> code;
  std::vector<double>  knums;
  std::vector<std::string> kstrs;
  std::unordered_map<std::string,uint16_t> names; // simple name table

  uint16_t nameId(const std::string& s){
    auto it = names.find(s);
    if(it!=names.end()) return it->second;
    uint16_t id = (uint16_t)names.size();
    names[s]=id; return id;
  }
  uint16_t addNum(double v){ knums.push_back(v); return (uint16_t)(knums.size()-1); }
  uint16_t addStr(const std::string& s){ kstrs.push_back(s); return (uint16_t)(kstrs.size()-1); }

  void emit(uint8_t b){ code.push_back(b); }
  void emit16(uint16_t v){ code.push_back((uint8_t)(v&0xFF)); code.push_back((uint8_t)(v>>8)); }
  void emitS16(int16_t v){ emit16((uint16_t)v); }
};
