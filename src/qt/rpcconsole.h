// Copyright (c) 2011-2017 The Bitcoin Core developers
// Copyright (c) 2018 The PRiVCY developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
#ifndef RPCCONSOLE_H
#define RPCCONSOLE_H
#include "net.h"
#include <QThread>
#include <QCompleter>
#include <QWidget>
#include <QDialog>
#include <QMenu>
#include <QSettings>

namespace Ui {
    class RPCConsole;
}
class ClientModel;
class CNodeCombinedStats;

QT_BEGIN_NAMESPACE
class QMenu;
class QItemSelection;
QT_END_NAMESPACE

/** Local Bitcoin RPC console. */
class RPCConsole: public QDialog
{
    Q_OBJECT

public:
    explicit RPCConsole(QWidget *parent = 0);
    ~RPCConsole();

    void setClientModel(ClientModel *model);

    enum MessageClass {
        MC_ERROR,
        MC_DEBUG,
        CMD_REQUEST,
        CMD_REPLY,
        CMD_ERROR
    };

protected:
    virtual bool eventFilter(QObject* obj, QEvent *event);

private slots:
    void on_lineEdit_returnPressed();
    void on_tabWidget_currentChanged(int index);
    /** open the debug.log from the current datadir */
    void on_openDebugLogfileButton_clicked();
    /** display messagebox with program parameters (same as bitcoin-qt --help) */
    void on_showCLOptionsButton_clicked();
    /** change the time range of the network traffic graph */
    void on_sldGraphRange_valueChanged(int value);
    /** update traffic statistics */
    void updateTrafficStats(quint64 totalBytesIn, quint64 totalBytesOut);
    void resizeEvent(QResizeEvent *event);
    void showEvent(QShowEvent *event);
    void hideEvent(QHideEvent *event);
    /** Show custom context menu on Peers tab */
    void showPeersTableContextMenu(const QPoint &point);
    /** Show custom context menu on Bans tab */
    void showBanTableContextMenu(const QPoint &point);
    /** Hides ban table if no bans are present */
    void showOrHideBanTableIfRequired();
    /** clear the selected node */
    void clearSelectedNode();
    /** Open external (default) editor with PRiVCY.conf */
    void showConfEditor();

  public slots:
    void clear(bool clearHistory = true);
    void fontBigger();
    void fontSmaller();
    void setFontSize(int newSize);

    void message(int category, const QString &message, bool html = false);
    /** Set number of connections shown in the UI */
    void setNumConnections(int count);
    /** Set network state shown in the UI */
    void setNetworkActive(bool networkActive);
    /** Set number of blocks shown in the UI */
    void setNumBlocks(int count, int countOfPeers);
    /** Go forward or back in history */
    void browseHistory(int offset);
    /** Scroll console view to end */
    void scrollToEnd();
    /** Handle selection of peer in peers list */
    void peerSelected(const QItemSelection &selected, const QItemSelection &deselected);
    /** Handle updated peer information */
    void peerLayoutChanged();
    /** Disconnect a selected node on the Peers tab */
    void disconnectSelectedNode();
    /** Ban a selected node on the Peers tab */
    void banSelectedNode(int bantime);
    /** Unban a selected node on the Bans tab */
    void unbanSelectedNode();

  signals:
    // For RPC command executor
    void stopExecutor();
    void cmdRequest(const QString &command);

private:
  static QString FormatBytes(quint64 bytes);
  Ui::RPCConsole *ui;
  ClientModel *clientModel;
  QStringList history;
  int historyPtr;
  QCompleter *autoCompleter;
  NodeId cachedNodeid;
  QMenu *peersTableContextMenu;
  QMenu *banTableContextMenu;
  QThread thread;
  int consoleFontSize;
  QSettings settings;

  void startExecutor();
  void setTrafficGraphRange(int mins);
  /** show detailed information on ui about selected node */
  void updateNodeDetail(const CNodeCombinedStats *stats);
  enum ColumnWidths
  {
      ADDRESS_COLUMN_WIDTH = 200,
      SUBVERSION_COLUMN_WIDTH = 100,
      PING_COLUMN_WIDTH = 80,
      BANSUBNET_COLUMN_WIDTH = 200,
      BANTIME_COLUMN_WIDTH = 250
  };

  /** Update UI with latest network info from model. */
  void updateNetworkState();

};

#endif // RPCCONSOLE_H