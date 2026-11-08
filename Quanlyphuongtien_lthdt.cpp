#define NOMINMAX
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#ifdef _WIN32
#include <windows.h>
#else
#include <sys/ioctl.h>
#include <unistd.h>
#endif
using namespace std;


const string MAU_RESET = "\033[0m", MAU_DAM = "\033[1m";
const string MAU_DO = "\033[91m", MAU_XANHLA = "\033[92m", MAU_VANG = "\033[93m";
const string MAU_XANHDUONG = "\033[94m", MAU_HONG = "\033[95m", MAU_XANHNGOC = "\033[96m";
const string MAU_TRANG = "\033[97m", MAU_XAM = "\033[90m";

const int RONG_BANG = 114;                                       
const int RONG_THE = 66;                                        
const int SO_COT = 8;                                            
const int DO_RONG_COT[SO_COT] = {7, 9, 14, 14, 8, 11, 22, 20};   
const int MAX = 1000;                                         

void KhoiTaoConsole();
int RongConsole();
void XoaManHinh();

int DoDai(const string &s);
string CatChuoi(const string &s, int n);
string RutGon(const string &s, int w);
string Lap(const string &s, int n);
string CanTrai(const string &s, int w);
string CanPhai(const string &s, int w);
string CanGiua(const string &s, int w);
string LeTrai(int w);
void InGiua(const string &s);
string HoaAscii(string s);
string ThuongAscii(string s);
string Trim(const string &s);
string DinhDangTien(long long v);

void ThongBaoLoi(const string &m);
void ThongBaoOk(const string &m);
void ThongBaoInfo(const string &m);
void InTieuDe(const string &m);
void TamDung();
void TamBiet();

string NhapDong(const string &nhan);
string NhapChuoi(const string &nhan, const string &loiRong);
int NhapSo(const string &nhan, int lo, int hi, const string &thongBao = "");
double NhapSoThuc(const string &nhan, double lo, double hi);

void MoThe(const string &tieuDe);
void DongThe(const string &nhan, const string &giaTri, const string &mau = MAU_TRANG);
void NganThe();
void DayThe();

void Banner();
void Menu();

struct Moc
{
    long long tuKm;
    long long phi;
    string nhan;
};

const int SO_MOC_XE_MAY = 6;
const Moc BANG_XE_MAY[SO_MOC_XE_MAY] = {
    {1000,  0,       "1.000 km (lan dau)"},
    {3000,  100000,  "3.000 km hoac 3 thang"},
    {6000,  150000,  "6.000 km"},
    {12000, 400000,  "12.000 km"},
    {18000, 800000,  "18.000-24.000 km"},
    {36000, 1500000, "36.000 km tro len"}
};

const int SO_MOC_O_TO = 6;
const Moc BANG_O_TO[SO_MOC_O_TO] = {
    {5000,  800000,  "5.000 km"},
    {10000, 1200000, "10.000 km"},
    {20000, 2000000, "20.000 km"},
    {40000, 4000000, "40.000 km"},
    {60000, 3500000, "60.000 km"},
    {80000, 8000000, "80.000-100.000 km"}
};

const int SO_MOC_XE_TAI = 4;
const Moc BANG_XE_TAI[SO_MOC_XE_TAI] = {
    {5000,  1000000, "5.000 km"},
    {10000, 1800000, "10.000 km"},
    {20000, 3000000, "20.000 km"},
    {40000, 7000000, "40.000 km"}
};

const int SO_KIEU_DANG = 5;
const string DANH_SACH_KIEU_DANG[SO_KIEU_DANG] = {"Sedan", "Hatchback", "SUV", "MPV", "Ban tai"};

class PhuongTien
{
protected:
    string id, bienSo, hang;
    int namSX;
    long long kmDaDi;

    void InBaoTri() const
    {
        NganThe();
        if (!CanBaoDuong()) {
            DongThe("Moc bao duong", "Chua can bao duong (duoi " + DinhDangTien(NguongBaoDuong()) + " km)", MAU_XAM);
            DongThe("PHI BAO TRI", "0 VND", MAU_DAM + MAU_XANHLA);
        }
        else {
            DongThe("Moc bao duong", LayMoc(MocHienTai()).nhan);
            DongThe("PHI BAO TRI", DinhDangTien(TinhPhiBaoTri()) + " VND", MAU_DAM + MAU_XANHLA);
        }
        DayThe();
    }

public:
    static const int NAM_MIN = 1900, NAM_MAX = 2026;
    static string ThongBaoNam() { return "Nam san xuat phai nam trong khoang tu 1900 den 2026."; }

    PhuongTien()
    {
        id = "";
        bienSo = "";
        hang = "";
        namSX = NAM_MAX;
        kmDaDi = 0;
    }

    PhuongTien(const string &i, const string &bs, const string &h, int nam, long long km)
    {
        id = i;
        bienSo = bs;
        hang = h;
        namSX = nam;
        kmDaDi = km;
    }

    PhuongTien(const PhuongTien &p)
    {
        id = p.id;
        bienSo = p.bienSo;
        hang = p.hang;
        namSX = p.namSX;
        kmDaDi = p.kmDaDi;
    }

    virtual ~PhuongTien() {}

    string GetId() const        { return id; }
    string GetBienSo() const    { return bienSo; }
    string GetHang() const      { return hang; }
    int GetNamSX() const        { return namSX; }
    long long GetKmDaDi() const { return kmDaDi; }

    void SetId(const string &v)     { id = v; }
    void SetBienSo(const string &v) { bienSo = v; }
    void SetHang(const string &v)   { hang = v; }
    void SetNamSX(int v)            { namSX = v; }
    void SetKmDaDi(long long v)     { kmDaDi = v; }

