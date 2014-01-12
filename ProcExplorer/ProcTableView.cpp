
#include <Windows.h>
#include <TlHelp32.h>
#include <Psapi.h>

#include <QMenu>
#include <QStandardItem>
#include <QString>
#include <QList>
#include <QDesktopServices>
#include <QUrl>
#include <QTimer>

#include "ProcExplorerMain.h"
#include "ProcTableView.h"
#include "ModuleDialog.h"
#include "PEFile.h"
#include "ModuleInfo.h"

static const wchar_t *g_dll_name = L"E:\\CodeBase\\Output\\TestDll.dll";
static const wchar_t *g_kernel_name = L"Kernel32.dll";
static const char *g_entry_func_name = "LoadLibraryW";

ProcTableView::ProcTableView(QWidget *parent)
    : QTableView(parent),
    ui(new Ui::ProcTableView),
    focus_index_(-1)
{
    ui->setupUi(this);

    verticalHeader()->setHidden(true);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    resizeRowsToContents();
    setSelectionBehavior(QAbstractItemView::SelectRows);

    // Set Right KeyDown Menu
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)),
        this, SLOT(show_menu(const QPoint &)));

    // create a timer to reflush the process id every second
    QTimer *_timer = new QTimer(this);
    connect(_timer, SIGNAL(timeout()), this, SLOT(read_the_process()));
    _timer->start(1000);
}

ProcTableView::~ProcTableView(void)
{

}

void ProcTableView::read_the_process(void)
{
    // backup the index
    focus_index_ = currentIndex().row();
    QModelIndex _index = currentIndex();
    // get the model
	QStandardItemModel *_model = (QStandardItemModel *)model();
    // remove the model row
    _model->removeRows(0, _model->rowCount());
    proc_info_.clear();

    // read the process info
    HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    int _proc_id_count = 0;
    PROCESSENTRY32 _proc_entry;
    _proc_entry.dwSize = sizeof(PROCESSENTRY32);
    bool bResult = Process32First(hSnapShot, &_proc_entry);
    while (bResult)
    {
        ProcessInfo *_proc = new ProcessInfo(this);
        _proc->init(&_proc_entry);
        proc_info_.push_back(_proc);

        _model->appendRow(_proc->toStandardItemList());
        bResult = Process32Next(hSnapShot, &_proc_entry);
    }
    CloseHandle(hSnapShot);

    //resizeColumnsToContents();
    selectRow(focus_index_);
    setCurrentIndex(_index);
    // show the processes num
//     ui_.label_proc_num_->setText(
//         "Processes number:" + QString::number(proc_info_.size()));
}

void ProcTableView::show_menu(const QPoint &_point)
{
    QMenu *_menu = new QMenu(this);
    QAction *_action = _menu->addAction("Injection");
    connect(_action, SIGNAL(triggered()), this, SLOT(injection()));
    QAction *_action_read_module = _menu->addAction("Module");
    connect(_action_read_module, SIGNAL(triggered()),
        this, SLOT(read_the_module()));
    QAction *_action_read_the_pe_file = _menu->addAction("Read PE file");
    connect(_action_read_the_pe_file, SIGNAL(triggered()),
        this, SLOT(read_the_pe_file()));
    QAction *_action_open_file_path = _menu->addAction("Open the file");
    connect(_action_open_file_path, SIGNAL(triggered()),
        this, SLOT(open_the_file()));

    _menu->move(QCursor::pos());
    focus_index_ = rowAt(_point.y());

    _menu->show();
}

void ProcTableView::injection(void)
{
    if (focus_index_ == -1) return;
    int _proc_id = proc_info_[focus_index_]->get_proc_id();

    HANDLE hProc = OpenProcess(
        PROCESS_CREATE_THREAD | PROCESS_VM_OPERATION | PROCESS_VM_WRITE,
        FALSE, _proc_id);
    DWORD dwSize = 48 * 2;
    LPVOID _addr = VirtualAllocEx(hProc, NULL, dwSize, MEM_COMMIT, PAGE_READWRITE);
    if (_addr == NULL) return;

    DWORD _dwDataWriten = 0;
    if (!WriteProcessMemory(hProc, _addr, g_dll_name, dwSize, &_dwDataWriten))
    {
        VirtualFree(_addr, NULL, MEM_DECOMMIT);
        CloseHandle(hProc);
        return;
    }
    if (dwSize != _dwDataWriten)
    {
        CloseHandle(hProc);
        VirtualFree(_addr, NULL, MEM_DECOMMIT);
        return;
    }

    HMODULE hKernel = GetModuleHandle(g_kernel_name);
    LPTHREAD_START_ROUTINE _entry_func = 
        (LPTHREAD_START_ROUTINE)GetProcAddress(hKernel, g_entry_func_name);

    DWORD dwRemoteThreadId = 0;
    HANDLE hRemoteThread = CreateRemoteThread(
        hProc, NULL, 0,
        _entry_func,
        _addr, NULL, &dwRemoteThreadId);
    if (hRemoteThread == NULL)
    {
        CloseHandle(hProc);
        VirtualFree(_addr, NULL, MEM_DECOMMIT);
        return;
    }

    WaitForSingleObject(hRemoteThread, INFINITE);
    CloseHandle(hRemoteThread);
    CloseHandle(hProc);
    return;
}

void ProcTableView::read_the_module(void)
{
    if (focus_index_ == -1) return;
    int _proc_id = proc_info_[focus_index_]->get_proc_id();

    HANDLE _hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, _proc_id);
    MODULEENTRY32W _module_entry;
    _module_entry.dwSize = sizeof(MODULEENTRY32W);
    bool bResult = Module32FirstW(_hSnapShot, &_module_entry);
    int _error = 0;
    if (!bResult)
    {
        _error = GetLastError();
    }
    while (bResult)
    {
        ModuleInfo *_item = new ModuleInfo(
            QString::fromUtf16(_module_entry.szModule),
            QString::fromUtf16(_module_entry.szExePath),
            (unsigned int)_module_entry.modBaseAddr,
            NULL);
        proc_info_[focus_index_]->add_module(_item);
        bResult = Module32NextW(_hSnapShot, &_module_entry);
    }
    ModuleDialog *_dialog = new ModuleDialog(this);
    _dialog->setModal(true);
    _dialog->set_data(proc_info_[focus_index_]);
    _dialog->show();
}

void ProcTableView::read_the_pe_file(void)
{
    if (focus_index_ == -1) return;
    QString _path = proc_info_[focus_index_]->get_proc_path();

    PEFile *_file = new PEFile(this);
    _file->read_the_pe_file(_path);
}

void ProcTableView::open_the_file(void)
{
    if (focus_index_ == -1) return;
    QString _path = proc_info_[focus_index_]->get_proc_path();
    _path = _path.left(_path.lastIndexOf('\\'));

    QDesktopServices::openUrl(QUrl("file:///" + _path, QUrl::TolerantMode));
}