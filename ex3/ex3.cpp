#include <windows.h>
#include <tchar.h>
#include <fcntl.h>
#include <io.h>
#include <stdio.h>
#include <time.h>

#define LIM 10000

typedef struct {
	int pos, ordem;
}dado;

HANDLE hMutex;

DWORD WINAPI Thread(LPVOID param);

void gotoxy(int x, int y);

int _tmain(int argc, LPTSTR argv[]) { //Sintaxe: programa N_threads
	TCHAR resp;
	int y, N_threads;
	DWORD threadId; //Id da thread a ser criada
	HANDLE * hT; //HANDLE/ponteiro para cada thread a ser criada

	dado * arrayDados;

	TCHAR executavel[MAX_PATH]; //Nome deste programa executável com args
	STARTUPINFO si; //Estrutura com dados iniciais para novo processo
	PROCESS_INFORMATION pi; //A ser preenchida com dados do novo processo

#ifdef UNICODE
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);
#endif

	system("cls");

	if (argc != 2) {
		_tprintf(TEXT("Erro no nº de argumentos: %s N_threads\n"), argv[0]);
		return -1;
	}
	N_threads = _ttoi(argv[1]);
	arrayDados = (dado *)malloc(N_threads * sizeof(dado));
	hT = (HANDLE *)malloc(N_threads * sizeof(HANDLE));
	srand((int)time(NULL));

	// mutex
	hMutex = CreateMutex(
		NULL,              // default security attributes
		FALSE,             // initially not owned
		TEXT("ex3"));             // unnamed mutex

	if (hMutex == NULL)
	{
		_tprintf(TEXT("CreateMutex error: %d\n"), GetLastError());
		return 1;
	}

	//Lançar um outro processo igual ao actual que dispute pelo mesmo mutex
	_tprintf(TEXT("[%d]Lançar outro processo igual a mim?(S/N)"),
		GetCurrentProcessId());
	_tscanf_s(TEXT("%c"), &resp, 1);
	if (resp == 'S' || resp == 's') {
		/*_stprintf_s(executavel, MAX_PATH, TEXT("%s %s %s %s"), argv[0], argv[1], argv[2],
			argv[3]);*/
		_stprintf_s(executavel, MAX_PATH, TEXT("%s %s"), argv[0], argv[1]);
		ZeroMemory(&si, sizeof(STARTUPINFO));//É equivalente preencher com 0s
		si.cb = sizeof(STARTUPINFO);
		_tprintf(TEXT("[%d]Processo a ser lançado:%s\n"), GetCurrentProcessId(), argv[0]);
		if (CreateProcess(NULL, executavel, NULL, NULL, 0, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi))
			_tprintf(TEXT("[%d]Sucesso\n"), GetCurrentProcessId());
		else {
			_tprintf(TEXT("[%d]Erro a criar processo\n"), GetCurrentProcessId());
			return -1;
		}
	}

	y = 10;
	//Criar as N threads
	for (int i = 0; i < N_threads; i++) {
		//y = rand() % 40;
		y++;
		arrayDados[i].pos = y;
		arrayDados[i].ordem = i + 1;
		hT[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Thread, (LPVOID)&arrayDados[i], 0, &threadId);
		if (hT[i] != NULL)
			_tprintf(TEXT("Lancei uma thread[%d] com id %d\n Prima qq tecla para começar..."),
				i, threadId);
		else
			_tprintf(TEXT("Erro ao criar Thread\n"));
	}

	WaitForSingleObject(hMutex, INFINITE);

	Sleep(5000);

	ReleaseMutex(hMutex);

	WaitForMultipleObjects(N_threads, hT, 1, INFINITE);

	_tprintf(TEXT("\n\nTodas as threads terminaram\n"));
	for (int i = 0; i < N_threads; i++)
		CloseHandle(hT[i]);

	CloseHandle(hMutex);

	//Libertar arrays dinâmicos
	free(arrayDados);
	free(hT);
	_tprintf(TEXT("[Thread Principal %d] Vou terminar..."), GetCurrentThreadId());

	_gettch();

	return 0;
}

/* ----------------------------------------------------- */
/* "Thread" - Funcao associada à Thread */
/* ----------------------------------------------------- */
DWORD WINAPI Thread(LPVOID param) {
	int i;
	dado * ptrDado = (dado *)param;
	_tprintf(TEXT("[Thread %d] Vou começar a trabalhar, ordem: %d, pos: %d\n"), GetCurrentThreadId(), ptrDado->ordem, ptrDado->pos);

	Sleep(50);

	for (i = 0; i < LIM; i++) {

		WaitForSingleObject(hMutex, INFINITE);

		gotoxy(4, ptrDado->pos);
		_tprintf(TEXT("Thread: %5d"), i);

		ReleaseMutex(hMutex);

	}
	return 0;
}

void gotoxy(int x, int y) {
	static HANDLE hStdout = NULL;
	COORD coord;
	coord.X = x;
	coord.Y = y;
	if (!hStdout)
		hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hStdout, coord);
}