    long long NguongBaoDuong() const { return LayMoc(0).tuKm; }
    bool CanBaoDuong() const         { return kmDaDi >= NguongBaoDuong(); }

    int MocHienTai() const
    {
        int kq = -1;
        for (int i = 0; i < SoMoc(); i++) {
            if (kmDaDi >= LayMoc(i).tuKm) kq = i;
        }
        return kq;
    }

    virtual long long TinhPhiBaoTri() const
    {
        int k = MocHienTai();
        if (k == -1) return 0;
        return LayMoc(k).phi;
    }

    virtual void HienThiThongTin() const
    {
        MoThe("CHI TIET PHUONG TIEN");
        DongThe("Loai xe", GetLoai(), MauSac() + MAU_DAM);
        DongThe("Ma ID", id);
        DongThe("Bien so", bienSo, MAU_DAM + MAU_TRANG);
        DongThe("Hang san xuat", hang);
        DongThe("Nam san xuat", to_string(namSX));
        DongThe("Km da di", DinhDangTien(kmDaDi) + " km");
    }

    virtual void Nhap() = 0;
    virtual void Xuat(ostream &os) const = 0;
    virtual void NhapThongSoRieng() = 0;      
    virtual string GetLoai() const = 0;
    virtual string ThongSoRieng() const = 0;
    virtual string MauSac() const = 0;
    virtual int SoMoc() const = 0;        
    virtual Moc LayMoc(int i) const = 0;   

    friend istream& operator>>(istream &is, PhuongTien &p)
    {
        p.Nhap();
        return is;
    }

    friend ostream& operator<<(ostream &os, const PhuongTien &p)
    {
        p.Xuat(os);
        return os;
    }
};

class XeMay : public PhuongTien
{
    int congSuat;

public:
    XeMay() : PhuongTien()
    {
        congSuat = 110;
    }

    XeMay(const string &i, const string &bs, const string &h, int nam, long long km, int cs)
        : PhuongTien(i, bs, h, nam, km)
    {
        congSuat = cs;
    }

    XeMay(const XeMay &x) : PhuongTien(x)
    {
        congSuat = x.congSuat;
    }

    int GetCongSuat() const { return congSuat; }
    void SetCongSuat(int v) { congSuat = v; }

    string GetLoai() const       { return "Xe may"; }
    string MauSac() const        { return MAU_VANG; }
    string ThongSoRieng() const  { return to_string(congSuat) + " cc"; }
    int SoMoc() const            { return SO_MOC_XE_MAY; }
    Moc LayMoc(int i) const      { return BANG_XE_MAY[i]; }

    void NhapThongSoRieng()
    {
        SetCongSuat(NhapSo("Cong suat (cc)", 1, 2000));
    }

    void Nhap()
    {
        SetBienSo(HoaAscii(NhapChuoi("Bien so (VD: 59X1-12345)", "Bien so khong duoc de trong.")));
        SetHang(NhapChuoi("Hang san xuat", "Hang san xuat khong duoc de trong."));
        SetNamSX(NhapSo("Nam san xuat (<= 2026)", NAM_MIN, NAM_MAX, ThongBaoNam()));
        SetKmDaDi(NhapSo("So km da di", 0, 10000000));
        NhapThongSoRieng();
    }

    void Xuat(ostream &os) const
    {
        os << "[" << GetId() << "] " << GetLoai() << " - BS: " << left << setw(11) << GetBienSo() << " - "
           << GetHang() << " (" << GetNamSX() << ") - " << GetKmDaDi() << " km - " << GetCongSuat() << " cc";
    }

    friend istream& operator>>(istream &is, XeMay &x)
    {
        x.Nhap();
        return is;
    }

    friend ostream& operator<<(ostream &os, const XeMay &x)
    {
        x.Xuat(os);
        return os;
    }

    void HienThiThongTin() const
    {
        PhuongTien::HienThiThongTin();
        DongThe("Cong suat", to_string(congSuat) + " cc");
        InBaoTri();
    }
};

class XeOto : public PhuongTien
{
    int soCho;
    string kieuDang;

public:
    XeOto() : PhuongTien()
    {
        soCho = 5;
        kieuDang = "Sedan";
    }

    XeOto(const string &i, const string &bs, const string &h, int nam, long long km, int sc, const string &kd)
        : PhuongTien(i, bs, h, nam, km)
    {
        soCho = sc;
        kieuDang = kd;
    }

    XeOto(const XeOto &x) : PhuongTien(x)
    {
        soCho = x.soCho;
        kieuDang = x.kieuDang;
    }

    int GetSoCho() const         { return soCho; }
    string GetKieuDang() const   { return kieuDang; }
    void SetSoCho(int v)         { soCho = v; }
    void SetKieuDang(const string &v) { kieuDang = v; }

    string GetLoai() const       { return "O to"; }
    string MauSac() const        { return MAU_XANHLA; }
    string ThongSoRieng() const  { return to_string(soCho) + " cho - " + kieuDang; }
    int SoMoc() const            { return SO_MOC_O_TO; }
    Moc LayMoc(int i) const      { return BANG_O_TO[i]; }

    void NhapThongSoRieng()
    {
        SetSoCho(NhapSo("So cho ngoi (2-50)", 2, 50));

        ThongBaoInfo(MAU_XAM + "Kieu dang:" + MAU_RESET);
        for (int i = 0; i < SO_KIEU_DANG; i++) {
            ThongBaoInfo(MAU_VANG + "[" + to_string(i + 1) + "]" + MAU_RESET + " " + DANH_SACH_KIEU_DANG[i]);
        }
        int c = NhapSo("Chon kieu dang", 1, SO_KIEU_DANG);
        SetKieuDang(DANH_SACH_KIEU_DANG[c - 1]);
    }

