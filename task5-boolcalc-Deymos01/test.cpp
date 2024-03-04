#include <string>
#include <gtest/gtest.h>

#include "boolexpr.h"
#include "errors.h"


TEST(BooleanExpressionString, TwoVariables_1) {
    BooleanExpression expr("x1 & x2");
    EXPECT_EQ("x1 & x2", std::string(expr));
}

TEST(BooleanExpressionString, TwoVariables_2) {
    BooleanExpression expr("x1 ^ x10");
    EXPECT_EQ("x1 ^ x10", std::string(expr));
}

TEST(BooleanExpressionString, TwoVariables_3) {
    BooleanExpression expr("(x1>x2)");
    EXPECT_EQ("x1 > x2", std::string(expr));
}

TEST(BooleanExpressionString, ThreeVariables_1) {
    BooleanExpression expr("x1&(x2vx3)");
    EXPECT_EQ("x1 & (x2 v x3)", std::string(expr));
}

TEST(BooleanExpressionString, ThreeVariables_2) {
    BooleanExpression expr("x1^x2<x5");
    EXPECT_EQ("x1 ^ x2 < x5", std::string(expr));
}

TEST(BooleanExpressionString, ThreeVariables_3) {
    BooleanExpression expr("x4v(x9vx1)");
    EXPECT_EQ("x4 v x9 v x1", std::string(expr));
}

TEST(BooleanExpressionString, AnyVariables_1) {
    BooleanExpression expr("(x1&x9)vx9+(x2=x1)");
    EXPECT_EQ("x1 & x9 v x9 + (x2 = x1)", std::string(expr));
}

TEST(BooleanExpressionString, AnyVariables_2) {
    BooleanExpression expr("x1< x2&(x1>x3)+ x1");
    EXPECT_EQ("x1 < x2 & (x1 > x3) + x1", std::string(expr));
}

TEST(BooleanExpressionString, AnyVariables_3) {
    BooleanExpression expr("x1=x1&x2+(x1|x3)");
    EXPECT_EQ("x1 = x1 & x2 + (x1 | x3)", std::string(expr));
}

TEST(BooleanExpressionConstructor, InvalidInput_1) {
    EXPECT_THROW(BooleanExpression expr("x1 &x3 v 2"), ErrorSymbol);
}

TEST(BooleanExpressionConstructor, InvalidInput_2) {
    EXPECT_THROW(BooleanExpression expr("x1 & x2 v x 9"), ErrorParameter);
}

TEST(BooleanExpressionConstructor, InvalidInput_3) {
    EXPECT_THROW(BooleanExpression expr("x1 & (x2 v x3"), ErrorBracketsClose);
}

TEST(BooleanExpressionConstructor, InvalidInput_4) {
    EXPECT_THROW(BooleanExpression expr("x2 + x1 < x3)"), ErrorBracketsOpen);
}

TEST(BooleanExpressionFindDNF, CannotFindDNF) {
    BooleanExpression expr("0");
    EXPECT_THROW(expr.dnf(), ErrorDNF);
}

TEST(BooleanExpressionFindDNF, FindDNF_1) {
    BooleanExpression expr("(~x1 & x2 +x3) &(x1&x3>x2)");
    EXPECT_EQ("~x1 & ~x2 & x3 v ~x1 & x2 & ~x3 v x1 & x2 & x3", std::string(expr.dnf()));
}

TEST(BooleanExpressionFindDNF, FindDNF_2) {
    BooleanExpression expr("x1vx2");
    EXPECT_EQ("~x1 & x2 v x1 & ~x2 v x1 & x2", std::string(expr.dnf()));
}

TEST(BooleanExpressionFindDNF, FindDNF_3) {
    BooleanExpression expr("x1>x2");
    EXPECT_EQ("~x1 & ~x2 v ~x1 & x2 v x1 & x2", std::string(expr.dnf()));
}

TEST(BooleanExpressionFindDNF, FindDNF_4) {
    BooleanExpression expr("x1+x2+x3");
    EXPECT_EQ("~x1 & ~x2 & x3 v ~x1 & x2 & ~x3 v x1 & ~x2 & ~x3 v x1 & x2 & x3", std::string(expr.dnf()));
}

TEST(BooleanExpressionFindDNF, FindDNF_5) {
    BooleanExpression expr("x1|x2^x3");
    EXPECT_EQ("x1 & x2 & ~x3", std::string(expr.dnf()));
}

TEST(BooleanExpressionFindCNF, CannotFindCNF) {
    BooleanExpression expr("1");
    EXPECT_THROW(expr.cnf(), ErrorCNF);
}

TEST(BooleanExpressionFindCNF, FindCNF_1) {
    BooleanExpression expr("(~x1 & x2 +x3) &(x1&x3>x2)");
    EXPECT_EQ("(x1 v x2 v x3) & (x1 v ~x2 v ~x3) & (~x1 v x2 v x3) & (~x1 v x2 v ~x3) & (~x1 v ~x2 v x3)",
              std::string(expr.cnf()));
}

TEST(BooleanExpressionFindCNF, FindCNF_2) {
    BooleanExpression expr("x1&x2");
    EXPECT_EQ("(x1 v x2) & (x1 v ~x2) & (~x1 v x2)", std::string(expr.cnf()));
}

