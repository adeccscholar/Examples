//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"

#include <iomanip>
#include <string>
#include <fstream>
#include <locale>
#include <exception>
#include <codecvt>

#include <filesystem>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------

namespace fs = std::filesystem;

__fastcall TForm1::TForm1(TComponent* Owner)
   : TForm(Owner)
{
}


void ReadFile(std::wstring const& strFile, TMemo* out) {
   std::wfstream ifs(strFile);
   try {
      ifs.exceptions(std::ios_base::badbit);
      std::wstring strRow;
   
      if(!ifs.is_open()) { 
         ;
         return;
         }

      std::locale loc (std::locale::empty(), new std::codecvt_utf8<wchar_t>);
      ifs.imbue(loc);

      while(std::getline(ifs, strRow)) {
         out->Lines->Add(strRow.c_str());
         }
      }
   catch(std::exception& ex) {
      out->Lines->Add("error reading stream:");
      out->Lines->Add(ex.what());
      }
   }


//---------------------------------------------------------------------------
void __fastcall TForm1::Button1Click(TObject *Sender) {

   fs::path path = fs::path(ParamStr(0).c_str()).parent_path();
   for(auto i = 0; i < 4; ++i) path = path.parent_path();
   fs::path directory = fs::path(L"Data");
   path = fs::weakly_canonical(path / directory / fs::path(L"testfile.txt"));
   Memo1->Lines->Add(path.wstring().c_str());
   // path = fs::weakly_canonical(path / directory / fs::path(L"testfile egyptian.txt"));

   ReadFile(path.wstring(), Memo1);   
}
//---------------------------------------------------------------------------
