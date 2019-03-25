//#include <windows.h>
//#include <tchar.h>
//#include <fcntl.h>
//#include <io.h>
//#include <stdio.h>
//#include <time.h>
//
//typedef struct {
//	int num, inicio, fim;
//} PARAM;
//
//HANDLE hMutex;
//
//DWORD WINAPI Thread(LPVOID param);
//
//void gotoxy(int x, int y);
//
//int _tmain(int argc, LPTSTR argv[]) {
//#ifdef UNICODE
//	_setmode(_fileno(stdin), _O_WTEXT);
//	_setmode(_fileno(stdout), _O_WTEXT);
//#endif
//
//	TCHAR resp;
//	int N, inicio, fim, c;
//	//DWORD tId; //Id da thread a ser criada no momento
//	HANDLE *hThread;//Array dinâmico de HANDLEs para cada thread a ser criada
//	TCHAR executavel[MAX_PATH]; //Nome deste programa executável com args
//	STARTUPINFO si; //Estrutura com dados iniciais para novo processo
//	PROCESS_INFORMATION pi; //A ser preenchida com dados do novo processo
//
//	system("cls");
//
//	if (argc != 2) {
//		_tprintf(TEXT("Erro no nº de argumentos: %s N_threads\n"), argv[0]);
//		return -1;
//	}
//
//	//Criação e inicialização do mutex com nome
//	hMutex = CreateMutex(
//		NULL,              // default security attributes
//		FALSE,             // initially not owned
//		TEXT("mutex"));             // named mutex
//	if (hMutex == NULL)
//	{
//		_tprintf(TEXT("CreateMutex error: %d\n"), GetLastError());
//		return 1;
//	}
//
//	//Cópia dos parâmetros passados, para tornar o código mais legível
//	N = _ttoi(argv[1]);
//	inicio = _ttoi(argv[2]);
//	fim = _ttoi(argv[3]);
//	//Cálculo do comprimento de intervalo para cada thread
//	c = (fim - inicio) / N;
//	//Array dinâmico para guardar N Handles de thread
//	hThread = (HANDLE *)malloc(N * sizeof(HANDLE));
//
//	srand((int)time(NULL));
//	//Lançar um outro processo igual ao actual que dispute pelo mesmo mutex
//	_tprintf(TEXT("[%d]Lançar outro processo igual a mim?(S/N)"),
//		GetCurrentProcessId());
//	_tscanf_s(TEXT("%c"), &resp, 1);
//	if (resp == 'S' || resp == 's') {
//		_stprintf_s(executavel, MAX_PATH, TEXT("%s %s %s %s"), argv[0], argv[1], argv[2],
//			argv[3]);
//		ZeroMemory(&si, sizeof(STARTUPINFO));//É equivalente preencher com 0s
//		si.cb = sizeof(STARTUPINFO);
//		_tprintf(TEXT("[%d]Processo a ser lançado:%s\n"), GetCurrentProcessId(),
//			argv[0]);
//		if (CreateProcess(NULL, executavel, NULL, NULL, 0, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi))
//			_tprintf(TEXT("[%d]Sucesso\n"), GetCurrentProcessId());
//		else {
//			_tprintf(TEXT("[%d]Erro a criar processo\n"), GetCurrentProcessId());
//			return -1;
//		}
//	}
//	_tprintf(TEXT("[%d]A Lançar %d threads para percorrer %d-%d ..."),
//		GetCurrentProcessId(), N, inicio, fim);
//	//Lançamento das Threads como antes
//
//}
//
///* ----------------------------------------------------- */
///* "Thread" - Funcao associada à Thread */
///* ----------------------------------------------------- */
//DWORD WINAPI Thread(LPVOID param) {
//	int i;
//	PARAM * dados = (PARAM *)param;
//	_tprintf(TEXT("[Thread %d] Vou começar a trabalhar, num: %d, inicio: %d, fim: %d\n"), GetCurrentThreadId(), dados->num, dados->inicio, dados->fim);
//
//	for (i = dados->inicio; i < dados->fim; i++) {
//
//		WaitForSingleObject(hMutex, INFINITE);
//
//		gotoxy(4, (dados->num) * 5);
//		_tprintf(TEXT("Thread: %5d"), i);
//
//		ReleaseMutex(hMutex);
//
//	}
//	return 0;
//}