TEST(BooleanExpressionFindCNF, FindCNF_3) {
    BooleanExpression expr("x1>x2");
    EXPECT_EQ("~x1 v x2", std::string(expr.cnf()));
}

TEST(BooleanExpressionFindCNF, FindCNF_4) {
    BooleanExpression expr("x1+x2+x3");
    EXPECT_EQ("(x1 v x2 v x3) & (x1 v ~x2 v ~x3) & (~x1 v x2 v ~x3) & (~x1 v ~x2 v x3)", std::string(expr.cnf()));
}

TEST(BooleanExpressionFindCNF, FindCNF_5) {
    BooleanExpression expr("x1|x2^x3");
    EXPECT_EQ(
            "(x1 v x2 v x3) & (x1 v x2 v ~x3) & (x1 v ~x2 v x3) & (x1 v ~x2 v ~x3) & (~x1 v x2 v x3) & (~x1 v x2 v ~x3) & (~x1 v ~x2 v ~x3)",
            std::string(expr.cnf()));
}

TEST(BooleanExpressionFindZhegalkin, FindZhegalkin_1) {
    BooleanExpression expr("(~x1 & x2 +x3) &(x1&x3>x2)");
    EXPECT_EQ("x3 + x2 + x1 & x3 + x1 & x2 + x1 & x2 & x3", std::string(expr.zhegalkin()));
}

TEST(BooleanExpressionFindZhegalkin, FindZhegalkin_2) {
    BooleanExpression expr("x1|x2");
    EXPECT_EQ("1 + x1 & x2", std::string(expr.zhegalkin()));
}

TEST(BooleanExpressionFindZhegalkin, FindZhegalkin_3) {
    BooleanExpression expr("x1>x2");
    EXPECT_EQ("1 + x1 + x1 & x2", std::string(expr.zhegalkin()));
}

TEST(BooleanExpressionFindZhegalkin, FindZhegalkin_4) {
    BooleanExpression expr("x1+x2+x3");
    EXPECT_EQ("x3 + x2 + x1", std::string(expr.zhegalkin()));
}

TEST(BooleanExpressionFindZhegalkin, FindZhegalkin_5) {
    BooleanExpression expr("x1|x2^x3");
    EXPECT_EQ("x1 & x2 + x1 & x2 & x3", std::string(expr.zhegalkin()));
}

TEST(BooleanExpressionPostClasses, ClassT0_1) {
    BooleanExpression expr("x1 + x2 + x3");
    EXPECT_EQ(true, expr.belongClassT0());
}

TEST(BooleanExpressionPostClasses, ClassT1_1) {
    BooleanExpression expr("x1 + x2 + x3");
    EXPECT_EQ(true, expr.belongClassT1());
}

TEST(BooleanExpressionPostClasses, ClassL_1) {
    BooleanExpression expr("x1 + x2 + x3");
    EXPECT_EQ(true, expr.belongClassL());
}

TEST(BooleanExpressionPostClasses, ClassM_1) {
    BooleanExpression expr("x1 + x2 + x3");
    EXPECT_EQ(false, expr.belongClassM());
}

TEST(BooleanExpressionPostClasses, ClassS_1) {
    BooleanExpression expr("x1 + x2 + x3");
    EXPECT_EQ(true, expr.belongClassS());
}

TEST(BooleanExpressionPostClasses, ClassT0_2) {
    BooleanExpression expr("x1 | (x2 ^ x3)");
    EXPECT_EQ(false, expr.belongClassT0());
}

TEST(BooleanExpressionPostClasses, ClassT1_2) {
    BooleanExpression expr("x1 | (x2 ^ x3)");
    EXPECT_EQ(true, expr.belongClassT1());
}

TEST(BooleanExpressionPostClasses, ClassL_2) {
    BooleanExpression expr("x1 | (x2 ^ x3)");
    EXPECT_EQ(false, expr.belongClassL());
}

TEST(BooleanExpressionPostClasses, ClassM_2) {
    BooleanExpression expr("x1 | (x2 ^ x3)");
    EXPECT_EQ(false, expr.belongClassM());
}

TEST(BooleanExpressionPostClasses, ClassS_2) {
    BooleanExpression expr("x1 | (x2 ^ x3)");
    EXPECT_EQ(false, expr.belongClassS());
}

TEST(BooleanExpressionPostClasses, ClassT0_3) {
    BooleanExpression expr("x1 < (x2 = x3)");
    EXPECT_EQ(true, expr.belongClassT0());
}

TEST(BooleanExpressionPostClasses, ClassT1_3) {
    BooleanExpression expr("x1 < (x2 = x3)");
    EXPECT_EQ(true, expr.belongClassT1());
}

TEST(BooleanExpressionPostClasses, ClassL_3) {
    BooleanExpression expr("x1 < (x2 = x3)");
    EXPECT_EQ(false, expr.belongClassL());
}

TEST(BooleanExpressionPostClasses, ClassM_3) {
    BooleanExpression expr("x1 < (x2 = x3)");
    EXPECT_EQ(false, expr.belongClassM());
}

TEST(BooleanExpressionPostClasses, ClassS_3) {
    BooleanExpression expr("x1 < (x2 = x3)");
    EXPECT_EQ(false, expr.belongClassS());
}