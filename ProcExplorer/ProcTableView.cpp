
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
#include <QItemSelection>
#include <QScrollBar>
#include <qDebug>

#include "ProcExplorerMain.h"
#include "ProcTableView.h"
#include "ModuleDialog.h"
#include "PEFile.h"
#include "ModModel.h"
#include "ProcModel.h"

static const wchar_t *g_dll_name = L"E:\\CodeBase\\Output\\TestDll.dll";
static const wchar_t *g_kernel_name = L"Kernel32.dll";
static const char *g_entry_func_name = "LoadLibraryW";

ProcTableView::ProcTableView(QWidget *parent)
    : QTableView(parent),
    focus_index_(-1)
{
    // 隐藏列表头
    verticalHeader()->setHidden(true);
    // 设置横表头最后项为伸展
    horizontalHeader()->setStretchLastSection(true);
    // 表不可编辑
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    // 行选项
    setSelectionBehavior(QAbstractItemView::SelectRows);
    // 单行选择
    setSelectionMode(QAbstractItemView::SingleSelection);
    // 列自适应宽度
    resizeColumnsToContents();

    // Set Right KeyDown Menu
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)),
        this, SLOT(show_menu(const QPoint &)));
    connect(this, SIGNAL(update_proc_num(int)),
        parent, SLOT(set_proc_num(int)));

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
    // 保存当前所选项和当前滚动条位置
    QModelIndex _index = currentIndex();
    int _value = verticalScrollBar()->value();
    // 清空Model数据
    ProcModel *_model = ((ProcModel *)model());
    _model->removeRows(0, _model->rowCount());

    // 获取进程数据（快照）
    HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32 _proc_entry;
    _proc_entry.dwSize = sizeof(PROCESSENTRY32);
    bool bResult = Process32First(hSnapShot, &_proc_entry);
    while (bResult)
    {
        if (!_model->addRow(_proc_entry))
        {
            qDebug() << "Update Row";
            _model->updateRow(_proc_entry);
        }
        else
        {
            qDebug() << "Add Row";
        }
        bResult = Process32Next(hSnapShot, &_proc_entry);
    }
    CloseHandle(hSnapShot);

    // 更新Model
    _model->update();
    // 恢复所选择项和滚动条位置
    setCurrentIndex(_index);
    verticalScrollBar()->setValue(_value);
    // 行自适应高度
    resizeRowsToContents();
    // 更新进程数
    emit update_proc_num(_model->rowCount());
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
    QModelIndex _item = ((ProcModel *)model())->index(
                            focus_index_, ProcModel::PROC_ID);
    int _proc_id = _item.data().toInt();

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
    QModelIndex _item = ((ProcModel *)model())->index(
                            focus_index_, ProcModel::PROC_ID);
    int _proc_id = _item.data().toInt();

    ModuleDialog *_dialog = new ModuleDialog(_proc_id, this);
    _dialog->setModal(true);
    // _dialog->set_data(proc_info_[focus_index_]);
    _dialog->show();
}

void ProcTableView::read_the_pe_file(void)
{
    if (focus_index_ == -1) return;
    QModelIndex _item = ((ProcModel *)model())->index(
                            focus_index_, ProcModel::PROC_PATH);
    QString _path = _item.data().toString();

    PEFile *_file = new PEFile(this);
    _file->read_the_pe_file(_path);
}

void ProcTableView::open_the_file(void)
{
    if (focus_index_ == -1) return;
    QModelIndex _item = ((ProcModel *)model())->index(
                            focus_index_, ProcModel::PROC_PATH);
    QString _path = _item.data().toString();
    _path = _path.left(_path.lastIndexOf('\\'));

    QDesktopServices::openUrl(QUrl("file:///" + _path, QUrl::TolerantMode));
}