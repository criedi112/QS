#include <windows.h>
#include<time.h>
#define N 8000
#define true 1
#define false 0

LRESULT CALLBACK Main(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK VLS(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK VLD(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK LSLD(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK VLSLD(HWND, UINT, WPARAM, LPARAM);

HINSTANCE	ghostInst = 0;

struct nodeS {
	int data;
	struct nodeS*next;
};

struct nodeD {
	int data;
	struct nodeD*previous, *next;
};

struct nodeS *headS,*endS;
struct nodeD *headD,*endD;

int main_closed = false;

void pushS(int data) {

	struct nodeS *new_node = (struct nodeS*)malloc(sizeof(struct nodeS));
	new_node->data = data;
	new_node->next = NULL;

	if (headS== NULL) {

		headS=endS = new_node;
		return;

	}

	endS->next = new_node;
	endS = new_node;

}

void pushD(int data) {

	struct nodeD *new_node = (struct nodeD*)malloc(sizeof(struct nodeD));
	new_node->data = data;
	new_node->next = new_node->previous = NULL;

	if (headD == NULL) {

		headD=endD = new_node;
		return;

	}

	new_node->previous = endD;
	endD->next = new_node;
	endD = new_node;

}

void init(int *v) {
	int value;
	srand(time(NULL));
	for (int i = 0; i < N; i++) {

		value = rand() % 100;
		v[i] = value;
		pushS(value);
		pushD(value);

	}
}

//vector QuickSort


void interchange(int *a, int *b) {

	int aux;
	aux = *a;
	*a = *b;
	*b = aux;

}

int Partition(int *v, int l, int r) {

	int pivot = v[r], pIndex = l;
	for (int i = l; i < r; i++) {
		if (v[i] < pivot) {
			interchange(v+i, v+pIndex);
			pIndex++;
		}
	}

	interchange(v+pIndex, v+r);
	return pIndex;

}

void QuickSort_Vector(int *v, int l, int r) {

	if (l >= r) return;
	int pIndex = Partition(v, l, r);
	QuickSort_Vector(v, l, pIndex - 1);
	QuickSort_Vector(v, pIndex + 1, r);

}

//Slist QuickSort

void change_Slist(struct nodeS*a, struct nodeS*b) {

	int temp = a->data;
	a->data = b->data;
	b->data = temp;

}

int Slist_partition(int start, int end) {

	struct nodeS*pivot = headS;
	for (int i = 1; i <= end; i++)
		pivot = pivot->next;

	struct nodeS*node = headS;
	for (int i = 1; i <= start; i++)
		node = node->next;

	struct nodeS*pIndex = node;
	int counter = start;

	for (int i = start; i < end; i++, node = node->next) {

		if (node->data < pivot->data) {

			change_Slist(node, pIndex);
			pIndex = pIndex->next;
			counter++;

		}

	}

	change_Slist(pIndex, pivot);
	return counter;
}

void QuickSort_Slist(int start, int end) {

	if (start >= end) return;
	int pIndex= Slist_partition(start, end);
	QuickSort_Slist(start, pIndex - 1);
	QuickSort_Slist(pIndex + 1, end);

}

//Dlist QuickSort

void change_Dlist(struct nodeD*a, struct nodeD*b) {

	int temp = a->data;
	a->data = b->data;
	b->data = temp;

}

int Dlist_partition(int start, int end) {

	struct nodeD*pivot = headD;
	for (int i = 1; i <= end; i++)
		pivot = pivot->next;

	struct nodeD*node = headD;
	for (int i = 1; i <= start; i++)
		node = node->next;

	struct nodeD*pIndex = node;
	int counter = start;

	for (int i = start; i < end; i++, node = node->next) {

		if (node->data < pivot->data) {

			change_Dlist(node, pIndex);
			pIndex = pIndex->next;
			counter++;

		}

	}

	change_Dlist(pIndex, pivot);
	return counter;
}

void QuickSort_Dlist(int start, int end) {

	if (start >= end) return;
	int pIndex = Dlist_partition(start, end);
	QuickSort_Dlist(start, pIndex - 1);
	QuickSort_Dlist(pIndex + 1, end);

}

clock_t v_start, v_end;
clock_t ls_start, ls_end;
clock_t ld_start, ld_end;
int v_elapse, ls_elapse, ld_elapse;

WNDCLASSW main, vls, vld, lsld, vlsld;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PWSTR pCmdLine, int nCmdShow) {

	int v[N];
	init(v);

	v_start = clock();
	QuickSort_Vector(v, 0, N - 1);
	v_end = clock();
	v_elapse = v_end - v_start;

	ls_start = clock();
	QuickSort_Slist(0,N-1);
	ls_end = clock();
	ls_elapse = ls_end - ls_start;

	ld_start = clock();
	QuickSort_Dlist(0, N - 1);
	ld_end = clock();
	ld_elapse = ld_end - ld_start;

	MSG  msg;
	HWND hwnd;

	//main
	main.style = CS_HREDRAW | CS_VREDRAW;
	main.cbClsExtra = 0;
	main.cbWndExtra = 0;
	main.lpszClassName = L"Main";
	main.hInstance = hInstance;
	main.hbrBackground = CreateSolidBrush(RGB(255, 255, 102));
	main.lpszMenuName = NULL;
	main.lpfnWndProc = Main;
	main.hCursor = LoadCursor(NULL, IDC_ARROW);
	main.hIcon = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassW(&main);
	hwnd = CreateWindowW(main.lpszClassName, L"QuickSort",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		0, 0, 1366, 768, NULL, NULL, hInstance, NULL);

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	//Vectori cu LS

	vls.style = CS_HREDRAW | CS_VREDRAW;
	vls.cbClsExtra = 0;
	vls.cbWndExtra = 0;
	vls.lpszClassName = L"VLS";
	vls.hInstance = hInstance;
	vls.hbrBackground = CreateSolidBrush(RGB(135, 206, 235));
	vls.lpszMenuName = NULL;
	vls.lpfnWndProc = VLS;
	vls.hCursor = LoadCursor(NULL, IDC_ARROW);
	vls.hIcon = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassW(&vls);

	//vectori cu ld

	vld.style = CS_HREDRAW | CS_VREDRAW;
	vld.cbClsExtra = 0;
	vld.cbWndExtra = 0;
	vld.lpszClassName = L"VLD";
	vld.hInstance = hInstance;
	vld.hbrBackground = CreateSolidBrush(RGB(135, 206, 235));
	vld.lpszMenuName = NULL;
	vld.lpfnWndProc = VLD;
	vld.hCursor = LoadCursor(NULL, IDC_ARROW);
	vld.hIcon = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassW(&vld);

	//LS cu LD

	lsld.style = CS_HREDRAW | CS_VREDRAW;
	lsld.cbClsExtra = 0;
	lsld.cbWndExtra = 0;
	lsld.lpszClassName = L"LSLD";
	lsld.hInstance = hInstance;
	lsld.hbrBackground = CreateSolidBrush(RGB(135, 206, 235));
	lsld.lpszMenuName = NULL;
	lsld.lpfnWndProc = LSLD;
	lsld.hCursor = LoadCursor(NULL, IDC_ARROW);
	lsld.hIcon = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassW(&lsld);

	//vectori ls ld

	vlsld.style = CS_HREDRAW | CS_VREDRAW;
	vlsld.cbClsExtra = 0;
	vlsld.cbWndExtra = 0;
	vlsld.lpszClassName = L"VLSLD";
	vlsld.hInstance = hInstance;
	vlsld.hbrBackground = CreateSolidBrush(RGB(135, 206, 235));
	vlsld.lpszMenuName = NULL;
	vlsld.lpfnWndProc = VLSLD;
	vlsld.hCursor = LoadCursor(NULL, IDC_ARROW);
	vlsld.hIcon = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassW(&vlsld);



	while (main_closed==false){
		
		GetMessage(&msg, NULL, 0, 0);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

void Choose_graph(POINT p) {

	if (p.x > 200 && p.x < 600 && p.y>50 && p.y < 150) {
		CreateWindowW(vls.lpszClassName, L"VLS",
			WS_OVERLAPPEDWINDOW | WS_VISIBLE,
			0, 0, 1366, 768, NULL, NULL, ghostInst, NULL);
	}
	if (p.x > 200 && p.x < 600 && p.y>200 && p.y < 300) {
		CreateWindowW(vld.lpszClassName, L"VLD",
			WS_OVERLAPPEDWINDOW | WS_VISIBLE,
			0, 0, 1366, 768, NULL, NULL, ghostInst, NULL);
	}
	if (p.x > 200 && p.x < 600 && p.y>350 && p.y < 450) {
		CreateWindowW(lsld.lpszClassName, L"LSLD",
			WS_OVERLAPPEDWINDOW | WS_VISIBLE,
			0, 0, 1366, 768, NULL, NULL, ghostInst, NULL);
	}
	if (p.x > 200 && p.x < 600 && p.y>500 && p.y < 600) {
		CreateWindowW(vlsld.lpszClassName, L"VLSLD",
			WS_OVERLAPPEDWINDOW | WS_VISIBLE,
			0, 0, 1366, 768, NULL, NULL, ghostInst, NULL);
	}

}

void Menu(HWND hwnd) {

	PAINTSTRUCT ps;

	DWORD color;
	HFONT hFont,holdFont;

	HDC hdc = BeginPaint(hwnd, &ps);

	hFont = CreateFontW(70, 0, 0, 0, FW_MEDIUM, 0, 0, 0, 0,
		0, 0, 0, 0, L"Font");
	holdFont = SelectObject(hdc, hFont);

	static wchar_t *title = L"QuickSort";

	SetBkColor(hdc, RGB(255, 255, 102));
	TextOutW(hdc, 800, 50, title, lstrlenW(title));

	HBRUSH hBrush = CreateSolidBrush(RGB(124, 252, 0));
	HBRUSH holdBrush = SelectObject(hdc, hBrush);

	SetBkColor(hdc, RGB(124, 252, 0));
	hFont = CreateFontW(30, 0, 0, 0, FW_MEDIUM, 0, 0, 0, 0,
		0, 0, 0, 0, L"Font");
	holdFont = SelectObject(hdc, hFont);

	Rectangle(hdc, 200, 50, 600, 150);
	Rectangle(hdc, 200, 200, 600, 300);
	Rectangle(hdc, 200, 350, 600, 450);
	Rectangle(hdc, 200, 500, 600, 600);

	static wchar_t *VLS = L"Vectori si liste simple";
	static wchar_t *VLD = L"Vectori si liste duble";
	static wchar_t *LSLD = L"Liste simple si liste duble";
	static wchar_t *VLSLD = L"Vectori,liste simple si liste duble";

	TextOutW(hdc, 280, 90, VLS, lstrlenW(VLS));
	TextOutW(hdc, 280, 240, VLD, lstrlenW(VLD));
	TextOutW(hdc, 270, 390, LSLD, lstrlenW(LSLD));
	TextOutW(hdc, 220, 540, VLSLD, lstrlenW(VLSLD));
	EndPaint(hwnd, &ps);

}

LRESULT CALLBACK Main(HWND hwnd, UINT msg,
	WPARAM wParam, LPARAM lParam) {

	POINT p;

	switch (msg) {

	case WM_LBUTTONDOWN:

		GetCursorPos(&p);
		Choose_graph(p);
		break;

	case WM_PAINT:
		Menu(hwnd);
		break;

	case WM_DESTROY:

		main_closed = true;
		PostQuitMessage(0);
		break;
	}

	return DefWindowProcW(hwnd, msg, wParam, lParam);
}

void delay() {
	for (int i = 0; i < 30000; i++);
}

void Draw_hist(HDC hdc,int x_s, int y_s, int x_e, int y_e) { ///////////

		for (int y = y_s-1; y > y_e; y--) {
			for (int x = x_s+1; x < x_e; x++){
				SetPixel(hdc, x, y, RGB(255, 0, 0));
				delay();
		}
	}
}

void Coordinates(HWND hwnd, int child) {

	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hwnd, &ps);

	MoveToEx(hdc, 150, 600, NULL);
	LineTo(hdc, 150, 100);
	MoveToEx(hdc, 150, 600, NULL);
	LineTo(hdc, 1000, 600);

	MoveToEx(hdc, 150, 100, NULL);
	LineTo(hdc, 170, 120);
	MoveToEx(hdc, 150, 100, NULL);
	LineTo(hdc, 130, 120);
	MoveToEx(hdc, 1000, 600, NULL);
	LineTo(hdc, 980, 580);
	MoveToEx(hdc, 1000, 600, NULL);
	LineTo(hdc, 980, 620);

	DWORD color;
	HFONT hFont, holdFont;
	hFont = CreateFontW(50, 0, 0, 0, FW_MEDIUM, 0, 0, 0, 0,
		0, 0, 0, 0, L"Font");
	holdFont = SelectObject(hdc, hFont);
	SetBkColor(hdc, RGB(135, 206, 235));

	static wchar_t *time = L"time";
	static wchar_t *data = L"data";

	TextOutW(hdc, 110, 50, time, lstrlenW(time));
	TextOutW(hdc, 1010, 570, data, lstrlenW(data));

	static wchar_t *vector = L"vector";
	static wchar_t *ls = L"Liste simplu";
	static wchar_t *ld = L"Liste dublu";

	//histograms

	switch (child) {
	case 1:
		if (v_elapse == 0) Draw_hist(hdc,300, 600, 350, 599);
		else Draw_hist(hdc, 400, 600, 450, 600 - v_elapse);
		Draw_hist(hdc, 700, 600, 750, 600 - ls_elapse);

		TextOutW(hdc, 380, 620, vector, lstrlenW(vector));
		TextOutW(hdc, 620, 620, ls, lstrlenW(ls));

		break;
	case 2:
		if (v_elapse == 0) Draw_hist(hdc, 300, 600, 350, 599);
		else Draw_hist(hdc, 300, 600, 350, 600 - v_elapse);
		Draw_hist(hdc, 700, 600, 750, 600 - ld_elapse);

		TextOutW(hdc, 280, 620, vector, lstrlenW(vector));
		TextOutW(hdc, 620, 620, ld, lstrlenW(ld));

		break;
	case 3:
		Draw_hist(hdc, 300, 600, 350, 600 - ls_elapse);
		Draw_hist(hdc, 700, 600, 750, 600 - ld_elapse);

		TextOutW(hdc, 220, 620, ls, lstrlenW(ls));
		TextOutW(hdc, 620, 620, ld, lstrlenW(ld));

		break;
	case 4:
		if (v_elapse == 0) Draw_hist(hdc, 300, 600, 350, 599);
		else Draw_hist(hdc, 300, 600, 350, 600 - v_elapse);
		Draw_hist(hdc, 600, 600, 650, 600 - ls_elapse);
		Draw_hist(hdc, 900, 600, 950, 600 - ld_elapse);

		TextOutW(hdc, 270, 620, vector, lstrlenW(vector));
		TextOutW(hdc, 520, 620, ls, lstrlenW(ls));
		TextOutW(hdc, 830, 620, ld, lstrlenW(ld));

		break;
	}



	EndPaint(hwnd, &ps);

}

LRESULT CALLBACK VLS(HWND hwnd, UINT msg,
	WPARAM wParam, LPARAM lParam) {

	switch (msg) {

	case WM_PAINT:
		Coordinates(hwnd, 1);
		break;

	case WM_DESTROY:
		
		PostQuitMessage(0);
		break;
	}

	return DefWindowProcW(hwnd, msg, wParam, lParam);
}

LRESULT CALLBACK VLD(HWND hwnd, UINT msg,
	WPARAM wParam, LPARAM lParam) {

	switch (msg) {

	case WM_PAINT:
		Coordinates(hwnd, 2);
		break;

	case WM_DESTROY:

		PostQuitMessage(0);
		break;
	}

	return DefWindowProcW(hwnd, msg, wParam, lParam);
}

LRESULT CALLBACK LSLD(HWND hwnd, UINT msg,
	WPARAM wParam, LPARAM lParam) {

	switch (msg) {

	case WM_PAINT:
		Coordinates(hwnd, 3);
		break;

	case WM_DESTROY:

		PostQuitMessage(0);
		break;
	}

	return DefWindowProcW(hwnd, msg, wParam, lParam);
}

LRESULT CALLBACK VLSLD(HWND hwnd, UINT msg,
	WPARAM wParam, LPARAM lParam) {

	switch (msg) {

	case WM_PAINT:
		Coordinates(hwnd, 4);
		break;

	case WM_DESTROY:

		PostQuitMessage(0);
		break;
	}

	return DefWindowProcW(hwnd, msg, wParam, lParam);
}