#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

/* ======================= Config DLL ======================= */
static HMODULE g_hDll = NULL;

/* Convenção de chamada (Windows): __stdcall */
#ifndef CALLCONV
#  define CALLCONV WINAPI
#endif

/* ======================= Assinaturas da DLL ======================= */
typedef int (CALLCONV *AbreConexaoImpressora_t)(int, const char *, const char *, int);
typedef int (CALLCONV *FechaConexaoImpressora_t)(void);
typedef int (CALLCONV *ImpressaoTexto_t)(const char *, int, int, int);
typedef int (CALLCONV *Corte_t)(int);
typedef int (CALLCONV *ImpressaoQRCode_t)(const char *, int, int);
typedef int (CALLCONV *ImpressaoCodigoBarras_t)(int, const char *, int, int, int);
typedef int (CALLCONV *AvancaPapel_t)(int);
typedef int (CALLCONV *AbreGavetaElgin_t)(int, int, int);
typedef int (CALLCONV *AbreGaveta_t)(int, int, int);
typedef int (CALLCONV *SinalSonoro_t)(int, int, int);
typedef int (CALLCONV *ImprimeXMLSAT_t)(const char *, int);
typedef int (CALLCONV *ImprimeXMLCancelamentoSAT_t)(const char *, const char *, int);
typedef int (CALLCONV *InicializaImpressora_t)(void);

/* ======================= Ponteiros ======================= */
static AbreConexaoImpressora_t        AbreConexaoImpressora        = NULL;
static FechaConexaoImpressora_t       FechaConexaoImpressora       = NULL;
static ImpressaoTexto_t               ImpressaoTexto               = NULL;
static Corte_t                        Corte                        = NULL;
static ImpressaoQRCode_t              ImpressaoQRCode              = NULL;
static ImpressaoCodigoBarras_t        ImpressaoCodigoBarras        = NULL;
static AvancaPapel_t                  AvancaPapel                  = NULL;
static AbreGavetaElgin_t              AbreGavetaElgin              = NULL;
static AbreGaveta_t                   AbreGaveta                   = NULL;
static SinalSonoro_t                  SinalSonoro                  = NULL;
static ImprimeXMLSAT_t                ImprimeXMLSAT                = NULL;
static ImprimeXMLCancelamentoSAT_t    ImprimeXMLCancelamentoSAT    = NULL;
static InicializaImpressora_t         InicializaImpressora         = NULL;

/* ======================= Configuração ======================= */
static int   g_tipo      = 1;
static char  g_modelo[64] = "i9";
static char  g_conexao[128] = "USB";
static int   g_parametro = 0;
static int   g_conectada = 0;

