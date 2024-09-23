import os

def define_ast(outDir , baseClass : str, subClasses : str  , includes=[]):
    subClassMetaData = parse(subClasses)

    path = os.path.join(outDir , baseClass+".hpp")

    writer = open(path , 'w' , encoding='UTF-8')
    writer.write("#pragma once\n")
    if(len(includes)):
        for i in includes:
            if(i[0] != '<'):
                writer.write(f'#include "{i}"\n')
            else:
                writer.write(f'#include {i}\n')
    writeClassDeclarations(writer , baseClass , subClassMetaData)

    writeVisitorImpl(writer , baseClass , subClassMetaData)

    writeSubClassImpl(writer , baseClass , subClassMetaData)

    writer.close()

def parse(subClasses : str):
    out = {}

    for subClass in subClasses:
        className = subClass.split(":::")[0].strip()
        fieldsList = subClass.split(":::")[1].strip().split(",")
        fieldsList = [i.strip() for i in fieldsList]

        dtype = []
        mname = []
        for field in fieldsList:
            typeName = field.split(' ')[0]
            identifier = field.split(' ')[1]
            dtype.append(typeName)
            mname.append(identifier)
        
        fields = [dtype , mname]
        out[className] = fields;
    

    return out;

def writeClassDeclarations(writer , baseClass , data):

    writer.write("#include <memory>\n")
    writer.write("#include <any>\n")
    writer.write("#include \"Token.hpp\"\n\n")

    writer.write(f'class {baseClass} {{\n')
    writer.write('public:\n')
    writer.write('\tclass Visitor;\n')

    for className in data.keys():
        writer.write(f'\tclass {className};\n')
    
    writer.write('\n')
    writer.write("\tvirtual std::any accept(Visitor& visitor) = 0;\n")
    writer.write(f'\tvirtual ~{baseClass}() = default;\n')

    writer.write("};\n\n")

def writeVisitorImpl(writer , baseClass , data):

    writer.write(f'class {baseClass}::Visitor{{\n')
    writer.write('public:\n')
    
    for className in data.keys():
        writer.write(f'\tvirtual std::any visit{className}{baseClass}(std::shared_ptr<{baseClass}::{className}> {baseClass.lower()}) = 0;\n')
    
    writer.write('\n\tvirtual ~Visitor() = default;\n')
    writer.write("};\n\n")

def writeSubClassImpl(writer , baseClass , data):

    for subClassName in data.keys():
        writer.write(f'class {baseClass}::{subClassName} : public {baseClass} , public std::enable_shared_from_this<{subClassName}> {{\n')
        writer.write("public:\n")

        constructorArgs = []
        initList = []

        typeNames = data[subClassName][0]
        names = data[subClassName][1]

        for dtype , mname in zip(typeNames , names):
            constructorArgs.append(dtype+" _"+mname)
            initList.append(f'{mname}({f'std::move(_{mname})'})')

            writer.write(f'\t{dtype} {mname};\n')

        writer.write(f'\n\t{subClassName}({", ".join(constructorArgs)})\n')
        writer.write(f'\t\t: {", ".join(initList)} '+ "{}\n")

        writer.write(f'\n\tinline std::any accept(Visitor& visitor) override {{\n')
        writer.write(f'\t\treturn visitor.visit{subClassName}{baseClass}(shared_from_this());\n')
        writer.write(f'\t}}\n')

        writer.write("};\n\n")



outDir = "./generated_ast"

if not os.path.exists(outDir):
    os.mkdir(outDir)

define_ast(outDir, "Expr", [
    "Assign   ::: Token name , std::shared_ptr<Expr> value",
    "Binary   ::: std::shared_ptr<Expr> left , Token op , std::shared_ptr<Expr> right",
    "Grouping ::: std::shared_ptr<Expr> expression",
    "Literal  ::: std::any value",
    "Unary    ::: Token op, std::shared_ptr<Expr> right",
    "Variable ::: Token name",
    "Logical  ::: std::shared_ptr<Expr> left , Token op , std::shared_ptr<Expr> right",
    "Call     ::: std::shared_ptr<Expr> callee , Token paren , std::vector<std::shared_ptr<Expr>> arguments"
],["<vector>"])

define_ast(outDir , "Stmt" , [
    "Expression     ::: std::shared_ptr<Expr> expression",
    "Print          ::: std::shared_ptr<Expr> expression",
    "Var            ::: Token name, std::shared_ptr<Expr> initializer",
    "Block          ::: std::vector<std::shared_ptr<Stmt>> statements",
    "If             ::: std::shared_ptr<Expr> expr , std::shared_ptr<Stmt> thenBranch , std::shared_ptr<Stmt> elseBranch",
    "While          ::: std::shared_ptr<Expr> condition, std::shared_ptr<Stmt> body",
    "Function       ::: Token name , std::vector<Token> params , std::vector<std::shared_ptr<Stmt>> body",
    "Return         ::: Token keyword , std::shared_ptr<Expr> value"
] , ["Expr.hpp" , "<vector>"])