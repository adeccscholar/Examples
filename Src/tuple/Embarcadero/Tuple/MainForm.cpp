//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <tuple>
#include <string>

#include "MainForm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmMain *frmMain;
//---------------------------------------------------------------------------
__fastcall TfrmMain::TfrmMain(TComponent* Owner)
    : TForm(Owner)
{
}






using test_type = std::tuple<int, int, double, std::string, int>;
test_type test;


bool Factory(int& para1, int& para2, double &para3, std::string& para4, int& para5) {
   para1 = 42;
   para2 = 3;
   para3 = 3.14155;
   para4 = "Hallo";
   para5 = 50;
   return true;
}

auto Factory(void) {
    return std::make_tuple(42, 3, 3.1415, std::string("Hallo"), 50 );
}





//---------------------------------------------------------------------------
void __fastcall TfrmMain::btnActionClick(TObject *Sender)
{
   test = { 42, 3, 3.1415, "Hallo", 50 };
   test_type test2 = Factory();
;
}
//---------------------------------------------------------------------------
