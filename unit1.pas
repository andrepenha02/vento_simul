{
SisDesig - Sistema de Designação de Antenas de Telemedidas

Autor: André Penha Sores Silva
andrepenha@gmail.com

}

unit Unit1;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, Forms, Controls, Graphics, Dialogs, StdCtrls, ExtCtrls,
  Menus, LazSerial, StrUtils, unit2, Math;

type

  { TForm1 }

  TForm1 = class(TForm)
    btnconfig: TButton;
    btnfechar: TButton;
    btnabrir: TButton;
    Button1: TButton;
    Button2: TButton;
    Button3: TButton;
    Button4: TButton;
    Button5: TButton;
    Button6: TButton;
    Button7: TButton;
    GroupBox1: TGroupBox;
    GroupBox2: TGroupBox;
    GroupBox4: TGroupBox;
    GroupBoxVeldireta: TGroupBox;
    Label1: TLabel;
    Label13: TLabel;
    Label14: TLabel;
    Label15: TLabel;
    Label2: TLabel;
    Label3: TLabel;
    Label4: TLabel;
    Label5: TLabel;
    Label6: TLabel;
    Label7: TLabel;
    MainMenu1: TMainMenu;
    MenuItem1: TMenuItem;
    MenuItem2: TMenuItem;
    MenuItem3: TMenuItem;
    MenuItem4: TMenuItem;
    Timer1: TTimer;
    comport3: TLazSerial;
    procedure btnabrirClick(Sender: TObject);
    procedure btnconfigClick(Sender: TObject);
    procedure btnfecharClick(Sender: TObject);
    procedure Button1Click(Sender: TObject);
    procedure Button2Click(Sender: TObject);
    procedure Button3Click(Sender: TObject);
    procedure Button4Click(Sender: TObject);
    procedure Button5Click(Sender: TObject);
    procedure Button6Click(Sender: TObject);
    procedure Button7Click(Sender: TObject);
    procedure comport3RxData(Sender: TObject);
    procedure FormActivate(Sender: TObject);
    procedure MenuItem3Click(Sender: TObject);
    procedure MenuItem4Click(Sender: TObject);
    procedure Timer1Timer(Sender: TObject);
  private

  public
    function func_adequa_formato(num_in:string): string;
  end;

var
  Form1: TForm1;

  cadencia_atual : double;

  velocidade_atual: double;

  c_gama: double;
  k_gama: double;
  vel_media: double;

  CurPos: integer;
  FTempStr: string;

implementation

{$R *.lfm}

{ TForm1 }


//função para ter na saida o formato XX.XX, sempre com duas casas decimais
function TForm1.func_adequa_formato(num_in:string): string;
var
  //adequado: string;
  valor1: double;
  aux1: string;
begin
valor1:= strtofloat(num_in);
aux1:= floattostr(valor1);

if (pos('.',aux1)=0)  then //se nao tiver o '.' na string, está no formato XX, só precisa acrescentar '.00' no final
  begin
    aux1:= aux1+'.00';
  end
else //se tiver ponto da string por star no formato XXX.XX ou XXX.X
  begin
    if ((length(aux1)) = ((pos('.',aux1))+1)) then //indica o formato X.X
      begin
          aux1:= aux1+'0';
      end;
    //para a caso de XX.XX não precisa fazer nada
  end;

//para o caso de ser um numero entre 0 e 9 precisa acrescenta 0 antes do X.XX
if (valor1<=9) then
    begin
        aux1:= '0'+aux1;
    end;

result:= aux1;
end;




procedure TForm1.btnconfigClick(Sender: TObject);
begin

   comport3.ShowSetupDialog;

  if comport3.device<>'' then //só habilita "abrir porta" se for selecionado uma porta
    begin
      btnabrir.Enabled := True;
    end;
end;

procedure TForm1.btnfecharClick(Sender: TObject);
begin

timer1.Enabled:= false;
comport3.Close; //fecha a conexao serial