/* ======================= Utilidades ======================= */
#define LOAD_FN(h, name)                                                         \
    do {                                                                         \
        name = (name##_t)GetProcAddress((HMODULE)(h), #name);                    \
        if (!(name)) {                                                           \
            fprintf(stderr, "Falha ao resolver símbolo %s (erro=%lu)\n",         \
                    #name, GetLastError());                                      \
            return 0;                                                            \
        }                                                                        \
    } while (0)

static void flush_entrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

/* ======================= Funções para manipular a DLL ======================= */
static int carregarFuncoes(void)
{
    g_hDll = LoadLibraryA("E1_Impressora01.dll");
    if (!g_hDll) {
        fprintf(stderr, "Erro ao carregar E1_Impressora01.dll (erro=%lu)\n", GetLastError());
        return 0;
    }

    LOAD_FN(g_hDll, AbreConexaoImpressora);
    LOAD_FN(g_hDll, FechaConexaoImpressora);
    LOAD_FN(g_hDll, ImpressaoTexto);
    LOAD_FN(g_hDll, Corte);
    LOAD_FN(g_hDll, ImpressaoQRCode);
    LOAD_FN(g_hDll, ImpressaoCodigoBarras);
    LOAD_FN(g_hDll, AvancaPapel);
    LOAD_FN(g_hDll, AbreGavetaElgin);
    LOAD_FN(g_hDll, AbreGaveta);
    LOAD_FN(g_hDll, SinalSonoro);
    LOAD_FN(g_hDll, ImprimeXMLSAT);
    LOAD_FN(g_hDll, ImprimeXMLCancelamentoSAT);
    LOAD_FN(g_hDll, InicializaImpressora);

    return 1;
}

static void liberarBiblioteca(void)
{
    if (g_hDll) {
        FreeLibrary(g_hDll);
        g_hDll = NULL;
    }
}

/* ======================= Funções a serem implementadas pelos alunos ======================= */

static void exibirMenu(void)
{
	printf("\n===SISTEMA IMPRESSORA ELGIN i9===\n");
	printf("1. Configurar Conexao: \n");
	printf("2. Abrir Conexao com Impressora: \n");
	printf("3. Fechar Conexao: \n");
	printf("4. Imprimir Texto: \n");
	printf("5. Imprimir QR Code: \n");
	printf("6. Imprimir Codigo de Barras: \n");
	printf("7. Imprimir XML SAT: \n");
	printf("8. Imprimir XML Cancelamento SAT: \n");
	printf("9. Abrir Gaveta (ELGIN): \n");
	printf("10. Abrir Gaveta (Generico): \n");
	printf("11. Emitir Sinal Sonoro: \n");
	printf("0. Sair: ");
	printf("Status: %s \n", g_conectada ? "CONECTADA": "DESCONECTADA");
	printf("Opcao: ");
	
    // TODO: implementar exibição do menu principal com as opções de impressão
}

static void configurarConexao(void)
{
	printf("\n=== CONFIGURAR CONEXAO===\n");
	printf("Tipo de Impressora: ('1. Termica'): ");
	if(scanf("%d", &g_tipo) != 1){
		flush_entrada();
		printf("Erro na entrada! Mantendo tipo padrao. \n");
		return;
	}
	flush_entrada();
	
	printf("Modelo da Impressora (ex i9): ");
	if(fgets(g_modelo, sizeof(g_modelo), stdin)){
		g_modelo[strcspn(g_modelo, "\n")] = '\0';
	}
	
	printf("Tipo de Conexao (USB, COM1, IP): ");
	if(fgets(g_conexao, sizeof(g_conexao), stdin)){
		g_conexao[strcspn(g_conexao, "\n")] = '\0';
	}
	
	printf("Parametro Adicional (0 para USB): ");
	if(scanf("%d", &g_parametro) != 1){
		flush_entrada();
		g_parametro = 0;
	}
	flush_entrada();
	
	printf("\n Configuracao salva \n");
	printf("  Tipo: %d \n", g_tipo);
	printf("  Modelo: %s \n", g_modelo);
	printf("  Conexao: %s \n", g_conexao);
	printf("  Parametro: %d \n", g_parametro);
	
    // TODO: pedir ao usuário tipo, modelo, conexão e parâmetro
}

static void abrirConexao(void)
{
	if(g_conectada){
		printf("Impressora ja esta conectada. \n");
		return;
}
		printf("\n===ABRINDO CONEXAO===\n");
		printf("Conectando a impressora %s via %s...\n", g_modelo, g_conexao);
		
		int resultado = AbreConexaoImpressora(g_tipo, g_modelo, g_conexao, g_parametro);
		
		if(resultado == 0){
			printf("Conexao estabalecida com sucesso. \n");
			g_conectada = 1;
			
			//Inicializa a impressora:
			if(InicializaImpressora() == 0){
				printf("Impressora inicializada. \n");
			}
		}else{
			printf("Erro ao conectar. (codigo: %d). \n", resultado);
			printf("Verifique se a impressora esta ligada e conectada. \n");
		}
	}
    // TODO: chamar AbreConexaoImpressora e validar retorno

static void fecharConexao(void)
{
	if(!g_conectada){
		printf("\n Nenhuma conexao ativa. \n");
		return;
	}
	
	printf("\n Fechando conexao...\n");
	int resultado = FechaConexaoImpressora();
	
	if(resultado == 0){
		printf("Conexao fechada com sucesso. \n");
		g_conectada = 0;
	}else{
		printf("Erro ao fechar conexao. (codigo: %d). \n", resultado);
	}
    // TODO: chamar FechaConexaoImpressora e tratar retorno
}

static void imprimirTexto(void)
{
	if(!g_conectada){
		printf("\n Erro: Impressora nao conectada. \n");
		return;
	}
	
	char texto[512];
	printf("\n ===IMPRESSSAO DE TEXTO\n");
	printf("Digite um texto a ser impresso: \n");
	flush_entrada();
	
	if(!fgets(texto, sizeof(texto), stdin)){
		printf("Erro ao ler o texto. \n");
		return;
	}
	
	//Remove \n no final:
	texto[strcspn(texto, "\n")] = '\0';
	
	printf("Imprimindo...\n");
	
	// Inclusão dos parâmetros: texto, alinhamento (0 = esq), fonte (0 = normal), tamanho (0 = normal):
	int resultado = ImpressaoTexto(texto, 0, 0, 0); //Alinhamentos do texto.
	
	if(resultado == 0){
		AvancaPapel(5);
		Corte(1);
		printf("Texto impresso com sucesso. \n");
	}else{
		printf("Erro ao imprimir texto. (codigo %d). \n", resultado);
	}
    // TODO: solicitar texto do usuário e chamar ImpressaoTexto
    // incluir AvancaPapel e Corte no final
    //AQUI ENTRA AQUILO QUE FALEI SOBRE A CONTINUIDADE DO CÓDIGO ENTRE AS FUNÇÕES!!!
}

static void imprimirQRCode(void)
{
	if(!g_conectada){
		printf("\n Erro: Impressora nao conectada. \n");
		return;
	}
	
	char conteudo[512];
	printf("\n===IMPRESSAO DE QRCODE===\n");
	printf("Digite o conteudo do QR CODE: \n");
	flush_entrada();
	
	if(!fgets(conteudo, sizeof(conteudo), stdin)){
		printf("Erro ao ler conteudo. \n");
		return;
	}
	
	conteudo[strcspn(conteudo, "\n")] = '\0';
	
	printf("Gerando QR CODE...\n");
	
	//Inclusão de Parâmetros: texto, tamanho qrCode (6), nívelCorreção (4=Alta).
	int resultado = ImpressaoQRCode(conteudo, 6, 4);
	
	if(resultado == 0){
		AvancaPapel(5);
		Corte(1);
		printf("QR Code impresso com sucesso. \n");
	}else{
		printf("Erro ao imprimir QR Code. (codigo: %d). \n", resultado);
	}
    // TODO: solicitar conteúdo do QRCode e chamar ImpressaoQRCode(texto, 6, 4)
    // incluir AvancaPapel e Corte no final
}

static void imprimirCodigoBarras(void)
{
	if(!g_conectada){
		printf("\nErro: Impressora nao conectada. \n");
		return;
	}
	
	printf("\n===IMPRESSAO CODIGO DE BARRAS===\n");
	printf("Imprimindo codigo de barras padrao...\n ");
	
	//Inclusão dos parâmetros: tipo, codigo, altura, largura.
	int resultado = ImpressaoCodigoBarras(8, "{A012345678912", 100, 2, 3);
	
	if(resultado == 0){
		AvancaPapel(5);
		Corte(1);
		printf("Codigo de barras impresso com sucesso. \n");
	}else{
		printf("Erro ao imprimir codigo de barras. (codigo %d). \n", resultado);
	}
    // TODO: usar ImpressaoCodigoBarras(8, "{A012345678912", 100, 2, 3)
    // incluir AvancaPapel e Corte no final
}

static void imprimirXMLSAT(void)
{
	if(!g_conectada){
		printf("\nErro: Impressora nao conectada. \n");
		return;
	}
	
	printf("\n===IMPRESSAO XML SAT===\n");
	
	FILE *fp = fopen("./XMLSAT.xml", "r");
	if(!fp){
		printf("Erro: Arquivo XMLSAT.xml nao encontrado. \n");
		printf("  Certifique-se que o arquivo existe no diretorio atual. \n");
		return;
	}
	
	//Leitura do conteúdo do arquivo:
	fseek(fp, 0, SEEK_END);
	long tamanho = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	
	char *xml = (char*)malloc(tamanho + 1);
	if(!xml){
		printf("Erro de memoria. \n");
		fclose(fp);
		return;
	}
	
	fread(xml, 1, tamanho, fp);
	xml[tamanho] = '\0';
	fclose(fp);
	
	printf("Imprimindo XML SAT. (%ld bytes)...\n", tamanho);
	
	//Inclusao de parâmetros:
	int resultado = ImprimeXMLSAT(xml, 0);
	
	free(xml);
	
	if(resultado == 0){
		AvancaPapel(5);
		Corte(1);
		printf("XML SAT impresso com sucesso. \n");
	}else{
		printf("Erro ao imprimir XML SAT. (codigo %d). \n", resultado);
	}
    // TODO: ler o arquivo ./XMLSAT.xml e enviar via ImprimeXMLSAT
    // incluir AvancaPapel e Corte no final
}

static void imprimirXMLCancelamentoSAT(void)
{
	if(!g_conectada){
		printf("\nErro: Impressora nao conectada. \n");
		return;
	}
	
	printf("\n===IMPRESSAO CANCELAMENTO XML SAT ===\n");
	
	FILE *fp = fopen("./CANC_SAT.xml", "r");
	if(!fp){
		printf("Erro: Arquivo CANC_SAT.xml nao encontrado. \n");
		printf("  Certifique-se de que o arquivo existe no diretorio atual. \n");
		return;
	}
	
	fseek(fp, 0, SEEK_END);
	long tamanho = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	
	char *xml = (char*)malloc(tamanho + 1);
	if(!xml){
		printf("Erro de memoria. \n");
		fclose(fp);
		return;
	}
	
	fread(xml, 1, tamanho, fp);
	xml[tamanho] = '\0';
	fclose(fp);
    // TODO: ler o arquivo ./CANC_SAT.xml e chamar ImprimeXMLCancelamentoSAT
    // incluir AvancaPapel e Corte no final
    
    const char *assinatura =
	
        "Q5DLkpdRijIRGY6YSSNsTWK1TztHL1vD0V1Jc4spo/CEUqICEb9SFy82ym8EhBRZ"
        "jbh3btsZhF+sjHqEMR159i4agru9x6KsepK/q0E2e5xlU5cv3m1woYfgHyOkWDNc"
        "SdMsS6bBh2Bpq6s89yJ9Q6qh/J8YHi306ce9Tqb/drKvN2XdE5noRSS32TAWuaQE"
        "Vd7u+TrvXlOQsE3fHR1D5f1saUwQLPSdIv01NF6Ny7jZwjCwv1uNDgGZONJdlTJ6"
        "p0ccqnZvuE70aHOI09elpjEO6Cd+orI7XHHrFCwhFhAcbalc+ZfO5b/+vkyAHS6C"
        "YVFCDtYR9Hi5qgdk31v23w==";
        
        printf("Imprimindo cancelamento SAT. (%ld bytes)...\n", tamanho);
        
        int resultado = ImprimeXMLCancelamentoSAT(xml, assinatura, 0);
        
        free(xml);
        
        //Inclusão dos parâmetros solicitados:
        if(resultado == 0){
        	AvancaPapel(5);
        	Corte(1);
        	printf("XML Cancelamento SAT impresso com sucesso. \n");
		}else{
			printf("Erro ao imprimir XML Cancelamento SAT. (codigo %d). \n", resultado);
		}
}

static void abrirGavetaElginOpc(void)
{
	if(!g_conectada){
		printf("\n Erro: Impressora nao conectada. \n");
		return;
	}
	
	printf("\n===ABRINDO GAVETA===\n");
	
	//INCLUSÃO DOS PARÂMETROS!
	int resultado = AbreGavetaElgin(1, 50, 50);
	
	if(resultado == 0){
		printf("Comando para abertura da gaveta enviado. \n");
	}else{
		printf("Erro ao abrir gaveta. (codigo %d). \n", resultado);
	}
    // TODO: chamar AbreGavetaElgin(1, 50, 50)
}

static void abrirGavetaOpc(void)
{
	if(!g_conectada){
		printf("\n Erro: Impressora nao conectada. \n");
		return;
	}
	
	printf("\n===ABRINDO GAVETA (GENERICO)===\n");
	
	//INCLUSÃO DOS PARÂMETROS!
	int resultado = AbreGaveta(1, 5, 10);
	
	if(resultado == 0){
		printf("Comando de abertura da gaveta enviado. \n");
	}else{
		printf("Erro ao abrir gaveta. (codigo %d). \n", resultado);
	}
    // TODO: chamar AbreGaveta(1, 5, 10)
}

static void emitirSinalSonoro(void)
{
	if(!g_conectada){
		printf("\n Erro: Impressora nao conectada. \n");
		return;
	}
	
	printf("\n ===EMITINDO SINAL SONORO===\n");
	
	//INCLUSÃO DOS PARÂMETROS!
	int resultado = SinalSonoro(4, 50, 5);
	
	if(resultado == 0){
		printf("Sinal Sonoro emitido. \n");
	}else{
		printf("Erro ao emitir sinal sonoro. (codigo %d). \n", resultado);
	}
    // TODO: chamar SinalSonoro(4, 50, 5)
}

/* ======================= Função principal ======================= */
int main(void)
{
	
	printf("Carregando biblioteca E1_Impressora01.dll...\n");
	
    if (!carregarFuncoes()) {
    	printf("\n Falha ao carregar a DLL \n");
    	printf("Verfique se o arquivo E1_Impressora01.dll esta no diretorio. \n");
    	system("pause");
        return 1;
    }
    
    printf("Biblioteca carregada com sucesso. \n");
    Sleep(1000);
    
    int opcao = 0;
    
    while(1){
    	system("cls");
    	exibirMenu();
    	
    	if(scanf("%d", &opcao) != 1){
    		flush_entrada();
    		printf("\n Opcao invalida. \n");
    		Sleep(1500);
    		continue;
		}
		flush_entrada();
		
		switch(opcao){
			case 0:
				printf("\n ===ENCERRANDO SISTEMA===\n");
				if(g_conectada){
					printf("Fechando conexao...\n");
				}
				liberarBiblioteca();
				printf("Ate logo. \n");
				return 0;
				
				case 1:
					configurarConexao();
					break;
					
					case 2:
						abrirConexao();
						break;
						
						case 3:
							fecharConexao();
							break;
							
							case 4:
								imprimirTexto();
								break;
								
								case 5:
									imprimirQRCode();
									break;
									
									case 6:
										imprimirCodigoBarras();
										break;
										
										case 7:
											imprimirXMLSAT();
											break;
											
											case 8:
												imprimirXMLCancelamentoSAT();
												break;
												
												case 9:
													abrirGavetaElginOpc();
													break;
													
													case 10:
														abrirGavetaOpc();
														break;
														
														case 11:
															emitirSinalSonoro();
															break;
															
															default:
																printf("\n Opcao invalida. Escolha entre 1 e 11. \n");
																break;
		}
		
		if(opcao != 0){
			printf("\n Pressione ENTER para continuar...");
			getchar();
		}
        //construir o menu e chamar as funçoes aqui!!!                
    }
    
    return 0;
}

