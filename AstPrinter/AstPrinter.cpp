// #include <iostream>

// #include "AstPrinter.hpp"
// #include "TokenType.hpp"



// int main()
// {
//     std::shared_ptr<Expr> expression = std::make_shared<Expr::Binary>(

//         std::make_shared<Expr::Unary>(
//             Token(TMINUS , "-" , nullptr , 1),
//             std::make_shared<Expr::Literal>(123.34)
//         ),

//         Token(TSTAR , "*" , nullptr , 1),

//         std::make_shared<Expr::Grouping>(
//             std::make_shared<Expr::Literal> (45.67)
//         )
//     );
//     std::cout<<AstPrinter{}.print(expression)<<"\n";
// }