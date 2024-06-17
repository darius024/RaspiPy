// Generated from /home/da1823/C/armv8_37/extension/RaspiPyParser.g4 by ANTLR 4.13.1
import org.antlr.v4.runtime.tree.ParseTreeListener;

/**
 * This interface defines a complete listener for a parse tree produced by
 * {@link RaspiPyParser}.
 */
public interface RaspiPyParserListener extends ParseTreeListener {
	/**
	 * Enter a parse tree produced by {@link RaspiPyParser#module}.
	 * @param ctx the parse tree
	 */
	void enterModule(RaspiPyParser.ModuleContext ctx);
	/**
	 * Exit a parse tree produced by {@link RaspiPyParser#module}.
	 * @param ctx the parse tree
	 */
	void exitModule(RaspiPyParser.ModuleContext ctx);
	/**
	 * Enter a parse tree produced by {@link RaspiPyParser#single_input}.
	 * @param ctx the parse tree
	 */
	void enterSingle_input(RaspiPyParser.Single_inputContext ctx);
	/**
	 * Exit a parse tree produced by {@link RaspiPyParser#single_input}.
	 * @param ctx the parse tree
	 */
	void exitSingle_input(RaspiPyParser.Single_inputContext ctx);
	/**
	 * Enter a parse tree produced by {@link RaspiPyParser#funcdef}.
	 * @param ctx the parse tree
	 */
	void enterFuncdef(RaspiPyParser.FuncdefContext ctx);
	/**
	 * Exit a parse tree produced by {@link RaspiPyParser#funcdef}.
	 * @param ctx the parse tree
	 */
	void exitFuncdef(RaspiPyParser.FuncdefContext ctx);
	/**
	 * Enter a parse tree produced by {@link RaspiPyParser#parameters}.
	 * @param ctx the parse tree
	 */
	void enterParameters(RaspiPyParser.ParametersContext ctx);
	/**
	 * Exit a parse tree produced by {@link RaspiPyParser#parameters}.
	 * @param ctx the parse tree
	 */
	void exitParameters(RaspiPyParser.ParametersContext ctx);
	/**
	 * Enter a parse tree produced by {@link RaspiPyParser#stmt}.
	 * @param ctx the parse tree
	 */
	void enterStmt(RaspiPyParser.StmtContext ctx);
	/**
	 * Exit a parse tree produced by {@link RaspiPyParser#stmt}.
	 * @param ctx the parse tree
	 */
	void exitStmt(RaspiPyParser.StmtContext ctx);
	/**
	 * Enter a parse tree produced by {@link RaspiPyParser#simple_stmts}.
	 * @param ctx the parse tree
	 */
	void enterSimple_stmts(RaspiPyParser.Simple_stmtsContext ctx);
	/**
	 * Exit a parse tree produced by {@link RaspiPyParser#simple_stmts}.
	 * @param ctx the parse tree
	 */
	void exitSimple_stmts(RaspiPyParser.Simple_stmtsContext ctx);
	/**
	 * Enter a parse tree produced by {@link RaspiPyParser#simple_stmt}.
	 * @param ctx the parse tree
	 */
	void enterSimple_stmt(RaspiPyParser.Simple_stmtContext ctx);
	/**
	 * Exit a parse tree produced by {@link RaspiPyParser#simple_stmt}.
	 * @param ctx the parse tree
	 */
	void exitSimple_stmt(RaspiPyParser.Simple_stmtContext ctx);
	/**
	 * Enter a parse tree produced by {@link RaspiPyParser#assignment_stmt}.
	 * @param ctx the parse tree
	 */
	void enterAssignment_stmt(RaspiPyParser.Assignment_stmtContext ctx);
	/**
	 * Exit a parse tree produced by {@link RaspiPyParser#assignment_stmt}.
	 * @param ctx the parse tree
	 */
	void exitAssignment_stmt(RaspiPyParser.Assignment_stmtContext ctx);
	/**
	 * Enter a parse tree produced by {@link RaspiPyParser#assign}.
	 * @param ctx the parse tree
	 */
	void enterAssign(RaspiPyParser.AssignContext ctx);
	/**
	 * Exit a parse tree produced by {@link RaspiPyParser#assign}.
	 * @param ctx the parse tree
	 */
	void exitAssign(RaspiPyParser.AssignContext ctx);
	/**
	 * Enter a parse tree produced by {@link RaspiPyParser#flow_stmt}.
	 * @param ctx the parse tree
	 */
	void enterFlow_stmt(RaspiPyParser.Flow_stmtContext ctx);
	/**
	 * Exit a parse tree produced by {@link RaspiPyParser#flow_stmt}.
	 * @param ctx the parse tree
	 */
	void exitFlow_stmt(RaspiPyParser.Flow_stmtContext ctx);
	/**
	 * Enter a parse tree produced by {@link RaspiPyParser#break_stmt}.
	 * @param ctx the parse tree
	 */
	void enterBreak_stmt(RaspiPyParser.Break_stmtContext ctx);
	/**
	 * Exit a parse tree produced by {@link RaspiPyParser#break_stmt}.
	 * @param ctx the parse tree
	 */
	void exitBreak_stmt(RaspiPyParser.Break_stmtContext ctx);
	/**
	 * Enter a parse tree produced by {@link RaspiPyParser#continue_stmt}.
	 * @param ctx the parse tree
	 */
	void enterContinue_stmt(RaspiPyParser.Continue_stmtContext ctx);
	/**
	 * Exit a parse tree produced by {@link RaspiPyParser#continue_stmt}.
	 * @param ctx the parse tree
	 */
	void exitContinue_stmt(RaspiPyParser.Continue_stmtContext ctx);
	/**
	 * Enter a parse tree produced by {@link RaspiPyParser#return_stmt}.
	 * @param ctx the parse tree
	 */
	void enterReturn_stmt(RaspiPyParser.Return_stmtContext ctx);
	/**
	 * Exit a parse tree produced by {@link RaspiPyParser#return_stmt}.
	 * @param ctx the parse tree
	 */
	void exitReturn_stmt(RaspiPyParser.Return_stmtContext ctx);
	/**
	 * Enter a parse tree produced by {@link RaspiPyParser#compound_stmt}.
	 * @param ctx the parse tree
	 */
	void enterCompound_stmt(RaspiPyParser.Compound_stmtContext ctx);
	/**
	 * Exit a parse tree produced by {@link RaspiPyParser#compound_stmt}.
	 * @param ctx the parse tree
	 */
	void exitCompound_stmt(RaspiPyParser.Compound_stmtContext ctx);
	/**
	 * Enter a parse tree produced by {@link RaspiPyParser#if_stmt}.
	 * @param ctx the parse tree
	 */
	void enterIf_stmt(RaspiPyParser.If_stmtContext ctx);
	/**
	 * Exit a parse tree produced by {@link RaspiPyParser#if_stmt}.
	 * @param ctx the parse tree
	 */
	void exitIf_stmt(RaspiPyParser.If_stmtContext ctx);
	/**
	 * Enter a parse tree produced by {@link RaspiPyParser#while_stmt}.
	 * @param ctx the parse tree
	 */
	void enterWhile_stmt(RaspiPyParser.While_stmtContext ctx);
	/**
	 * Exit a parse tree produced by {@link RaspiPyParser#while_stmt}.
	 * @param ctx the parse tree
	 */
	void exitWhile_stmt(RaspiPyParser.While_stmtContext ctx);
	/**
	 * Enter a parse tree produced by {@link RaspiPyParser#for_stmt}.
	 * @param ctx the parse tree
	 */
	void enterFor_stmt(RaspiPyParser.For_stmtContext ctx);
	/**
	 * Exit a parse tree produced by {@link RaspiPyParser#for_stmt}.
	 * @param ctx the parse tree
	 */
	void exitFor_stmt(RaspiPyParser.For_stmtContext ctx);
	/**
	 * Enter a parse tree produced by {@link RaspiPyParser#block}.
	 * @param ctx the parse tree
	 */
	void enterBlock(RaspiPyParser.BlockContext ctx);
	/**
	 * Exit a parse tree produced by {@link RaspiPyParser#block}.
	 * @param ctx the parse tree
	 */
	void exitBlock(RaspiPyParser.BlockContext ctx);
	/**
	 * Enter a parse tree produced by {@link RaspiPyParser#test}.
	 * @param ctx the parse tree
	 */
	void enterTest(RaspiPyParser.TestContext ctx);
	/**
	 * Exit a parse tree produced by {@link RaspiPyParser#test}.
	 * @param ctx the parse tree
	 */
	void exitTest(RaspiPyParser.TestContext ctx);
	/**
	 * Enter a parse tree produced by {@link RaspiPyParser#or_test}.
	 * @param ctx the parse tree
	 */
	void enterOr_test(RaspiPyParser.Or_testContext ctx);
	/**
	 * Exit a parse tree produced by {@link RaspiPyParser#or_test}.
	 * @param ctx the parse tree
	 */
	void exitOr_test(RaspiPyParser.Or_testContext ctx);
	/**
	 * Enter a parse tree produced by {@link RaspiPyParser#and_test}.
	 * @param ctx the parse tree
	 */
	void enterAnd_test(RaspiPyParser.And_testContext ctx);
	/**
	 * Exit a parse tree produced by {@link RaspiPyParser#and_test}.
	 * @param ctx the parse tree
	 */
	void exitAnd_test(RaspiPyParser.And_testContext ctx);
	/**
	 * Enter a parse tree produced by {@link RaspiPyParser#not_test}.
	 * @param ctx the parse tree
	 */
	void enterNot_test(RaspiPyParser.Not_testContext ctx);
	/**
	 * Exit a parse tree produced by {@link RaspiPyParser#not_test}.
	 * @param ctx the parse tree
	 */
	void exitNot_test(RaspiPyParser.Not_testContext ctx);
	/**
	 * Enter a parse tree produced by {@link RaspiPyParser#comparison}.
	 * @param ctx the parse tree
	 */
	void enterComparison(RaspiPyParser.ComparisonContext ctx);
	/**
	 * Exit a parse tree produced by {@link RaspiPyParser#comparison}.
	 * @param ctx the parse tree
	 */
	void exitComparison(RaspiPyParser.ComparisonContext ctx);
	/**
	 * Enter a parse tree produced by {@link RaspiPyParser#comp_op}.
	 * @param ctx the parse tree
	 */
	void enterComp_op(RaspiPyParser.Comp_opContext ctx);
	/**
	 * Exit a parse tree produced by {@link RaspiPyParser#comp_op}.
	 * @param ctx the parse tree
	 */
	void exitComp_op(RaspiPyParser.Comp_opContext ctx);
	/**
	 * Enter a parse tree produced by {@link RaspiPyParser#expr}.
	 * @param ctx the parse tree
	 */
	void enterExpr(RaspiPyParser.ExprContext ctx);
	/**
	 * Exit a parse tree produced by {@link RaspiPyParser#expr}.
	 * @param ctx the parse tree
	 */
	void exitExpr(RaspiPyParser.ExprContext ctx);
	/**
	 * Enter a parse tree produced by {@link RaspiPyParser#atom_expr}.
	 * @param ctx the parse tree
	 */
	void enterAtom_expr(RaspiPyParser.Atom_exprContext ctx);
	/**
	 * Exit a parse tree produced by {@link RaspiPyParser#atom_expr}.
	 * @param ctx the parse tree
	 */
	void exitAtom_expr(RaspiPyParser.Atom_exprContext ctx);
	/**
	 * Enter a parse tree produced by {@link RaspiPyParser#atom}.
	 * @param ctx the parse tree
	 */
	void enterAtom(RaspiPyParser.AtomContext ctx);
	/**
	 * Exit a parse tree produced by {@link RaspiPyParser#atom}.
	 * @param ctx the parse tree
	 */
	void exitAtom(RaspiPyParser.AtomContext ctx);
	/**
	 * Enter a parse tree produced by {@link RaspiPyParser#name}.
	 * @param ctx the parse tree
	 */
	void enterName(RaspiPyParser.NameContext ctx);
	/**
	 * Exit a parse tree produced by {@link RaspiPyParser#name}.
	 * @param ctx the parse tree
	 */
	void exitName(RaspiPyParser.NameContext ctx);
	/**
	 * Enter a parse tree produced by {@link RaspiPyParser#exprlist}.
	 * @param ctx the parse tree
	 */
	void enterExprlist(RaspiPyParser.ExprlistContext ctx);
	/**
	 * Exit a parse tree produced by {@link RaspiPyParser#exprlist}.
	 * @param ctx the parse tree
	 */
	void exitExprlist(RaspiPyParser.ExprlistContext ctx);
	/**
	 * Enter a parse tree produced by {@link RaspiPyParser#testlist}.
	 * @param ctx the parse tree
	 */
	void enterTestlist(RaspiPyParser.TestlistContext ctx);
	/**
	 * Exit a parse tree produced by {@link RaspiPyParser#testlist}.
	 * @param ctx the parse tree
	 */
	void exitTestlist(RaspiPyParser.TestlistContext ctx);
}