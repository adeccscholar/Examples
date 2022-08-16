// ConsoleApplication2.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet die Ausführung des Programms.
//
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#include <codecvt>
#include <iostream>
#include <string>
#include <fstream>
#include <locale>
#include <stdexcept>
#include <filesystem>

#include <io.h>
#include <fcntl.h> 
#include <Windows.h>

namespace fs = std::filesystem;

void ReadFile(std::wstring const& strFile) {
   std::wstring strLine;
   try {
      std::wifstream file(strFile);
      file.exceptions(std::ios_base::badbit);
      if (!file.is_open()) {
         std::wcerr << L"File \"" << strFile << "\" can't open.\n";
         return;
         }
         
      std::locale loc(std::locale::empty(), new std::codecvt_utf8<wchar_t>);
      // std::locale loc(std::locale("de_DE"), new std::codecvt_utf8<wchar_t>);
      //std::locale loc(std::locale("ar_EG"), new std::codecvt_utf8<wchar_t>);
      file.imbue(loc);
      int iLine = 0;
      while (getline(file, strLine)) {
         std::wcout << strLine << std::endl;
         }

      }
   catch (std::ios_base::failure const& ex) {
      std::cerr << "Error" << std::endl
         << ex.what() << std::endl
         << "ec: " << ex.code() << std::endl;
   }
   catch (std::exception& ex) {
      std::cerr << "Error" << std::endl
         << ex.what() << std::endl;
   }
}

int wmain(int argc, wchar_t* argv[]) {
   _setmode(_fileno(stdout), _O_U16TEXT);
   fs::path directory = fs::path(L"..\\..\\..\\..\\Data");
   fs::path path = fs::path(argv[0]).parent_path();
   path = fs::weakly_canonical(path / directory / fs::path(L"testfile.txt"));
   // path = fs::weakly_canonical(path / directory / fs::path(L"testfile egyptian.txt"));
   std::wcout << path.wstring() << std::endl;
   ReadFile(path.wstring());

   getchar();
   }
