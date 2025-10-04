#include <clang/AST/AST.h>
#include <clang/AST/ASTConsumer.h>
#include <clang/AST/DeclCXX.h>
#include <clang/ASTMatchers/ASTMatchers.h>
#include <clang/Basic/Specifiers.h>
#include <clang/Basic/Visibility.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/Tooling/Tooling.h>
#include <clang/Tooling/CommonOptionsParser.h>
#include <cstddef>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <llvm/ADT/StringRef.h>
#include <llvm/Support/CommandLine.h>
#include <llvm/Support/Error.h>
#include <llvm/Support/raw_ostream.h>
#include <memory>
#include <string>

using namespace clang::ast_matchers;

const char* location;
static llvm::cl::OptionCategory MyToolCategory("class-naming-check options");

class ClassNamingCheckConsumer : public clang::ASTConsumer
{
public:
    void HandleTranslationUnit(clang::ASTContext &Context) override
    {
        auto Matcher = cxxRecordDecl(isExpansionInMainFile(), isDefinition()).bind("class");
        auto Matches = match(Matcher, Context);
        
        std::ofstream generatedSource(std::filesystem::current_path().string() + "/../" + std::string(location).substr(0, std::string(location).find_last_of('/')) + "/test.hpp");

        generatedSource << 
            "class GeneratedClassDefinitions\n{\n";

        const auto &SM = Context.getSourceManager();

        std::string publicOut("");
        std::string privateOut("");
        std::string protectedOut(""); 

        for (const auto &Match : Matches)
        {
            const auto *Class = Match.getNodeAs<clang::CXXRecordDecl>("class");
            if (!Class || !SM.isInMainFile(Class->getLocation())) continue;


            for (auto field : Class->fields()) {

                switch (field->getAccess()) {
                    case clang::AS_public:    publicOut += "\t" + field->getType().getAsString() + " " + field->getNameAsString() + ";\n"; break;
                    case clang::AS_protected: protectedOut += "\t" + field->getType().getAsString() + " " + field->getNameAsString() + ";\n"; break;
                    case clang::AS_private:   privateOut += "\t" + field->getType().getAsString() + " " + field->getNameAsString() + ";\n"; break;
                    case clang::AS_none:      break;
                }
            }
        }

        generatedSource <<
            "private:\n" <<
            privateOut <<
            "protected:\n" <<
            protectedOut <<
            "public:\n" <<
            publicOut <<
            "\tstatic void ProcessToken(const char* _token)\n\t{\n" <<
            "\t\treturn;\n\t}\n};";
        generatedSource.close();
    }
};

class ClassNamingCheckAction : public clang::ASTFrontendAction
{
public:
    std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance &CI, llvm::StringRef File) override
    {
        (void)CI;
        (void)File;
        return std::make_unique<ClassNamingCheckConsumer>();
    }
};

class oopsBadName
{

};

int main(int argc, const char **argv)
{
    location = argv[0];
    auto ExpectedParser = clang::tooling::CommonOptionsParser::create(argc, argv, MyToolCategory, llvm::cl::OneOrMore);

    if (!ExpectedParser)
    {
        llvm::errs() << ExpectedParser.takeError();
        return 1;
    }

    auto &OptionsParser = ExpectedParser.get();
    clang::tooling::ClangTool Tool(OptionsParser.getCompilations(), OptionsParser.getSourcePathList());
    
    return Tool.run(clang::tooling::newFrontendActionFactory<ClassNamingCheckAction>().get());
}