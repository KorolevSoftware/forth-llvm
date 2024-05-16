#pragma once
#include <variant>
#include <string>
#include <vector>
#include <memory>

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Type.h"

#include "enviroment.h"

struct Instruction { // base
	virtual llvm::Value* codegen(llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& llvm_module, Enviroment& variables) = 0;
	virtual ~Instruction() = default;
};

struct FunctionCallAST: Instruction { // forth functions and runtime
	FunctionCallAST(std::string name, std::variant <int, float, std::string> value);
	llvm::Value* codegen(llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& llvm_module, Enviroment& variables) override;
	std::string name;
	std::variant <int, float, std::string> value;
};

struct ControlFlowAST: Instruction { // if
	ControlFlowAST(std::vector <std::shared_ptr<Instruction>> &&if_body, std::vector <std::shared_ptr<Instruction>> then_body);
	llvm::Value* codegen(llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& llvm_module, Enviroment& variables) override;
	std::vector <std::shared_ptr<Instruction>> if_body;
	std::vector <std::shared_ptr<Instruction>> then_body;
};

struct LoopAST: Instruction { // for
	LoopAST(std::vector <std::shared_ptr<Instruction>> loop_body);
	llvm::Value* codegen(llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& llvm_module, Enviroment& variables) override;
	std::vector <std::shared_ptr<Instruction>> loop_body;
};

struct FunctionPrototypeAST { // func
	enum class argument_type {
		Intager,
		Float,
		String,
		Void,
	};
	FunctionPrototypeAST(std::string name);
	FunctionPrototypeAST(std::string name, argument_type argument);
	llvm::Function* codegen(llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& llvm_module, Enviroment& variables);

	std::string name;
	argument_type argument;
};

struct FunctionAST: Instruction {
	FunctionAST(std::unique_ptr<FunctionPrototypeAST> proto, std::vector <std::shared_ptr<Instruction>> body);
	llvm::Value* codegen(llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& llvm_module, Enviroment& variables) override;
	std::unique_ptr<FunctionPrototypeAST> proto;
	std::vector <std::shared_ptr<Instruction>> body;
};
