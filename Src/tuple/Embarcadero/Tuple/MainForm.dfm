object frmMain: TfrmMain
  Left = 0
  Top = 0
  Caption = 'frmMain'
  ClientHeight = 1027
  ClientWidth = 1937
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -25
  Font.Name = 'Segoe UI'
  Font.Style = []
  PixelsPerInch = 200
  TextHeight = 35
  object Panel1: TPanel
    Left = 1625
    Top = 0
    Width = 312
    Height = 1027
    Margins.Left = 6
    Margins.Top = 6
    Margins.Right = 6
    Margins.Bottom = 6
    Align = alRight
    TabOrder = 0
    object btnAction: TButton
      Left = 16
      Top = 34
      Width = 274
      Height = 53
      Margins.Left = 6
      Margins.Top = 6
      Margins.Right = 6
      Margins.Bottom = 6
      Caption = 'btnAction'
      TabOrder = 0
      OnClick = btnActionClick
    end
  end
  object Panel2: TPanel
    Left = 0
    Top = 0
    Width = 1625
    Height = 1027
    Margins.Left = 6
    Margins.Top = 6
    Margins.Right = 6
    Margins.Bottom = 6
    Align = alClient
    TabOrder = 1
    object Splitter1: TSplitter
      Left = 1
      Top = 835
      Width = 1623
      Height = 6
      Cursor = crVSplit
      Margins.Left = 6
      Margins.Top = 6
      Margins.Right = 6
      Margins.Bottom = 6
      Align = alBottom
      ExplicitLeft = -10
      ExplicitTop = 679
    end
    object memOutput: TMemo
      Left = 1
      Top = 1
      Width = 1623
      Height = 834
      Margins.Left = 6
      Margins.Top = 6
      Margins.Right = 6
      Margins.Bottom = 6
      Align = alClient
      Lines.Strings = (
        'memOutput')
      TabOrder = 0
    end
    object memError: TMemo
      Left = 1
      Top = 841
      Width = 1623
      Height = 185
      Margins.Left = 6
      Margins.Top = 6
      Margins.Right = 6
      Margins.Bottom = 6
      Align = alBottom
      Lines.Strings = (
        'memError')
      TabOrder = 1
    end
  end
end
