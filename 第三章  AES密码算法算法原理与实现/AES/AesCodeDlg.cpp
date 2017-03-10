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
//Aes�ַ�������
void CAesCodeDlg::OnBAesEn() 
{
	// TODO: Add your control notification handler code here
	unsigned char inBuff[25],ouBuff[25],Key[25];
	memset(inBuff,0,25);
	memset(ouBuff,0,25);
	GetDlgItemText(IDC_EKey,(char*)Key,24);
	ISD_Aes aes(24,Key);
	
	GetDlgItemText(IDC_EAesEn,(char*)inBuff,24);
	if(strlen((char*)inBuff)>16)MessageBox("����ֻ�ܼ���16�ֽڵ��ַ��������ڽض�");
	
	aes.ISD_Cipher(inBuff,ouBuff); //��Ϊ���Ϊ16���ֽڣ�ÿ���ֽ���������ĸ�����ֱ�ʾ��
	CString str="",strTmp;		//ʵ�������32����ĸ������,����ASCII��ֵ����127�Ļ������롣
	for(int i=0;i<16;i++)
	{
		strTmp.Format("%02x",ouBuff[i]);   //��ʵ�൱�ڰ�ouBuff��ASCIIֵ���������16���Ƶ���ʽ���
		str+=strTmp;
	}
	//MessageBox(str,"���ܺ�");
	SetDlgItemText(IDC_EAesEn,str);
}
////////////////////////////////////////////////////////////////////////////////////////////////
//Aes�ַ�������
void CAesCodeDlg::OnBAesDe() 
{
	// TODO: Add your control notification handler code here
	unsigned char inBuff[33],ouBuff[25],Key[25];  //����Ҫע��32���ַ����ַ�����Ҫ��33���ֽ����洢��
	//��Ϊ�и�������
	memset(inBuff,0,32);
	memset(ouBuff,0,25);
	GetDlgItemText(IDC_EKey,(char*)Key,24);
	ISD_Aes aes(24,Key);
	
	GetDlgItemText(IDC_EAesEn,(char*)inBuff,33);
	unsigned char temp[25];
	for(int j=0;j<16;j++)
	{
		temp[j]=char2num(inBuff[2*j])*16+char2num(inBuff[2*j+1]);// ���ַ������ʾ��16����ASCII��ֵת����������ASCII��ֵ
	}
    
	aes.ISD_decryption(temp,ouBuff);//"dda97ca4......ec0d7191"
	
	SetDlgItemText(IDC_EAesDe,CString(ouBuff));
}

////////////////////////////////////////////////////////////////////////////////////////////////
//�ַ�ASCII��ֵ���ַ�����ֵ��ת�� �� '0'ת����0�� 'a'ת����10
int CAesCodeDlg::char2num(char ch)
{
	if(ch>='0'&&ch<='9')return ch-'0';
	else if(ch>='a'&&ch<='f')return ch-'a'+10;
	return -1;
}
////////////////////////////////////////////////////////////////////////////////////////////////
//�ļ�ѡ���
void CAesCodeDlg::OnBFile() 
{
	// TODO: Add your control notification handler code here
	CFileDialog fdlg(1,NULL,NULL,OFN_HIDEREADONLY ,"All Files(*.*)|*.*||");
	if(IDOK!=fdlg.DoModal())return;
	EnDe_filename=fdlg.GetPathName();	
	SetDlgItemText(IDC_EFile,EnDe_filename);
}
////////////////////////////////////////////////////////////////////////////////////////////////
//Aes�ļ�����
void CAesCodeDlg::OnBFileEn() 
{
	// TODO: Add your control notification handler code here
	if(EnDe_filename=="")return;
	FILE* finput;
	FILE* foutput;
	finput=fopen((LPCTSTR)EnDe_filename,"rb");
	if(!finput)
	{
		MessageBox("�ļ��򿪴���","����",MB_OK);
		return;
	}
	fseek(finput,0,SEEK_END);
	long lFileLen=ftell(finput); //ftell()���������ļ�λ��ָʾ���ĵ�ǰֵ����������������ļ���β�������ֵ�����ļ�����
	fseek(finput,0,SEEK_SET);
	long blocknum=lFileLen/16;
	long leftnum=lFileLen%16;
	EnDe_filename+=".en";
	foutput=fopen((LPCTSTR)EnDe_filename,"wb");
	if(!foutput)
	{
		MessageBox("�ļ��򿪴���","����",MB_OK);
		fclose(finput);
		return;
	}
	unsigned char inBuff[25],ouBuff[25];
	ISD_Aes aes(16,(unsigned char*)"\x0\x1\x2\x3\x4\x5\x6\x7\x8\x9\xa\xb\xc\xd\xe\xf");
	for(long i=0;i<blocknum;i++)
	{
		fread(inBuff,1,16,finput);  //��ȡ16������ÿ������ĳ�����1�ֽ�
		aes.ISD_Cipher(inBuff,ouBuff);
		fwrite(ouBuff,1,16,foutput);
		m_prog.SetPos(int(100*i/blocknum));   //���ܽ�������������
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
	MessageBox("���ܳɹ���");
	SetDlgItemText(IDC_EFile,EnDe_filename);
	m_prog.SetPos(0);
}
////////////////////////////////////////////////////////////////////////////////////////////////
//Aes�ļ�����
void CAesCodeDlg::OnBFileDe() 
{
	// TODO: Add your control notification handler code here
	if(EnDe_filename=="")return;
	FILE* finput;
	FILE* foutput;
	finput=fopen((LPCTSTR)EnDe_filename,"rb");
	if(!finput)
	{
		MessageBox("�ļ��򿪴���","����",MB_OK);
		return;
	}
	fseek(finput,0,SEEK_END);
	long lFileLen=ftell(finput); //ftell()���������ļ�λ��ָʾ���ĵ�ǰֵ����������������ļ���β�������ֵ�����ļ�����
	fseek(finput,0,SEEK_SET);
	long blocknum=lFileLen/16;
	long leftnum=lFileLen%16;
	EnDe_filename+=".de";
	foutput=fopen((LPCTSTR)EnDe_filename,"wb");
	if(!foutput)
	{
		MessageBox("�ļ��򿪴���","����",MB_OK);
		fclose(finput);
		return;
	}
	unsigned char inBuff[25],ouBuff[25];
	ISD_Aes aes(16,(unsigned char*)"\x0\x1\x2\x3\x4\x5\x6\x7\x8\x9\xa\xb\xc\xd\xe\xf");
	for(long i=0;i<blocknum;i++)
	{
		fread(inBuff,1,16,finput);  //��ȡ16������ÿ������ĳ�����1�ֽ�
		aes.ISD_decryption(inBuff,ouBuff);
		fwrite(ouBuff,1,16,foutput);
		m_prog.SetPos(int(100*i/blocknum));   //���ܽ�������������
	}
	if(leftnum)
	{
		MessageBox("�ļ��������𻵻�Ǿ�aes���ܹ�");
	}
	fclose(finput);
	fclose(foutput);
	MessageBox("���ܳɹ���");
	SetDlgItemText(IDC_EFile,EnDe_filename);
	m_prog.SetPos(0);
}
