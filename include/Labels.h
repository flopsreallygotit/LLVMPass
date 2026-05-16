#ifndef LABELS_H_
#define LABELS_H_

#include "llvm/IR/Value.h"
#include <string>

namespace mypass {
std::string ShortLabel(const llvm::Value *V);
std::string EscapeForDot(const std::string &s);
} // mypass

#endif // LABELS_H_