    void Nhap()
    {
        SetBienSo(HoaAscii(NhapChuoi("Bien so (VD: 59X1-12345)", "Bien so khong duoc de trong.")));
        SetHang(NhapChuoi("Hang san xuat", "Hang san xuat khong duoc de trong."));
        SetNamSX(NhapSo("Nam san xuat (<= 2026)", NAM_MIN, NAM_MAX, ThongBaoNam()));
        SetKmDaDi(NhapSo("So km da di", 0, 10000000));
        NhapThongSoRieng();
    }

    void Xuat(ostream &os) const
    {
        os << "[" << GetId() << "] " << GetLoai() << " - BS: " << left << setw(11) << GetBienSo() << " - "
           << GetHang() << " (" << GetNamSX() << ") - " << GetKmDaDi() << " km - "
           << GetSoCho() << " cho - " << GetKieuDang();
    }

    friend istream& operator>>(istream &is, XeOto &x)
    {
        x.Nhap();
        return is;
    }

    friend ostream& operator<<(ostream &os, const XeOto &x)
    {
        x.Xuat(os);
        return os;
    }

    void HienThiThongTin() const
    {
        PhuongTien::HienThiThongTin();
        DongThe("So cho ngoi", to_string(soCho) + " cho");
        DongThe("Kieu dang", kieuDang);
        InBaoTri();
    }
};

class XeTai : public PhuongTien
{
    double taiTrong;

    static string DinhDangTai(double v)
    {
        ostringstream ss;
        ss << fixed << setprecision(1) << v;
        return ss.str();
    }

public:
    XeTai() : PhuongTien()
    {
        taiTrong = 1.0;
    }

    XeTai(const string &i, const string &bs, const string &h, int nam, long long km, double tt)
        : PhuongTien(i, bs, h, nam, km)
    {
        taiTrong = tt;
    }

    XeTai(const XeTai &x) : PhuongTien(x)
    {
        taiTrong = x.taiTrong;
    }

    double GetTaiTrong() const { return taiTrong; }
    void SetTaiTrong(double v) { taiTrong = v; }

    string GetLoai() const       { return "Xe tai"; }
    string MauSac() const        { return MAU_HONG; }
    string ThongSoRieng() const  { return "Tai " + DinhDangTai(taiTrong) + " tan"; }
    int SoMoc() const            { return SO_MOC_XE_TAI; }
    Moc LayMoc(int i) const      { return BANG_XE_TAI[i]; }

    void NhapThongSoRieng()
    {
        SetTaiTrong(NhapSoThuc("Tai trong (tan)", 0.1, 100));
    }

    void Nhap()
    {
        SetBienSo(HoaAscii(NhapChuoi("Bien so (VD: 59X1-12345)", "Bien so khong duoc de trong.")));
        SetHang(NhapChuoi("Hang san xuat", "Hang san xuat khong duoc de trong."));
        SetNamSX(NhapSo("Nam san xuat (<= 2026)", NAM_MIN, NAM_MAX, ThongBaoNam()));
        SetKmDaDi(NhapSo("So km da di", 0, 10000000));
        NhapThongSoRieng();
    }

    void Xuat(ostream &os) const
    {
        os << "[" << GetId() << "] " << GetLoai() << " - BS: " << left << setw(11) << GetBienSo() << " - "
           << GetHang() << " (" << GetNamSX() << ") - " << GetKmDaDi() << " km - "
           << "Tai " << DinhDangTai(GetTaiTrong()) << " tan";
    }

    friend istream& operator>>(istream &is, XeTai &x)
    {
        x.Nhap();
        return is;
    }

    friend ostream& operator<<(ostream &os, const XeTai &x)
    {
        x.Xuat(os);
        return os;
    }

    void HienThiThongTin() const
    {
        PhuongTien::HienThiThongTin();
        DongThe("Tai trong", DinhDangTai(taiTrong) + " tan");
        InBaoTri();
    }
};

string KeNgang(const string &le, const string &trai, const string &giua, const string &phai, const int w[], int n)
{
    string s = trai;
    for (int i = 0; i < n; i++) {
        s += Lap("─", w[i]);
        if (i < n - 1) s += giua;
        else s += phai;
    }
    return le + MAU_XANHNGOC + s + MAU_RESET + "\n";
}

string OBang(const string &chu, int rong, const string &mau, char canh)
{
    int w = rong - 2;
    string t = RutGon(chu, w);
    if (canh == 'r') t = CanPhai(t, w);
    else if (canh == 'c') t = CanGiua(t, w);
    else t = CanTrai(t, w);
    return " " + mau + t + MAU_RESET + " ";
}

