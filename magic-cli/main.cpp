#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

Function *createFunc(Module *Mod, IRBuilder<> &Builder, StringRef Name) {
  FunctionType *FuncType = FunctionType::get(Builder.getInt32Ty(), false);
  Function *Func = Function::Create(FuncType, Function::ExternalLinkage, Name, Mod);
  return Func;
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

int main(int argc, char *argv[]) {
  LLVMContext Ctx;
  Module ModuleOb("my compiler", Ctx);
  IRBuilder<> Builder(Ctx);
  Function *Func = createFunc(&ModuleOb, Builder, "foo");
  createGlob(&ModuleOb, Builder, "g_x");
  BasicBlock *Entry = createBB(Ctx, Func, "entry");
  Builder.SetInsertPoint(Entry);
  Builder.CreateRet(Builder.getInt32(0));
  verifyFunction(*Func);
  ModuleOb.print(outs(), nullptr);
  return 0;
}
