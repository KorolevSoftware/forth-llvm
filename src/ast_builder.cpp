#include "ast_builder.h"
#include "enviroment.h"

#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar/Reassociate.h"
#include "llvm/Transforms/Scalar/GVN.h"
#include "llvm/Transforms/Scalar/SimplifyCFG.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Target/TargetOptions.h"
#include "llvm/TargetParser/Host.h"
#include "llvm/IR/LegacyPassManager.h"

namespace { // Private
	std::vector< std::shared_ptr<Instruction>> parse_body(std::vector<Token>::iterator& iter, std::vector<Token>::iterator& iter_end);

	std::unique_ptr<FunctionPrototypeAST> parse_func_proto(std::vector<Token>::iterator& iter, std::vector<Token>::iterator& iter_end) {
		iter++; //skip func begin token = :
		Token& current_name = *iter;
		iter++; // skip func name token = name
		return std::make_unique<FunctionPrototypeAST>(std::get<std::string>(current_name.value));
	}

	std::shared_ptr<Instruction> parse_function(std::vector<Token>::iterator& iter, std::vector<Token>::iterator& iter_end) {
		std::unique_ptr<FunctionPrototypeAST> proto = parse_func_proto(iter, iter_end);
		std::vector< std::shared_ptr<Instruction>> body = parse_body(iter, iter_end);

		return std::make_shared<FunctionAST>(std::move(proto), body);
	}

	std::shared_ptr<Instruction> parse_runtime_command(std::vector<Token>::iterator& iter, std::vector<Token>::iterator& iter_end) {
		Token& current_token = *iter;
		return std::make_shared<FunctionCallAST>(std::get<std::string>(current_token.value), 1);
	}

	std::shared_ptr<Instruction> parse_runtime_push(std::vector<Token>::iterator& iter, std::vector<Token>::iterator& iter_end) {
		Token& current_token = *iter;
		return std::make_shared<FunctionCallAST>("push", current_token.value);
	}

	std::vector< std::shared_ptr<Instruction>> parse_body(std::vector<Token>::iterator& iter, std::vector<Token>::iterator& iter_end) {
		std::vector<std::shared_ptr<Instruction>> body;

		while (iter != iter_end) {
			Token& current_token = *iter;

			switch (current_token.type) {

			case token_type::FunctionBegin:
				body.emplace_back( parse_function(iter, iter_end) );
				break;

			case token_type::Number:
				body.emplace_back(parse_runtime_push(iter, iter_end));
				break;

			case token_type::Multip:
			case token_type::Drop: 
			case token_type::Dublicate:
				body.emplace_back(parse_runtime_command(iter, iter_end));
				break;

			case token_type::FunctionEnd:
				return body;
			}
			iter++;
		}
		return body;
	}
}

void ASTBuilder::build(std::vector<Token> tokens) {
	std::vector<Token>::iterator start = tokens.begin();
	std::vector<Token>::iterator end = tokens.end();

	std::vector<FunctionPrototypeAST> runtime_func{
		// PUSH
		{"push_integer", FunctionPrototypeAST::argument_type::Intager},
		{"push_string", FunctionPrototypeAST::argument_type::String},
		{"push_float", FunctionPrototypeAST::argument_type::Float},
		// Operators
		{"drop"},
		{"	"},
		{"dup"},
	};

	static llvm::ExitOnError ExitOnErr;

	auto res = parse_body(start, end);

	llvm::LLVMContext context;
	llvm::IRBuilder<> builder(context);
	llvm::Module llvm_module("forth", context);

	
	Enviroment enviroment;

	for (auto& ins : runtime_func) {
		ins.codegen(context, builder, llvm_module, enviroment);
	}

	for (auto& ins : res) {
		ins->codegen(context, builder, llvm_module, enviroment);
	}
	using namespace llvm;
	llvm_module.print(llvm::errs(), nullptr);

	InitializeAllTargetInfos();
	InitializeAllTargets();
	InitializeAllTargetMCs();
	InitializeAllAsmParsers();
	InitializeAllAsmPrinters();

	auto TargetTriple = sys::getDefaultTargetTriple();

	llvm_module.setTargetTriple(TargetTriple);

	std::string Error;
	auto Target = TargetRegistry::lookupTarget(TargetTriple, Error);

	if (!Target) {
		errs() << Error;
		return ;
	}
	auto CPU = "generic";
	auto Features = "";

	TargetOptions opt;
	auto TheTargetMachine = Target->createTargetMachine(
		TargetTriple, CPU, Features, opt, Reloc::PIC_);

	llvm_module.setDataLayout(TheTargetMachine->createDataLayout());

	auto Filename = "output.o";
	std::error_code EC;
	raw_fd_ostream dest(Filename, EC, sys::fs::OF_None);

	if (EC) {
		errs() << "Could not open file: " << EC.message();
		return;
	}

	legacy::PassManager pass;
	auto FileType = CodeGenFileType::CGFT_ObjectFile;

	if (TheTargetMachine->addPassesToEmitFile(pass, dest, nullptr, FileType)) {
		errs() << "TheTargetMachine can't emit a file of this type";
		return;
	}

	pass.run(llvm_module);
	dest.flush();

	outs() << "Wrote " << Filename << "\n";

}