if not comport3.Active then
  begin
    btnfechar.Enabled := False;
    btnabrir.Enabled := False;
    btnconfig.Enabled:=True;
    Button4.Enabled:= false;
    Button5.Enabled:= false;

  end
else
  begin
    showmessage('Falha ao finalizar conexão serial');
  end;
end;

procedure TForm1.Button1Click(Sender: TObject);
var
  cadencia_in  : string;
  aux          : double;
begin


//recebendo dados de um inputbox
//o operador irá informar o azimute alternativo

cadencia_in:= inputbox('Cadência','Digite o valor da cadência em Segundos:',floattostr(cadencia_atual));

if cadencia_in<> '' then //testa se o usuário apagou o inputbox e clicou em OK
  begin
    try
      aux:= strtofloat(cadencia_in);
        if (aux>0) then
          begin
            label1.Caption:= ' '+ cadencia_in +' s ';
            timer1.interval:= round(aux*1000);
            cadencia_atual:= strtofloat(cadencia_in);
          end
        else
          begin
            showmessage('Valor digitado inválido.');
            exit;
          end;
    except
      showmessage('Valor digitado inválido.');
      exit;
    end;
  end
else
  begin
      exit;
  end;






end;

procedure TForm1.Button2Click(Sender: TObject);
var
  k_in: string;
  c_in: string;
  aux: double;
begin

   timer1.Enabled:= true;
   Button1.Enabled:= false;

   c_in := inputbox('c-fator de escala','Digite o valor da c (fator de escala): ',floattostr(c_gama));

if c_in<> '' then //testa se o usuário apagou o inputbox e clicou em OK
  begin
    try
      aux:= strtofloat(c_in);
      label4.Caption:= ' '+ c_in +' ';
      c_gama:= strtofloat(c_in);
    except
      showmessage('Valor digitado inválido.');
      exit;
    end;
  end
else
  begin
      exit;
  end;

   k_in := inputbox('k-parâmetro de forma','Digite o valor da k (parâmetro de forma): ',floattostr(k_gama));

if k_in<> '' then //testa se o usuário apagou o inputbox e clicou em OK
  begin
    try
      aux:= strtofloat(k_in);
      label5.Caption:= ' '+ k_in +' ';
      k_gama:= strtofloat(k_in);
    except
      showmessage('Valor digitado inválido.');
      exit;
    end;
  end
else
  begin
      exit;
  end;


  Form1.Button7Click(Sender);



end;

procedure TForm1.Button3Click(Sender: TObject);
begin
   velocidade_atual:= roundto(vel_media,-2);
   label14.color:= clBlue;
   label14.caption:= ' '+floattostr(velocidade_atual)+' m/s ';
end;

procedure TForm1.Button4Click(Sender: TObject);
begin

    timer1.enabled:= true;
    Button5.Enabled:= true;
    Button4.Enabled:= false;
end;

procedure TForm1.Button5Click(Sender: TObject);
begin

    timer1.enabled:= false;
    Button4.Enabled:= true;
    Button5.Enabled:= false;
end;

procedure TForm1.Button6Click(Sender: TObject);
var
  velocidade_in : string;
  aux         : double;
begin
  velocidade_in:= inputbox('Velocidade Vento','Digite o valor da Velocidade do Vento (m/s):',floattostr(velocidade_atual));

if velocidade_in<> '' then //testa se o usuário apagou o inputbox e clicou em OK
  begin
    try
      aux:= roundto(strtofloat(velocidade_in),-2);
      if (aux>=0) and (aux<=99.99) then
        begin
            label14.Caption:= ' '+ floattostr(roundto(aux,-2)) +' m/s ';
            velocidade_atual:= strtofloat(velocidade_in);
            label14.color:= clBlack;
        end
      else
        begin
            showmessage('Valor digitado inválido.');
            exit;
        end;
    except
      showmessage('Valor digitado inválido.');
      exit;
    end;
  end
