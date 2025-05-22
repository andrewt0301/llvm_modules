#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/raw_ostream.h"

#include <vector>

using namespace llvm;

static const std::vector <std::string> FunArgs{"a", "b"};

Function *createFunc(Module *Mod, IRBuilder<> &Builder, StringRef Name) {
  std::vector<Type *> Integers(FunArgs.size(), Builder.getInt32Ty());
  FunctionType *FuncType = FunctionType::get(Builder.getInt32Ty(), Integers, false);
  Function *Func = Function::Create(FuncType, Function::ExternalLinkage, Name, Mod);
  return Func;
}

void setFuncArgs(Function *Func, std::vector<std::string> FunArgs) {
  size_t Idx = 0;
  for (auto It = Func->arg_begin(); It != Func->arg_end(); ++It)
    It->setName(FunArgs[Idx++]);
}

BasicBlock *createBB(LLVMContext &Ctx, Function *Func, StringRef Name) {
  return BasicBlock::Create(Ctx, Name, Func); 
}

GlobalVariable *createGlob(Module *Mod, IRBuilder<> &Builder, StringRef Name) {
  Mod->getOrInsertGlobal(Name, Builder.getInt32Ty());
  GlobalVariable *Var = Mod->getNamedGlobal(Name);
  Var->setLinkage(GlobalAlias::CommonLinkage);
  Var->setAlignment(Align(4));
  return Var;
}

Value *createArith(IRBuilder<> &Builder, Value *L, Value *R) {
  return Builder.CreateMul(L, R, "multmp");
}

int main(int argc, char *argv[]) {
  LLVMContext Ctx;
  Module ModuleOb("my compiler", Ctx);
  IRBuilder<> Builder(Ctx);
  Function *Func = createFunc(&ModuleOb, Builder, "foo");
  createGlob(&ModuleOb, Builder, "g_x");
  setFuncArgs(Func, FunArgs);
  BasicBlock *Entry = createBB(Ctx, Func, "entry");
  Builder.SetInsertPoint(Entry);
  Value *Arg1 = Func->arg_begin();
  Value *Constant = Builder.getInt32(16);
  Value *Val = createArith(Builder, Arg1, Constant);
  Builder.CreateRet(Val);
  verifyFunction(*Func);
  ModuleOb.print(outs(), nullptr);
  return 0;
}
