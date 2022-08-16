object Form1: TForm1
  Left = 0
  Top = 0
  Caption = 'Form1'
  ClientHeight = 1284
  ClientWidth = 2079
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -25
  Font.Name = 'Segoe UI'
  Font.Style = []
  PixelsPerInch = 200
  TextHeight = 35
  object Memo1: TMemo
    Left = 0
    Top = 0
    Width = 1802
    Height = 1284
    Margins.Left = 6
    Margins.Top = 6
    Margins.Right = 6
    Margins.Bottom = 6
    Align = alClient
    Lines.Strings = (
      'Memo1')
    ScrollBars = ssVertical
    TabOrder = 0
  end
  object Panel1: TPanel
    Left = 1802
    Top = 0
    Width = 277
    Height = 1284
    Margins.Left = 6
    Margins.Top = 6
    Margins.Right = 6
    Margins.Bottom = 6
    Align = alRight
    Caption = 'Panel1'
    TabOrder = 1
    object Button1: TButton
      Left = 67
      Top = 33
      Width = 156
      Height = 52
      Margins.Left = 6
      Margins.Top = 6
      Margins.Right = 6
      Margins.Bottom = 6
      Caption = 'Button1'
      TabOrder = 0
      OnClick = Button1Click
    end
  end
end
