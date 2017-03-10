// AesCodeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AesCode.h"
#include "AesCodeDlg.h"

#include "Aes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAesCodeDlg dialog

CAesCodeDlg::CAesCodeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAesCodeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAesCodeDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAesCodeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAesCodeDlg)
	DDX_Control(pDX, IDC_EnDeProg, m_prog);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAesCodeDlg, CDialog)
	//{{AFX_MSG_MAP(CAesCodeDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BAesEn, OnBAesEn)
	ON_BN_CLICKED(IDC_BAesDe, OnBAesDe)
	ON_BN_CLICKED(IDC_BFile, OnBFile)
	ON_BN_CLICKED(IDC_BFileEn, OnBFileEn)
	ON_BN_CLICKED(IDC_BFileDe, OnBFileDe)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAesCodeDlg message handlers

BOOL CAesCodeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	SetDlgItemText(IDC_EAesEn,"");
	EnDe_filename="";
	m_prog.SetRange(0,100);
	m_prog.SetPos(0);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CAesCodeDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CAesCodeDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CAesCodeDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}
////////////////////////////////////////////////////////////////////////////////////////////////
//Aes字符串加密
void CAesCodeDlg::OnBAesEn() 
{
	// TODO: Add your control notification handler code here
	unsigned char inBuff[25],ouBuff[25],Key[25];
	memset(inBuff,0,25);
	memset(ouBuff,0,25);
	GetDlgItemText(IDC_EKey,(char*)Key,24);
	ISD_Aes aes(24,Key);
	
	GetDlgItemText(IDC_EAesEn,(char*)inBuff,24);
	if(strlen((char*)inBuff)>16)MessageBox("本例只能加密16字节的字符串，大于截断");
	
	aes.ISD_Cipher(inBuff,ouBuff); //因为输出为16个字节，每个字节用两个字母或数字表示。
	CString str="",strTmp;		//实际输出是32个字母或数字,否则ASCII码值超出127的会变成乱码。
	for(int i=0;i<16;i++)
	{
		strTmp.Format("%02x",ouBuff[i]);   //其实相当于把ouBuff的ASCII值这个数字以16进制的形式输出
		str+=strTmp;
	}
	//MessageBox(str,"加密后");
	SetDlgItemText(IDC_EAesEn,str);
}
////////////////////////////////////////////////////////////////////////////////////////////////
//Aes字符串解密
void CAesCodeDlg::OnBAesDe() 
{
	// TODO: Add your control notification handler code here
	unsigned char inBuff[33],ouBuff[25],Key[25];  //还是要注意32个字符的字符串需要用33个字节来存储，
	//因为有个结束符
	memset(inBuff,0,32);
	memset(ouBuff,0,25);
	GetDlgItemText(IDC_EKey,(char*)Key,24);
	ISD_Aes aes(24,Key);
	
	GetDlgItemText(IDC_EAesEn,(char*)inBuff,33);
	unsigned char temp[25];
	for(int j=0;j<16;j++)
	{
		temp[j]=char2num(inBuff[2*j])*16+char2num(inBuff[2*j+1]);// 将字符字面表示的16进制ASCII码值转换成真正的ASCII码值
	}
    
	aes.ISD_decryption(temp,ouBuff);//"dda97ca4......ec0d7191"
	
	SetDlgItemText(IDC_EAesDe,CString(ouBuff));
}

