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
#include <filesystem>
#include <fstream>
#include <llvm/ADT/StringRef.h>
#include <llvm/Support/CommandLine.h>
#include <llvm/Support/Error.h>
#include <llvm/Support/raw_ostream.h>
#include <memory>
#include <string>

using namespace clang::ast_matchers;

std::string publicOut("");
std::string privateOut("");
std::string protectedOut(""); 

const char* location;
static llvm::cl::OptionCategory MyToolCategory("class-naming-check options");

class ClassNamingCheckConsumer : public clang::ASTConsumer
{
public:
    void HandleTranslationUnit(clang::ASTContext &Context) override
    {
        auto Matcher = cxxRecordDecl(isExpansionInMainFile(), isDefinition(), isDerivedFrom("Component")).bind("component");
        auto Matches = match(Matcher, Context);
        // std::string(location).substr(0, std::string(location).find_last_of('/')) + 
        //printf("%s\n", (std::filesystem::current_path().string() + "/../engine/test.hpp").c_str());


        const auto &SM = Context.getSourceManager();

        for (const auto &Match : Matches)
        {
            const auto *Class = Match.getNodeAs<clang::CXXRecordDecl>("component");
            if (!Class || !SM.isInMainFile(Class->getLocation())) continue;
            
            protectedOut += "// " + Class->getNameAsString() + '\n';

            for (auto field : Class->fields()) {
                switch (field->getAccess()) {
                    case clang::AS_public:    publicOut += "\t" + field->getType().getAsString() + " " + field->getNameAsString() + ";\n"; break;
                    case clang::AS_protected: protectedOut += "\t" + field->getType().getAsString() + " " + field->getNameAsString() + ";\n"; break;
                    case clang::AS_private:   privateOut += "\t" + field->getType().getAsString() + " " + field->getNameAsString() + ";\n"; break;
                    case clang::AS_none:      break;
                }
            }
        }
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

int main(int argc, const char **argv)
{
    location = argv[0];
    auto ExpectedParser = clang::tooling::CommonOptionsParser::create(argc, argv, MyToolCategory, llvm::cl::OneOrMore);

    std::ofstream generatedSource(std::filesystem::current_path().string() + "/engine/misc/test.hpp");
    generatedSource <<
        "#include \"../include/ptc_engine.hpp\"\n"
        "#define _Bool bool\n\n"
        "class GeneratedClassDefinitions\n{\n";

    if (!ExpectedParser)
    {
        llvm::errs() << ExpectedParser.takeError();
        return 1;
    }

    auto &OptionsParser = ExpectedParser.get();
    clang::tooling::ClangTool Tool(OptionsParser.getCompilations(), OptionsParser.getSourcePathList());
    
    Tool.run(clang::tooling::newFrontendActionFactory<ClassNamingCheckAction>().get());

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

    return 0;
}