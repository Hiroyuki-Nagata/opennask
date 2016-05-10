/* MacroDrawParser.hh */
/* Created by Enomoto Sanshiro on 19 October 2001. */
/* Last updated by Enomoto Sanshiro on 19 October 2001. */


#ifndef __MACRO_DRAW_PARSER_HH
#define __MACRO_DRAW_PARSER_HH


#include "Canvas.hh"
#include "CanvasMessenger.hh"
#include "ParaParser.hh"


class TMacroDrawParser: public TParaStandardParser {
  public:
    TMacroDrawParser(TCanvas* Canvas);
    virtual ~TMacroDrawParser();
  protected:
    virtual TParaTokenTable* CreateTokenTable(void);
    virtual TParaObjectPrototypeTable* CreateObjectPrototypeTable(void);
    virtual TParaBuiltinFunctionTable* CreateBuiltinFunctionTable(void);
    virtual TParaPackage* CreatePackage(void);
  private:
    TCanvas* _Canvas;
    TCanvasMessenger* _CanvasMessenger;
};


class TMacroEntry: public TParaPackageEntry {
  public:
    TMacroEntry(void);
    virtual ~TMacroEntry();
    virtual TParaPackageEntry* Clone(void);
    virtual bool HasEntryWordsOf(TParaTokenizer* Tokenizer);
    virtual void Parse(TParaTokenizer* Tokenizer, TParaStatementParser* StatementParser, TParaSymbolTable* SymbolTable) throw(TScriptException);
    virtual TParaValue Execute(const std::vector<TParaValue*>& ArgumentList, TParaSymbolTable* SymbolTable) throw(TScriptException);
  protected:
    TParaStatement* _Statement;
};


#endif