void VeBang(PhuongTien *const v[], int sl, const string &tieuDe)
{
    const string m = LeTrai(RONG_BANG);
    const string vien = MAU_XANHNGOC + "│" + MAU_RESET;

    InGiua(MAU_XANHNGOC + MAU_DAM + "=== " + tieuDe + " (" + to_string(sl) + " xe) ===" + MAU_RESET);
    cout << KeNgang(m, "╭", "┬", "╮", DO_RONG_COT, SO_COT);

    const string tenCot[SO_COT] = {"ID", "Loai xe", "Bien so", "Hang", "Nam SX", "Km da di", "Thong so rieng", "Phi bao tri"};
    const char canh[SO_COT] = {'l', 'l', 'l', 'l', 'c', 'r', 'l', 'r'};
    cout << m << vien;
    for (int i = 0; i < SO_COT; i++) {
        cout << OBang(tenCot[i], DO_RONG_COT[i], MAU_DAM + MAU_HONG, canh[i]) << vien;
    }
    cout << "\n" << KeNgang(m, "├", "┼", "┤", DO_RONG_COT, SO_COT);

    if (sl == 0) {
        cout << m << vien << MAU_XAM << CanGiua("(Khong co du lieu)", RONG_BANG - 2) << MAU_RESET << vien << "\n";
    }
    for (int k = 0; k < sl; k++) {
        const PhuongTien *p = v[k];
        string phi = "Chua can bao duong";
        string mauPhi = MAU_XAM;
        if (p->CanBaoDuong()) {
            phi = DinhDangTien(p->TinhPhiBaoTri()) + " VND";
            mauPhi = MAU_DAM + MAU_XANHLA;
        }
        cout << m << vien
             << OBang(p->GetId(), DO_RONG_COT[0], MAU_VANG + MAU_DAM, 'l') << vien
             << OBang(p->GetLoai(), DO_RONG_COT[1], p->MauSac() + MAU_DAM, 'l') << vien
             << OBang(p->GetBienSo(), DO_RONG_COT[2], MAU_DAM + MAU_TRANG, 'l') << vien
             << OBang(p->GetHang(), DO_RONG_COT[3], MAU_TRANG, 'l') << vien
             << OBang(to_string(p->GetNamSX()), DO_RONG_COT[4], MAU_TRANG, 'c') << vien
             << OBang(DinhDangTien(p->GetKmDaDi()), DO_RONG_COT[5], MAU_TRANG, 'r') << vien
             << OBang(p->ThongSoRieng(), DO_RONG_COT[6], MAU_TRANG, 'l') << vien
             << OBang(phi, DO_RONG_COT[7], mauPhi, 'r') << vien << "\n";
    }
    cout << KeNgang(m, "╰", "┴", "╯", DO_RONG_COT, SO_COT) << "\n";
}

void VeBangPhi()
{
    const int W[3] = {24, 26, 20};
    const int TONG = W[0] + W[1] + W[2] + 4;
    const string m = LeTrai(TONG);
    const string vien = MAU_XANHNGOC + "│" + MAU_RESET;

    const Moc *bang[3] = {BANG_XE_MAY, BANG_O_TO, BANG_XE_TAI};
    const int soMoc[3] = {SO_MOC_XE_MAY, SO_MOC_O_TO, SO_MOC_XE_TAI};
    const string ten[3] = {"1. XE MAY", "2. O TO", "3. XE TAI"};
    const string mau[3] = {MAU_VANG + MAU_DAM, MAU_XANHLA + MAU_DAM, MAU_HONG + MAU_DAM};

    InGiua(MAU_XANHNGOC + MAU_DAM + "=== BANG CHI PHI BAO DUONG XE DINH KY ===" + MAU_RESET);
    cout << KeNgang(m, "╭", "┬", "╮", W, 3);
    cout << m << vien << OBang("Loai xe", W[0], MAU_DAM + MAU_HONG, 'c') << vien
         << OBang("Moc km", W[1], MAU_DAM + MAU_HONG, 'c') << vien
         << OBang("Chi phi", W[2], MAU_DAM + MAU_HONG, 'c') << vien << "\n";
    cout << KeNgang(m, "├", "┼", "┤", W, 3);

    for (int g = 0; g < 3; g++) {
        int n = soMoc[g];
        int dongNhan = (n - 1) / 2;  
        for (int r = 0; r < n; r++) {
            string tenNhom = "";
            if (r == dongNhan) tenNhom = ten[g];
            cout << m << vien << OBang(tenNhom, W[0], mau[g], 'c') << vien
                 << OBang(bang[g][r].nhan, W[1], MAU_TRANG, 'c') << vien
                 << OBang(DinhDangTien(bang[g][r].phi) + " VND", W[2], MAU_DAM + MAU_XANHLA, 'c') << vien << "\n";
            if (r < n - 1) {
                cout << m << vien << string(W[0], ' ') << MAU_XANHNGOC << "├" << Lap("─", W[1]) << "┼"
                     << Lap("─", W[2]) << "┤" << MAU_RESET << "\n";
            }
        }
        if (g < 2) cout << KeNgang(m, "├", "┼", "┤", W, 3);
    }
    cout << KeNgang(m, "╰", "┴", "╯", W, 3) << "\n";
}

class QuanLyPhuongTien
{
    PhuongTien *ds[MAX]; 
    int n; 
    int demId;  

    string SinhId(const string &tienTo)
    {
        demId++;
        ostringstream ss;
        ss << tienTo << setw(3) << setfill('0') << demId;
        return ss.str();
    }

    int TimViTri(const string &id) const
    {
        for (int i = 0; i < n; i++) {
            if (HoaAscii(ds[i]->GetId()) == HoaAscii(id)) return i;
        }
        return -1;
    }

    bool TrungBienSo(const string &bs, int boQua = -1) const
    {
        for (int i = 0; i < n; i++) {
            if (i != boQua && HoaAscii(ds[i]->GetBienSo()) == HoaAscii(bs)) return true;
        }
        return false;
    }

    int ChonXe() const
    {
        string id = NhapDong("Nhap ID phuong tien (Enter de huy)");
        if (id.empty()) return -1;
        int i = TimViTri(id);
        if (i < 0) ThongBaoLoi("Khong tim thay phuong tien co ID \"" + id + "\".");
        return i;
    }

    void ThemCuoi(PhuongTien *p)
    {
        ds[n] = p;
        n++;
    }

    static bool DungTruoc(const PhuongTien *a, const PhuongTien *b, int kieu)
    {
        switch (kieu) {
            case 1:  return a->TinhPhiBaoTri() > b->TinhPhiBaoTri(); 
            case 2:  return a->TinhPhiBaoTri() < b->TinhPhiBaoTri();
            case 3:  return a->GetNamSX() > b->GetNamSX();     
            case 4:  return a->GetKmDaDi() > b->GetKmDaDi();  
            default: return a->GetHang() < b->GetHang();
        }
    }

public:
    QuanLyPhuongTien()
    {
        n = 0;
        demId = 0;
    }

