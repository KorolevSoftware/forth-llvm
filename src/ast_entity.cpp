#include <llvm/IR/Verifier.h>

#include "ast_entity.h"

// Constructors

FunctionCallAST::FunctionCallAST(std::string name, std::variant<int, float, std::string> value):name(name), value(value){}
ControlFlowAST::ControlFlowAST(std::vector<std::shared_ptr<Instruction>> &&if_body, std::vector<std::shared_ptr<Instruction>> then_body): if_body(if_body), then_body(then_body) {}
LoopAST::LoopAST(std::vector<std::shared_ptr<Instruction>> loop_body) : loop_body(loop_body) {}
FunctionPrototypeAST::FunctionPrototypeAST(std::string name): name(name), argument(argument_type::Void) {}
FunctionPrototypeAST::FunctionPrototypeAST(std::string name, argument_type argument): name(name), argument(argument) {}
FunctionAST::FunctionAST(std::unique_ptr<FunctionPrototypeAST> proto, std::vector<std::shared_ptr<Instruction>> body) : proto(std::move(proto)), body(body) {}


// Code generations

llvm::Function* FunctionPrototypeAST::codegen(llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& llvm_module, Enviroment& variables) {
	llvm::FunctionType* func_type;

	switch (argument) {
	case FunctionPrototypeAST::argument_type::Intager:
		func_type = llvm::FunctionType::get(llvm::Type::getVoidTy(context), { llvm::Type::getInt32Ty(context) }, false);
		break;

	case FunctionPrototypeAST::argument_type::Float:
		func_type = llvm::FunctionType::get(llvm::Type::getVoidTy(context), { llvm::Type::getBFloatTy(context) }, false);
		break;

	case FunctionPrototypeAST::argument_type::String:
		func_type = llvm::FunctionType::get(llvm::Type::getVoidTy(context), { llvm::Type::getInt8PtrTy(context) }, false);

		break;

	case FunctionPrototypeAST::argument_type::Void:
		func_type = llvm::FunctionType::get(llvm::Type::getVoidTy(context), {}, false);
		break;
	}

	llvm::Function* function = llvm::Function::Create(func_type, llvm::Function::ExternalLinkage, name, llvm_module);

	return function;
}

llvm::Value* FunctionAST::codegen(llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& llvm_module, Enviroment& variables) {
	// First, check for an existing function from a previous 'extern' declaration.
	llvm::Function* function = llvm_module.getFunction(proto.get()->name);

	if (!function)
		function = proto->codegen(context, builder, llvm_module, variables);

	if (!function)
		return nullptr;

	// Create a new basic block to start insertion into.
	llvm::BasicBlock* BB = llvm::BasicBlock::Create(context, "entry", function);
	builder.SetInsertPoint(BB);

	for (auto instructions : body) {
		instructions->codegen(context, builder, llvm_module, variables);
	}

	// Insert the block terminator
	builder.CreateRetVoid();
	
	verifyFunction(*function);

	return function;

}

llvm::Value* FunctionCallAST::codegen(llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& llvm_module, Enviroment& variables) {
	llvm::Function* function = llvm_module.getFunction(name);
	if (!function)
		return nullptr;
	//return LogErrorV("Unknown function referenced");
	
	if (name == "push_integer") {
		llvm::Value* arg_value = llvm::ConstantInt::get(context, llvm::APInt(32, std::get<int>(value), true));
		return builder.CreateCall(function, { arg_value });
	}

	if (name == "push_float") {
		llvm::Value* arg_value = llvm::ConstantFP::get(context, llvm::APFloat(std::get<float>(value)));
		return builder.CreateCall(function, { arg_value });
	}

	if (name == "push_string") {
		llvm::Value* arg_value = builder.CreateGlobalStringPtr(llvm::StringRef("Hello, world!"));
		return builder.CreateCall(function, { arg_value });
	}

	return builder.CreateCall(function, {});
}

llvm::Value* ControlFlowAST::codegen(llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& llvm_module, Enviroment& variables) {
	return nullptr;
}

llvm::Value* LoopAST::codegen(llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& llvm_module, Enviroment& variables) {
	return nullptr;
}