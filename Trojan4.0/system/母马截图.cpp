#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <windows.h>
#include <wingdi.h>
//#include <GdiPlus.h>
#pragma comment(lib,"gdi32.lib")
 
void echo(CHAR *str);
int CaptureImage(HWND hWnd, CHAR *dirPath, CHAR *filename);
void Capture();

void Capture(){
	system("md C:\\test");  
	time_t now;
	struct tm *curTime;
	char filename[256];
	int number = 0;
	int i=0;
	for(i=0;i<10;i++){
		now = time(NULL);
		curTime = localtime(&now);
		sprintf(filename,"%d",number++);
	    CaptureImage(GetDesktopWindow(), "C:/test/",filename);  
	}
} 
 
int main()
{
	Capture();
	//system("xcopy C:/1 C:/123.zip");
    return 0; 
}
 
/**
 * �������
 */
void echo(CHAR *str)
{
    MessageBox(NULL, str, NULL, MB_OK);
}
 
/**
 * GDI ��������
 *
 * ���� hwnd   Ҫ�����Ĵ��ھ��
 * ���� dirPath    ��ͼ���Ŀ¼
 * ���� filename ��ͼ����
 */
int CaptureImage(HWND hwnd, CHAR *dirPath, CHAR filename[256])
{
    HANDLE hDIB;
    HANDLE hFile;
    DWORD dwBmpSize;
    DWORD dwSizeofDIB;
    DWORD dwBytesWritten;
    CHAR FilePath[MAX_PATH];
    HBITMAP hbmScreen = NULL;
    BITMAP bmpScreen;
    BITMAPFILEHEADER bmfHeader;
    BITMAPINFOHEADER bi;
    CHAR *lpbitmap;
    INT width = GetSystemMetrics(SM_CXSCREEN);  // ��Ļ��
    INT height = GetSystemMetrics(SM_CYSCREEN);  // ��Ļ��
    HDC hdcScreen = GetDC(NULL); // ȫ��ĻDC
    HDC hdcMemDC = CreateCompatibleDC(hdcScreen); // ���������ڴ�DC
 
    if (!hdcMemDC)
    {
        echo(TEXT("CreateCompatibleDC has failed"));
        goto done;
    }
 
    // ͨ������DC ����һ������λͼ
    hbmScreen = CreateCompatibleBitmap(hdcScreen, width, height);
 
    if (!hbmScreen)
    {
        echo(TEXT("CreateCompatibleBitmap Failed"));
        goto done;
    }
 
    // ��λͼ�鴫�͵����Ǽ��ݵ��ڴ�DC��
    SelectObject(hdcMemDC, hbmScreen);
    if (!BitBlt(
                hdcMemDC,    // Ŀ��DC
                0, 0,        // Ŀ��DC�� x,y ����
                width, height, // Ŀ�� DC �Ŀ��
                hdcScreen,   // ��ԴDC
                0, 0,        // ��ԴDC�� x,y ����
                SRCCOPY))    // ճ����ʽ
    {
        echo(TEXT("BitBlt has failed"));
        goto done;
    }
 
    // ��ȡλͼ��Ϣ������� bmpScreen ��
    GetObject(hbmScreen, sizeof(BITMAP), &bmpScreen);
 
    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = bmpScreen.bmWidth;
    bi.biHeight = bmpScreen.bmHeight;
    bi.biPlanes = 1;
    bi.biBitCount = 32;
    bi.biCompression = BI_RGB;
    bi.biSizeImage = 0;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrUsed = 0;
    bi.biClrImportant = 0;
 
    dwBmpSize = ((bmpScreen.bmWidth * bi.biBitCount + 31) / 32) * 4 * bmpScreen.bmHeight;
 
    // �� 32-bit Windows ϵͳ��, GlobalAlloc �� LocalAlloc ���� HeapAlloc ��װ����
    // handle ָ�����Ĭ�ϵĶ�. ���Կ����� HeapAlloc Ҫ��
    hDIB = GlobalAlloc(GHND, dwBmpSize);
    lpbitmap = (char *)GlobalLock(hDIB);
 
    // ��ȡ����λͼ��λ���ҿ��������һ�� lpbitmap ��.
    GetDIBits(
        hdcScreen,  // �豸�������
        hbmScreen,  // λͼ���
        0,          // ָ�������ĵ�һ��ɨ����
        (UINT)bmpScreen.bmHeight, // ָ��������ɨ������
        lpbitmap,   // ָ����������λͼ���ݵĻ�������ָ��
        (BITMAPINFO *)&bi, // �ýṹ�屣��λͼ�����ݸ�ʽ
        DIB_RGB_COLORS // ��ɫ���ɺ졢�̡�����RGB������ֱ��ֵ����
    );
 
 
    wsprintf(FilePath, "%s\%s.bmp", dirPath, filename);
 
    // ����һ���ļ��������ļ���ͼ
    hFile = CreateFile(
                FilePath,
                GENERIC_WRITE,
                0,
                NULL,
                CREATE_ALWAYS,
                FILE_ATTRIBUTE_NORMAL,
                NULL
            );
 
    // �� ͼƬͷ(headers)�Ĵ�С, ����λͼ�Ĵ�С����������ļ��Ĵ�С
    dwSizeofDIB = dwBmpSize + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
 
    // ���� Offset ƫ����λͼ��λ(bitmap bits)ʵ�ʿ�ʼ�ĵط�
    bmfHeader.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER);
 
    // �ļ���С
    bmfHeader.bfSize = dwSizeofDIB;
 
    // λͼ�� bfType �������ַ��� "BM"
    bmfHeader.bfType = 0x4D42; //BM
 
    dwBytesWritten = 0;
    WriteFile(hFile, (LPSTR)&bmfHeader, sizeof(BITMAPFILEHEADER), &dwBytesWritten, NULL);
    WriteFile(hFile, (LPSTR)&bi, sizeof(BITMAPINFOHEADER), &dwBytesWritten, NULL);
    WriteFile(hFile, (LPSTR)lpbitmap, dwBmpSize, &dwBytesWritten, NULL);
 
    // �������ڴ沢�ͷ�
    GlobalUnlock(hDIB);
    GlobalFree(hDIB);
 
    // �ر��ļ����
    CloseHandle(hFile);
 
    // ������Դ
done:
    DeleteObject(hbmScreen);
    DeleteObject(hdcMemDC);
    ReleaseDC(NULL, hdcScreen);
 
    return 0;
}