    QuanLyPhuongTien(int _demID)
    {
        n = 0;
        demId = _demID;
    }

    QuanLyPhuongTien(const QuanLyPhuongTien &a) = delete;
    QuanLyPhuongTien& operator=(const QuanLyPhuongTien &a) = delete;

    ~QuanLyPhuongTien()
    {
        for (int i = 0; i < n; i++) delete ds[i];
    }

    int GetDemId() const   { return demId; }
    void SetDemId(int v)   { demId = v; }
    int GetSoLuong() const { return n; }
    PhuongTien* GetXe(int i) const
    {
        if (i < 0 || i >= n) return NULL;
        return ds[i];
    }

    void VeGiaoDien(bool hienMenu) const
    {
        XoaManHinh();
        Banner();
        VeBang(ds, n, "DANH SACH PHUONG TIEN");
        if (hienMenu) Menu();
    }

    bool Them(PhuongTien *p, const string &tienTo)
    {
        if (n >= MAX) {
            ThongBaoLoi("Danh sach da day, khong the them.");
            delete p;
            return false;
        }
        if (TrungBienSo(p->GetBienSo())) {
            ThongBaoLoi("Bien so da ton tai, khong the them.");
            delete p;
            return false;
        }
        p->SetId(SinhId(tienTo));
        ThemCuoi(p);
        return true;
    }

    void NapMau()
    {
        ThemCuoi(new XeMay(SinhId("XM"), "59X1-12345", "Yamaha", 2022, 8500, 155));
        ThemCuoi(new XeOto(SinhId("OT"), "51A-123.45", "Toyota", 2020, 62000, 5, "Sedan"));
        ThemCuoi(new XeTai(SinhId("XT"), "51C-678.90", "Hino", 2018, 160000, 5.0));
        ThemCuoi(new XeMay(SinhId("XM"), "59P2-67890", "Honda", 2023, 21000, 125));
        ThemCuoi(new XeOto(SinhId("OT"), "30G-555.66", "Hyundai", 2024, 12300, 7, "SUV"));
    }

    void ThemMoi()
    {
        VeGiaoDien(false);
        InTieuDe("THEM PHUONG TIEN MOI");
        ThongBaoInfo(MAU_VANG + "[1]" + MAU_RESET + " Xe may");
        ThongBaoInfo(MAU_XANHLA + "[2]" + MAU_RESET + " O to");
        ThongBaoInfo(MAU_HONG + "[3]" + MAU_RESET + " Xe tai");
        ThongBaoInfo(MAU_DO + "[0]" + MAU_RESET + " Huy");

        int c = NhapSo("Chon loai xe", 0, 3);
        if (c == 0) return;

        PhuongTien *p = NULL;
        string tienTo;
        if (c == 1) {
            XeMay *newOne = new XeMay() ;
            cin >> *newOne;
            p = newOne;
            tienTo = "XM";
        }
        else if (c == 2) {
            XeOto *newOne = new XeOto();
            cin >> *newOne;
            p = newOne;
            tienTo = "OT";
        }
        else {
            XeTai* newOne = new XeTai();
            cin >> *newOne;
            p = newOne;
            tienTo = "XT";
        }
        if (Them(p, tienTo)) ThongBaoOk("Da them: " + p->GetId() + " - " + p->GetBienSo());
    }

    void CapNhat()
    {
        VeGiaoDien(false);
        InTieuDe("CAP NHAT THONG TIN");
        int i = ChonXe();
        if (i < 0) return;
        PhuongTien *p = ds[i];

        cout << "\n";
        p->HienThiThongTin();
        cout << "\n";
        ThongBaoInfo(MAU_VANG + "[1]" + MAU_RESET + " Bien so");
        ThongBaoInfo(MAU_VANG + "[2]" + MAU_RESET + " Hang");
        ThongBaoInfo(MAU_VANG + "[3]" + MAU_RESET + " Nam SX");
        ThongBaoInfo(MAU_VANG + "[4]" + MAU_RESET + " Km da di");
        ThongBaoInfo(MAU_VANG + "[5]" + MAU_RESET + " Thong so rieng");
        ThongBaoInfo(MAU_DO + "[0]" + MAU_RESET + " Huy");

        int c = NhapSo("Chon muc can sua", 0, 5);
        switch (c) {
            case 1:
                while (true) {
                    string bs = HoaAscii(NhapChuoi("Bien so moi", "Bien so khong duoc de trong."));
                    if (TrungBienSo(bs, i)) {
                        ThongBaoLoi("Bien so da ton tai.");
                        continue;
                    }
                    p->SetBienSo(bs);
                    break;
                }
                break;
            case 2:
                p->SetHang(NhapChuoi("Hang moi", "Hang san xuat khong duoc de trong."));
                break;
            case 3:
                p->SetNamSX(NhapSo("Nam san xuat moi (<= 2026)", PhuongTien::NAM_MIN, PhuongTien::NAM_MAX,
                                   PhuongTien::ThongBaoNam()));
                break;
            case 4:
                p->SetKmDaDi(NhapSo("So km da di moi", 0, 10000000));
                break;
            case 5:
                p->NhapThongSoRieng();
                break;
            default:
                return;
        }

        string phiMoi = "chua can bao duong";
        if (p->CanBaoDuong()) phiMoi = DinhDangTien(p->TinhPhiBaoTri()) + " VND";
        ThongBaoOk("Cap nhat thanh cong! Phi bao tri moi: " + phiMoi);
    }