else
  begin
      exit;
  end;



end;

procedure TForm1.Button7Click(Sender: TObject);
var
  alfa: double;
  f_alfa: double;
  g: double;
begin

alfa := 1+(1/k_gama);
//funcao gama
//ASSIS et al. (1996)
//calculo para alfa>0
f_alfa := 1-(1/(12*(power(alfa,2))))+(1/(360*(power(alfa,4))))-(1/(1260*(power(alfa,6))));
g := Sqrt((2*pi)/alfa )* power( (Exp(1)),(alfa*(ln(alfa)-f_alfa )) );

vel_media := c_gama*g;
label6.Caption:= ' '+ floattostr(roundto(vel_media,-2)) +' m/s ';


end;

procedure TForm1.comport3RxData(Sender: TObject);
var
  Str: string;

begin

Str := comport3.ReadData;

CurPos := Pos(char(10), Str);
if CurPos = 0 then
    begin
        FTempStr := FTempStr + Str;
    end
else
    begin
        FTempStr := FTempStr + Str;
        FTempStr:= StringReplace(FTempStr,char(10),'',[rfReplaceAll, rfIgnoreCase]);

        label7.caption:= FTempStr;
        FTempStr:='';
        Str:='';
end;


end;

procedure TForm1.FormActivate(Sender: TObject);
var
    Arquivo_file_trajet: TStringList; //conteudo original do trajetoria.txt
    cont_linhas        : integer;
    buffer_dados       : string;
    buffer_dados_aux   : string;

    val_elevacao       : double;
    val_azimute        : double;
    quantum            : double;

    val_elev_codif     : double;
    val_azim_codif     : double;

    val_elev_codif_string : string;
    val_azim_codif_string : string;

    tamanho            : integer;
    num_zeros          : integer;
    cont_zeros         : integer;

    comando_antena     : string;

    i                  : integer;
    j                  : integer;
begin

velocidade_atual := 5.32;
//configurando em runtime o separador decimal
//para evitar erros caso as configuracoes regionais
{*** é depreciado, foi trocado pelo DefaultFormatSettings.DecimalSeparator}
//DecimalSeparator := '.';   //para padrao ingles
DefaultFormatSettings.DecimalSeparator:='.';

cadencia_atual:= 3;  //inicializa a cadencia com 3 Hz


c_gama:= 6;
k_gama:=2;

vel_media:=5.3;

FTempStr:='';

end;

procedure TForm1.MenuItem3Click(Sender: TObject);
begin
  close;
end;

procedure TForm1.MenuItem4Click(Sender: TObject);
begin



form2.showmodal;//só deixa trabalhar nesse formulário
                     //obriga o usuario a fechar a tela Sobre
                     //para voltar ao aplicativo


end;

procedure TForm1.Timer1Timer(Sender: TObject);
var
   string_comando    : string;
   elevacao_tracado  : integer;
   azimute_tracado   : integer;

   tempo_ponto       : double;
   tempo_adequado_el : integer;
   tempo_adequado_az : integer;

   tx_vel: string;

begin

  tx_vel:= func_adequa_formato( floattostr(roundto(velocidade_atual,-2))); //adequada para ao formato XX.XX antes de transmitir
  comport3.WriteData(tx_vel+char(10));
  //comport3.WriteData((floattostr(velocidade_atual))+char(10)+char(13));
end;

procedure TForm1.btnabrirClick(Sender: TObject);
begin
  try

  // Abrindo a conexão serial
  comport3.Open;

  if comport3.Active then
    begin
      btnconfig.Enabled := False;
      btnabrir.Enabled:=False;
      btnfechar.Enabled := True;
      Button4.Enabled:= true;


    end
    else
      begin
        showmessage('FALHA ao abrir conexão serial com ('+comport3.Device+')');
      end;
        Except on E : Exception do
          begin
            showmessage('ERRO ao abrir conexão: Detalhes> '+E.Message);
          end;
      end;
end;



end.
























