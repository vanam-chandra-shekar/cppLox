

def defineAst(outputDir : str , baseClass : str , types : list):
        path = outputDir + "/"+ baseClass+".hpp"

        file = open(path , 'w' , encoding="UTF-8")

        file.write("#include \"Token.hpp\"\n\n")

        file.write("class "+baseClass+" {\n")
        file.write("public:\n")
        
        for type in types:
                subClassName = type.split(':')[0].strip()
                file.write("\tclass "+subClassName+";\n")

        file.write("};\n\n")

        for type in types:
                subClassName = type.split(':')[0].strip()
                fields = type.split(':')[1].strip()
                defineType(file , baseClass , subClassName , fields);


        file.close();


def defineType(file, baseClass: str, className: str, fields: str):
    # Start defining the class
    file.write("class " + baseClass + "::" + className + " : public " + baseClass + " {\n")
    file.write("public:\n")

    # Split the fields into a list
    listOfFields = fields.split(" , ")

    # Declare the fields
    for field in listOfFields:
        field_type, field_name = field.split(" ")
        file.write("\t" + field_type + " " + field_name + ";\n")

    # Constructor with parameters
    file.write("\n\t" + className + "(" + fields + ") :\n")

    # Initialize the fields in the constructor initializer list
    initializer_list = []
    for field in listOfFields:
        field_type, field_name = field.split(" ")
        initializer_list.append(field_name + "(" + field_name + ")")
    file.write("\t\t" + ", ".join(initializer_list) + " {}\n")

    # Closing the class definition
    file.write("};\n\n")



defineAst("TreeNodes" , "Expr" , [
    "Binary     : Expr left , Token op , Expr right",
    "Grouping   : Expr expression",
    "Literal    : Object value",
    "Unary      : Token op , Expr right"
]);