    void Xoa()
    {
        VeGiaoDien(false);
        InTieuDe("XOA PHUONG TIEN");
        int i = ChonXe();
        if (i < 0) return;

        cout << LeTrai(RONG_BANG) << " " << *ds[i] << "\n";
        string x = ThuongAscii(NhapDong("Xac nhan xoa? (c = co / k = khong)"));
        if (x == "c" || x == "co" || x == "y") {
            delete ds[i];
            for (int k = i; k < n - 1; k++) ds[k] = ds[k + 1];
            n--;
            ThongBaoOk("Da xoa phuong tien.");
        }
        else {
            ThongBaoInfo(MAU_XAM + "Da huy thao tac xoa.");
        }
    }

    void TimKiem()
    {
        VeGiaoDien(false);
        InTieuDe("TIM KIEM PHUONG TIEN");
        ThongBaoInfo(MAU_VANG + "[1]" + MAU_RESET + " Theo bien so");
        ThongBaoInfo(MAU_VANG + "[2]" + MAU_RESET + " Theo hang");
        ThongBaoInfo(MAU_VANG + "[3]" + MAU_RESET + " Theo loai xe");
        ThongBaoInfo(MAU_DO + "[0]" + MAU_RESET + " Huy");

        int c = NhapSo("Chon kieu tim", 0, 3);
        if (c == 0) return;

        PhuongTien *kq[MAX];
        int sl = 0;
        if (c == 3) {
            ThongBaoInfo(MAU_VANG + "[1]" + MAU_RESET + " Xe may");
            ThongBaoInfo(MAU_XANHLA + "[2]" + MAU_RESET + " O to");
            ThongBaoInfo(MAU_HONG + "[3]" + MAU_RESET + " Xe tai");
            int t = NhapSo("Chon loai xe", 1, 3);

            string loai = "Xe tai";
            if (t == 1) loai = "Xe may";
            else if (t == 2) loai = "O to";

            for (int i = 0; i < n; i++) {
                if (ds[i]->GetLoai() == loai) {
                    kq[sl] = ds[i];
                    sl++;
                }
            }
        }
        else {
            string tuKhoa = ThuongAscii(NhapDong(c == 1 ? "Nhap bien so (hoac mot phan)" : "Nhap ten hang (hoac mot phan)"));
            for (int i = 0; i < n; i++) {
                string s = ThuongAscii(c == 1 ? ds[i]->GetBienSo() : ds[i]->GetHang());
                if (s.find(tuKhoa) != string::npos) {
                    kq[sl] = ds[i];
                    sl++;
                }
            }
        }

        XoaManHinh();
        Banner();
        VeBang(kq, sl, "KET QUA TIM KIEM");
    }

    void SapXep()
    {
        VeGiaoDien(false);
        InTieuDe("SAP XEP DANH SACH");
        ThongBaoInfo(MAU_VANG + "[1]" + MAU_RESET + " Phi bao tri giam dan");
        ThongBaoInfo(MAU_VANG + "[2]" + MAU_RESET + " Phi bao tri tang dan");
        ThongBaoInfo(MAU_VANG + "[3]" + MAU_RESET + " Nam SX moi nhat");
        ThongBaoInfo(MAU_VANG + "[4]" + MAU_RESET + " Km da di giam dan");
        ThongBaoInfo(MAU_VANG + "[5]" + MAU_RESET + " Hang A-Z");
        ThongBaoInfo(MAU_DO + "[0]" + MAU_RESET + " Huy");

        int c = NhapSo("Chon cach sap xep", 0, 5);
        if (c == 0) return;

        for (int i = 1; i < n; i++) {
            PhuongTien *x = ds[i];
            int j = i - 1;
            while (j >= 0 && DungTruoc(x, ds[j], c)) {
                ds[j + 1] = ds[j];
                j--;
            }
            ds[j + 1] = x;
        }

        VeGiaoDien(false);
        ThongBaoOk("Da sap xep danh sach.");
    }

    void XemChiTiet()
    {
        VeGiaoDien(false);
        InTieuDe("XEM CHI TIET PHUONG TIEN");
        int i = ChonXe();
        if (i < 0) return;
        cout << "\n";
        ds[i]->HienThiThongTin(); 
    }

    void ThongKe() const
    {
        XoaManHinh();
        Banner();

        int nm = 0, no = 0, nt = 0;
        long long tong = 0;
        const PhuongTien *tonNhat = NULL;
        for (int i = 0; i < n; i++) {
            const PhuongTien *p = ds[i];
            if (p->GetLoai() == "Xe may") nm++;
            else if (p->GetLoai() == "O to") no++;
            else nt++;

            tong += p->TinhPhiBaoTri();
            if (tonNhat == NULL || p->TinhPhiBaoTri() > tonNhat->TinhPhiBaoTri()) tonNhat = p;
        }

        MoThe("THONG KE PHUONG TIEN");
        DongThe("Tong so xe", to_string(n) + " xe", MAU_DAM + MAU_TRANG);
        DongThe("Xe may", to_string(nm) + " xe", MAU_VANG);
        DongThe("O to", to_string(no) + " xe", MAU_XANHLA);
        DongThe("Xe tai", to_string(nt) + " xe", MAU_HONG);
        NganThe();

        long long trungBinh = 0;
        if (n > 0) trungBinh = tong / n;
        DongThe("Tong phi bao tri", DinhDangTien(tong) + " VND", MAU_DAM + MAU_XANHLA);
        DongThe("Phi trung binh", DinhDangTien(trungBinh) + " VND", MAU_XANHLA);
        if (tonNhat != NULL) {
            DongThe("Ton phi nhat", tonNhat->GetId() + " - " + tonNhat->GetBienSo() + " (" +
                                    DinhDangTien(tonNhat->TinhPhiBaoTri()) + ")", MAU_DO);
        }
        DayThe();
    }

    void XemBangPhi() const
    {
        XoaManHinh();
        Banner();
        VeBangPhi();
    }
};

