#include "syscall.h"
#include "copyright.h"

int main()
{
    PrintString("Danh sach thanh vien nhom: \n");
    PrintString("Pham Viet Hoa - 18120381\n");
    PrintString("Trinh Van Minh - 1712601\n");
    PrintString("Nguyen Tan Phat - 1712652\n");
    PrintString("Truong Cong Quoc Cuong - 18120299\n");
    PrintString("Ho Minh Tri - 18120249\n\n\n");
    PrintString("****************************************************************\n");
    PrintString("*            Gioi thieu mot so chuong trinh ho tro:            *\n");
    PrintString("* -Chuong trinh ASCII hien thi ma octal-ki tu cua nhung ki tu  *\n");
    PrintString("*doc duoc dua theo bang ma ASCII                               *\n");
    PrintString("* -Chuong trinh sort cho phep nguoi dung nhap mang n so nguyen *\n");
    PrintString("*(n<100), nguoi dung chon mot trong hai cach sap xep tang dan  *\n");
    PrintString("*(nhap 1) va giam dan (nhap 0)                                 *\n");
    PrintString("****************************************************************\n");
    Halt();
    return 0;
}