////////////////////////////////////////////////////////////////////////////////////////////////
//字符ASCII码值到字符字面值的转换 如 '0'转换成0， 'a'转换成10
int CAesCodeDlg::char2num(char ch)
{
	if(ch>='0'&&ch<='9')return ch-'0';
	else if(ch>='a'&&ch<='f')return ch-'a'+10;
	return -1;
}
////////////////////////////////////////////////////////////////////////////////////////////////
//文件选择框
void CAesCodeDlg::OnBFile() 
{
	// TODO: Add your control notification handler code here
	CFileDialog fdlg(1,NULL,NULL,OFN_HIDEREADONLY ,"All Files(*.*)|*.*||");
	if(IDOK!=fdlg.DoModal())return;
	EnDe_filename=fdlg.GetPathName();	
	SetDlgItemText(IDC_EFile,EnDe_filename);
}
////////////////////////////////////////////////////////////////////////////////////////////////
//Aes文件加密
void CAesCodeDlg::OnBFileEn() 
{
	// TODO: Add your control notification handler code here
	if(EnDe_filename=="")return;
	FILE* finput;
	FILE* foutput;
	finput=fopen((LPCTSTR)EnDe_filename,"rb");
	if(!finput)
	{
		MessageBox("文件打开错误","出错",MB_OK);
		return;
	}
	fseek(finput,0,SEEK_END);
	long lFileLen=ftell(finput); //ftell()函数返回文件位置指示符的当前值，即如果现在是在文件结尾，则这个值就是文件长度
	fseek(finput,0,SEEK_SET);
	long blocknum=lFileLen/16;
	long leftnum=lFileLen%16;
	EnDe_filename+=".en";
	foutput=fopen((LPCTSTR)EnDe_filename,"wb");
	if(!foutput)
	{
		MessageBox("文件打开错误","出错",MB_OK);
		fclose(finput);
		return;
	}
	unsigned char inBuff[25],ouBuff[25];
	ISD_Aes aes(16,(unsigned char*)"\x0\x1\x2\x3\x4\x5\x6\x7\x8\x9\xa\xb\xc\xd\xe\xf");
	for(long i=0;i<blocknum;i++)
	{
		fread(inBuff,1,16,finput);  //读取16个对象，每个对象的长度是1字节
		aes.ISD_Cipher(inBuff,ouBuff);
		fwrite(ouBuff,1,16,foutput);
		m_prog.SetPos(int(100*i/blocknum));   //加密进度条进度设置
	}
	if(leftnum)
	{
		memset(inBuff,0,16);
		fread(inBuff,1,leftnum,finput);
		aes.ISD_Cipher(inBuff,ouBuff);
		fwrite(ouBuff,1,16,foutput);
	}
	fclose(finput);
	fclose(foutput);
	MessageBox("加密成功！");
	SetDlgItemText(IDC_EFile,EnDe_filename);
	m_prog.SetPos(0);
}
////////////////////////////////////////////////////////////////////////////////////////////////
//Aes文件解密
void CAesCodeDlg::OnBFileDe() 
{
	// TODO: Add your control notification handler code here
	if(EnDe_filename=="")return;
	FILE* finput;
	FILE* foutput;
	finput=fopen((LPCTSTR)EnDe_filename,"rb");
	if(!finput)
	{
		MessageBox("文件打开错误","出错",MB_OK);
		return;
	}
	fseek(finput,0,SEEK_END);
	long lFileLen=ftell(finput); //ftell()函数返回文件位置指示符的当前值，即如果现在是在文件结尾，则这个值就是文件长度
	fseek(finput,0,SEEK_SET);
	long blocknum=lFileLen/16;
	long leftnum=lFileLen%16;
	EnDe_filename+=".de";
	foutput=fopen((LPCTSTR)EnDe_filename,"wb");
	if(!foutput)
	{
		MessageBox("文件打开错误","出错",MB_OK);
		fclose(finput);
		return;
	}
	unsigned char inBuff[25],ouBuff[25];
	ISD_Aes aes(16,(unsigned char*)"\x0\x1\x2\x3\x4\x5\x6\x7\x8\x9\xa\xb\xc\xd\xe\xf");
	for(long i=0;i<blocknum;i++)
	{
		fread(inBuff,1,16,finput);  //读取16个对象，每个对象的长度是1字节
		aes.ISD_decryption(inBuff,ouBuff);
		fwrite(ouBuff,1,16,foutput);
		m_prog.SetPos(int(100*i/blocknum));   //加密进度条进度设置
	}
	if(leftnum)
	{
		MessageBox("文件可能已损坏或非经aes加密过");
	}
	fclose(finput);
	fclose(foutput);
	MessageBox("解密成功！");
	SetDlgItemText(IDC_EFile,EnDe_filename);
	m_prog.SetPos(0);
}
