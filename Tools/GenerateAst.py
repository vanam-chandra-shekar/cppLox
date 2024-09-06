import os

def define_ast(outDir , baseClass : str, subClasses : str ):
    subClassMetaData = parse(subClasses)

    path = os.path.join(outDir , baseClass+".hpp")

    writer = open(path , 'w' , encoding='UTF-8')

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

    writer.write("#pragma once\n")
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
    "Binary   ::: std::shared_ptr<Expr> left , Token op , std::shared_ptr<Expr> right",
    "Grouping ::: std::shared_ptr<Expr> expression",
    "Literal  ::: Object value",
    "Unary    ::: Token op, std::shared_ptr<Expr> right"
])