void Banner()
{
    const int W = RONG_BANG - 2;
    cout << "\n";
    cout << LeTrai(RONG_BANG) << MAU_XANHNGOC << "╭" << Lap("─", W) << "╮" << MAU_RESET << "\n";
    cout << LeTrai(RONG_BANG) << MAU_XANHNGOC << "│" << MAU_RESET << CanGiua("", W)
         << MAU_XANHNGOC << "│" << MAU_RESET << "\n";
    cout << LeTrai(RONG_BANG) << MAU_XANHNGOC << "│" << MAU_RESET
         << CanGiua(MAU_DAM + MAU_VANG + "QUAN LY PHUONG TIEN GIAO THONG" + MAU_RESET, W)
         << MAU_XANHNGOC << "│" << MAU_RESET << "\n";
    cout << LeTrai(RONG_BANG) << MAU_XANHNGOC << "│" << MAU_RESET
         << CanGiua(MAU_XAM + "Xe may | O to | Xe tai" + MAU_RESET, W)
         << MAU_XANHNGOC << "│" << MAU_RESET << "\n";
    cout << LeTrai(RONG_BANG) << MAU_XANHNGOC << "│" << MAU_RESET << CanGiua("", W)
         << MAU_XANHNGOC << "│" << MAU_RESET << "\n";
    cout << LeTrai(RONG_BANG) << MAU_XANHNGOC << "╰" << Lap("─", W) << "╯" << MAU_RESET << "\n\n";
}

void MucMenu(const string &so, const string &ten, const string &mau = MAU_VANG + MAU_DAM)
{
    ThongBaoInfo(mau + so + MAU_RESET + " " + ten);
}

void Menu()
{
    ThongBaoInfo(MAU_HONG + MAU_DAM + "===== MENU CHUC NANG =====" + MAU_RESET);
    MucMenu("[1]", "Them phuong tien moi");
    MucMenu("[2]", "Cap nhat thong tin");
    MucMenu("[3]", "Xoa phuong tien");
    MucMenu("[4]", "Tim kiem phuong tien");
    MucMenu("[5]", "Sap xep danh sach");
    MucMenu("[6]", "Xem chi tiet (da hinh)");
    MucMenu("[7]", "Thong ke & tong phi");
    MucMenu("[8]", "Xem bang phi bao tri");
    MucMenu("[0]", "Thoat chuong trinh", MAU_DO + MAU_DAM);
    cout << "\n";
}

int main()
{
    KhoiTaoConsole();
    QuanLyPhuongTien ql;
    ql.NapMau();

    while (true) {
        ql.VeGiaoDien(true);
        int c = NhapSo("Chon chuc nang (0-8)", 0, 8);

        switch (c) {
            case 1: ql.ThemMoi();     break;
            case 2: ql.CapNhat();     break;
            case 3: ql.Xoa();         break;
            case 4: ql.TimKiem();     break;
            case 5: ql.SapXep();      break;
            case 6: ql.XemChiTiet();  break;
            case 7: ql.ThongKe();     break;
            case 8: ql.XemBangPhi();  break;
            case 0:
                TamBiet();
                return 0;
        }
        TamDung();
    }
}

void KhoiTaoConsole()
{
#ifdef _WIN32
    SetConsoleOutputCP(65001); 
    SetConsoleCP(65001);
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;
    if (GetConsoleMode(h, &mode)) SetConsoleMode(h, mode | 0x0004);
#endif
}

int RongConsole()
{
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
        return csbi.srWindow.Right - csbi.srWindow.Left + 1;
#else
    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0 && w.ws_col > 0) return w.ws_col;
#endif
    return 120;
}

void XoaManHinh()
{
    cout << "\033[2J\033[3J\033[H" << flush;
}

int DoDai(const string &s)
{
    int dem = 0;
    size_t i = 0;
    while (i < s.size()) {
        unsigned char c = (unsigned char)s[i];
        if (c == 0x1B) { 
            while (i < s.size() && s[i] != 'm') i++;
            i++;
            continue;
        }
        if ((c & 0xC0) != 0x80) dem++;        
        i++;
    }
    return dem;
}

string CatChuoi(const string &s, int n)
{
    int dem = 0;
    size_t i = 0;
    for (; i < s.size(); i++) {
        if ((((unsigned char)s[i]) & 0xC0) != 0x80) {
            if (dem == n) break;
            dem++;
        }
    }
    return s.substr(0, i);
}

string RutGon(const string &s, int w)
{
    if (DoDai(s) <= w) return s;
    if (w <= 3) return CatChuoi(s, w);
    return CatChuoi(s, w - 3) + "...";
}

string Lap(const string &s, int n)
{
    string kq;
    for (int i = 0; i < n; i++) kq += s;
    return kq;
}

string CanTrai(const string &s, int w)
{
    int thieu = w - DoDai(s);
    if (thieu < 0) thieu = 0;
    return s + string(thieu, ' ');
}

string CanPhai(const string &s, int w)
{
    int thieu = w - DoDai(s);
    if (thieu < 0) thieu = 0;
    return string(thieu, ' ') + s;
}

string CanGiua(const string &s, int w)
{
    int thieu = w - DoDai(s);
    if (thieu < 0) thieu = 0;
    int trai = thieu / 2;
    return string(trai, ' ') + s + string(thieu - trai, ' ');
}

string LeTrai(int w)
{
    int le = (RongConsole() - w) / 2;
    if (le < 0) le = 0;
    return string(le, ' ');
}

void InGiua(const string &s)
{
    int le = (RongConsole() - DoDai(s)) / 2;
    if (le < 0) le = 0;
    cout << string(le, ' ') << s << "\n";
}

string HoaAscii(string s)
{
    for (size_t i = 0; i < s.size(); i++) s[i] = (char)toupper((unsigned char)s[i]);
    return s;
}

string ThuongAscii(string s)
{
    for (size_t i = 0; i < s.size(); i++) s[i] = (char)tolower((unsigned char)s[i]);
    return s;
}

string Trim(const string &s)
{
    size_t dau = s.find_first_not_of(" \t\r\n");
    if (dau == string::npos) return "";
    size_t cuoi = s.find_last_not_of(" \t\r\n");
    return s.substr(dau, cuoi - dau + 1);
}

string DinhDangTien(long long v)
{
    string s = to_string(v);
    string kq;
    int dem = 0;
    for (int i = (int)s.size() - 1; i >= 0; i--) {
        kq.insert(kq.begin(), s[i]);
        dem++;
        if (dem % 3 == 0 && i > 0) kq.insert(kq.begin(), '.');
    }
    return kq;
}

void ThongBaoLoi(const string &m)
{
    cout << LeTrai(RONG_BANG) << MAU_DO << MAU_DAM << " [!] " << MAU_RESET << MAU_DO << m << MAU_RESET << "\n";
}

void ThongBaoOk(const string &m)
{
    cout << LeTrai(RONG_BANG) << MAU_XANHLA << MAU_DAM << " [OK] " << MAU_RESET << MAU_XANHLA << m << MAU_RESET << "\n";
}

void ThongBaoInfo(const string &m)
{
    cout << LeTrai(RONG_BANG) << " " << m << MAU_RESET << "\n";
}

void InTieuDe(const string &m)
{
    cout << "\n";
    InGiua(MAU_VANG + MAU_DAM + "=== " + m + " ===" + MAU_RESET);
    cout << "\n";
}

void TamBiet()
{
    XoaManHinh();
    cout << "\n\n";
    InGiua(MAU_DAM + MAU_XANHNGOC + "Cam on ban da su dung chuong trinh Quan ly phuong tien giao thong!" + MAU_RESET);
    InGiua(MAU_XAM + "Hen gap lai." + MAU_RESET);
    cout << "\n";
}

void TamDung()
{
    cout << "\n" << LeTrai(RONG_BANG) << MAU_XAM << " Nhan Enter de tiep tuc..." << MAU_RESET << flush;
    string s;
    if (!getline(cin, s)) {   
        TamBiet();
        exit(0);
    }
}

string NhapDong(const string &nhan)
{
    cout << LeTrai(RONG_BANG) << MAU_XANHNGOC << MAU_DAM << " >> " << MAU_RESET << MAU_TRANG << nhan << ": "
         << MAU_VANG << flush;
    string s;
    if (!getline(cin, s)) { 
        TamBiet();
        exit(0);
    }
    cout << MAU_RESET;
    return Trim(s);
}

string NhapChuoi(const string &nhan, const string &loiRong)
{
    while (true) {
        string s = NhapDong(nhan);
        if (!s.empty()) return s;
        ThongBaoLoi(loiRong);
    }
}

int NhapSo(const string &nhan, int lo, int hi, const string &thongBao)
{
    while (true) {
        istringstream ss(NhapDong(nhan));
        long long v = 0;
        char thua;
        if (!(ss >> v) || (ss >> thua)) {
            ThongBaoLoi("Vui long nhap mot so nguyen hop le.");
            continue;
        }
        if (v < lo || v > hi) {
            if (thongBao.empty())
                ThongBaoLoi("Gia tri phai nam trong khoang " + to_string(lo) + " - " + to_string(hi) + ".");
            else
                ThongBaoLoi(thongBao);
            continue;
        }
        return (int)v;
    }
}

double NhapSoThuc(const string &nhan, double lo, double hi)
{
    while (true) {
        string s = NhapDong(nhan);
        for (size_t i = 0; i < s.size(); i++) {
            if (s[i] == ',') s[i] = '.';
        }
        istringstream ss(s);
        double v = 0;
        char thua;
        if (!(ss >> v) || (ss >> thua)) {
            ThongBaoLoi("Vui long nhap mot so hop le.");
            continue;
        }
        if (v < lo || v > hi) {
            ostringstream os;
            os << "Gia tri phai nam trong khoang " << lo << " - " << hi << ".";
            ThongBaoLoi(os.str());
            continue;
        }
        return v;
    }
}

void MoThe(const string &tieuDe)
{
    cout << LeTrai(RONG_THE) << MAU_XANHNGOC << "╭" << Lap("─", RONG_THE - 2) << "╮" << MAU_RESET << "\n";
    cout << LeTrai(RONG_THE) << MAU_XANHNGOC << "│" << MAU_RESET << MAU_DAM << MAU_HONG
         << CanGiua(tieuDe, RONG_THE - 2) << MAU_RESET << MAU_XANHNGOC << "│" << MAU_RESET << "\n";
    cout << LeTrai(RONG_THE) << MAU_XANHNGOC << "├" << Lap("─", RONG_THE - 2) << "┤" << MAU_RESET << "\n";
}

void DongThe(const string &nhan, const string &giaTri, const string &mau)
{
    cout << LeTrai(RONG_THE) << MAU_XANHNGOC << "│" << MAU_RESET << " " << MAU_XANHLA << CanTrai(nhan, 16)
         << MAU_RESET << ": " << mau << CanTrai(RutGon(giaTri, 44), 44) << MAU_RESET << " "
         << MAU_XANHNGOC << "│" << MAU_RESET << "\n";
}

void NganThe()
{
    cout << LeTrai(RONG_THE) << MAU_XANHNGOC << "├" << Lap("─", RONG_THE - 2) << "┤" << MAU_RESET << "\n";
}

void DayThe()
{
    cout << LeTrai(RONG_THE) << MAU_XANHNGOC << "╰" << Lap("─", RONG_THE - 2) << "╯" << MAU_RESET << "\n";
}